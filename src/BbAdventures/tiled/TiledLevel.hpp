// #pragma once
// #include <memory>
// #include <tuple>
// #include <BbAdventures/tiled/TiledMap.hpp>
// #include <GoonEngine/primitives/point.h>
// struct SDL_Surface;
// struct SDL_Texture;
// namespace Bba
// {
//     /**
//      * @brief A level that is loaded from tiled, this is loaded from json and contains all tiles, objects, properties, etc
//      */
//     class TiledLevel
//     {
//     public:
//         /**
//          * @brief Construct a new Tiled Level object
//          *
//          * @param filename The file to load from json
//          */
//         TiledLevel(const char *filename);
//         const inline std::string &GetName() const { return _name; }
//         inline Point GetSize() { return Point{_mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight}; }
//         std::vector<TiledMap::TiledObject> GetAllObjects();
//         inline std::vector<TiledMap::TiledObject> GetAllSolidObjects() const { return _mapData->SolidObjects; }
//         void RestartLevel();
//         ~TiledLevel();
//         inline Point GetGravity() { return _gravity; }
//         inline std::tuple<std::string &, float, float, float> GetBgmData() { return {_bgmName, _bgmStart, _bgmEnd, _volume}; }
//         std::string GetNextLevel();

//     private:
//         /**
//          * @brief Loads all the surfaces from the tilesets in this level, so that we can blit them properly
//          */
//         void LoadSurfaces();
//         void LoadSolidObjects();
//         /**
//          * @brief Create a Background Atlas object and blits everything to it
//          */
//         void CreateBackgroundAtlas();
//         /**
//          * @brief Get the Surface For Gid object
//          *
//          * @param gid gid of the tile
//          * @param tileset the tileset for this tile
//          * @return struct SDL_Surface* The loaded surface
//          */
//         struct SDL_Surface *GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset);

//         void LoadGravity();
//         void LoadBgm();

//     private:
//         std::string _name;
//         std::vector<std::pair<std::string, struct SDL_Surface *>> _loadedTilesets;
//         struct SDL_Texture *_loadedAtlas;
//         std::unique_ptr<TiledMap> _mapData;
//         Point _gravity;
//         std::string _bgmName = "";
//         float _bgmStart = 0.0, _bgmEnd = 0.0, _volume = 0.0;
//     };
// }