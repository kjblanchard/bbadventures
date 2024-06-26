
#include <GoonEngine/content/bgm.h>
#include <GoonEngine/content/content.h>
#include <GoonEngine/content/image.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/prim/point.h>
#include <GoonEngine/utils.h>

#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/ui/Textbox.hpp>
using namespace Bba;

static geBgm* _textBgm = nullptr;
const float _timeWait = 0.05;
const gePoint _boxSize = {300, 75};

Textbox::Textbox() {
	Text = nullptr;
	geColor c = {0, 0, 100, 200};
	_backgroundTexture = geImageNewRenderTarget("textboxBackground", _boxSize.x, _boxSize.y, &c);
}

geImage* Textbox::BackgroundImage() {
	// Increase refcount, and send image.
	auto i = geGetLoadedContent(geContentTypeImage, "textboxBackground");
	if (i && i->Data.Image) {
		return i->Data.Image;
	}
	return nullptr;
}

void Textbox::DisplayText(geText* t) {
	Text = t;
	_revealedLetters = 0;
	_currentTime = 0;
	_isComplete = false;
	if (!_textBgm) {
		_textBgm = geBgmNew("typing");
		geBgmSetBackground(_textBgm, true);
		geBgmLoad(_textBgm);
	}
	State::IsDisplayingText = true;
	geBgmPlay(_textBgm, 1.0, -1);
}

void Textbox::Interact(geText* t) {
	if (State::IsDisplayingText) {
		if (!_isComplete) {
			geTextSetNumDrawCharacters(Text, geTextLength(Text));
			geBgmStop(_textBgm);
			_isComplete = true;
			return;
		} else {
			UnDisplayText();
		}
	} else {
		DisplayText(t);
	}
}
void Textbox::UnDisplayText() {
	geTextSetNumDrawCharacters(Text, 0);
	Text = nullptr;
	_isComplete = false;
	geBgmStop(_textBgm);
	State::IsDisplayingText = false;
}
void Textbox::Update() {
	if (!Text || _isComplete) {
		return;
	}
	auto deltatime = State::DeltaTime;
	_currentTime += deltatime;
	if (_currentTime > _timeWait) {
		_currentTime -= _timeWait;
		++_revealedLetters;
		geTextSetNumDrawCharacters(Text, _revealedLetters);
	}
	if (_revealedLetters >= geTextLength(Text)) {
		geBgmStop(_textBgm);
		_isComplete = true;
	}
}
void Textbox::Draw() {
	if (Text) {
		geTextDraw(Text);
		// Draw a outline around the text
		// auto bounds = geTextBoundingBox(Text);
		auto loc = geTextLocation(Text);
		auto c = geColor{255, 255, 255, 255};
		auto d = geRectangle{loc.x, loc.y, _boxSize.x, _boxSize.y};
		geUtilsDrawRect(&d, &c);
		d.x -= 1;
		d.y -= 1;
		d.w += 2;
		d.h += 2;
		geUtilsDrawRect(&d, &c);
	}
}