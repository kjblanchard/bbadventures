#pragma once
#include <GoonEngine/prim/point.h>
#include <GoonEngine/prim/rectangle.h>

struct CameraComponent {
	geRectangle Box;
	gePoint Bounds;
};