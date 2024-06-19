#include <GoonEngine/content/sfx.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/input/joystick.h>
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

static bool handleMovement(Directions& d, geVec2& m, AnimationComponent& a, int playerNum, int joystickNum) {
	if (State::IsDisplayingText) {
		return false;
	}
	bool moved = false;
	bool isRunning = false;
	auto moveSpeed = _moveSpeed;
	a.AnimationSpeed = 1.0;

	// If player is player 0, we should check keyboard and joystick, else just joystick.
	if (playerNum == 0 && (geKeyJustPressed(geKey_LSHIFT) || geKeyHeldDown(geKey_LSHIFT))) {
		isRunning = true;
	}
	if (joystickNum != JOYSTICK_DEFAULT && (geGamepadButtonJustPressed(joystickNum, geGameControllerButtonX) || geGamepadButtonHeldDown(joystickNum, geGameControllerButtonX))) {
		isRunning = true;
	}

	if (isRunning) {
		const int multiplier = 2.0;
		moveSpeed *= multiplier;
		a.AnimationSpeed = multiplier;
	}

	if (joystickNum != JOYSTICK_DEFAULT) {
		auto xStick = geGamepadLeftAxisXFloat(joystickNum);
		auto yStick = geGamepadLeftAxisYFloat(joystickNum);
		if (fabs(xStick) > fabs(yStick)) {
			if (xStick > 0.1) {
				d = Directions::East;
			} else if (xStick < -0.1) {
				d = Directions::West;
			}
		} else {
			if (yStick > 0.1) {
				d = Directions::South;
			} else if (yStick < -0.1) {
				d = Directions::North;
			}
		}
		// Check for joystick movement
		if (xStick || yStick) {
			m.x += moveSpeed * xStick * State::DeltaTime;
			m.y += moveSpeed * yStick * State::DeltaTime;
			moved = true;
		}
	}

	if (!moved) {
		if ((playerNum == 0 && (geKeyJustPressed(geKey_W) || geKeyHeldDown(geKey_W))) || (joystickNum != JOYSTICK_DEFAULT && (geGamepadButtonHeldDown(joystickNum, geGameControllerButtonDPAD_UP) || geGamepadButtonJustPressed(joystickNum, geGameControllerButtonDPAD_UP)))) {
			d = Directions::North;
			moved = true;
			m.y -= moveSpeed * State::DeltaTime;
		}
		if ((playerNum == 0 && (geKeyJustPressed(geKey_A) || geKeyHeldDown(geKey_A))) || (joystickNum != JOYSTICK_DEFAULT && (geGamepadButtonHeldDown(joystickNum, geGameControllerButtonDPAD_LEFT) || geGamepadButtonJustPressed(joystickNum, geGameControllerButtonDPAD_LEFT)))) {
			d = Directions::West;
			moved = true;
			m.x -= moveSpeed * State::DeltaTime;
		}
		if ((playerNum == 0 && (geKeyJustPressed(geKey_S) || geKeyHeldDown(geKey_S))) || (joystickNum != JOYSTICK_DEFAULT && (geGamepadButtonHeldDown(joystickNum, geGameControllerButtonDPAD_DOWN) || geGamepadButtonJustPressed(joystickNum, geGameControllerButtonDPAD_DOWN)))) {
			d = Directions::South;
			moved = true;
			m.y += moveSpeed * State::DeltaTime;
		}
		if ((playerNum == 0 && (geKeyJustPressed(geKey_D) || geKeyHeldDown(geKey_D))) || (joystickNum != JOYSTICK_DEFAULT && (geGamepadButtonHeldDown(joystickNum, geGameControllerButtonDPAD_RIGHT) || geGamepadButtonJustPressed(joystickNum, geGameControllerButtonDPAD_RIGHT)))) {
			d = Directions::East;
			moved = true;
			m.x += moveSpeed * State::DeltaTime;
		}
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
	auto moved = handleMovement(d, tryMoveSpeed, a, p.PlayerNum, p.ControllerNum);
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
	if (((p.PlayerNum == 0 && geKeyJustPressed(geKey_SPACE)) || (p.ControllerNum != JOYSTICK_DEFAULT && geGamepadButtonJustPressed(p.ControllerNum, geGameControllerButtonA))) && go.HasComponent<InteractorComponent>()) {
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
		if (!State::CurrentLevel) {
			LogWarn("Somehow couldn't add player, current level doesn't exist?");
			return;
		}
		auto go = new GameObject();
		LocationComponent l = LocationComponent();
		l.Location.x = ps.Location.x + (i * 5);
		l.Location.y = ps.Location.y;
		PlayerComponent p = PlayerComponent();
		p.Direction = ps.SpawnDirection;
		p.PlayerNum = i;
		p.ControllerNum = State::PlayerControllerMap[p.PlayerNum];
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
		State::CurrentLevel->AddGameObjectToLevel(go);
	}
}
static void startPlayersEach(GameObject g, PlayerComponent p) {
	auto a = g.GetComponent<AnimationComponent>();
	auto letter = GetLetterForDirection(p.Direction);
	a.Animation->PlayAnimation("walk" + std::string(letter));
}
static void updatePlayerJoystickEach(GameObject go, PlayerComponent& p) {
	if (p.ControllerNum == JOYSTICK_DEFAULT) {
		int firstPadPressed;
		if (geGamepadButtonJustPressedAnyPad(geGameControllerButtonSTART, &firstPadPressed)) {
			LogWarn("Assigning player pad num %d", firstPadPressed);
			p.ControllerNum = firstPadPressed;
			State::PlayerControllerMap[p.PlayerNum] = firstPadPressed;
		}
	}
}

void UpdatePlayerJoysticks() {
	// We should check to see if there are any players that don't have an assigned joystick
	GameObject::ForEach<PlayerComponent>(updatePlayerJoystickEach);

	// Check to see if there are any controllers that are not assigned to players and players are less than 2
	// Check to see if any of the unassigned controllers are pressing the start button
	// We should create the second player and assign the controller if so.
	// Camera should only follow player 1
	// Can not walk outside of camera bounds.
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