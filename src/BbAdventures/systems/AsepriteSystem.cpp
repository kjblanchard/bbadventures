#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/shared/state.hpp>
#include <memory>
namespace Bba {
void LoadAnimationComponents() {
	auto view = GameObject::_registry.view<AnimationComponent>();
	for (auto&& [_, a] : view.each()) {
		a.Animation = std::make_unique<AsepriteAnimation>(a.AnimationName);
		a.AnimationImage = geImageNewFromFile(a.Animation->Filename().c_str());
	}
}
void UpdateAnimationComponents() {
	auto msTime = State::DeltaTime * 1000;
	auto view = GameObject::_registry.view<AnimationComponent>();
	for (auto&& [_, a] : view.each()) {
		if (!a.Playing) {
			continue;
		}
		a.Animation->UpdateAnimation(msTime);
	}
}

void DrawAnimationComponents() {
	auto view = GameObject::_registry.view<AnimationComponent, LocationComponent>();
	for (auto&& [_, a, l] : view.each()) {
		auto s = a.Animation->FrameCoords();
		auto d = geRectangle{(int)l.Location.x + a.Offset.x, (int)l.Location.y + a.Offset.y, s.w, s.h};
		geImageDraw(a.AnimationImage, &s, &d);
	}
}
}  // namespace Bba