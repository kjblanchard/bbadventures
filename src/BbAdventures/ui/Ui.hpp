#pragma once
#include <BbAdventures/ui/Panel.hpp>
#include <BbAdventures/ui/Textbox.hpp>
#include <memory>

namespace Bba {
class Ui {
   public:
	Ui();
	void Update();
	void Draw();

   private:
	std::unique_ptr<Panel> _panel;
	std::unique_ptr<Textbox> _textbox;
};
}  // namespace Bba