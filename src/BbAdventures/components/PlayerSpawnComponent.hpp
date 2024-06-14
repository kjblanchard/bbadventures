#pragma once
#include <GoonEngine/prim/point.h>
#include <BbAdventures/shared/constants.hpp>
namespace Bba {
struct PlayerSpawnComponent {
	gePoint Location;
	int SpawnLocationId;
	Directions SpawnDirection;
};
}  // namespace Bba