#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
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
	d.Box.x = 0;
	d.Box.y = 0;
	d.Box.w = t.Width;
	d.Box.h = t.Height;
	auto a = AnimationComponent();
	a.AnimationName = "player";
	a.Offset = gePoint{0,0};

	go->AddComponent<DebugDrawComponent>(d);
	go->AddComponent<LocationComponent>(l);
	go->AddComponent<PlayerComponent>(p);
	go->AddComponent<AnimationComponent>(a);
	return go;
}

}  // namespace Bba