#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/ui/Textbox.hpp>
using namespace Bba;

const float timeWait = 0.10;

Textbox::Textbox() {
}

void Textbox::DisplayText(geText* t) {
	Text = t;
	_revealedLetters = 0;
	_currentTime = 0;
}
void Textbox:: UnDisplayText() {
	geTextSetNumDrawCharacters(Text, 0);
	Text = nullptr;
}
void Textbox::Update() {
	if (!Text) {
		return;
	}
	auto deltatime = State::DeltaTime;
	_currentTime += deltatime;
	if (_currentTime > timeWait) {
		_currentTime -= timeWait;
		++_revealedLetters;
		geTextSetNumDrawCharacters(Text, _revealedLetters);
	}
}
void Textbox::Draw() {
	if (Text) {
		geTextDraw(Text);
	}
}