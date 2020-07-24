#pragma once
#include "Vector2.h"
#include "Tween.h"

namespace blocks {
	template <class T>
	struct Animation {
		sb::Tween<T> tween;
		bool playing = false;
		float t = 0;

		inline void start() {
			t = 0;
			playing = true;
		}

		inline void stop() {
			t = tween.getDuration();
			playing = false;
		}

		inline void update(float ds) {
			if (isPlaying())
				t += ds;
		}

		inline T value() { return tween.value(t); }

		inline T targetValue() const { return tween.targetValue(); }

		inline bool isPlaying() { return playing && t < tween.getDuration(); }

	};

	typedef Animation<float> Animationf;
	typedef Animation<
		sb::Vector2f> Animation2f;
}