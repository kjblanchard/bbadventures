#pragma once
#include <GoonEngine/prim/point.h>
#include <GoonEngine/prim/rectangle.h>

struct CameraComponent {
	geRectangleF Box;
	gePoint Bounds;
};