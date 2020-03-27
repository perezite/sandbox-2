#include "Tween.h"
#include "Logger.h"

namespace sb
{
	Tween::EasingCommand Tween::getCurrentEasingCommand(float t) const {
		for (std::size_t i = 0; i < _easingCommands.size(); i++) {
			if (t >= _easingCommands[i].t0 && t < _easingCommands[i].t1)
				return _easingCommands[i];
		}

		SB_ERROR("invalid parameter value");
		return EasingCommand();
	}

	float Tween::value(float t) const {
		if (_easingCommands.empty())
			return 0;

		if (t < _easingCommands[0].t0)
			return _easingCommands[0].from;

		if (t >= _easingCommands[_easingCommands.size() - 1].t1)
			return _easingCommands[_easingCommands.size() - 1].to;

		EasingCommand easingCommand = getCurrentEasingCommand(t);
		return easingCommand.easing(t, easingCommand.t0, easingCommand.t1, easingCommand.from, easingCommand.to);
	}

	float Tween::getDuration() const {
		return _easingCommands.empty() ? 0 : _easingCommands[_easingCommands.size() - 1].t1;
	}

	Tween Tween::None(float value) {
		return Tween().linear(value, value, 0);
	}

	Tween& Tween::wait(float duration) {
		float value = !_easingCommands.empty() ? _easingCommands[_easingCommands.size() - 1].to : 0;
		return wait(value, duration);
	}
}

