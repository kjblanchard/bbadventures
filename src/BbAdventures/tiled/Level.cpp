#include "Level.hpp"

#include <GoonEngine/content/bgm.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/components/CameraComponent.hpp>
#include <BbAdventures/entities/Camera.hpp>
#include <BbAdventures/entities/Solid.hpp>
#include <BbAdventures/gnpch.hpp>
#include <BbAdventures/shared/constants.hpp>
#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/systems/Systems.hpp>
#include <BbAdventures/tiled/Level.hpp>
#include <BbAdventures/tiled/TiledMap.hpp>
#include <BbAdventures/ui/Panel.hpp>
#include <nlohmann/json.hpp>
namespace Bba {
extern std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;
}
using namespace Bba;
static std::vector<std::pair<std::string, geImage *>> _imagesCache;
static Level *lastLevel = nullptr;
static const int _bufferSize = 255;

Level::Level(const char *filename)
	: _background(nullptr) {
	_name = filename;
	_mapData = std::make_unique<TiledMap>(filename);
	LoadSurfaces();
}

Level::~Level() {
	Bba::FreeAnimationComponents();
	if (_background) {
		// TODO should we actually clear this?  Save for manual cleanup.
		// geImageFree(_background);
	}
	for (auto &&go : _gameObjects) {
		go->FreeGameObject();
		delete (go);
	}

	_gameObjects.clear();
	// TODO should we actually clear this?  Save for manual cleanup.
	// geBgmDelete(_bgm);
}

void Level::LoadSurfaces() {
	for (auto &tileset : _mapData->Tilesets) {
		if (tileset.Type == TilesetType::Image) {
			for (auto &tile : tileset.Tiles) {
				auto surfacePath = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + tile.Image;
				char buf[_bufferSize];
				geGetLoadFilename(buf, sizeof(buf), surfacePath.c_str());
				if (!CheckIfTilesetIsCached(buf)) {
					auto i = geImageNewFromFile(buf);
					_imagesCache.push_back({tile.Image, i});
				}
			}
		} else {
			auto surfacePath = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + tileset.Image;
			char buf[_bufferSize];
			geGetLoadFilename(buf, sizeof(buf), surfacePath.c_str());
			if (!CheckIfTilesetIsCached(buf)) {
				auto i = geImageNewFromFile(buf);
				_imagesCache.push_back({tileset.Image, i});
			}
		}
	}
}

bool Level::CheckIfTilesetIsCached(const std::string &name) const {
	return std::any_of(_imagesCache.begin(), _imagesCache.end(),
					   [&name](const auto &cachedTileset) {
						   return cachedTileset.first == name;
					   });
}

geImage *Level::GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset) {
	if (tileset->Type == TilesetType::Image) {
		for (auto &tile : tileset->Tiles) {
			if (tile.Id + tileset->FirstGid == gid) {
				for (auto surface : _imagesCache) {
					if (surface.first == tile.Image)
						return surface.second;
				}
			}
		}
	} else {
		for (auto surface : _imagesCache) {
			if (surface.first == tileset->Image)
				return surface.second;
		}
	}
	printf("Could not find loaded surface for gid %ud\n", gid);
	return nullptr;
}

static void LevelLoaded() {
	State::IsLoadingMap = false;
}

void Level::LoadNewLevel() {
	auto l = new Bba::Level(Bba::State::NextMapName.c_str());
	lastLevel = Bba::State::CurrentLevel;
	Bba::State::CurrentLevel = l;
	// Load things that potentially the last level would of loaded already
	l->LoadAllGameObjects();
	// Create new camera and set bounds
	auto c = NewCamera();
	auto &cc = c->GetComponent<CameraComponent>();
	auto worldSize = l->GetSize();
	cc.Bounds.x = worldSize.x;
	cc.Bounds.y = worldSize.y;
	l->AddGameObjectToLevel(c);
	l->RestartLevel();
	if (lastLevel) {
		delete (lastLevel);
	}
	// Load things
	Bba::LoadPlayers();
	Bba::LoadAnimationComponents();
	Bba::LoadTextInteractions();
	// Start
	l->StartBgm();
	Bba::StartPlayers();
	Bba::UpdateCamera();
	// End and fade out
	State::FadePanel->FadeIn(LevelLoaded);
}

