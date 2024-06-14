#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/TextInteractionComponent.hpp>
namespace Bba {
void LoadTextInteractions() {
	GameObject::ForEach<TextInteractionComponent>([](GameObject g, TextInteractionComponent& t) {
		t.TextImage = geTextNew(t.Text.c_str(), "BitPotion", 32);
		auto xBound = 300;
		geTextSetBounds(t.TextImage, xBound, 0);
		geTextSetNumDrawCharacters(t.TextImage, 0);
		auto p = gePoint{100, 200};
		geTextSetLocation(t.TextImage, p.x, p.y);
		geTextLoad(t.TextImage);

	});
}

}  // namespace Bba