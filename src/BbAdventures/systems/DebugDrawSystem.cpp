#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>
#include <GoonEngine/prim/rectangle.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
namespace Bba {
void DrawDebugDrawComponents() {
	auto view = GameObject::_registry.view<const LocationComponent, DebugDrawComponent>();
	for (auto [_, l, d] : view.each()) {
		auto r = geRectangle{l.Location.x + d.Box.x, l.Location.y + d.Box.y, d.Box.w, d.Box.h};
		auto c = geColor{255, 255, 255, 255};
		geUtilsDrawRect(&r, &c);
	}
}

}  // namespace Bba