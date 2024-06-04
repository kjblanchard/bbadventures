#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/aseprite/AsepriteDocument.hpp>
using namespace Bba;

std::unordered_map<std::string, std::shared_ptr<AsepriteDocument>> AsepriteAnimation::_asepriteDocuments;

AsepriteAnimation::AsepriteAnimation() : _filePath(), _animNum(0), _frame(0), _frameTime(0), _aseDocument(nullptr) {
	_rect = std::make_unique<geRectangle>();
}

AsepriteAnimation::~AsepriteAnimation() {
}

void AsepriteAnimation::Load() {
	auto iter = _asepriteDocuments.find(_filePath);
	if (iter != _asepriteDocuments.end()) {
		_aseDocument = iter->second;
	}

	_asepriteDocuments[_filePath] = std::make_shared<AsepriteDocument>(_filePath);
	_aseDocument = _asepriteDocuments[_filePath];
}

void AsepriteAnimation::UpdateAnimation(double d) {
	_frameTime += d;
	auto& frameData = _aseDocument->frames[_frame];
	auto& animData = _aseDocument->meta.frameTags[_animNum];
	while (_frameTime >= frameData.duration) {
		_frameTime -= frameData.duration;
        // At this point, next frame shows where to move next.  We have bounds as well.
        // We need to switch to the next frame, as well as assign the next nextframe.
        auto previousFrame = _frame;
        _frame = _nextFrame;
        if(animData.direction == "pingpong") {

        }
        else {

        }
	}
}

void AsepriteAnimation::PlayAnimation(std::string a) {
}

geRectangle& AsepriteAnimation::FrameCoords() {
}