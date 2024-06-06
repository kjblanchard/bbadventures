#pragma once
#include <GoonEngine/prim/rectangle.h>
#include <string>
namespace Bba {
struct PlayerExitComponent {
	geRectangle BoundingBox;
	int SpawnLocationId;
    std::string NextMap;
};
}  // namespace Bba