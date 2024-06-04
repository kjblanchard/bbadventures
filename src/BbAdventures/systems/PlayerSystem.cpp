#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
namespace Bba {
void UpdatePlayers() {
	auto view = GameObject::_registry.view<LocationComponent, PlayerComponent>();
	for (auto [_, l, p] : view.each()) {
		if (geKeyJustPressed(geKey_W) || geKeyHeldDown(geKey_W)) {
			--l.Location.y;
		}
		if (geKeyJustPressed(geKey_A) || geKeyHeldDown(geKey_A)) {
			--l.Location.x;
		}
		if (geKeyJustPressed(geKey_S) || geKeyHeldDown(geKey_S)) {
			++l.Location.y;
		}
		if (geKeyJustPressed(geKey_D) || geKeyHeldDown(geKey_D)) {
			++l.Location.x;
		}
	}
}

}  // namespace Bba