#include <GoonEngine/debug.h>
#include <GoonEngine/easing.h>

#include <BbAdventures/shared/state.hpp>
#include <BbAdventures/tween/Tween.hpp>
using namespace Bba;
Tween::Tween(float start, float end, float duration, Easings ease) : _begin(start), _end(end), _duration(duration), _easeType(ease) {
}
float Tween::Value() {
	if (_currentDuration >= _duration) {
		return _end;
	}
	float progress = 0;
	switch (_easeType) {
		case Easings::Linear:
			progress = geLinearInterpolation(_currentDuration / _duration);
			return _begin + ((_end - _begin) * progress);
		default:
			LogError("Ease type not implemented!");
			return 0;
	}
}
bool Tween::Update() {
	if (_currentDuration >= _duration)
		return true;
	_currentDuration += State::DeltaTime;
	return _currentDuration >= _duration;
}