void Level::LoadAllGameObjects() {
	for (auto &obj : _mapData->Objects) {
		auto type = obj.ObjectType;
		auto iter = GameSpawnMap.find(type);
		if (iter == GameSpawnMap.end())
			continue;
		auto go = (*iter).second(obj);
		if (!go) {
			LogWarn("Could not create object of type %s", type.c_str());
		}
		_gameObjects.push_back(go);
	}
}

void Level::RestartLevel() {
	CreateBackgroundImage();
	LoadSolidObjects();
	StartPlayers();
}

void Level::CreateBackgroundImage() {
	if (_background)
		return;
	_background = geImageNewRenderTarget(_name.c_str(), _mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight, nullptr);
	for (auto &group : _mapData->Groups) {
		if (group.Name != "background") {
			continue;
		}
		for (auto &groupLayer : group.Layers) {
			for (int y = 0; y < groupLayer.Height; ++y) {
				for (int x = 0; x < groupLayer.Width; ++x) {
					auto index = (y * groupLayer.Width) + x;
					auto tileGid = groupLayer.Data[index];
					if (tileGid == 0)
						continue;
					auto tiledMapTileset = _mapData->GetGidTiledMapTileset(tileGid);
					auto tileset = _mapData->GetTiledMapTilesetTileset(tiledMapTileset);
					auto tileSurface = GetSurfaceForGid(tileGid, tileset);
					auto sourceRect = _mapData->GetGidSourceRect(tileGid);
					auto dstX = x * _mapData->TileWidth;
					auto dstY = y * _mapData->TileHeight;
					// Adjust background image as tiled draws it from the opposite end.
					if (tileset->Type == TilesetType::Image) {
						dstY -= (sourceRect.h - _mapData->TileHeight);
					}
					auto dstRect = geRectangle{dstX, dstY, sourceRect.w, sourceRect.h};
					if (!tileSurface || !_background) {
						continue;
					}

					geImageDrawImageToImage(tileSurface, _background, &sourceRect, &dstRect);
				}
			}
		}
	}
}

void Level::LoadSolidObjects() {
	for (auto &solid : _mapData->SolidObjects) {
		auto go = NewSolidObject(solid);
		_gameObjects.push_back(go);
	}
	const int boxSize = 16;
	auto size = GetSize();
	geRectangle top = {0, 0, size.x, boxSize};
	geRectangle right = {size.x + 1, 0, boxSize, size.y};
	geRectangle bottom = {0, size.y + 1, size.x, boxSize};
	geRectangle left = {-boxSize, 0, boxSize, size.y};
	_gameObjects.push_back(NewSolidObject(top));
	_gameObjects.push_back(NewSolidObject(right));
	_gameObjects.push_back(NewSolidObject(bottom));
	_gameObjects.push_back(NewSolidObject(left));
}

void Level::StartBgm() {
	for (auto &&prop : _mapData->Properties) {
		if (prop.Name == "bgm") {
			auto bgmName = std::get<std::string>(prop.Value);
			_bgm = geBgmNew(bgmName.c_str());
			if (bgmName != State::PlayingMusic) {
				State::PlayingMusic = bgmName;
				geBgmLoad(_bgm);
				geBgmPlay(_bgm, 1.0, -1);
			}
			break;
		}
	}
}

void Level::Draw() {
	if (_background) {
		// Try to reduce jitter / blurryness from the camera offset on the background due to float/int
		// Other options // SDL_RenderSetViewport, rendering full src and offset dst, something else?
		int camX = static_cast<int>(State::CameraX);
		int camY = static_cast<int>(State::CameraY);
		// Calculate the fractional part of the camera coordinates
		float offsetX = State::CameraX - camX;
		float offsetY = State::CameraY - camY;
		// If the map is smaller than the screen size, the source size should be the size of it.
		auto size = GetSize();
		geRectangle s;
		s.x = camX;
		s.y = camY;
		s.w = size.x <= SCREEN_WIDTH ? size.x : SCREEN_WIDTH;
		s.h = size.y <= SCREEN_HEIGHT ? size.y : SCREEN_HEIGHT;
		geRectangleF d;
		d.x = -offsetX;	 // Offset by the fractional part
		d.y = -offsetY;	 // Offset by the fractional part
		d.w = size.x <= SCREEN_WIDTH ? size.x : SCREEN_WIDTH;
		d.h = size.y <= SCREEN_HEIGHT ? size.y : SCREEN_HEIGHT;
		geImageDrawF(_background, &s, &d);
	}
}