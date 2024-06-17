#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/SolidObjectComponent.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/entities/Solid.hpp>
namespace Bba {
    GameObject* NewSolidObject(geRectangle& r) {
	auto go = new GameObject();
	auto s = SolidObjectComponent();
    s.BoxCollider.x = r.x;
    s.BoxCollider.y = r.y;
    s.BoxCollider.w = r.w;
    s.BoxCollider.h = r.h;
    auto l = LocationComponent();
    l.Location.x = s.BoxCollider.x;
    l.Location.y = s.BoxCollider.y;
	go->AddComponent<SolidObjectComponent>(s);
	go->AddComponent<LocationComponent>(l);
	return go;
    }

GameObject* NewSolidObject(TiledMap::TiledObject& t) {
	auto go = new GameObject();
	auto s = SolidObjectComponent();
    s.BoxCollider.x = t.X;
    s.BoxCollider.y = t.Y;
    s.BoxCollider.w = t.Width;
    s.BoxCollider.h = t.Height;
    // auto d = DebugDrawComponent();
    // d.Box.x = 0;
    // d.Box.y = 0;
    // d.Box.w = s.BoxCollider.w;
    // d.Box.h = s.BoxCollider.h;
    auto l = LocationComponent();
    l.Location.x = s.BoxCollider.x;
    l.Location.y = s.BoxCollider.y;
	go->AddComponent<SolidObjectComponent>(s);
	go->AddComponent<LocationComponent>(l);
	// go->AddComponent<DebugDrawComponent>(d);
	return go;
}

}  // namespace Bba