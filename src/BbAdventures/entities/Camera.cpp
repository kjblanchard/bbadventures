#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/shared/constants.hpp>
#include <BbAdventures/components/CameraComponent.hpp>
#include <BbAdventures/entities/Camera.hpp>
namespace Bba {

GameObject* NewCamera() {
	auto go = new GameObject();
	CameraComponent c = CameraComponent();
	c.Box.x = 0;
	c.Box.y = 0;
    c.Box.w = SCREEN_WIDTH;
    c.Box.h = SCREEN_HEIGHT;
    c.Bounds.x = 0;
    c.Bounds.y = 0;
	go->AddComponent<CameraComponent>(c);
	return go;
}
// 	auto go = new GameObject();
// 	LocationComponent l = LocationComponent();
// 	l.Location.x = t.X;
// 	l.Location.y = t.Y;
// 	PlayerComponent p = PlayerComponent();
// 	p.Direction = Directions::South;
// 	auto d = DebugDrawComponent();
// 	d.Box.x = 2;
// 	d.Box.y = 6;
// 	d.Box.w = 16;
// 	d.Box.h = 28;
// 	auto a = AnimationComponent();
// 	a.AnimationName = "player";
// 	a.Offset = gePoint{0, 0};
// 	auto r = RigidBodyComponent();
// 	r.OffsetX = 2;
// 	r.OffsetY = 6;
// 	r.W = 16;
// 	r.H = 28;
// 	auto i = InteractorComponent();
// 	i.Box = geRectangle{-14, -14, 40, 40};

// 	go->AddComponent<DebugDrawComponent>(d);
// 	go->AddComponent<RigidBodyComponent>(r);
// 	go->AddComponent<LocationComponent>(l);
// 	go->AddComponent<PlayerComponent>(p);
// 	go->AddComponent<AnimationComponent>(a);
// 	go->AddComponent<InteractorComponent>(i);

// 	if (go->HasComponent<InteractorComponent>()) {
// 		LogWarn("InteractorComponent successfully added to player.");
// 	} else {
// 		LogWarn("Failed to add InteractorComponent to player.");
// 	}
// 	return go;
// }

}  // namespace Bba