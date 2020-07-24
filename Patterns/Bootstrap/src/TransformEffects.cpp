#include "TransformEffects.h"

namespace blocks
{
	void TransformEffects::smoothMoveTo(const sb::Vector2f& end, sb::Transformable& target, sb::Tween2f& normalizedTween, float duration) {
		sb::Vector2f effectPosition = target.getPosition() + _drift.value();
		target.setPosition(end);
		sb::Vector2f offset = effectPosition - end;
		normalizedTween.stretchDuration(duration);
		normalizedTween.scale(offset);
		_drift.tween = normalizedTween;
		_drift.start();
	}

	void TransformEffects::smoothRotateTo(float radians, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
		float effectRotation = target.getRotation() + _spin.value();
		float offset = effectRotation - radians;
		target.setRotation(radians);
		normalizedTween.stretchDuration(duration);
		normalizedTween.scale(offset);
		_spin.tween = normalizedTween;
		_spin.start();
	}

	bool TransformEffects::isPlaying() {
		return _drift.isPlaying() || _wobble.isPlaying() || _spin.isPlaying();
	}

	void TransformEffects::bounceTo(const sb::Vector2f& end, sb::Transformable& target, float duration) {
		sb::Tween2f tween = sb::Tween2f().bounceOut(sb::Vector2f(1), sb::Vector2f(0), 1);
		smoothMoveTo(end, target, tween, duration);
	}

	void TransformEffects::pop(sb::Transformable& target, float duration) {
		_wobble.tween = sb::Tweenf().quintInOut(_wobble.value(), 0.4f, 0.2f).expoOut(0.4f, 0, 0.4f);
		_wobble.start();
	}

	void TransformEffects::spinTo(float radians, sb::Transformable& target, float duration) {
		sb::Tweenf tween = sb::Tweenf().bounceOut(1, 0, 1);
		smoothRotateTo(radians, target, tween, duration);
	}

	void TransformEffects::spinBy(float radians, sb::Transformable& target, float duration) {
		spinTo(target.getRotation() + radians, target, duration);
	}

	void TransformEffects::implode(sb::Transformable& target, float duration) {
		_wobble.tween = sb::Tweenf().backIn(_wobble.value(), -1, duration);
		_wobble.start();

		sb::Tweenf tween = sb::Tweenf().backInOut(1, 0, 1);
		smoothRotateBy(sb::random(-45, 45) * sb::ToRadian, target, tween, duration * 0.3f);
	}

	void TransformEffects::apply(sb::Transform& transform) {
		sb::Transform effectRotation(sb::Vector2f(0), sb::Vector2f(1), _spin.value());
		sb::Transform effectTranslation(_drift.value(), sb::Vector2f(1), 0);
		sb::Transform effectScale(sb::Vector2f(0), (1 + _wobble.value()) * sb::Vector2f(1), 0);
		transform = effectTranslation * transform * effectRotation * effectScale;
	}

	void TransformEffects::stop() {
		_drift.stop();
		_wobble.stop();
		_spin.stop();
	}

	void TransformEffects::update(float ds) {
		_drift.update(ds);
		_wobble.update(ds);
		_spin.update(ds);
	}
}