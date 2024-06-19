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
#include <BbAdventures/tween/Tween.hpp>
#include <BbAdventures/ui/Ui.hpp>

static const std::string defaultLevel = "debugTown";
static Bba::Ui* _ui;
// Splash screens
static bool _showingSplashScreens;
static geImage* _logo;
static Bba::Tween* _fadeInTween = nullptr;
static Bba::Tween* _fadeOutTween = nullptr;
static std::string _assetPath = "assets";
static std::string _imgPath = "img";

void Update(double deltatime) {
	Bba::State::DeltaTime = deltatime;
	if (_showingSplashScreens) {
		if (_fadeInTween->Update() && _fadeOutTween->Update()) {
			_showingSplashScreens = false;
			_ui = new Bba::Ui();
			Bba::State::NextMapName = defaultLevel;
			Bba::Level::LoadNewLevel();
		} else {
			if (!_fadeInTween->Complete()) {
				auto doneValue = (int)_fadeInTween->Value();
				LogWarn("Done value is %d", doneValue);
				geImageStaticSetAlpha(_logo, doneValue);
			} else {
				auto doneValue = (int)_fadeOutTween->Value();
				geImageStaticSetAlpha(_logo, doneValue);
			}
		}
		return;
	}
	if (!Bba::State::IsLoadingMap) {
		Bba::UpdatePlayerJoysticks();
		Bba::UpdatePlayers();
		Bba::UpdateAnimationComponents();
		Bba::UpdateCamera();
	}
	_ui->Update();
}

void Draw() {
	if (_showingSplashScreens) {
		// draw splash screens
		auto d = geRectangle{0, 0, geImageWidth(_logo), geImageHeight(_logo)};
		geImageDraw(_logo, nullptr, &d);
		return;
	}
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
	auto surfacePath = _assetPath + '/' + _imgPath + '/' + std::string("supergoongameslogo.png");
	char buf[1000];
	geGetLoadFilename(buf, sizeof(buf), surfacePath.c_str());
	_logo = geImageNewFromFile(buf);
	_fadeInTween = new Bba::Tween(0, 255, 1.5, Bba::Easings::Linear);
	_fadeOutTween = new Bba::Tween(255, 0, 1.5, Bba::Easings::Linear);
	_showingSplashScreens = true;
	geLoadAllContent();
	geImageSetAlpha(_logo, 0);
	gePlayLoop();
	delete (Bba::State::CurrentLevel);
	geUnloadAllContent();
	return 0;
}
