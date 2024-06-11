#pragma once
#include <GoonEngine/prim/rectangle.h>
namespace Bba {
struct SolidObjectComponent {
	geRectangle BoxCollider;

	inline geRectangleF BoxColliderF() { return geRectangleF{(float)BoxCollider.x, (float)BoxCollider.y, (float)BoxCollider.w, (float)BoxCollider.h}; }
};
}  // namespace Bba