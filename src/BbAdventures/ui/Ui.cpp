#include "Ui.hpp"

#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/ui/Ui.hpp>
using namespace Bba;

Bba::Ui::Ui() {
	_panel = std::make_unique<Panel>();
	_textbox = std::make_unique<Textbox>();
	Bba::State::TextDisplay = _textbox.get();
	Bba::State::FadePanel = _panel.get();
}

void Bba::Ui::Update() {
	_textbox->Update();
	_panel->Update();
}

void Bba::Ui::Draw() {
	_panel->Draw();
	_textbox->Draw();
}
