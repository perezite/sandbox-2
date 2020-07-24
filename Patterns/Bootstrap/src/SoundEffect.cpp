#include "SoundEffect.h"
#include <algorithm>

namespace blocks {
	bool SoundEffect::isGarbage(float delay) {
		return delay <= 0;
	}

	std::map<std::string, sb::Sound>& SoundEffect::getSoundPool() {
		static std::map<std::string, sb::Sound> soundPool;
		return soundPool;
	}

	sb::Sound& SoundEffect::getSound() {
		return getSoundPool()[_assetPath];
	}

	SoundEffect::SoundEffect() : _playbackIntervalSeconds(0), _secondsToNextPlayback(0)
	{ }

	void SoundEffect::play(float delaySeconds) {
		_delays.push_back(delaySeconds);
	}

	void SoundEffect::loadFromAssetPool(const std::string& assetPath) {
		std::map<std::string, sb::Sound>& pool = getSoundPool();
		if (pool.find(assetPath) == pool.end())
			pool[assetPath].loadFromAsset(assetPath);

		_assetPath = assetPath;
	}

	void SoundEffect::update(float ds) {
		_secondsToNextPlayback -= ds;

		for (size_t i = 0; i < _delays.size(); i++) {
			_delays[i] -= ds;
			if (_delays[i] <= 0 && _secondsToNextPlayback <= 0) {
				getSound().play();
				_secondsToNextPlayback = _playbackIntervalSeconds;
			}
		}

		_delays.erase(std::remove_if(_delays.begin(), _delays.end(), isGarbage), _delays.end());
	}
}