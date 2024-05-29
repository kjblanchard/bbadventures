#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
#include <GoonEngine/ui/text.h>

static SDL_Texture *thing = nullptr;
static geRectangle thingLoc = {100, 100, 400, 150};
static geColor color = geColor{255, 255, 255, 255};

void Update(double deltatime) {}

void Draw() {
	geDrawRect(&thingLoc, &color);
	geDrawTexture(thing, nullptr, &thingLoc, false);
}

void initBgm() {
	// This should be done inside of the engine
	geInitializeBgmContentType();
	geBgm *bgm = geBgmNew("1-2", 8.727f, 0.0f);
	geBgmPlay(bgm, 1.0, -1);
}

int main() {
	geInitializeEngine();
	geGameSetUpdateFunc(Update);
	geGameSetDrawFunc(Draw);
	// initBgm();
	// geInitializeTextSubsystem("assets/fonts/BitPotion.ttf", 32);
	geInitializeFontContentType();
	geFont* font = geFontNew("BitPotion", 32);
	geLoadAllContent();
	auto size = Point{400, 150};
	thing = geCreateTextureForString("Hello world!", &size,
									 geColor{255, 0, 0, 255});
	gePlayLoop();
	geUnloadAllContent();
	return 0;
}