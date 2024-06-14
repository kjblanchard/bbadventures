#pragma once
#include <GoonEngine/content/text.h>
typedef struct geImage geImage;

namespace Bba {
class Textbox {
   public:
	Textbox();
	void Interact(geText* t);
	void DisplayText(geText* t);
	void UnDisplayText();
	void Update();
	void Draw();
	inline bool Complete() { return _isComplete; }
	geText* Text;
	// TODO this should be private?
	geImage* _backgroundTexture;

   private:
	float _currentTime = 0;
	int _revealedLetters = 0;
	bool _isComplete = false;

};

}  // namespace Bba
