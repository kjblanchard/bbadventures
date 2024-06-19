#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/CameraComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/PlayerComponent.hpp>
#include <BbAdventures/shared/constants.hpp>
#include <BbAdventures/shared/state.hpp>
namespace Bba {

void getFollowTarget(CameraComponent& cc) {
	GameObject::ForEach<PlayerComponent, LocationComponent>([&cc](GameObject, PlayerComponent& pc, LocationComponent& lc) {
		if (pc.PlayerNum != 0) {
			return;
		}
		cc.FollowTarget = &lc;
	});
}

void UpdateCamera() {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	if (!c.has_value()) {
		return;
	}
	auto& cc = c->GetComponent<CameraComponent>();
	if (!cc.FollowTarget) {
		getFollowTarget(cc);
	}
	auto pl = cc.FollowTarget;
	cc.Box.x = pl->Location.x - (SCREEN_WIDTH / 2);
	cc.Box.y = pl->Location.y - (SCREEN_HEIGHT / 2);
	if (cc.Box.x < 0) {
		cc.Box.x = 0;
	} else if (cc.Box.x > cc.Bounds.x - SCREEN_WIDTH) {
		cc.Box.x = std::max(cc.Bounds.x - SCREEN_WIDTH, 0);
	}
	if (cc.Box.y < 0) {
		cc.Box.y = 0;
	} else if (cc.Box.y > (cc.Bounds.y - SCREEN_HEIGHT)) {
		cc.Box.y = cc.Bounds.y - SCREEN_HEIGHT;
	}
	State::CameraX = cc.Box.x;
	State::CameraY = cc.Box.y;
}
}  // namespace Bba