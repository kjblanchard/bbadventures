#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
#include <GoonEngine/prim/color.h>
#include <GoonEngine/prim/point.h>
#include <GoonEngine/prim/rectangle.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/systems/Systems.hpp>
#include <BbAdventures/tiled/Level.hpp>
#include <BbAdventures/ui/Panel.hpp>

static geBgm *bgm = nullptr;
// static geRectangle thingLoc = {120, 200, 200, 200};
// static geColor color = {255, 255, 255, 255};
static Bba::Panel *panel;
// const float timeWait = 0.10;
std::string defaultLevel = "debugTown";
int revealedLetters = 0;
float currentTime = 0;

void Update(double deltatime) {
	Bba::State::DeltaTime = deltatime;
	// currentTime += deltatime;
	// if (currentTime > timeWait) {
	// 	currentTime -= timeWait;
	// 	++revealedLetters;
	// 	geTextSetNumDrawCharacters(testText, revealedLetters);
	// }
	if (!Bba::State::IsLoadingMap) {
		Bba::UpdatePlayers();
		Bba::UpdateAnimationComponents();
	}
	panel->Update();
}

void Draw() {
	if (Bba::State::CurrentLevel) {
		Bba::State::CurrentLevel->Draw();
	}
	Bba::DrawDebugDrawComponents();
	Bba::DrawAnimationComponents();
	panel->Draw();
}

void initBgm() {
	bgm = geBgmNew("town2");
}

int main() {
	geInitializeEngine();
	geGameSetUpdateFunc(Update);
	geGameSetDrawFunc(Draw);
	initBgm();
	geLoadAllContent();
	if (bgm) {
		geBgmPlay(bgm, 1.0, -1);
	}
	panel = new Bba::Panel();
	Bba::State::NextMapName = defaultLevel;
	Bba::Level::LoadNewLevel();
	Bba::State::FadePanel = panel;
	gePlayLoop();
	geUnloadAllContent();
	return 0;
}
