#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/PlayerExitComponent.hpp>
#include <BbAdventures/entities/PlayerExit.hpp>
#include <BbAdventures/shared/state.hpp>
namespace Bba {

GameObject* NewPlayerExit(TiledMap::TiledObject& obj) {
	auto go = new GameObject();
	auto p = PlayerExitComponent();
	p.BoundingBox.x = obj.X;
	p.BoundingBox.y = obj.Y;
	p.BoundingBox.w = obj.Width;
	p.BoundingBox.h = obj.Height;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "loadLocation") {
			p.SpawnLocationId = std::get<int>(prop.Value);
		}
		if (prop.Name == "loadMap") {
			p.NextMap = std::get<std::string>(prop.Value);
		}
	}
	go->AddComponent<PlayerExitComponent>(p);
	return go;
}

}  // namespace Bba
