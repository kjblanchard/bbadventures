#include <BbAdventures/shared/state.hpp>
using namespace Bba;
int State::NumPlayers = 1;
double State::DeltaTime = 0;
std::string State::MapName = "";
Directions State::SpawnDirection = Directions::South;
int State::SpawnLocation = 0;
std::string State::NextMapName = "";
Level* State::CurrentLevel;
bool State::IsLoadingMap = false;
bool State::IsDisplayingText = false;
Panel* State::FadePanel = nullptr;
Textbox* State::TextDisplay = nullptr;
std::string State::PlayingMusic = "";
float State::CameraX = 0;
float State::CameraY = 0;

