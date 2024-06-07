#pragma once
#include <GoonEngine/content/image.h>

#include <functional>
namespace Bba {
class Panel {
   public:
	Panel();
	void FadeOut(std::function<void(void)> afterFunc);
	void FadeIn(std::function<void(void)> afterFunc);
	void Update();
	void Draw();
	geImage* Image;
	int Alpha;
};

}  // namespace Bba
