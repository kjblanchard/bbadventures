#pragma once
#include <GoonEngine/content/image.h>
#include <GoonEngine/prim/point.h>

#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <memory>
#include <string>
namespace Bba {
struct AnimationComponent {
	AsepriteAnimation* Animation;
	std::string AnimationName;
	float AnimationSpeed;
	geImage* AnimationImage;
	gePoint Offset;
	bool Playing;
};
}  // namespace Bba