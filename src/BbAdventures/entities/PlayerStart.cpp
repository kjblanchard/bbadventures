#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/PlayerSpawnComponent.hpp>
#include <BbAdventures/entities/Player.hpp>
#include <BbAdventures/entities/PlayerStart.hpp>
#include <BbAdventures/shared/state.hpp>
namespace Bba {

GameObject* NewPlayerSpawn(TiledMap::TiledObject& obj) {
	auto go = new GameObject();
	auto p = PlayerSpawnComponent();
	p.Location.x = obj.X;
	p.Location.y = obj.Y;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "loadLocation") {
			p.SpawnLocationId = std::get<int>(prop.Value);
		}
	}
    go->AddComponent<PlayerSpawnComponent>(p);
	return go;
}

}  // namespace Bba
