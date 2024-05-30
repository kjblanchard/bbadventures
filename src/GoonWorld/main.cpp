#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
// #include <GoonEngine/ui/text.h>
// #include <GoonEngine/content/textv2.h>

static geRichText *thing = nullptr;
static geRectangle thingLoc = {100, 100, 400, 150};
static geColor color = geColor{255, 255, 255, 255};

void Update(double deltatime) {}

void Draw() {
	geDrawRect(&thingLoc, &color);
	geRichTextDraw(thing);
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
	SetLogLevel(Log_LDebug);
	// initBgm();
	// geInitializeTextSubsystem("assets/fonts/BitPotion.ttf", 32);
	geInitializeFontContentType();
	geInitializeTextv2ContentType();
	auto size = Point{400, 150};
	thing = geRichTextNew("Hello world!", "BitPotion", 32, &size);
	geRichTextSetDrawRect(thing, &thingLoc);
	geLoadAllContent();
	gePlayLoop();
	geRichTextFree(thing);
	geUnloadAllContent();
	return 0;
}