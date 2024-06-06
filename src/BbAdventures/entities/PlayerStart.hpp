#pragma once
#include <BbAdventures/tiled/TiledMap.hpp>

namespace Bba {
    class GameObject;
    GameObject* NewPlayerSpawn(TiledMap::TiledObject&);
}