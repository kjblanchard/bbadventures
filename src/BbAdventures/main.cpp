#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
#include <GoonEngine/prim/color.h>
#include <GoonEngine/prim/point.h>
#include <GoonEngine/prim/rectangle.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/systems/Systems.hpp>
#include <BbAdventures/tiled/Level.hpp>

static geText *testText = nullptr;
static geBgm *bgm = nullptr;
static geRectangle thingLoc = {120, 200, 200, 200};
static geColor color = {255, 255, 255, 255};
static Bba::Level *level;
const float timeWait = 0.10;
int revealedLetters = 0;
float currentTime = 0;

void Update(double deltatime) {
	currentTime += deltatime;
	if (currentTime > timeWait) {
		currentTime -= timeWait;
		++revealedLetters;
		geTextSetNumDrawCharacters(testText, revealedLetters);
	}
	Bba::UpdatePlayers();
}

void Draw() {
	level->Draw();
	Bba::UpdateDebugDrawComponents();
	geTextDraw(testText);
	gePoint s = geTextGetTextSize(testText);
	geRectangle r;
	r.x = 118, r.y = 200;
	r.w = s.x + 4;
	r.h = s.y + 2;
	geUtilsDrawRect(&r, &color);
}

void initBgm() {
	bgm = geBgmNew("town2");
}

int main() {
	geInitializeEngine();
	geGameSetUpdateFunc(Update);
	geGameSetDrawFunc(Draw);
	initBgm();
	testText = geTextNew("Hello you prettiest girl of all time my wife!!! Time for chicken sleep time!!", "BitPotion", 32);
	auto xBound = 300;
	geTextSetBounds(testText, xBound, 0);
	geTextSetNumDrawCharacters(testText, revealedLetters);
	auto p = gePoint{thingLoc.x, thingLoc.y};
	geTextSetLocation(testText, p.x, p.y);
	geLoadAllContent();
	if (bgm) {
		geBgmPlay(bgm, 1.0, -1);
	}
	// auto level = Bba::TiledMap("debugTown");
	level = new Bba::Level("debugTown");
	level->LoadAllGameObjects();
	level->RestartLevel();
	gePlayLoop();
	geTextFree(testText);
	geUnloadAllContent();
	return 0;
}