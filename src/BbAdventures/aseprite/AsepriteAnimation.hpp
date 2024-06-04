#pragma once
#include <GoonEngine/prim/rectangle.h>

#include <memory>
#include <string>
#include <unordered_map>
namespace Bba {
class AsepriteDocument;
class AsepriteAnimation {
   public:
	AsepriteAnimation();
	~AsepriteAnimation();
	void Load();
	void UpdateAnimation(double d);
	void PlayAnimation(std::string a);
	geRectangle& FrameCoords();

   private:
	std::string _filePath;
	int _frame;
	int _nextFrame;
	int _animNum;
	float _frameTime;
	std::unique_ptr<geRectangle> _rect;
	std::shared_ptr<AsepriteDocument> _aseDocument;
	static std::unordered_map<std::string, std::shared_ptr<AsepriteDocument>> _asepriteDocuments;
};
}  // namespace Bba