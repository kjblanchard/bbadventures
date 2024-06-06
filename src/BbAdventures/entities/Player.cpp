#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
#include <BbAdventures/components/RigidBodyComponent.hpp>
#include <BbAdventures/entities/Player.hpp>
namespace Bba {

GameObject* NewPlayer(TiledMap::TiledObject& t) {
	auto go = new GameObject();
	LocationComponent l = LocationComponent();
	l.Location.x = t.X;
	l.Location.y = t.Y;
	PlayerComponent p = PlayerComponent();
	p.Direction = Directions::South;
	auto d = DebugDrawComponent();
	d.Box.x = 2;
	d.Box.y = 6;
	d.Box.w = 16;
	d.Box.h = 28;
	auto a = AnimationComponent();
	a.AnimationName = "player";
	a.Offset = gePoint{0, 0};
	auto r = RigidBodyComponent();
	r.OffsetX = 2;
	r.OffsetY = 6;
	r.W = 16;
	r.H = 28;

	go->AddComponent<DebugDrawComponent>(d);
	go->AddComponent<RigidBodyComponent>(r);
	go->AddComponent<LocationComponent>(l);
	go->AddComponent<PlayerComponent>(p);
	go->AddComponent<AnimationComponent>(a);
	return go;
}

}  // namespace Bba