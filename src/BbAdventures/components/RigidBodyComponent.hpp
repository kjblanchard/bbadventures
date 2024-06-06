#pragma once
#include <GoonEngine/prim/rectangle.h>
namespace Bba {
struct RigidBodyComponent {
	float OffsetX, OffsetY;
	int W, H;
	geRectangle GetRect() {
		return geRectangle { (int)OffsetX, (int)OffsetY, W, H };
	}
};
}  // namespace Bba