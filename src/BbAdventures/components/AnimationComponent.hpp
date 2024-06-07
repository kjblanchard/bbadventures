#pragma once
#include <GoonEngine/content/image.h>
#include <GoonEngine/prim/point.h>
#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <memory>

#include <string>
namespace Bba {
struct AnimationComponent {
	// std::shared_ptr<AsepriteAnimation> Animation;
	std::unique_ptr<AsepriteAnimation> Animation;
	std::string AnimationName;
	geImage* AnimationImage;
	gePoint Offset;
	bool Playing;

};
}  // namespace Bba