#include <BbAdventures/gnpch.hpp>
#include <BbAdventures/shared/constants.hpp>
#include <BbAdventures/tiled/Level.hpp>
#include <BbAdventures/tiled/TiledMap.hpp>
#include <nlohmann/json.hpp>
using namespace Bba;
static std::vector<std::pair<std::string, geImage *>> _imagesCache;

Level::Level(const char *filename)
	: _background(nullptr) {
	_mapData = std::make_unique<TiledMap>(filename);
	_name = filename;
	LoadSurfaces();
}
std::vector<TiledMap::TiledObject> Level::GetAllObjects() {
	return _mapData->Objects;
}

Level::~Level() {
	geImageFree(_background);
}

void Level::LoadSurfaces() {
	for (auto &tileset : _mapData->Tilesets) {
		if (tileset.Type == TilesetType::Image) {
			for (auto &tile : tileset.Tiles) {
				auto surfacePath = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + tile.Image;
				auto i = geImageNewFromFile(surfacePath.c_str());
				_imagesCache.push_back({tile.Image, i});
			}
		} else {
			auto surfacePath = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + tileset.Image;
			auto i = geImageNewFromFile(surfacePath.c_str());
			_imagesCache.push_back({tileset.Image, i});
		}
	}
}
void Level::LoadSolidObjects() {
	for (auto &solid : _mapData->SolidObjects) {
		auto box = geRectangle{solid.X, solid.Y, solid.Width, solid.Height};
		// auto body = gpBodyNewStatic(box);
		// gpSceneAddStaticBody(body);
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
	_background = geImageNewRenderTarget("needgoodnameplz", _mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight);
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
						geImageDrawImageToImage(tileSurface, _background, &sourceRect, &dstRect);
					}
				}
			}
		}
	}
}

void Level::RestartLevel() {
	LoadSolidObjects();
}