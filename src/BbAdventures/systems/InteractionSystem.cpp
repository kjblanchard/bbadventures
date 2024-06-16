#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>

#include <BbAdventures/base/GameObject.hpp>
#include <BbAdventures/components/TextInteractionComponent.hpp>
#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/ui/Textbox.hpp>
namespace Bba {
void LoadTextInteractions() {
	GameObject::ForEach<TextInteractionComponent>([](GameObject, TextInteractionComponent& t) {
		//
		t.TextImage = geTextNew(t.Text.c_str(), "BitPotion", 32);
		auto tb = State::TextDisplay->BackgroundImage();
		if (!tb) {
			LogCritical("Background image is not loaded for some reason, exiting.");
		}
		auto w = geImageWidth(tb);
		auto h = geImageHeight(tb);
		geTextSetBounds(t.TextImage, w, h);
		geTextSetNumDrawCharacters(t.TextImage, 0);
		geTextSetImage(t.TextImage, tb);
		auto p = gePoint{100, 200};
		geTextSetLocation(t.TextImage, p.x, p.y);
		geTextLoad(t.TextImage);
	});
}

}  // namespace Bba