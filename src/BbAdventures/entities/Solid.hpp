#pragma once
#include <BbAdventures/tiled/TiledMap.hpp>

namespace Bba {
    class GameObject;
    GameObject* NewSolidObject(TiledMap::TiledObject&);
    GameObject* NewSolidObject(geRectangle& r);
}