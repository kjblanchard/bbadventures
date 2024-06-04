#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>

#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
namespace Bba {
void UpdatePlayers() {
	auto view = GameObject::_registry.view<LocationComponent, PlayerComponent, AnimationComponent>();
	for (auto [_, l, p, a] : view.each()) {
		auto d = p.Direction;
		std::string letter = "";
		auto moved = false;
		if (geKeyJustPressed(geKey_W) || geKeyHeldDown(geKey_W)) {
			d = Directions::North;
			moved = true;
			letter = 'U';
			--l.Location.y;
		}
		if (geKeyJustPressed(geKey_A) || geKeyHeldDown(geKey_A)) {
			d = Directions::West;
			moved = true;
			letter = 'L';
			--l.Location.x;
		}
		if (geKeyJustPressed(geKey_S) || geKeyHeldDown(geKey_S)) {
			d = Directions::South;
			moved = true;
			letter = 'D';
			++l.Location.y;
		}
		if (geKeyJustPressed(geKey_D) || geKeyHeldDown(geKey_D)) {
			d = Directions::East;
			moved = true;
			letter = 'R';
			++l.Location.x;
		}
		a.Playing = moved;
		if (d != p.Direction) {
			a.Animation->PlayAnimation("walk" + letter);
			p.Direction = d;
		}
	}
}

void LoadPlayers() {
}

}  // namespace Bba