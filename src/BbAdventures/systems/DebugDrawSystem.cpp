#include <GoonEngine/debug.h>
#include <GoonEngine/input/keyboard.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/LocationComponent.hpp>
#include <BbAdventures/components/DebugDrawComponent.hpp>
#include <GoonEngine/utils.h>
namespace Bba {
void UpdateDebugDrawComponents() {
	auto view = GameObject::_registry.view<const LocationComponent, DebugDrawComponent>();
	for (auto [_, l, d] : view.each()) {
        auto r = geRectangle{l.Location.x + d.Box.x, l.Location.y + d.Box.y, d.Box.w, d.Box.h };
        auto c = geColor{255,255,255,255};
        geUtilsDrawRect(&r, &c );

	}
}

}  // namespace Bba