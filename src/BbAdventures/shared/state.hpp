#pragma once
#include <BbAdventures/shared/constants.hpp>
#include <BbAdventures/tiled/Level.hpp>
#include <string>
namespace Bba {
class State {
   public:
	static int NumPlayers;
	static double DeltaTime;
	static std::string MapName;
	static int SpawnLocation;
	static Directions SpawnDirection;
	static std::string NextMapName;
	static Level* CurrentLevel;
	static bool IsLoadingMap;

   private:
};
}  // namespace Bba