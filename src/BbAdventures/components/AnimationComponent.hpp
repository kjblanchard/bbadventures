#pragma once
#include <GoonEngine/content/image.h>
#include <GoonEngine/prim/point.h>

#include <string>
namespace Bba {
class AsepriteAnimation;
struct AnimationComponent {
	AsepriteAnimation* Animation;
	std::string AnimationName;
	geImage* AnimationImage;
	gePoint Offset;
	bool Playing;
};
}  // namespace Bba