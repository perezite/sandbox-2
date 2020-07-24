#pragma once
#include "Sound.h"
#include <vector>
#include <map>
#include <string>

namespace blocks {
	class SoundEffect {
		std::vector<float> _delays;
		std::string _assetPath;
		float _playbackIntervalSeconds;
		float _secondsToNextPlayback;

	protected:
		static bool isGarbage(float delay);
		static std::map<std::string, sb::Sound>& getSoundPool();
		sb::Sound& getSound();

	public:
		SoundEffect();

		inline void setPlaybackInterval(float seconds) { _playbackIntervalSeconds = seconds; }
		void play(float delaySeconds = 0);
		void loadFromAssetPool(const std::string& assetPath);
		void update(float ds);
	};
}