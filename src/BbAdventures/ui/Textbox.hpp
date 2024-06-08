#pragma once
#include <GoonEngine/content/text.h>

namespace Bba {
class Textbox {
   public:
	Textbox();
    void DisplayText(geText* t);
	void Update();
	void Draw();
	geText* Text;
    private:
    float _currentTime = 0;
    int _revealedLetters = 0;
};

}  // namespace Bba
