
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/TextInteractionComponent.hpp>
#include <BbAdventures/entities/TextInteraction.hpp>
namespace Bba {

GameObject* NewTextInteraction(TiledMap::TiledObject& t) {
	auto go = new GameObject();
	LocationComponent l = LocationComponent();
	l.Location.x = t.X;
	l.Location.y = t.Y;
	auto ti = TextInteractionComponent();
	for (auto&& prop : t.Properties) {
		if (prop.Name == "text") {
			ti.Text = std::get<std::string>(prop.Value);
			break;
		}
	}
	go->AddComponent<LocationComponent>(l);
	go->AddComponent<TextInteractionComponent>(ti);
	return go;
}

}  // namespace Bba