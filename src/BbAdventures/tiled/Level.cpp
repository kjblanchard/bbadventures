#include <GoonEngine/debug.h>
#include <GoonEngine/utils.h>

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

Level::Level(const char *filename)
	: _background(nullptr) {
	_mapData = std::make_unique<TiledMap>(filename);
	_name = filename;
	LoadSurfaces();
}
Level::~Level() {
	if (_background) {
		// TODO should we actually clear this?  Save for manual cleanup.
		// geImageFree(_background);
	}
	for (auto &&go : _gameObjects) {
		go->FreeGameObject();
		delete (go);
	}

	_gameObjects.clear();
}
std::vector<TiledMap::TiledObject> Level::GetAllObjects() {
	return _mapData->Objects;
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

void Level::LoadSurfaces() {
	for (auto &tileset : _mapData->Tilesets) {
		if (tileset.Type == TilesetType::Image) {
			for (auto &tile : tileset.Tiles) {
				auto surfacePath = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + tile.Image;
				char buf[1000];
				GetLoadFilename(buf, sizeof(buf), surfacePath.c_str());
				auto i = geImageNewFromFile(buf);
				_imagesCache.push_back({tile.Image, i});
			}
		} else {
			auto surfacePath = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + tileset.Image;
			char buf[1000];
			GetLoadFilename(buf, sizeof(buf), surfacePath.c_str());
			auto i = geImageNewFromFile(buf);
			_imagesCache.push_back({tileset.Image, i});
		}
	}
}
void Level::LoadSolidObjects() {
	for (auto &solid : _mapData->SolidObjects) {
		auto go = NewSolidObject(solid);
		_gameObjects.push_back(go);
	}
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
	printf("Could not find loaded surface for git %ud\n", gid);
	return nullptr;
}
void Level::CreateBackgroundImage() {
	if (_background)
		return;
	_background = geImageNewRenderTarget(_name.c_str(), _mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight, nullptr);
	for (auto &group : _mapData->Groups) {
		if (group.Name == "background") {
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
}

void Level::RestartLevel() {
	CreateBackgroundImage();
	LoadSolidObjects();
	Bba::StartPlayers();
}
void Level::UnloadLevel() {
	Bba::FreeAnimationComponents();
}

static void LevelLoaded() {
	State::IsLoadingMap = false;
}

void Level::LoadNewLevel() {
	auto l = new Bba::Level(Bba::State::NextMapName.c_str());
	lastLevel = Bba::State::CurrentLevel;
	Bba::State::CurrentLevel = l;
	l->LoadAllGameObjects();
	l->RestartLevel();
	if (lastLevel) {
		lastLevel->UnloadLevel();
		delete (lastLevel);
	}
	Bba::LoadPlayers();
	Bba::LoadAnimationComponents();
	Bba::LoadTextInteractions();
	Bba::StartPlayers();
	State::FadePanel->FadeIn(LevelLoaded);
}

void Level::Draw() {
	if (_background) {
		geRectangle r;
		r.x = 0;
		r.y = 0;
		r.w = 512;
		r.h = 288;
		geImageDraw(_background, NULL, &r);
	}
}