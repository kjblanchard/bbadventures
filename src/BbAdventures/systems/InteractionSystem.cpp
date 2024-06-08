#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/TextInteractionComponent.hpp>
namespace Bba {
static geRectangle thingLoc = {120, 200, 200, 200};
void LoadTextInteractions() {
	auto view = GameObject::_registry.view<TextInteractionComponent>();
	for (auto&& [_, t] : view.each()) {
		t.TextImage = geTextNew(t.Text.c_str(), "BitPotion", 32);
		geTextLoad(t.TextImage);
		auto xBound = 300;
		geTextSetBounds(t.TextImage, xBound, 0);
		geTextSetNumDrawCharacters(t.TextImage, 0);
		auto p = gePoint{thingLoc.x, thingLoc.y};
		geTextSetLocation(t.TextImage, p.x, p.y);
	}
}

}  // namespace Bba