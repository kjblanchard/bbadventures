#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>

#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
#include <BbAdventures/components/RigidBodyComponent.hpp>
#include <BbAdventures/components/SolidObjectComponent.hpp>
#include <BbAdventures/shared/state.hpp>

// static Directions:: GetOverlapDirection(box1, overlapArea *BoundingBox) Direction {
// 	if overlapArea.Width < overlapArea.Height {
// 		if overlapArea.X > box1.X {
// 			return East
// 		}
// 		return West
// 	}
// 	if overlapArea.Y > box1.Y {
// 		return South
// 	}
// 	return North
// }

const int moveSpeed = 100;
namespace Bba {
static Directions GetOverlapDirection(geRectangle* obj, geRectangle* overlapBox) {
	if (overlapBox->w < overlapBox->h) {
		if (overlapBox->x > obj->x) {
			return Directions::East;
		}
		return Directions::West;
	}
	if (overlapBox->y > obj->y) {
		return Directions::South;
	}
	return Directions::North;
}

void UpdatePlayers() {
	auto view = GameObject::_registry.view<LocationComponent, PlayerComponent, AnimationComponent, RigidBodyComponent>();
	for (auto [_, l, p, a, r] : view.each()) {
		auto d = p.Direction;
		std::string letter = "";
		auto moved = false;
		auto tryMoveSpeed = geVec2{0, 0};
		if (geKeyJustPressed(geKey_W) || geKeyHeldDown(geKey_W)) {
			d = Directions::North;
			moved = true;
			letter = 'U';
			tryMoveSpeed.y -= moveSpeed * State::DeltaTime;
		}
		if (geKeyJustPressed(geKey_A) || geKeyHeldDown(geKey_A)) {
			d = Directions::West;
			moved = true;
			letter = 'L';
			tryMoveSpeed.x -= moveSpeed * State::DeltaTime;
		}
		if (geKeyJustPressed(geKey_S) || geKeyHeldDown(geKey_S)) {
			d = Directions::South;
			moved = true;
			letter = 'D';
			tryMoveSpeed.y += moveSpeed * State::DeltaTime;
		}
		if (geKeyJustPressed(geKey_D) || geKeyHeldDown(geKey_D)) {
			d = Directions::East;
			moved = true;
			letter = 'R';
			tryMoveSpeed.x += moveSpeed * State::DeltaTime;
		}
		// Check if we can move
		auto view = GameObject::_registry.view<SolidObjectComponent>();
		auto playerRbRect = r.GetRect();
		playerRbRect.x += l.Location.x + tryMoveSpeed.x;
		playerRbRect.y += l.Location.y + tryMoveSpeed.y;
		bool collision = false;
		for (auto [_, s] : view.each()) {
			if (geRectangleIsOverlap(&playerRbRect, &s.BoxCollider)) {
				auto r = geRectangleGetOverlapRect(&playerRbRect, &s.BoxCollider);
				auto d = GetOverlapDirection(&playerRbRect, &r);
				switch (d) {
					case Directions::North:
						l.Location.y += tryMoveSpeed.y += r.h;
						l.Location.x += tryMoveSpeed.x;
						collision = true;
						break;
					case Directions::East:
						l.Location.x += tryMoveSpeed.x -= r.w;
						l.Location.y += tryMoveSpeed.y;
						collision = true;

						break;
					case Directions::South:
						l.Location.y += tryMoveSpeed.y -= r.h;
						l.Location.x += tryMoveSpeed.x;
						collision = true;
						break;

					case Directions::West:
						l.Location.x += tryMoveSpeed.x += r.w;
						l.Location.y += tryMoveSpeed.y;
						collision = true;
						break;
					default:
						break;
				}
			}
		}
		if (!collision) {
			l.Location.x += tryMoveSpeed.x;
			l.Location.y += tryMoveSpeed.y;
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