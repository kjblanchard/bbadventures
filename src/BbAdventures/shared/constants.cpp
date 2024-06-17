#include <GoonEngine/prim/rectangle.h>

#include <BbAdventures/shared/constants.hpp>
namespace Bba {
// #ifdef GN_PLATFORM_MACOS
// std::string ASSET_PREFIX = "../Resources/assets";
// #else
std::string ASSET_PREFIX = "assets";
// #endif
std::string TILED_PREFIX = "tiled";
std::string IMAGES_PREFIX = "images";
int SCREEN_WIDTH = 512;
int SCREEN_HEIGHT = 288;

Directions GetOverlapDirection(geRectangle* obj, geRectangle* overlapBox) {
	if (overlapBox->w < overlapBox->h) {
		if (overlapBox->x > obj->x) {
			return Directions::East;
		}
		return Directions::West;
	}
	if (overlapBox->y > obj->y) {
		return Directions::South;
	}
	return Directions::North;
}

Directions GetOverlapDirectionF(geRectangleF* obj, geRectangleF* overlapBox) {
	if (overlapBox->w < overlapBox->h) {
		if (overlapBox->x > obj->x) {
			return Directions::East;
		}
		return Directions::West;
	}
	if (overlapBox->y > obj->y) {
		return Directions::South;
	}
	return Directions::North;
}
}  // namespace Bba
