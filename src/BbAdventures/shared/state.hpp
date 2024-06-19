#pragma once
#include <BbAdventures/shared/constants.hpp>
#include <BbAdventures/tiled/Level.hpp>
#include <string>
namespace Bba {
	class Panel;
	class Textbox;
class State {
   public:
	static int NumPlayers;
	static double DeltaTime;
	static std::string MapName;
	static int SpawnLocation;
	static Directions SpawnDirection;
	static std::string NextMapName;
	static std::string PlayingMusic;
	static Level* CurrentLevel;
	static bool IsLoadingMap;
	static bool IsDisplayingText;
	// TODO this shouldn't be here.
	static Panel* FadePanel;
	static Textbox* TextDisplay;

	static float CameraX;
	static float CameraY;
	// Controllers
	static unsigned int PlayerControllerMap[2];

   private:
};
}  // namespace Bba