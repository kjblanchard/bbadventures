#pragma once
#include <GoonEngine/prim/point.h>
#include <GoonEngine/prim/rectangle.h>
namespace Bba {
struct LocationComponent;

struct CameraComponent {
	geRectangleF Box;
	gePoint Bounds;
	LocationComponent* FollowTarget;
};

}  // namespace Bba
