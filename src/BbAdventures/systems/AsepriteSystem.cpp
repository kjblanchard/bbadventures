#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/shared/state.hpp>
#include <memory>
namespace Bba {
void LoadAnimationComponents() {
	GameObject::ForEach<AnimationComponent>([](GameObject g, AnimationComponent& a) {
		a.Animation = new AsepriteAnimation(a.AnimationName);
		a.AnimationImage = geImageNewFromFile(a.Animation->Filename().c_str());
	});
}

void UpdateAnimationComponents() {
	auto msTime = State::DeltaTime * 1000;
	GameObject::ForEach<AnimationComponent>([&msTime](GameObject g, AnimationComponent& a) {
		if (!a.Playing) {
			return;
		}
		a.Animation->UpdateAnimation(msTime);
	});
}

void DrawAnimationComponents() {
	GameObject::ForEach<AnimationComponent, LocationComponent>([](GameObject g, AnimationComponent& a, LocationComponent& l) {
		auto s = a.Animation->FrameCoords();
		auto d = geRectangle{(int)(l.Location.x + a.Offset.x) - State::CameraX, (int)(l.Location.y + a.Offset.y) - State::CameraY, s.w, s.h};
		geImageDraw(a.AnimationImage, &s, &d);
	});
}

void FreeAnimationComponents() {
	// auto view = GameObject::_registry.view<AnimationComponent>();
	GameObject::ForEach<AnimationComponent>([](GameObject g, AnimationComponent& a) {
		delete (a.Animation);
	});
}
}  // namespace Bba