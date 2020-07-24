#pragma once

#include "Transformable.h"
#include "Animation.h"

namespace blocks
{
	class TransformEffects : public sb::Transformable {
		Animation2f _drift;
		Animationf _wobble;
		Animationf _spin;

	protected:
		inline void smoothRotateBy(float radians, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
			smoothRotateTo(target.getRotation() + radians, target, normalizedTween, duration);
		}

		inline void smoothMoveBy(const sb::Vector2f& amount, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
			driftTo(target.getPosition() + amount, target, duration);
		}

		void smoothMoveTo(const sb::Vector2f& end, sb::Transformable& target, sb::Tween2f& normalizedTween, float duration);
		void smoothRotateTo(float radians, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration);

	public:
		inline void driftTo(const sb::Vector2f& end, sb::Transformable& target, float duration = 0.2f) {
			sb::Tween2f tween = sb::Tween2f().sineOut(sb::Vector2f(1), sb::Vector2f(0), 1);
			smoothMoveTo(end, target, tween, duration);
		}

		inline void driftBy(const sb::Vector2f& amount, sb::Transformable& target, float duration = 0.2f) {
			driftTo(target.getPosition() + amount, target, duration);
		}

		inline void bounceBy(const sb::Vector2f& amount, sb::Transformable& target, float duration = 0.2f) {
			bounceTo(target.getPosition() + amount, target, duration);
		}

		bool isPlaying();
		void bounceTo(const sb::Vector2f& end, sb::Transformable& target, float duration = 0.5f);
		void pop(sb::Transformable& target, float duration = 0.15f);
		void spinTo(float radians, sb::Transformable& target, float duration = 0.5f);
		void spinBy(float radians, sb::Transformable& target, float duration = 0.5f);
		void implode(sb::Transformable& target, float duration = 0.8f);
		void apply(sb::Transform& transform);
		void stop();
		void update(float ds);
	};
}