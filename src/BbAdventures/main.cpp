#include <GoonEngine/content/content.h>
#include <GoonEngine/content/text.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/game.h>
#include <GoonEngine/prim/color.h>
#include <GoonEngine/prim/point.h>
#include <GoonEngine/prim/rectangle.h>
#include <GoonEngine/utils.h>
#include <SDL2/SDL.h>

#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/systems/Systems.hpp>
#include <BbAdventures/tiled/Level.hpp>
#include <BbAdventures/ui/Ui.hpp>

static const std::string defaultLevel = "debugTown";
static Bba::Ui* _ui;

void Update(double deltatime) {
	Bba::State::DeltaTime = deltatime;
	if (!Bba::State::IsLoadingMap) {
		Bba::UpdatePlayerJoysticks();
		Bba::UpdatePlayers();
		Bba::UpdateAnimationComponents();
		Bba::UpdateCamera();
	}
	_ui->Update();
}

void Draw() {
	if (Bba::State::CurrentLevel) {
		Bba::State::CurrentLevel->Draw();
	}
	Bba::DrawAnimationComponents();
	Bba::DrawDebugDrawComponents();
	_ui->Draw();
}

int main(int, char*[]) {
	geInitializeEngine();
	geGameSetUpdateFunc(Update);
	geGameSetDrawFunc(Draw);
	geLoadAllContent();
	_ui = new Bba::Ui();
	Bba::State::NextMapName = defaultLevel;
	Bba::Level::LoadNewLevel();
	gePlayLoop();
	delete(Bba::State::CurrentLevel);
	geUnloadAllContent();
	return 0;
}
