#pragma once
#include <string>
namespace Bba {

extern std::string ASSET_PREFIX;
extern std::string TILED_PREFIX;
extern std::string IMAGES_PREFIX;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
enum class Directions {
	North,
	East,
	South,
	West,
};
inline const char* GetLetterForDirection(Directions d) {
	switch (d) {
		case Directions::North:
			return "U";
		case Directions::East:
			return "R";
		case Directions::South:
			return "D";
		case Directions::West:
			return "L";
	}
	return "";
}
}  // namespace Bba