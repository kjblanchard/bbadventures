#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/InteractorComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
#include <BbAdventures/components/PlayerExitComponent.hpp>
#include <BbAdventures/components/PlayerSpawnComponent.hpp>
#include <BbAdventures/components/RigidBodyComponent.hpp>
#include <BbAdventures/components/SolidObjectComponent.hpp>
#include <BbAdventures/components/TextInteractionComponent.hpp>
#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/ui/Panel.hpp>
#include <BbAdventures/ui/Textbox.hpp>

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
	if (State::IsLoadingMap) {
		return;
	}
	auto view = GameObject::_registry.view<PlayerComponent>();
	for (auto&& [e, p] : view.each()) {
		auto go = GameObject(e);
		auto& l = go.GetComponent<LocationComponent>();
		auto& a = go.GetComponent<AnimationComponent>();
		auto& r = go.GetComponent<RigidBodyComponent>();
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

		for (auto&& [_, pe] : peView.each()) {
			if (geRectangleIsOverlap(&playerRbRect, &pe.BoundingBox)) {
				State::IsLoadingMap = true;
				State::NextMapName = pe.NextMap;
				State::SpawnLocation = pe.SpawnLocationId;
				// Trigger fadeout
				State::FadePanel->FadeOut(Level::LoadNewLevel);
			}
		}
		// Check if we should interact
		if (geKeyJustPressed(geKey_SPACE) && go.HasComponent<InteractorComponent>()) {
			// If we are displaying text, close it.
			if (State::TextDisplay->Text) {
				State::TextDisplay->Text = nullptr;
				break;
			}
			auto& i = go.GetComponent<InteractorComponent>();
			// Check to see if we are interacting
			auto tView = GameObject::_registry.view<LocationComponent, TextInteractionComponent>();
			for (auto&& [_, li, ti] : tView.each()) {
				gePoint p = {(int)li.Location.x, (int)li.Location.y};
				auto ir = geRectangle{(int)l.Location.x + i.Box.x, (int)l.Location.y + i.Box.y, i.Box.w, i.Box.h};

				if (geUtilsIsPointInRect(&ir, &p)) {
					State::TextDisplay->DisplayText(ti.TextImage);
				}
			}
		}
	}
}

void LoadPlayers() {
	auto view = GameObject::_registry.view<PlayerSpawnComponent>();
	for (auto&& [_, ps] : view.each()) {
		if (ps.SpawnLocationId == State::SpawnLocation) {
			for (size_t i = 0; i < State::NumPlayers; i++) {
				auto go = new GameObject();
				LocationComponent l = LocationComponent();
				l.Location.x = ps.Location.x + (i * 5);
				l.Location.y = ps.Location.y;
				PlayerComponent p = PlayerComponent();
				p.Direction = Directions::South;
				auto a = AnimationComponent();
				a.AnimationName = "player";
				a.Offset = gePoint{0, 0};
				auto r = RigidBodyComponent();
				r.OffsetX = 2;
				r.OffsetY = 20;
				r.W = 16;
				r.H = 14;
				auto ic = InteractorComponent();
				ic.Box = geRectangle{-14, -14, 40, 40};
				go->AddComponent<RigidBodyComponent>(r);
				go->AddComponent<LocationComponent>(l);
				go->AddComponent<PlayerComponent>(p);
				go->AddComponent<AnimationComponent>(a);
				go->AddComponent<InteractorComponent>(ic);
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