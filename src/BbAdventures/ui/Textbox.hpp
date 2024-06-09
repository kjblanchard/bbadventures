#pragma once
#include <GoonEngine/content/text.h>

namespace Bba {
class Textbox {
   public:
	Textbox();
	void DisplayText(geText* t);
	void UnDisplayText();
	void Update();
	void Draw();
	inline bool Complete() { return _isComplete; }
	geText* Text;

   private:
	float _currentTime = 0;
	int _revealedLetters = 0;
	bool _isComplete = false;
};

}  // namespace Bba
