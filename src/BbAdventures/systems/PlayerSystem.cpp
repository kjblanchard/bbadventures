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

static geSfx* _sfx = nullptr;
static const geRectangle _interactionRect = {-14, -14, 20, 20};
static const int _moveSpeed = 100;

namespace Bba {

static bool handleMovement(Directions& d, geVec2& m, AnimationComponent& a) {
	if (State::IsDisplayingText) {
		return false;
	}
	bool moved = false;
	auto moveSpeed = _moveSpeed;
	bool run = false;
	a.AnimationSpeed = 1.0;
	if (geKeyJustPressed(geKey_LSHIFT) || geKeyHeldDown(geKey_LSHIFT)) {
		const int multiplier = 2.0;
		moveSpeed *= multiplier;
		a.AnimationSpeed = multiplier;
	}
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
	auto moved = handleMovement(d, tryMoveSpeed, a);
	// Check if we can move
	auto playerRbRect = r.GetRectF();
	playerRbRect.x += l.Location.x + tryMoveSpeed.x;
	playerRbRect.y += l.Location.y + tryMoveSpeed.y;
	geVec2 desiredPosition = {l.Location.x + tryMoveSpeed.x, l.Location.y + tryMoveSpeed.y};
	bool collision = false;
	GameObject::ForEach<SolidObjectComponent>([&collision, &playerRbRect, &desiredPosition](GameObject, SolidObjectComponent s) {
		auto bcf = s.BoxColliderF();
		if (geRectangleFIsOverlap(&playerRbRect, &bcf)) {
			auto r = geRectangleFGetOverlapRect(&playerRbRect, &bcf);
			auto d = GetOverlapDirectionF(&playerRbRect, &r);
			switch (d) {
				case Directions::North:
					desiredPosition.y += r.h;
					collision = true;
					break;
				case Directions::East:
					desiredPosition.x -= r.w;
					collision = true;
					break;
				case Directions::South:
					desiredPosition.y -= r.h;
					collision = true;
					break;
				case Directions::West:
					desiredPosition.x += r.w;
					collision = true;
					break;
				default:
					break;
			}
		}
	});
	l.Location.x = desiredPosition.x;
	l.Location.y = desiredPosition.y;
	a.Playing = moved;
	if (d != p.Direction) {
		// Update the Animation for the new direction
		auto letter = GetLetterForDirection(d);
		a.Animation->PlayAnimation("walk" + std::string(letter));
		p.Direction = d;
		// Update the interaction rect
		// auto& dd = go.GetComponent<DebugDrawComponent>(); //Update debug rect if needed
		switch (d) {
			case Directions::East:
				i.Box.x = r.W;
				i.Box.y = r.H / 2;
				break;
			case Directions::West:
				i.Box.x = r.W - _interactionRect.w;
				i.Box.y = r.H / 2;
				break;
			case Directions::North:
				i.Box.y = (_interactionRect.y + r.OffsetY - _interactionRect.h) + r.H;
				i.Box.x = r.W + (_interactionRect.x / 2);
				break;
			case Directions::South:
				i.Box.y = r.H - _interactionRect.y;
				i.Box.x = r.W + (_interactionRect.x / 2);
				break;
		}
	}
	// Check if we overlapped with a exit after moving
	playerRbRect = r.GetRectF();
	playerRbRect.x += l.Location.x + tryMoveSpeed.x;
	playerRbRect.y += l.Location.y + tryMoveSpeed.y;
	GameObject::ForEach<PlayerExitComponent>([&playerRbRect](GameObject, PlayerExitComponent pe) {
		auto pebf = geRectangleF{(float)pe.BoundingBox.x, (float)pe.BoundingBox.y, (float)pe.BoundingBox.w, (float)pe.BoundingBox.h};
		if (geRectangleFIsOverlap(&playerRbRect, &pebf)) {
			if (!_sfx) {
				_sfx = geSfxNew("transition");
				geSfxLoad(_sfx);
			}
			geSfxPlay(_sfx, 1.0);
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
			State::TextDisplay->Interact(State::TextDisplay->Text);
			return;
		}

		// Check to see if we are interacting
		GameObject::ForEach<LocationComponent, TextInteractionComponent>([&l, &i](GameObject, LocationComponent li, TextInteractionComponent ti) {
			auto interactionRect = geRectangle{(int)li.Location.x, (int)li.Location.y, ti.Size.x, ti.Size.y};
			auto ir = geRectangle{(int)l.Location.x + i.Box.x, (int)l.Location.y + i.Box.y, i.Box.w, i.Box.h};

			if (geRectangleIsOverlap(&ir, &interactionRect)) {
				State::TextDisplay->Interact(ti.TextImage);
			}
		});
	}
}

static void loadPlayerEach(GameObject, PlayerSpawnComponent& ps) {
	if (ps.SpawnLocationId != State::SpawnLocation) {
		return;
	}
	for (size_t i = 0; i < (unsigned int)State::NumPlayers; i++) {
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
		ic.Box = _interactionRect;
		auto dd = DebugDrawComponent();
		dd.Box = _interactionRect;
		go->AddComponent<RigidBodyComponent>(r);
		go->AddComponent<LocationComponent>(l);
		go->AddComponent<PlayerComponent>(p);
		go->AddComponent<AnimationComponent>(a);
		go->AddComponent<InteractorComponent>(ic);
		// go->AddComponent<DebugDrawComponent>(dd);
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