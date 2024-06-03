#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
#include <GoonEngine/primitives/color.h>
#include <GoonEngine/utils.h>

static geText *testText = nullptr;
static geBgm *bgm = nullptr;
static geRectangle thingLoc = {0, 20, 200, 500};
static geColor color = {255, 255, 255, 255};
const float timeWait = 0.10;
int revealedLetters = 0;
float currentTime = 0;

void Update(double deltatime) {
	currentTime += deltatime;
	if(currentTime > timeWait) {
		currentTime -= timeWait;
		++revealedLetters;
		geTextSetNumDrawCharacters(testText, revealedLetters);
	}

}

void Draw() {
	geTextDraw(testText);
	gePoint s = geTextGetTextSize(testText);
	geRectangle r;
	r.x = 0, r.y = 20;
	r.w = s.x;
	r.h = s.y;
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
	// testText = geTextNew("Hello goon babes in the world!", "Roboto-Regular", 32);
	testText = geTextNew("Hello you prettiest girl of all time my wife!", "BitPotion", 48);
	auto xBound = 400;
	geTextSetBounds(testText, xBound, 0);
	geTextSetNumDrawCharacters(testText, revealedLetters);
	auto p = gePoint{thingLoc.x, thingLoc.y};
	geTextSetLocation(testText, p.x, p.y);
	geLoadAllContent();
	if (bgm) {
		geBgmPlay(bgm, 1.0, -1);
	}
	gePlayLoop();
	geTextFree(testText);
	geUnloadAllContent();
	return 0;
}