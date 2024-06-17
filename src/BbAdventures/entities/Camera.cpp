#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/shared/constants.hpp>
#include <BbAdventures/components/CameraComponent.hpp>
#include <BbAdventures/entities/Camera.hpp>
namespace Bba {

GameObject* NewCamera() {
	auto go = new GameObject();
	CameraComponent c = CameraComponent();
	c.Box.x = 0;
	c.Box.y = 0;
    c.Box.w = SCREEN_WIDTH;
    c.Box.h = SCREEN_HEIGHT;
    c.Bounds.x = 0;
    c.Bounds.y = 0;
	go->AddComponent<CameraComponent>(c);
	return go;
}
}  // namespace Bba