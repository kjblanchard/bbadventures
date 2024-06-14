#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>
#include <GoonEngine/prim/rectangle.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/shared/state.hpp>
namespace Bba {

void DrawDebugDrawComponents() {
	Bba::GameObject::ForEach<LocationComponent, DebugDrawComponent>(
		[](Bba::GameObject gameObject, const LocationComponent& location, const DebugDrawComponent& debugDraw) {
			auto r = geRectangleF{(location.Location.x + debugDraw.Box.x) - State::CameraX,(location.Location.y + debugDraw.Box.y) - State::CameraY, (float)debugDraw.Box.w, (float)debugDraw.Box.h};
			auto c = geColor{255, 255, 255, 255};
			geUtilsDrawRectF(&r, &c);
		});
}

}  // namespace Bba