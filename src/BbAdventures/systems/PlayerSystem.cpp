#include <GoonEngine/content/sfx.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/AnimationComponent.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
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

static geSfx* sfx = nullptr;

const geRectangle interactionRect = {-14, -14, 30, 30};
const int moveSpeed = 100;
namespace Bba {

static Directions getOverlapDirection(geRectangle* obj, geRectangle* overlapBox) {
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

static bool handleMovement(Directions& d, geVec2& m) {
	if (State::IsDisplayingText) {
		return false;
	}
	bool moved = false;
	if (geKeyJustPressed(geKey_W) || geKeyHeldDown(geKey_W)) {
		d = Directions::North;
		moved = true;
		m.y -= moveSpeed * State::DeltaTime;
	}
	if (geKeyJustPressed(geKey_A) || geKeyHeldDown(geKey_A)) {
		d = Directions::West;
		moved = true;
		m.x -= moveSpeed * State::DeltaTime;
	}
	if (geKeyJustPressed(geKey_S) || geKeyHeldDown(geKey_S)) {
		d = Directions::South;
		moved = true;
		m.y += moveSpeed * State::DeltaTime;
	}
	if (geKeyJustPressed(geKey_D) || geKeyHeldDown(geKey_D)) {
		d = Directions::East;
		moved = true;
		m.x += moveSpeed * State::DeltaTime;
	}
	return moved;
}

static void updatePlayersEach(GameObject go, PlayerComponent& p) {
	auto& l = go.GetComponent<LocationComponent>();
	auto& a = go.GetComponent<AnimationComponent>();
	auto& r = go.GetComponent<RigidBodyComponent>();
	auto& i = go.GetComponent<InteractorComponent>();
	auto d = p.Direction;
	auto tryMoveSpeed = geVec2{0, 0};
	auto moved = handleMovement(d, tryMoveSpeed);
	// Check if we can move
	auto playerRbRect = r.GetRect();
	playerRbRect.x += l.Location.x + tryMoveSpeed.x;
	playerRbRect.y += l.Location.y + tryMoveSpeed.y;
	bool collision = false;
	GameObject::ForEach<SolidObjectComponent>([&l, &tryMoveSpeed, &collision, &playerRbRect](GameObject g, SolidObjectComponent s) {
		// auto sView = GameObject::_registry.view<SolidObjectComponent>();
		// for (auto [_, s] : sView.each()) {
		if (geRectangleIsOverlap(&playerRbRect, &s.BoxCollider)) {
			auto r = geRectangleGetOverlapRect(&playerRbRect, &s.BoxCollider);
			auto d = getOverlapDirection(&playerRbRect, &r);
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
	});
	if (!collision) {
		l.Location.x += tryMoveSpeed.x;
		l.Location.y += tryMoveSpeed.y;
	}
	a.Playing = moved;
	if (d != p.Direction) {
		// Update the Animation for the new direction
		auto letter = GetLetterForDirection(d);
		a.Animation->PlayAnimation("walk" + std::string(letter));
		p.Direction = d;
		// Update the interaction rect
		// int iOffsetX = 0, iOffsetY = 0;
		auto& dd = go.GetComponent<DebugDrawComponent>();
		switch (d) {
			case Directions::East:
				// i.Box.x = interactionRect.x - r.W + interactionRect.w;
				i.Box.x = r.W;
				i.Box.y = r.H + interactionRect.y;
				break;
			case Directions::West:
				i.Box.x = -r.W + interactionRect.x;
				i.Box.y = r.H + interactionRect.y;
				break;
			case Directions::North:
				// i.Box.y = r.H - interactionRect.y;
				// i.Box.y = interactionRect.y + r.H + r.OffsetY - interactionRect.h;
				i.Box.y = (interactionRect.y + r.OffsetY - interactionRect.h) + r.H;
				i.Box.x = r.W + interactionRect.x;
				break;
			case Directions::South:
				i.Box.y = r.H - interactionRect.y;
				i.Box.x = r.W + interactionRect.x;
				break;
				}
		dd.Box.x = i.Box.x;
		dd.Box.y = i.Box.y;
// 		switch (d) {
//     case Directions::East:
//         // Set x and y for East direction
//         i.Box.x = r.W + interactionRect.x;  // Adjust x based on the width of r and starting x of interactionRect
//         i.Box.y = interactionRect.y;         // Set y starting at the y position of interactionRect
//         break;

//     case Directions::West:
//         // Set x and y for West direction
//         i.Box.x = interactionRect.x - r.W;  // Adjust x by subtracting the width of r from the starting x of interactionRect
//         i.Box.y = interactionRect.y;        // Set y starting at the y position of interactionRect
//         break;

//     case Directions::North:
//         // Set x and y for North direction
//         i.Box.x = interactionRect.x;                    // Set x starting at the x position of interactionRect
//         i.Box.y = interactionRect.y + r.H - interactionRect.h;  // Adjust y to account for the height of r and the offset in interactionRect
//         break;

//     case Directions::South:
//         // Set x and y for South direction
//         i.Box.x = interactionRect.x;                    // Set x starting at the x position of interactionRect
//         i.Box.y = interactionRect.y + r.H;             // Adjust y to be below the starting y of interactionRect by the height of r
//         break;
// }

// // Copy the box position to dd.Box
// dd.Box.x = i.Box.x;
// dd.Box.y = i.Box.y;

		// switch (d) {
		// 	case Directions::East:
		// 		i.Box.x = r.W + interactionRect.x;
		// 		i.Box.y = interactionRect.y;
		// 		break;
		// 	case Directions::West:
		// 		i.Box.x = interactionRect.x - r.W;
		// 		i.Box.y = interactionRect.y;
		// 		break;
		// 	case Directions::North:
		// 		i.Box.x = interactionRect.x;
		// 		i.Box.y = interactionRect.y + r.H - interactionRect.h;
		// 		break;
		// 	case Directions::South:
		// 		i.Box.x = interactionRect.x;
		// 		i.Box.y = interactionRect.y + r.H;
		// 		break;
		// }

		// dd.Box.x = i.Box.x;
		// dd.Box.y = i.Box.y;
	}
	// Check if we overlapped with a exit after moving
	playerRbRect = r.GetRect();
	playerRbRect.x += l.Location.x + tryMoveSpeed.x;
	playerRbRect.y += l.Location.y + tryMoveSpeed.y;
	GameObject::ForEach<PlayerExitComponent>([&playerRbRect](GameObject g, PlayerExitComponent pe) {
		if (geRectangleIsOverlap(&playerRbRect, &pe.BoundingBox)) {
			if (!sfx) {
				sfx = geSfxNew("transition");
				geSfxLoad(sfx);
			}
			geSfxPlay(sfx, 1.0, 1);
			State::IsLoadingMap = true;
			State::NextMapName = pe.NextMap;
			State::SpawnLocation = pe.SpawnLocationId;
			// Trigger fadeout
			State::FadePanel->FadeOut(Level::LoadNewLevel);
		}
	});
	// Check if we should interact
	if (geKeyJustPressed(geKey_SPACE) && go.HasComponent<InteractorComponent>()) {
		// If we are displaying text, close it.
		if (State::TextDisplay->Text) {
			State::TextDisplay->UnDisplayText();
			State::IsDisplayingText = false;
			return;
		}

		// Check to see if we are interacting
		GameObject::ForEach<LocationComponent, TextInteractionComponent>([&l, &i](GameObject g, LocationComponent li, TextInteractionComponent ti) {
			gePoint p = {(int)li.Location.x, (int)li.Location.y};
			auto ir = geRectangle{(int)l.Location.x + i.Box.x, (int)l.Location.y + i.Box.y, i.Box.w, i.Box.h};

			if (geUtilsIsPointInRect(&ir, &p)) {
				// Start the sound
				State::IsDisplayingText = true;
				State::TextDisplay->DisplayText(ti.TextImage);
			}
		});
	}
}

static void loadPlayerEach(GameObject g, PlayerSpawnComponent& ps) {
	if (ps.SpawnLocationId != State::SpawnLocation) {
		return;
	}
	for (size_t i = 0; i < State::NumPlayers; i++) {
		auto go = new GameObject();
		LocationComponent l = LocationComponent();
		l.Location.x = ps.Location.x + (i * 5);
		l.Location.y = ps.Location.y;
		PlayerComponent p = PlayerComponent();
		p.Direction = ps.SpawnDirection;
		auto a = AnimationComponent();
		a.AnimationName = "player";
		a.Offset = gePoint{0, 0};
		auto r = RigidBodyComponent();
		r.OffsetX = 8;
		r.OffsetY = 20;
		r.W = 10;
		r.H = 14;
		auto ic = InteractorComponent();
		ic.Box = interactionRect;
		auto dd = DebugDrawComponent();
		dd.Box = interactionRect;
		go->AddComponent<RigidBodyComponent>(r);
		go->AddComponent<LocationComponent>(l);
		go->AddComponent<PlayerComponent>(p);
		go->AddComponent<AnimationComponent>(a);
		go->AddComponent<InteractorComponent>(ic);
		go->AddComponent<DebugDrawComponent>(dd);
		if (State::CurrentLevel) {
			State::CurrentLevel->AddGameObjectToLevel(go);
			return;
		}
		LogWarn("Somehow couldn't add player, current level doesn't exist?");
	}
}
static void startPlayersEach(GameObject g, PlayerComponent p) {
	auto a = g.GetComponent<AnimationComponent>();
	auto letter = GetLetterForDirection(p.Direction);
	a.Animation->PlayAnimation("walk" + std::string(letter));
}

void UpdatePlayers() {
	if (State::IsLoadingMap) {
		return;
	}
	GameObject::ForEach<PlayerComponent>(updatePlayersEach);
}

void LoadPlayers() {
	GameObject::ForEach<PlayerSpawnComponent>(loadPlayerEach);
}

void StartPlayers() {
	GameObject::ForEach<PlayerComponent>(startPlayersEach);
}
}  // namespace Bba