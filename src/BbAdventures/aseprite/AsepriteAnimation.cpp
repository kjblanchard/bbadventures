#include <GoonEngine/debug.h>

#include <BbAdventures/aseprite/AsepriteAnimation.hpp>
#include <BbAdventures/aseprite/AsepriteDocument.hpp>
using namespace Bba;

std::unordered_map<std::string, std::shared_ptr<AsepriteDocument>> AsepriteAnimation::_asepriteDocuments;

AsepriteAnimation::AsepriteAnimation(std::string n) : _animNum(0), _frame(0), _nextFrame(0), _frameTime(0), _aseDocument(nullptr) {
	_rect = std::make_unique<geRectangle>();
	_filePath = n;
	Load();
}

AsepriteAnimation::~AsepriteAnimation() {
}
std::string AsepriteAnimation::Filename() {
	auto f = "assets/img/" + _aseDocument->meta.image;
	return f;
}

void AsepriteAnimation::Load() {
	auto iter = _asepriteDocuments.find(_filePath);
	if (iter != _asepriteDocuments.end()) {
		_aseDocument = iter->second;
	}
	_asepriteDocuments[_filePath] = std::make_shared<AsepriteDocument>(_filePath);
	_aseDocument = _asepriteDocuments[_filePath];
	UpdateRect();
}

void AsepriteAnimation::UpdateAnimation(double d) {
	_frameTime += d;
	auto frameData = _aseDocument->frames[_frame];
	auto animData = _aseDocument->meta.frameTags[_animNum];
	while (_frameTime >= frameData.duration) {
		_frameTime -= frameData.duration;
		if (animData.direction == "pingpong") {
			if (_reverse) {
				_nextFrame = _frame - 1;
				if (_nextFrame < animData.from) {
					_nextFrame = animData.from + 1;
					_reverse = false;
				}
			} else {
				_nextFrame = _frame + 1;
				if (_nextFrame > animData.to) {
					_nextFrame = animData.to - 1;
					_reverse = true;
				}
			}
		} else {
			if (animData.direction == "forward") {
				_nextFrame = (_frame + 1 > animData.to) ? animData.from : _frame + 1;
			} else if (animData.direction == "reverse") {
				_nextFrame = (_frame - 1 < animData.from) ? animData.to : _frame - 1;
			}
		}
		_frame = _nextFrame;
		frameData = _aseDocument->frames[_frame];
		UpdateRect();
	}
}

void AsepriteAnimation::PlayAnimation(std::string a) {
	for (size_t i = 0; i < _aseDocument->meta.frameTags.size(); i++) {
		auto& anim = _aseDocument->meta.frameTags[i];
		if (anim.name == a) {
			_animNum = i;
			_frame = anim.from;
			_frameTime = 0;
			UpdateRect();
			return;
		}
	}
	LogWarn("Could not find animation for %s", a.c_str());
}
void AsepriteAnimation::UpdateRect() {
	_rect->x = _aseDocument->frames[_frame].frame.x;
	_rect->y = _aseDocument->frames[_frame].frame.y;
	_rect->w = _aseDocument->frames[_frame].frame.w;
	_rect->h = _aseDocument->frames[_frame].frame.h;
}

geRectangle& AsepriteAnimation::FrameCoords() {
	return *(_rect.get());
}