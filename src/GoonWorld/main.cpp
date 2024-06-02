#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
#include <GoonEngine/primitives/color.h>
#include <GoonEngine/utils.h>

static geText *testText = nullptr;
static geBgm *bgm = nullptr;
static geRectangle thingLoc = {0, 0, 200, 500};
static geColor color = {255, 255, 255, 255};
static geColor color2 = {255, 0, 0, 255};

void Update(double deltatime) {}

void Draw() {
	auto p = geTextMeasureDebug(testText);
	// auto p = geTextGetTextSize(testText);
	geTextDrawNative(testText);
	geRectangle r;
	r.x = r.y = 0;
	r.w = p.x;
	r.h = p.y;
	geUtilsDrawRect(&r, &color);
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
	testText = geTextNew("Hello goon babes in the world!", "Roboto-Regular", 32);
	geTextSetDrawRect(testText, &thingLoc);
	geLoadAllContent();
	if (bgm) {
		geBgmPlay(bgm, 1.0, -1);
	}
	gePlayLoop();
	geTextFree(testText);
	// geUnloadAllContent();
	return 0;
}