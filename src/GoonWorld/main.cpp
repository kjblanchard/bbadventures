#include <GoonEngine/primitives/color.h>
#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/text.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/utils.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>

static geText *thing = nullptr;
static geBgm *bgm = nullptr;
static geRectangle thingLoc = {100, 100, 400, 150};

void Update(double deltatime) {}

void Draw() {
	// geDrawRect(&thingLoc, &color);
	geTextDraw(thing);
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
	thing = geTextNew("Hello world!", "BitPotion", 32);
	geTextSetDrawRect(thing, &thingLoc);
	geLoadAllContent();
	if (bgm) {
		geBgmPlay(bgm, 1.0, -1);
	}
	gePlayLoop();
	geTextFree(thing);
	// geUnloadAllContent();
	return 0;
}