#include <BbAdventures/tween/Tween.hpp>
#include <BbAdventures/ui/Panel.hpp>
#include <memory>
using namespace Bba;
static std::function<void(void)> _afterFunc;
static std::unique_ptr<Tween> _tween;
Panel::Panel() : Alpha(0) {
	auto c = geColor{0, 0, 0, 0};
	Image = geImageNewRenderTarget("fadeScreen", 1024, 576, &c);
}
void Panel::FadeOut(std::function<void(void)> afterFunc) {
	_afterFunc = afterFunc;
	_tween = std::make_unique<Tween>(0, 255, 0.25, Easings::Linear);
}

void Panel::FadeIn(std::function<void(void)> afterFunc) {
	_afterFunc = afterFunc;
	_tween = std::make_unique<Tween>(255, 0, 0.25, Easings::Linear);
	puts("Fade in done");
}

void Panel::Update() {
	// If there is a tween, lets update it.
	if (_tween) {
		auto completed = _tween->Update();
		if (completed) {
			Alpha = (int)_tween->Value();
			geImageSetAlpha(Image, Alpha);
			_tween = nullptr;

			// Delete the tween and perform the function
			if (_afterFunc) {
				_afterFunc();
			}
			return;
		}
		Alpha = (int)_tween->Value();
		geImageSetAlpha(Image, Alpha);
	}
}

void Panel::Draw() {
	auto r = geRectangle{0, 0, 1024, 576};
	geImageDraw(Image, nullptr, &r);
}
