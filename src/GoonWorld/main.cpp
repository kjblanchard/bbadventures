#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/textv2.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/clock.h>
#include <GoonEngine/utils.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>

static geRichText *thing = nullptr;
static geBgm *bgm = nullptr;
static geRectangle thingLoc = {100, 100, 400, 150};
static geColor color = geColor{255, 255, 255, 255};

void Update(double deltatime) {}

void Draw() {
	geDrawRect(&thingLoc, &color);
	geRichTextDraw(thing);
	geUtilsDrawFps();
}

void initBgm() {
	bgm = geBgmNew("1-2", 8.727f, 0.0f);
}

int main() {
	// SetLogLevel(Log_LDebug);
	geInitializeEngine();
	geGameSetUpdateFunc(Update);
	geGameSetDrawFunc(Draw);
	initBgm();
	auto size = Point{400, 150};
	thing = geRichTextNew("Hello world!", "BitPotion", 32, &size);
	geRichTextSetDrawRect(thing, &thingLoc);
	geLoadAllContent();
	if (bgm) {
		geBgmPlay(bgm, 1.0, -1);
	}
	gePlayLoop();
	geRichTextFree(thing);
	// geUnloadAllContent();
	return 0;
}