#pragma once
#include <GoonEngine/content/image.h>
#include <GoonEngine/prim/point.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/tiled/TiledMap.hpp>
#include <memory>
#include <tuple>
namespace Bba {
class Panel;
/**
 * @brief A level that is loaded from tiled, this is loaded from json and contains all tiles, objects, properties, etc
 */
class Level {
   public:
	/**
	 * @brief Construct a new Tiled Level object
	 *
	 * @param filename The file to load from json
	 */
	Level(const char *filename);
	~Level();
	static void LoadNewLevel();
	const inline std::string &GetName() const { return _name; }
	inline gePoint GetSize() { return gePoint{_mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight}; }
	inline void AddGameObjectToLevel(GameObject *g) { _gameObjects.push_back(g); }
	void UnloadLevel();
	void LoadAllGameObjects();
	std::vector<TiledMap::TiledObject> GetAllObjects();
	inline std::vector<TiledMap::TiledObject> GetAllSolidObjects() const { return _mapData->SolidObjects; }
	Panel *LoadPanel;
	void Draw();
	void RestartLevel();

   private:
	/**
	 * @brief Loads all the surfaces from the tilesets in this level, so that we can blit them properly
	 */
	void LoadSurfaces();
	void LoadSolidObjects();
	/**
	 * @brief Create a Background Atlas object and blits everything to it
	 */
	void CreateBackgroundImage();
	/**
	 * @brief Get the Surface For Gid object
	 *
	 * @param gid gid of the tile
	 * @param tileset the tileset for this tile
	 * @return struct SDL_Surface* The loaded surface
	 */
	geImage *GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset);

   private:
	std::string _name;
	geImage *_background;
	std::vector<GameObject *> _gameObjects;
	std::unique_ptr<TiledMap> _mapData;
};
}  // namespace Bba