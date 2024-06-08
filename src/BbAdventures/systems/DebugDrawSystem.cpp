#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>
#include <GoonEngine/prim/rectangle.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
namespace Bba {

void DrawDebugDrawComponents() {
	Bba::GameObject::ForEach<LocationComponent, DebugDrawComponent>(
		[](Bba::GameObject gameObject, const LocationComponent& location, const DebugDrawComponent& debugDraw) {
			auto r = geRectangle{(int)location.Location.x + debugDraw.Box.x, (int)location.Location.y + debugDraw.Box.y, debugDraw.Box.w, debugDraw.Box.h};
			auto c = geColor{255, 255, 255, 255};
			geUtilsDrawRect(&r, &c);
		});
}

}  // namespace Bba