#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>

#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
#include <BbAdventures/components/PlayerExitComponent.hpp>
#include <BbAdventures/components/PlayerSpawnComponent.hpp>
#include <BbAdventures/components/RigidBodyComponent.hpp>
#include <BbAdventures/components/SolidObjectComponent.hpp>
#include <BbAdventures/shared/state.hpp>

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
		auto sView = GameObject::_registry.view<SolidObjectComponent>();
		auto playerRbRect = r.GetRect();
		playerRbRect.x += l.Location.x + tryMoveSpeed.x;
		playerRbRect.y += l.Location.y + tryMoveSpeed.y;
		bool collision = false;
		for (auto [_, s] : sView.each()) {
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
		// Check if we overlapped with a exit after moving
		playerRbRect = r.GetRect();
		playerRbRect.x += l.Location.x + tryMoveSpeed.x;
		playerRbRect.y += l.Location.y + tryMoveSpeed.y;
		auto peView = GameObject::_registry.view<PlayerExitComponent>();
		for (auto [_, pe] : peView.each()) {
			if(geRectangleIsOverlap(&playerRbRect, &pe.BoundingBox)) {
				State::IsLoadingMap = true;
				State::NextMapName = pe.NextMap;
				State::SpawnLocation = pe.SpawnLocationId;
			}
		}
	}
}

void LoadPlayers() {
	auto view = GameObject::_registry.view<PlayerSpawnComponent>();
	for (auto [_, p] : view.each()) {
		if (p.SpawnLocationId == State::SpawnLocation) {
			for (size_t i = 0; i < State::NumPlayers; i++) {
				auto go = new GameObject();
				LocationComponent l = LocationComponent();
				l.Location.x = p.Location.x + (i * 5);
				l.Location.y = p.Location.y;
				PlayerComponent p = PlayerComponent();
				p.Direction = Directions::South;
				auto a = AnimationComponent();
				a.AnimationName = "player";
				a.Offset = gePoint{0, 0};
				auto r = RigidBodyComponent();
				r.OffsetX = 2;
				r.OffsetY = 6;
				r.W = 16;
				r.H = 28;
				go->AddComponent<RigidBodyComponent>(r);
				go->AddComponent<LocationComponent>(l);
				go->AddComponent<PlayerComponent>(p);
				go->AddComponent<AnimationComponent>(a);
				if (State::CurrentLevel) {
					State::CurrentLevel->AddGameObjectToLevel(go);
					continue;
				}
				LogWarn("Somehow couldn't add player, current level doesn't exist?");
				/* code */
			}
		}
	}
}

}  // namespace Bba