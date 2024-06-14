#include <SDL2/SDL.h>
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
#include <BbAdventures/ui/Textbox.hpp>

static Bba::Panel *panel;
static Bba::Textbox *textbox;
std::string defaultLevel = "debugTown";
int revealedLetters = 0;
float currentTime = 0;

void Update(double deltatime) {
	Bba::State::DeltaTime = deltatime;
	if (!Bba::State::IsLoadingMap) {
		Bba::UpdatePlayers();
		Bba::UpdateAnimationComponents();
		Bba::UpdateCamera();
	}
	textbox->Update();
	panel->Update();
}

void Draw() {
	if (Bba::State::CurrentLevel) {
		Bba::State::CurrentLevel->Draw();
	}
	Bba::DrawDebugDrawComponents();
	Bba::DrawAnimationComponents();
	panel->Draw();
	textbox->Draw();
}

int main(int argc, char *argv[]) {
	geInitializeEngine();
	geGameSetUpdateFunc(Update);
	geGameSetDrawFunc(Draw);
	geLoadAllContent();
	panel = new Bba::Panel();
	textbox = new Bba::Textbox;
	Bba::State::NextMapName = defaultLevel;
	Bba::Level::LoadNewLevel();
	Bba::State::FadePanel = panel;
	Bba::State::TextDisplay = textbox;
	gePlayLoop();
	geUnloadAllContent();
	return 0;
}
