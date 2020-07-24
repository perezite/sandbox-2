#include "Window.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Quad.h"
#include "Input.h"
#include "Stopwatch.h"
#include "TextureSheet.h"
#include "Sprite.h"
#include "Math.h"
#include "Tween.h"
#include "ParticleSystem.h"
#include "Box.h"
#include "Music.h"
#include "Sound.h"
#include <cstddef>
#include <vector>
#include <map>
#include <algorithm>

void init0(std::vector<sb::Sprite>& stones, sb::Texture& stonesTex) {
    sb::Vector2i size(128, 128);
    sb::Vector2f scale(0.2f, 0.2f);

    stones[0].setTexture(stonesTex, sb::IntRect(0 * size.x, 0 * size.y, size.x, size.y));
    stones[1].setTexture(stonesTex, sb::IntRect(0 * size.x, 1 * size.y, size.x, size.y));
    stones[2].setTexture(stonesTex, sb::IntRect(1 * size.x, 1 * size.y, size.x, size.y));
    stones[3].setTexture(stonesTex, sb::IntRect(2 * size.x, 1 * size.y, size.x, size.y));
    stones[4].setTexture(stonesTex, sb::IntRect(0 * size.x, 2 * size.y, size.x, size.y));
    stones[5].setTexture(stonesTex, sb::IntRect(1 * size.x, 2 * size.y, size.x, size.y));
    stones[6].setTexture(stonesTex, sb::IntRect(2 * size.x, 2 * size.y, size.x, size.y));

    for (std::size_t i = 0; i < stones.size(); i++) {
        sb::Vector2i gridPos(i % 3, i / 3);
        float x = (gridPos.x - 1) * scale.x * 1.1f;
        float y = (gridPos.y - 1) * scale.y * 1.1f;
        stones[i].setPosition(x, y);
        stones[i].setScale(scale);
    }
}

void draw0(sb::DrawBatch& batch, std::vector<sb::Sprite>& stones) {
    for (std::size_t i = 0; i < stones.size(); i++) {
        batch.draw(stones[i]);
    }
}

void demo0() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    sb::DrawBatch batch(512);
    sb::Texture stoneTex;
    std::vector<sb::Sprite> stones(7);

    stoneTex.loadFromAsset("Textures/Stones.png");
    init0(stones, stoneTex);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear();
        draw0(batch, stones);
        window.draw(batch);
        window.display();
    }
}

void init1(sb::Mesh& background, const sb::Camera& camera) {
    sb::Vector2f extent(camera.getWidth() * 0.5f, camera.getWidth() * camera.getInverseAspectRatio() * 0.5f);
    sb::Color bottomColor = sb::Color(252.0f / 255.0f, 182.0f / 255.0f, 159.0f / 255.0f, 1);
    sb::Color topColor = sb::Color(255.0f / 255.0f, 236.0f / 255.0f, 210.0f / 255.0f, 1);
    background[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), bottomColor);
    background[1] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), bottomColor);
    background[2] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), topColor);
    background[3] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), topColor);
}

void demo1() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    sb::Mesh background(4, sb::PrimitiveType::TriangleStrip);

    init1(background, window.getCamera());

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear();
        window.draw(background.getVertices(), sb::PrimitiveType::TriangleStrip);
        window.display();
    }
}

void initCamera(sb::Camera& camera, float minInverseAspectRatio) {
    float minHeight = minInverseAspectRatio;
    float height = camera.getWidth() * camera.getInverseAspectRatio();
    if (height < minHeight) {
        float ratio = minHeight / height;
        camera.setWidth(camera.getWidth() * ratio);
    }
}

void demo2() {
    float inverseAspect = 2 / 4.0f;
    sb::Window window(400, int(400 * inverseAspect));
    sb::Texture coordinatesTex;
    sb::Sprite coordinates;
    sb::Quad reference;

    initCamera(window.getCamera(), 3 / 2.0f);
    coordinatesTex.loadFromAsset("Textures/CoordinateSystem.png");
    coordinates.setTexture(coordinatesTex);
    reference.setScale(0.1f, 1.45f);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(coordinates);
        window.draw(reference);

        window.display();
    }
}

inline float getCameraHeight(sb::Camera& camera) {
    return camera.getWidth() * camera.getInverseAspectRatio();
}

inline sb::Color colorFromRgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
    return sb::Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

class Border2 : public sb::Drawable, public sb::Transformable {
private:
    sb::Mesh _mesh;
    float _height;

public:
    Border2(float height, float thickness)
        : _mesh(10, sb::PrimitiveType::TriangleStrip), _height(height) {
        sb::Vector2f extent(0.5f, height / 2);
        sb::Color innerColor = colorFromRgba(143, 211, 244);
        sb::Color outerColor = colorFromRgba(132, 250, 176);

        _mesh[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
        _mesh[1] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
        _mesh[2] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), outerColor);
        _mesh[3] = sb::Vertex(sb::Vector2f(+extent.x - thickness, -extent.y + thickness), innerColor);
        _mesh[4] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), outerColor);
        _mesh[5] = sb::Vertex(sb::Vector2f(+extent.x - thickness, +extent.y - thickness), innerColor);
        _mesh[6] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), outerColor);
        _mesh[7] = sb::Vertex(sb::Vector2f(-extent.x + thickness, +extent.y - thickness), innerColor);
        _mesh[8] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
        _mesh[9] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
    }

    inline float getHeight() const { return _height; }

    void attachToTop(sb::Camera& targetCamera) {
        float middle = getCameraHeight(targetCamera) / 2 - getHeight() / 2;
        translate(0, middle);
    }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        states.transform *= getTransform();
        target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
    }
};

void demo3() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    sb::Mesh background(4, sb::PrimitiveType::TriangleStrip);
    Border2 border(0.9f * getCameraHeight(window.getCamera()), 0.01f);

    init1(background, window.getCamera());
    border.attachToTop(window.getCamera());

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear();
        window.draw(background.getVertices(), background.getPrimitiveType());
        window.draw(border);
        window.display();
    }
}

class Grid2 : public sb::Drawable {
private:
    sb::Mesh _mesh;

public:
    Grid2(std::size_t numColums, std::size_t numRows, float width, float height, float thickness, const sb::Color& color = sb::Color(1, 0, 0, 1))
        : _mesh((numRows + numColums + 2) * 6, sb::PrimitiveType::TriangleStrip)
    {
        addVerticalLines(numColums + 1, numRows + 1, width, height, thickness, color);
        addHorizontalLines(numRows + 1, width, height, thickness, color);
    }

    void addVerticalLines(std::size_t numColumns, std::size_t indexOffset, float width, float height, float thickness, const sb::Color& color) {
        float delta = width / (numColumns - 1);
        for (std::size_t i = 0; i < numColumns; i++)
            addVerticalLine(i + indexOffset, i * delta - width / 2, height, thickness, color);
    }

    void addVerticalLine(std::size_t index, float horizontalPosition, float height, float thickness, const sb::Color& color) {
        _mesh[index * 6 + 0] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, -height / 2), color);
        _mesh[index * 6 + 1] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, -height / 2), color);
        _mesh[index * 6 + 2] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, -height / 2), color);
        _mesh[index * 6 + 3] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, +height / 2), color);
        _mesh[index * 6 + 4] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, +height / 2), color);
        _mesh[index * 6 + 5] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, +height / 2), color);
    }

    void addHorizontalLines(std::size_t numRows, float width, float height, float thickness, const sb::Color& color) {
        float delta = height / (numRows - 1);
        for (std::size_t i = 0; i < numRows; i++)
            addHorizontalLine(i, i * delta - height / 2, width, thickness, color);
    }

    void addHorizontalLine(std::size_t index, float verticalPosition, float width, float thickness, const sb::Color& color) {
        _mesh[index * 6 + 0] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition - thickness / 2), color);
        _mesh[index * 6 + 1] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition - thickness / 2), color);
        _mesh[index * 6 + 2] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition + thickness / 2), color);
        _mesh[index * 6 + 3] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition - thickness / 2), color);
        _mesh[index * 6 + 4] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition + thickness / 2), color);
        _mesh[index * 6 + 5] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition + thickness / 2), color);
    }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        target.draw(_mesh.getVertices(), _mesh.getPrimitiveType());
    }
};

void demo4() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    Grid2 grid(10, 18, 1, getCameraHeight(window.getCamera()), 0.005f);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(grid);
        window.display();
    }
}

template <class T>
struct Animation {
	sb::Tween<T> tween;
	bool playing = false;
	float t = 0;

	void start() {
		t = 0;
		playing = true;
	}

	void stop() {
		t = tween.getDuration();
		playing = false;
	}

	void update(float ds) {
		if (isPlaying())
			t += ds;
	}

	inline T value() { return tween.value(t); }

	T targetValue() const { return tween.targetValue(); }

	bool isPlaying() { return playing && t < tween.getDuration(); }

};

typedef Animation<float> Animationf;
typedef Animation<sb::Vector2f> Animation2f;

template <class T>
struct Behaviour {
protected:
	T& parent;

	Behaviour(T& parent_) : parent(parent_)
	{ }

	Behaviour(const Behaviour& other) : parent(other.parent) {
	}

	Behaviour& operator=(const Behaviour& other) {
		return *this;
	}
};

class TransformEffects2 : public sb::Transformable, public Behaviour<sb::Transformable> {
	Animation2f _drift;
	Animationf _spin;

	Animation2f _driftOld;
	Animationf _spin2;
	Animationf _wobble;

public:
	TransformEffects2(sb::Transformable& parent) : Behaviour(parent) { }

	inline bool isMoving() { return _driftOld.isPlaying(); }

	inline bool isRotating() { return _spin2.isPlaying(); }

	inline bool isScaling() { return _wobble.isPlaying(); }

	sb::Vector2f getTargetPosition() { 
		return _driftOld.isPlaying() ? _driftOld.targetValue() : parent.getPosition(); 
	}

	float getTargetRotation() { 
		return _spin2.isPlaying() ? _spin2.targetValue() : parent.getRotation(); 
	}

	sb::Vector2f getTargetScale() { 
		return _wobble.isPlaying() ? sb::Vector2f(_wobble.targetValue()) : parent.getScale(); 
	}

	void driftOld(const sb::Vector2f& target, float duration = 0.15f) {
		const sb::Vector2f& pos = parent.getPosition();
		_driftOld.tween = sb::Tween2f().sineOut(pos, target, duration);
		_driftOld.start();
	}
	
	void driftTo(const sb::Vector2f& target, float duration = 0.15f) {
		sb::Vector2f effectPosition = parent.getPosition() + _drift.value();
		parent.setPosition(target);
		sb::Vector2f offset = effectPosition - parent.getPosition();
		_drift.tween = sb::Tween2f().sineOut(offset, sb::Vector2f(0), duration);
		_drift.start();
	}

	inline void driftBy(const sb::Vector2f& amount, float duration = 0.15f) {
		driftTo(parent.getPosition() + amount, duration);
	}

	void bounce(const sb::Vector2f& target, float duration = 0.5f) {
		const sb::Vector2f& pos = parent.getPosition();
		_driftOld.tween = sb::Tween2f().bounceOut(pos, target, duration);
		_driftOld.start();
	}

	void spinOld(float radians) {
		_spin2.tween = sb::Tweenf().bounceOut(parent.getRotation(), radians, 0.5f);
		_spin2.start();
	}

	void spinTo(float radians, float duration = 0.5f) {
		float effectRotation = parent.getRotation() + _spin.value();
		float offset = effectRotation - radians;
		parent.setRotation(radians);
		_spin.tween = sb::Tweenf().bounceOut(offset, 0, duration);
		_spin.start();
	}

	void spinBy(float radians, float duration = 0.5f) {
		spinTo(parent.getRotation() + radians, duration);
	}

	void pop() {
		if (!_wobble.isPlaying()) {
			float scale = parent.getScale().x;
			_wobble.tween = sb::Tweenf().quintInOut(scale, 1.4f * scale, 0.2f)
				.expoOut(1.4f * scale, scale, 0.4f);
			_wobble.start();
		}
	}

	void die(float duration = 0.8f) {
		_wobble.tween = sb::Tweenf().backInOut(parent.getScale().x, 0, duration);
		float angle = parent.getRotation();
		_spin2.tween = sb::Tweenf().backInOut(angle, angle + sb::random(-90, 90) * sb::ToRadian, 0.3f * duration);
		_wobble.start();
		_spin2.start();
	}

	void update(float ds) {
		SB_ERROR("deprecated");

		_drift.update(ds);
		_spin.update(ds);

		if (_drift.isPlaying())
			setPosition(_drift.value());
		if (_spin.isPlaying())
			setRotation(_spin.value());

		_driftOld.update(ds);
		_wobble.update(ds);
		_spin2.update(ds);

		if (_wobble.isPlaying())
			parent.setScale(_wobble.value());
		if (_driftOld.isPlaying())
			parent.setPosition(_driftOld.value());
		if (_spin2.isPlaying())
			parent.setRotation(_spin2.value());
	}
};

class SoundEffect {
	std::vector<float> _delays;
	std::string _assetPath;
	float _playbackIntervalSeconds;
	float _secondsToNextPlayback;

protected:
	static bool isGarbage(float delay) {
		return delay <= 0;
	}

	static std::map<std::string, sb::Sound>& getSoundPool() {
		static std::map<std::string, sb::Sound> soundPool;
		return soundPool;
	}

	sb::Sound& getSound() {
		return getSoundPool()[_assetPath];
	}

public:
	SoundEffect() : _playbackIntervalSeconds(0), _secondsToNextPlayback(0)
	{ }

	inline void setPlaybackInterval(float seconds) { _playbackIntervalSeconds = seconds; }

	void play(float delaySeconds = 0) {
		_delays.push_back(delaySeconds);
	}

	void loadFromAssetPool(const std::string& assetPath) {
		std::map<std::string, sb::Sound>& pool = getSoundPool();
		if (pool.find(assetPath) == pool.end())
			pool[assetPath].loadFromAsset(assetPath);

		_assetPath = assetPath;
	}

	void update(float ds) {
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
};

class TransformEffects : public sb::Transformable {
	Animation2f _drift;
	Animationf _wobble;
	Animationf _spin;

protected:
	void smoothMoveTo(const sb::Vector2f& end, sb::Transformable& target, sb::Tween2f& normalizedTween, float duration) {
		sb::Vector2f effectPosition = target.getPosition() + _drift.value();
		target.setPosition(end);
		sb::Vector2f offset = effectPosition - end;
		normalizedTween.stretchDuration(duration);
		normalizedTween.scale(offset);
		_drift.tween = normalizedTween;
		_drift.start();
	}

	inline void smoothMoveBy(const sb::Vector2f& amount, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
		driftTo(target.getPosition() + amount, target, duration);
	}

	void smoothRotateTo(float radians, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
		float effectRotation = target.getRotation() + _spin.value();
		float offset = effectRotation - radians;
		target.setRotation(radians);
		normalizedTween.stretchDuration(duration);
		normalizedTween.scale(offset);
		_spin.tween = normalizedTween;
		_spin.start();
	}

	inline void smoothRotateBy(float radians, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
		smoothRotateTo(target.getRotation() + radians, target, normalizedTween, duration);
	}

public:
	bool isPlaying() {
		return _drift.isPlaying() || _wobble.isPlaying() || _spin.isPlaying();
	}

	inline void driftTo(const sb::Vector2f& end, sb::Transformable& target, float duration = 0.2f) {
		sb::Tween2f tween = sb::Tween2f().sineOut(sb::Vector2f(1), sb::Vector2f(0), 1);
		smoothMoveTo(end, target, tween, duration);
	}

	inline void driftBy(const sb::Vector2f& amount, sb::Transformable& target, float duration = 0.2f) {
		driftTo(target.getPosition() + amount, target, duration);
	}

	void bounceTo(const sb::Vector2f& end, sb::Transformable& target, float duration = 0.5f) {
		sb::Tween2f tween = sb::Tween2f().bounceOut(sb::Vector2f(1), sb::Vector2f(0), 1);
		smoothMoveTo(end, target, tween, duration);
	}

	inline void bounceBy(const sb::Vector2f& amount, sb::Transformable& target, float duration = 0.2f) {
		bounceTo(target.getPosition() + amount, target, duration);
	}

	void pop(sb::Transformable& target, float duration = 0.15f) {
		_wobble.tween = sb::Tweenf().quintInOut(_wobble.value(), 0.4f, 0.2f).expoOut(0.4f, 0, 0.4f);
		_wobble.start();
	}

	void spinTo(float radians, sb::Transformable& target, float duration = 0.5f) {
		sb::Tweenf tween = sb::Tweenf().bounceOut(1, 0, 1);
		smoothRotateTo(radians, target, tween, duration);
	}

	void spinBy(float radians, sb::Transformable& target, float duration = 0.5f) {
		spinTo(target.getRotation() + radians, target, duration);
	}

	void implode(sb::Transformable& target, float duration = 0.8f) {
		_wobble.tween = sb::Tweenf().backIn(_wobble.value(), -1, duration);
		_wobble.start();

		sb::Tweenf tween = sb::Tweenf().backInOut(1, 0, 1);
		smoothRotateBy(sb::random(-45, 45) * sb::ToRadian, target, tween, duration * 0.3f);
	}

	void apply(sb::Transform& transform) {
		sb::Transform effectRotation(sb::Vector2f(0), sb::Vector2f(1), _spin.value());
		sb::Transform effectTranslation(_drift.value(), sb::Vector2f(1), 0);
		sb::Transform effectScale(sb::Vector2f(0), (1 + _wobble.value()) * sb::Vector2f(1), 0);
		transform = effectTranslation * transform * effectRotation * effectScale;
	}

	void stop() {
		_drift.stop();
		_wobble.stop();
		_spin.stop();
	}

	void update(float ds) {
		_drift.update(ds);
		_wobble.update(ds);
		_spin.update(ds);
	}
};

class Light {
    sb::Vector2f _direction;

public: 
    Light(const sb::Vector2f& direction = sb::Vector2f(0, -1)) : _direction(direction)
    { }

    inline void setDirection(const sb::Vector2f& direction) { _direction = direction; }

    inline const sb::Vector2f& getDirection() const { return _direction; }
};

sb::Color createColor(int r, int g = 255, int b = 255, int a = 255) {
	return sb::Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

std::map<char, sb::Color>& getBlockColors() {
	static const int alpha = 200;
	static std::map<char, sb::Color> colors = {
		{ 'i', createColor(0, 240, 240, alpha) },
		{ 'j', createColor(0, 0, 240, alpha) },
		{ 'l', createColor(240, 160, 0, alpha) },
		{ 'o', createColor(240, 240, 0, alpha) },
		{ 's', createColor(0, 240, 0, alpha) },
		{ 't', createColor(160, 0, 240, alpha) },
		{ 'z', createColor(240, 0, 0, alpha) },
		{ 'm', createColor(0, 240, 240, alpha) }
	};

	return colors;
}

inline sb::Color& getBlockColor(char type) { return getBlockColors()[type]; }

class BlockExplosion : public sb::ParticleSystem {
	bool _isActive;

protected:
	static sb::Texture& getTexture() {
		static sb::Texture texture("Textures/Particle1.png");
		return texture;
	}

public:
	BlockExplosion(size_t numParticles, char type = 'i') : ParticleSystem(numParticles), _isActive(false)
	{
		setLifetime(1);
		setEmissionRatePerSecond(0);
		setParticleLifetimeRange(2.f * sb::Vector2f(0.1f, 1));
		setParticleSpeedRange(sb::Vector2f(0.1f, 3));
		setEmissionType(sb::ParticleSystem::EmissionType::Random);
		setParticleSizeRange(sb::Vector2f(0.01f, 0.25f));
		setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
		getTexture().enableMipmap(true);
		setTexture(getTexture());
		setType(type);
		addBurst(0, 50);
	}

	inline bool isActive() { return _isActive; }

	void setType(char type) {
		type = tolower(type);
		setParticleColor(getBlockColor(type));
	}

	virtual void update(float ds) {
		if (_isActive) {
			ParticleSystem::update(ds);
			if (!isPlaying())
				_isActive = false;
		}
	}

	void explode() {
		if (!_isActive) {
			reset();
			_isActive = true;
		}
	}
};

class BlockCollisionEffect : public sb::ParticleSystem {
public: 
	enum struct Position { Left, Top, Right, Bottom };

private:
	enum class State { Idle, Playing };
	State _state;

protected:
	static sb::Texture& getTexture() {
		static sb::Texture texture("Textures/Particle1.png");
		return texture;
	}

	void setDirection(Position direction) {
		if (direction == Position::Left) {
			setPosition(-0.5f, 0);
			setRotation(-90 * sb::ToRadian);
		}
		else if (direction == Position::Top) {
			setPosition(0, 0.5f);
			setRotation(0 * sb::ToRadian);
		}
		else if (direction == Position::Right) {
			setPosition(0.5f, 0);
			setRotation(90 * sb::ToRadian);
		}
		else if (direction == Position::Bottom) {
			setPosition(0, -0.5f);
			setRotation(180 * sb::ToRadian);
		}
	}

public:
	BlockCollisionEffect(size_t numParticles) : sb::ParticleSystem(numParticles), _state(State::Idle)
	{
		setLifetime(0.5f);
		setEmissionRatePerSecond(0);
		setEmissionShape(sb::Box(1, 0.01f));
		setEmissionType(sb::ParticleSystem::EmissionType::Random);
		setParticleLifetimeRange(2.f * sb::Vector2f(0.1f, 0.5f));
		setParticleSpeedRange(sb::Vector2f(0.1f, 1));
		setParticleSizeRange(1.f * sb::Vector2f(0.01f, 0.13f));
		setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
		setParticleColor(createColor(139, 69, 19, 255));
		setTexture(getTexture());
		getTexture().enableMipmap(true);
	}

	inline bool isPlaying() const { return _state == State::Playing; }

	virtual void update(float ds) {
		ParticleSystem::update(ds);
		if (_state == State::Playing) {
			if (!sb::ParticleSystem::isPlaying())
				_state = State::Idle;
		}
	}

	void play(float delaySeconds = 0, Position direction = Position::Top) {
		setDirection(direction);

		if (_state != State::Playing) {
			clearBursts();
			addBurst(delaySeconds, 20);
			reset();
			_state = State::Playing;
		}
	}
};

class Block : public sb::Drawable, public sb::Transformable {
public:
	enum struct State {
		Alive, Imploding, Exploding, Garbage
	};

private:
    sb::Sprite _sprite;
    const Light* _light;
	State _state;
	BlockExplosion _explosion;
	BlockCollisionEffect _collisionEffect;
	bool _isExplosionEnabled;
	sb::Transform _implosionStartTransform;
	TransformEffects _effects;
	TransformEffects2 _effects2;

protected:
    static sb::Texture& getSheet() {
        static sb::Texture texture("Textures/Blocks.png");
        return texture;
    }

    inline const sb::IntRect getTextureArea(std::size_t x, std::size_t y) {
        static sb::Vector2i blockSize(128, 128);
        return sb::IntRect(x * blockSize.x, y * blockSize.y, blockSize.x, blockSize.y);
    }

    void setTexture(char type) {
        if (type == 'i')
            _sprite.setTexture(getSheet(), getTextureArea(1, 2));
        else if (type == 'j')
            _sprite.setTexture(getSheet(), getTextureArea(2, 2));
		else if (type == 'l')
			_sprite.setTexture(getSheet(), getTextureArea(1, 1));
		else if (type == 'o')
			_sprite.setTexture(getSheet(), getTextureArea(0, 2));
		else if (type == 's')
			_sprite.setTexture(getSheet(), getTextureArea(0, 1));
		else if (type == 't')
			_sprite.setTexture(getSheet(), getTextureArea(0, 0));
		else if (type == 'z')
			_sprite.setTexture(getSheet(), getTextureArea(2, 1));
        else if (type == 'm')
            _sprite.setTexture(getSheet(), getTextureArea(1, 2));
        else
            SB_ERROR("Invalid Tetromino type " << type);
    }

    sb::Vector2f transformDirection(const sb::Transform& transform, sb::Vector2f& dir) {
        const float* m = transform.getMatrix();
        return sb::Vector2f(
            m[0] * dir.x + m[3] * dir.y,
            m[1] * dir.x + m[4] * dir.y);
    }

    void updateLighting(const sb::Transform& transform) {
        if (_light) {
            sb::Vector2f up(0, 1);
            sb::Vector2f transformedUp = transformDirection(transform, up);
            float angle = sb::angle(-_light->getDirection(), transformedUp) + 45 * sb::ToRadian;
            angle = angle < 0 ? angle + 2 * sb::Pi : angle;
            int steps = int(angle / (90 * sb::ToRadian));
             _sprite.setRotation(-steps * 90 * sb::ToRadian);
        }
    }

	void implode(float ds) {
		if (!_effects.isPlaying()) {
			_explosion.explode();
			_implosionStartTransform = getTransform();
			_state = State::Exploding;
		}
	}

	void explode(float ds) {
		if (!_explosion.isActive() || !_isExplosionEnabled)
			_state = State::Garbage;
	}

public:
    Block(char type = 'i') : _light(NULL), _state(State::Alive), _explosion(128, type), _collisionEffect(128),
		_isExplosionEnabled(true), _effects2(*this)
    {
		_explosion.setScale(1.25f);
		_collisionEffect.setPosition(0, 0.5f);
        setType(type);
    }

	inline const State& getState() const { return _state; }

	inline TransformEffects2& getEffects2() { SB_WARNING("Obsolete!") ; return _effects2; }

	inline TransformEffects& getEffects() { return _effects; }

	inline const BlockCollisionEffect& getCollisionEffect() const { return _collisionEffect; }

	inline BlockCollisionEffect& getCollisionEffect() { return _collisionEffect; }
	
	inline void setColor(const sb::Color& color) { _sprite.setColor(color); }

    void setType(char type) {
        type = tolower(type);
        setTexture(type);
    }

    inline void setLight(const Light& light) { _light = &light; }

	inline void enableExplosion(bool enable) { _isExplosionEnabled = enable; }

	void die(float duration = 0.8f) {
		getEffects().implode(*this, duration);
		_state = State::Imploding;
	}

	void update(float ds) {
		_effects.update(ds);
		_explosion.update(ds);
		_collisionEffect.update(ds);

		if (_state == State::Imploding)
			implode(ds);
		if (_state == State::Exploding)
			explode(ds);
	}

	void drawExplosion(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= _implosionStartTransform;
		target.draw(_explosion, _implosionStartTransform);
	}

	void drawCollisionEffect(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_collisionEffect, states);
	}

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		_effects.apply(states.transform);
        updateLighting(states.transform);
        target.draw(_sprite, states);
    }
};

void init5(std::vector<Block>& blocks) {
    static std::vector<char> types = { 'i', 'j', 'm' };
    for (size_t i = 0; i < blocks.size(); i++) {
        blocks[i].setType(types[rand() % types.size()]);
        blocks[i].setPosition(sb::random2D(-0.5f, 0.5f));
        blocks[i].setScale(sb::random(0.1f, 0.2f));
    }
}

void draw5(sb::DrawBatch& batch, std::vector<Block>& blocks) {
    for (size_t i = 0; i < blocks.size(); i++)
        batch.draw(blocks[i]);
}

void demo5() {
    float aspect = 3 / 2.0f;
    sb::DrawBatch batch(512);
    sb::Window window(400, int(400 * aspect));
    std::vector<Block> blocks(20);

    init5(blocks);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        draw5(batch, blocks);
        window.draw(batch);
        window.display();
    }
}

inline int getWindowHeight(int width) {
    float aspect = 3 / 2.0f;
    return int(width * aspect);
}

template <class V>
void computeExtents(std::vector<V> v, V& lower, V& upper) {
	lower = upper = V(v[0].x, v[0].y);

	for (size_t i = 0; i < v.size(); i++) {
		V& val = v[i];
		if (val.x < lower.x)
			lower.x = val.x;
		if (val.x > upper.x)
			upper.x = val.x;
		if (val.y < lower.y)
			lower.y = val.y;
		if (val.y > upper.y)
			upper.y = val.y;
	}
}

sb::FloatRect getTransformedBounds(sb::FloatRect& bounds, const sb::Transform& transform) {
	std::vector<sb::Vector2f> edges(4);
	edges[0] = sb::Vector2f(bounds.left, bounds.bottom);
	edges[1] = sb::Vector2f(bounds.left + bounds.width, bounds.bottom);
	edges[2] = sb::Vector2f(bounds.left, bounds.bottom + bounds.height);
	edges[3] = sb::Vector2f(bounds.left + bounds.width, bounds.bottom + bounds.height);
	for (size_t i = 0; i < edges.size(); i++)
		edges[i] *= transform;

	sb::Vector2f min, max;
	computeExtents(edges, min, max);
	return sb::FloatRect(min.x, min.y, max.x - min.x, max.y - min.y);
}

class Boundable {
public:
	virtual sb::FloatRect getBounds() = 0;
};

class BubbleEffect : public sb::ParticleSystem {
	size_t _counter;

protected:
	static sb::Texture& getTexture() {
		static sb::Texture texture("Textures/Particle2.png");
		return texture;
	}

	void updateEmitter(const sb::FloatRect& bounds, float size) {
		if ((_counter % 15 == 0) && getState() == sb::ParticleSystem::State::Alive) {
			setPosition(bounds.left + 0.5f * bounds.width, bounds.top());
			setScale(size);
			setEmissionShape(sb::Box(bounds.width / size, 0.01f));

			float maxLength = std::max(bounds.width, bounds.height);
			float factor = (bounds.width / maxLength);
			setEmissionRatePerSecond(factor * 5);
			//std::cout << factor * 5 << std::endl;
		}

		_counter++;
	}

public:
	BubbleEffect(size_t maxParticles, const sb::Color& color) : sb::ParticleSystem(maxParticles), _counter(0) {
		setTexture(getTexture());
		setParticleColor(color);
		setEmissionRatePerSecond(0);
		setParticleSizeRange(1.5f * sb::Vector2f(0.1f, 0.13f));
		setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
		setEmissionShape(sb::Box(1, 0.01f));
		setEmissionType(sb::ParticleSystem::EmissionType::Directional);
		setEmissionDirection(sb::Vector2f(0, 1));
		setParticleSpeedRange(sb::Vector2f(2.5f));
		setScale(0.3f);
	}

	void setParticleSize(float size) {
		setParticleSizeRange(size * sb::Vector2f(0.1f, 0.13f));
	}

	void update(Boundable& boundable, Transformable& transformable, float ds) {
		sb::ParticleSystem::update(ds);
		updateEmitter(boundable.getBounds(), transformable.getScale().x);
	}
};

class Tetromino : public sb::Drawable, public sb::Transformable, public Boundable {
public:
	enum class State { Alive, Dying, Garbage };

private:
	char _type;
    std::vector<Block> _blocks;
    std::vector<sb::Vector2i> _blockPositions;
    sb::IntRect _blockBounds;
	TransformEffects _effects;
	TransformEffects2 _effects2;
	BubbleEffect _bubbleEffect;
	SoundEffect _collisionSound;
	State _state;

protected:
    void clear() {
        _blocks.clear();
        _blockPositions.clear();	
    }

    void computeBlockBounds() {
        sb::Vector2i minimum, maximum;
        computeExtents(_blockPositions, minimum, maximum);
        _blockBounds = sb::IntRect(minimum.x, minimum.y, maximum.x - minimum.x + 1, maximum.y - minimum.y + 1);
    }

    void createBlocks(const std::vector<sb::Vector2i>& positions, char type) {
        computeBlockBounds();
        for (size_t i = 0; i < positions.size(); i++) {
            Block block;
            block.setPosition((float)positions[i].x, (float)positions[i].y);
            block.setType(type);
            _blocks.push_back(block);
        }
    }

    void createBlocks(char type) {
		if (type == 'i')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(2, 0) };
		else if (type == 'j')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(1, -1) };
		else if (type == 'l')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(-1, -1) };
		else if (type == 'o')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(0, 1), sb::Vector2i(1, 1) };
		else if (type == 's')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1), sb::Vector2i(-1, -1) };
		else if (type == 't')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1) };
        else if (type == 'z')
            _blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(0, -1), sb::Vector2i(1, -1) };
        else if (type == 'm')
            _blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0) };

        createBlocks(_blockPositions, type);
    }

	bool isInInterval(float value, float min, float max) {
		return value >= min && value < max;
	}

	std::vector<int> getXBlockCoordinates() {
		std::vector<int> xCoordinates(_blockPositions.size());
		for (size_t i = 0; i < xCoordinates.size(); i++)
			xCoordinates[i] = _blockPositions[i].x;
		return xCoordinates;
	}

	std::vector<int> getYBlockCoordinates() {
		std::vector<int> yCoordinates(_blockPositions.size());
		for (size_t i = 0; i < yCoordinates.size(); i++)
			yCoordinates[i] = _blockPositions[i].y;
		return yCoordinates;
	}

	sb::Vector2i getMinimumBlockCoordinates() {
		std::vector<int> xCoordinates = getXBlockCoordinates();
		std::vector<int> yCoordinates = getYBlockCoordinates();
		auto minX = *std::min_element(xCoordinates.begin(), xCoordinates.end());
		auto minY = *std::min_element(yCoordinates.begin(), yCoordinates.end());
		return sb::Vector2i(minX, minY);
	}

	sb::Vector2i getMaximumBlockCoordinates() {
		std::vector<int> xCoordinates = getXBlockCoordinates();
		std::vector<int> yCoordinates = getYBlockCoordinates();
		auto maxX = *std::max_element(xCoordinates.begin(), xCoordinates.end());
		auto maxY = *std::max_element(yCoordinates.begin(), yCoordinates.end());
		return sb::Vector2i(maxX, maxY);
	}

	std::vector<size_t> getBlocksInRow(int y) {
		std::vector<size_t> blocksInRow;
		for (size_t i = 0; i < _blockPositions.size(); i++) {
			if (_blockPositions[i].y == y)
				blocksInRow.push_back(i);
		}

		return blocksInRow;
	}

	std::vector<size_t> getBlocksInColumn(int x) {
		std::vector<size_t> blocksInColumn;
		for (size_t i = 0; i < _blockPositions.size(); i++) {
			if (_blockPositions[i].x == x)
				blocksInColumn.push_back(i);
		}
		return blocksInColumn;
	}

	inline float getProperRotation(float degrees) {
		degrees = fmod(degrees, 360.f);
		degrees = degrees > 0 ? 360 - degrees : -degrees;
		return degrees;
	}

	std::vector<size_t> getBottomBlocks() {
		float rotation = getProperRotation(getRotation() * sb::ToDegrees);
		if (isInInterval(rotation, 45, 135))
			return getBlocksInColumn(getMaximumBlockCoordinates().x);
		else if (isInInterval(rotation, 135, 225))
			return getBlocksInRow(getMaximumBlockCoordinates().y);
		else if (isInInterval(rotation, 225, 315))
			return getBlocksInColumn(getMinimumBlockCoordinates().x);
		else
			return getBlocksInRow(getMinimumBlockCoordinates().y);
	}

	BlockCollisionEffect::Position getBlockCollisionEffectPosition() {
		float rotation = getProperRotation(getRotation() * sb::ToDegrees);
		if (isInInterval(rotation, 45, 135))
			return BlockCollisionEffect::Position::Right;
		else if (isInInterval(rotation, 135, 225))
			return BlockCollisionEffect::Position::Top;
		else if (isInInterval(rotation, 225, 315))
			return BlockCollisionEffect::Position::Left;
		else
			return BlockCollisionEffect::Position::Bottom;
	}

	bool isCollisionEffectPlaying() const {
		for (size_t i = 0; i < _blocks.size(); i++) {
			if (_blocks[i].getCollisionEffect().isPlaying())
				return true;
		}

		return false;
	}

	void updateDying() {
		if (!isCollisionEffectPlaying() && _bubbleEffect.getState() == sb::ParticleSystem::State::Garbage)
			_state = State::Garbage;
	}

public:
    Tetromino(char type = 'i') : _effects2(*this), _bubbleEffect(256, getBlockColors()[type]), _state(State::Alive) {
        setType(type);
		_collisionSound.loadFromAssetPool("Sounds/Collision.wav");
    }

	inline State getState() const { return _state; }

	inline TransformEffects& getEffects() { return _effects; }

	inline TransformEffects2& getEffects2() { SB_WARNING("obsolete"); return _effects2; }

	inline char getType() const { return _type; }

	inline static const std::vector<char> getTypes() {
		return{ 'i', 'j', 'l', 'o', 's', 't', 'z' };
	}

	const std::vector<sb::Vector2f> getBlockPositions() {
		std::vector<sb::Vector2f> blockPositions;
		blockPositions.reserve(_blocks.size());
		for (size_t i = 0; i < _blocks.size(); i++)
			blockPositions.push_back(getTransform() * _blocks[i].getPosition());

		return blockPositions;
	}

	inline const sb::Color& getColor() const { return getBlockColor(_type); }

	void setColor(const sb::Color& color) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].setColor(color);
	}

    void setType(char type) {
        clear();
		_type = tolower(type);
        createBlocks(_type);
    }

    inline void setLight(Light& light) { 
        for (std::size_t i = 0; i < _blocks.size(); i++)
            _blocks[i].setLight(light);
    }

	inline const sb::IntRect& getBlockBounds() const { return _blockBounds; }

    sb::FloatRect getBounds() {
        sb::Vector2f blockSize(1.f, 1.f);
        sb::FloatRect bounds(_blockBounds.left - 0.5f, _blockBounds.bottom - 0.5f, (float)_blockBounds.width, (float)_blockBounds.height);

		return getTransformedBounds(bounds, getTransform());
    }

	void die() {
		_bubbleEffect.die();
		_state = State::Dying;
	}

	void playCollisionEffect(float secondsDelay) {
		std::vector<size_t> bottomBlocks = getBottomBlocks();
		BlockCollisionEffect::Position effectPosition = getBlockCollisionEffectPosition();

		bool playSound = false;
		for (size_t i = 0; i < bottomBlocks.size(); i++) {
			BlockCollisionEffect& blockEffect = _blocks[bottomBlocks[i]].getCollisionEffect();
			playSound = playSound || !blockEffect.isPlaying();
			blockEffect.play(secondsDelay, effectPosition);
		}

		if (playSound)
			_collisionSound.play(secondsDelay);
	}

	void update(float ds) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].update(ds);

		_effects.update(ds);
		_bubbleEffect.update(*this, *this, ds);
		_collisionSound.update(ds);

		if (_state == State::Dying)
			updateDying();
	}

	void drawCollisionEffect(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();

		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].drawCollisionEffect(target, states);
	}

	void drawBubbleEffect(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		target.draw(_bubbleEffect, states);
	}

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		_effects.apply(states.transform);
        for (size_t i = 0; i < _blocks.size(); i++)
            target.draw(_blocks[i], states);
    }
};

void init6(std::vector<Tetromino>& tetrominos) {
    srand(385949385);
    static std::vector<char> types = { 'i', 'j' };
    for (size_t i = 0; i < tetrominos.size(); i++) {
        tetrominos[i].setType(types[rand() % types.size()]);
        tetrominos[i].setPosition(sb::random2D(-0.5f, 0.5f));
        tetrominos[i].setScale(sb::random(0.01f, 0.1f) * tetrominos[i].getScale());
    }
}

void draw6(sb::DrawBatch& batch, std::vector<Tetromino>& tetrominos) {
    for (size_t i = 0; i < tetrominos.size(); i++)
        batch.draw(tetrominos[i]);
}

void demo6() {
    sb::DrawBatch batch(512);
    sb::Window window(400, getWindowHeight(400));
    std::vector<Tetromino> tetrominos(10);

    init6(tetrominos);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        draw6(batch, tetrominos);
        window.draw(batch);
        window.display();
    }
}

namespace {
    float getSeconds() {
        static sb::Stopwatch sw;
        return sw.getElapsedSeconds();
    }

    static float lastElapsed = 0;

	void resetDeltaSeconds() {
		lastElapsed = getSeconds();
	}

    float getDeltaSeconds() {
        float elapsed = getSeconds();
        float delta = elapsed - lastElapsed;
        lastElapsed = elapsed;
        return delta;
    }
}

void update7(sb::Triangle& spotlight, Light& light, Block& block) {
    float t = getSeconds();
    sb::Vector2f relativePosition(0.2f * cos(-t), 0.2f * sin(-t));
    spotlight.setPosition(block.getPosition() + relativePosition);
    light.setDirection(-relativePosition);
}

void demo7() {
    sb::Window window(400, getWindowHeight(400));
    Light light1;
    Light light2;
    Block block1('i');
    Block block2('j');
    sb::Triangle spotlight1;

    block1.setScale(0.2f, 0.2f);
    block1.setPosition(-0.2f, 0.2f);
    block1.setLight(light1);
    block2.setScale(0.2f, 0.2f);
    block2.setPosition(0.2f, -0.2f);
    block2.setLight(light2);
    spotlight1.setScale(0.05f, 0.05f);

    while (window.isOpen()) {
        float ds = getDeltaSeconds();
        sb::Input::update();
        window.update();
        update7(spotlight1, light1, block1);
        block2.rotate(ds);

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(block1);
        window.draw(block2);
        window.draw(spotlight1);
        window.display();
    }
}

void init8a(Tetromino& tetromino, sb::Triangle& spotlight, Light& light) {
    tetromino.setScale(0.1f * tetromino.getScale());
    tetromino.setPosition(-0.2f, 0.2f);
    tetromino.setLight(light);
    spotlight.setScale(0.05f, 0.05f);
}

void init8b(Tetromino& tetromino, Light& light) {
    tetromino.setScale(0.1f * tetromino.getScale());
    tetromino.setPosition(0.2f, -0.2f);
    tetromino.setLight(light);
}

void update8(sb::Triangle& spotlight, Light& light, Tetromino& tetromino) {
    float t = getSeconds();
    sb::Vector2f relativePosition(0.2f * cos(-t), 0.2f * sin(-t));
    spotlight.setPosition(tetromino.getPosition() + relativePosition);
    light.setDirection(-relativePosition);
}

void demo8() {
    sb::Window window(400, getWindowHeight(400));
    Light light1;
    Light light2;
    sb::Triangle spotlight;
    Tetromino tetromino1('i');
    Tetromino tetromino2('j');

    init8a(tetromino1, spotlight, light1);
    init8b(tetromino2, light2);

    while (window.isOpen()) {
        float ds = getDeltaSeconds();
        sb::Input::update();
        window.update();
        update8(spotlight, light1, tetromino1);
        tetromino2.rotate(ds);

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(tetromino1);
        window.draw(tetromino2);
        window.draw(spotlight);
        window.display();
    }
}

class Line : public sb::Drawable, public sb::Transformable {
    std::vector<sb::Vector2f> _points;
    float _thickness;
    sb::Color _color;
    std::vector<sb::Vertex> _vertices;
    bool _verticesNeedUpdate;

protected:
    void addSegment(const sb::Vector2f& start, const sb::Vector2f& end) {
        float half = _thickness / 2;
        sb::Vector2f dist = end - start;
        sb::Vector2f perp = sb::Vector2f(-dist.y, dist.x).normalized();
        std::vector<sb::Vertex> segment(6);

        segment[0] = sb::Vertex(start - half * perp, _color);
        segment[1] = segment[0];
        segment[2] = sb::Vertex(start + half * perp, _color);
        segment[3] = sb::Vertex(end - half * perp, _color);
        segment[4] = sb::Vertex(end + half * perp, _color);
        segment[5] = segment[4];

        _vertices.insert(_vertices.end(), segment.begin(), segment.end());
    }

	void updateVertices() {
		_vertices.clear();
		for (size_t i = 1; i < _points.size(); i++)
			addSegment(_points[i - 1], _points[i]);

		_verticesNeedUpdate = false;
	}

public:
    Line(float thickness = 0.1f, const sb::Color& color = sb::Color(1, 0, 0, 1)) 
        : _thickness(thickness), _color(color), _verticesNeedUpdate(false) 
    { }

	const std::vector<sb::Vertex>& getVertices() { 
		if (_verticesNeedUpdate)
			updateVertices();
		return _vertices; 
	}

	inline void addPoint(const sb::Vector2f& point) { addPoint(point.x, point.y); }

    void addPoint(float x, float y) {
        _points.push_back(sb::Vector2f(x, y));
        _verticesNeedUpdate = true;
    }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        states.transform *= getTransform();
        if (_verticesNeedUpdate)
            updateVertices();

        target.draw(_vertices, sb::PrimitiveType::TriangleStrip, states);
    }
};

void computeOutline(const sb::FloatRect rect, Line& line) {
    line.addPoint(rect.left, rect.bottom);
    line.addPoint(rect.right(), rect.bottom);
    line.addPoint(rect.right(), rect.top());
    line.addPoint(rect.left, rect.top());
    line.addPoint(rect.left, rect.bottom);
}

void demo9() {
    sb::Window window(400, getWindowHeight(400));
    Line line(0.01f);
    sb::FloatRect bounds(-0.3f, -0.4f, 0.2f, 0.3f);
    computeOutline(bounds, line);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(line);
        window.display();
    }
}

void drawOutline(sb::DrawTarget& target, const sb::FloatRect& rect, float thickness = 0.01f, const sb::Color& color = sb::Color(1, 0, 0, 1)) {
	Line outline(thickness, color);
	computeOutline(rect, outline);
	target.draw(outline);
}

bool isTouchGoingDown(sb::Window& window, Tetromino& tetromino) {
	if (sb::Input::isTouchGoingDown(1)) {
		sb::Vector2f touch = sb::Input::getTouchPosition(window);
		if (tetromino.getBounds().contains(touch))
			return true;
	}

	return false;
}

void demo10() {
    sb::Window window(400, getWindowHeight(400));
    Light light;
    Tetromino tetromino('j');

	tetromino.setPosition(0.2f, 0.2f);
    tetromino.setScale(0.15f);
    tetromino.setLight(light);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();
        if (isTouchGoingDown(window, tetromino))
            tetromino.rotate(-90 * sb::ToRadian);

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(tetromino);
		drawOutline(window, tetromino.getBounds(), 0.01f);

        window.display();
    }
}

bool isTouchDown(sb::Window& window, Tetromino& tetromino) {
	if (sb::Input::isTouchDown(1)) {
		sb::Vector2f touch = sb::Input::getTouchPosition(window);
		if (tetromino.getBounds().contains(touch))
			return true;
	}

	return false;
}

void demo11() {
	sb::Window window(400, getWindowHeight(400));
	Light light;
	Tetromino tetromino('j');

	tetromino.setScale(0.15f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		if (isTouchDown(window, tetromino))
			tetromino.setPosition(sb::Input::getTouchPosition(window).x, tetromino.getPosition().y);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		drawOutline(window, tetromino.getBounds(), 0.01f);
		
		window.display();
	}
}

class Grid : public sb::Drawable, public sb::Transformable {
	sb::Vector2i _gridSize;
	float _thickness;
	sb::Color _color;
	std::vector<sb::Vertex> _vertices;

protected:
	void computeLine(const sb::Vector2f& start, const sb::Vector2f& end) {
		Line line(_thickness, _color);
		line.addPoint(start);
		line.addPoint(end);
		_vertices.insert(_vertices.end(), line.getVertices().begin(), line.getVertices().end());
	}

	inline sb::Vector2f getSize() {
		return sb::Vector2f(1, _gridSize.y / float(_gridSize.x));
	}

	void computeVerticalLines() {
		sb::Vector2f size = getSize();
		sb::Vector2f halfSize = 0.5f * size;
		float delta = size.x / _gridSize.x;
		for (int i = 0; i <= _gridSize.x; i++) {
			sb::Vector2f start(i * delta - halfSize.x, -halfSize.y);
			sb::Vector2f end(i * delta - halfSize.x, +halfSize.y);
			computeLine(start, end);
		}
	}

	void computeHorizontalLines() {
		sb::Vector2f size = getSize();
		sb::Vector2f halfSize = 0.5f * size;
		float delta = size.y / _gridSize.y;
		for (int i = 0; i <= _gridSize.y; i++) {
			sb::Vector2f start(-halfSize.x, i * delta - halfSize.y);
			sb::Vector2f end(+halfSize.x, i * delta -halfSize.y);
			computeLine(start, end);
		}
	}

	void computeLines() {
		_vertices.clear();
		sb::Vector2f delta(1.f / _gridSize.x, 1.f / _gridSize.y);
		
		computeVerticalLines();
		computeHorizontalLines();
	}

public:
	Grid(sb::Vector2i gridSize, float thickness = 0.1f, const sb::Color& color = sb::Color(1, 0, 0, 1)) 
		: _gridSize(gridSize), _thickness(thickness), _color(color)
	{
		computeLines();
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_vertices, sb::PrimitiveType::TriangleStrip, states);
	}
};

void demo12() {
	sb::Window window(400, getWindowHeight(400));
	Grid grid(sb::Vector2i(10, 15), 0.005f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(grid);
		window.display();
	}
}

void demo13() {
	sb::Window window(400, getWindowHeight(400));
	Tetromino tetromino('m');

	tetromino.setScale(0.25f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

class StripedQuad : public sb::Drawable, public sb::Transformable {
	sb::Mesh _mesh;
	size_t _numStripes;
	float _alpha;

protected:
	void createMesh(const sb::Vector2f& extent) {
		float stripeWidth = 2 * extent.x / _numStripes;
		static const sb::Color black(0, 0, 0, _alpha);
		static const sb::Color white(1, 1, 1, 0);

		for (size_t i = 0; i < _numStripes; i++) {
			bool isEven = i % 2 == 0;
			const sb::Color& stripeBottomColor = isEven ? black : white;
			const sb::Color& stripeTopColor = isEven ? black : white;
			float stripeLeft = -extent.x + i * stripeWidth;
			float stripeRight = stripeLeft + stripeWidth;
			_mesh[i * 6 + 0] = sb::Vertex(sb::Vector2f(stripeLeft, -extent.y), stripeBottomColor);
			_mesh[i * 6 + 1] = sb::Vertex(sb::Vector2f(stripeLeft, -extent.y), stripeBottomColor);
			_mesh[i * 6 + 2] = sb::Vertex(sb::Vector2f(stripeLeft, +extent.y), stripeTopColor);
			_mesh[i * 6 + 3] = sb::Vertex(sb::Vector2f(stripeRight, -extent.y), stripeBottomColor);
			_mesh[i * 6 + 4] = sb::Vertex(sb::Vector2f(stripeRight, +extent.y), stripeTopColor);
			_mesh[i * 6 + 5] = sb::Vertex(sb::Vector2f(stripeRight, +extent.y), stripeTopColor);
		}
	}

public:
	StripedQuad(size_t numStripes = 1, float alpha = 0.04f) : _mesh(6 * numStripes, sb::PrimitiveType::TriangleStrip),
		_numStripes(numStripes), _alpha(alpha)
	{
		createMesh(sb::Vector2f(0.5f, 0.5f));
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
	}
};

class Border : public sb::Drawable, public sb::Transformable {
private:
	sb::Mesh _mesh;

public:
	Border(float thickness, float height = 1, bool addThicknessToSize = false, 
		const sb::Color& innerColor = createColor(236, 140, 105, 100), 
		const sb::Color& outerColor = createColor(237, 110, 160, 100))
		: _mesh(10, sb::PrimitiveType::TriangleStrip)
	{
		sb::Vector2f extent(0.5f, height / 2);
		extent = addThicknessToSize ? extent + sb::Vector2f(thickness / 2) : extent;
		_mesh[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
		_mesh[1] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
		_mesh[2] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), outerColor);
		_mesh[3] = sb::Vertex(sb::Vector2f(+extent.x - thickness, -extent.y + thickness), innerColor);
		_mesh[4] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), outerColor);
		_mesh[5] = sb::Vertex(sb::Vector2f(+extent.x - thickness, +extent.y - thickness), innerColor);
		_mesh[6] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), outerColor);
		_mesh[7] = sb::Vertex(sb::Vector2f(-extent.x + thickness, +extent.y - thickness), innerColor);
		_mesh[8] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
		_mesh[9] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
	}
};

class Board : public sb::Drawable, public sb::Transformable {
	sb::DrawBatch _batch;
	sb::Vector2i _boardSize;

	Grid _grid;
	StripedQuad _stripes;
	Border _border;
	bool _isGridEnabled;
	std::vector<Block> _blocks;
	Light _light;
	Tetromino _tetromino;
	bool _hasTetromino;
	float _stepIntervalInSeconds;
	float _secondsSinceLastStep;
	bool _isFull;
	size_t _linesCleared;
	bool _isAutodropEnabled;
	std::vector<Tetromino> _dyingTetrominoes;
	SoundEffect _explosionSound;
	SoundEffect _popSound;
	SoundEffect _spinSound;

protected:

	void createTetromino(Tetromino& tetromino, const sb::Vector2i& boardPosition) {
		tetromino.setPosition(boardToWorldPosition(boardPosition));
		tetromino.setScale(getCellSize());
		_tetromino = tetromino;
		_tetromino.setLight(_light);
		_hasTetromino = true;
	}

	void createRandomTetromino() {
		const std::vector<char> types = Tetromino::getTypes();
		createTetromino(types[rand() % types.size()]);
		if (isInvalid(_tetromino))
			die();
	}

	sb::Vector2f boardToWorldPosition(const sb::Vector2i& boardPos) {
		sb::Vector2f size(1, _boardSize.y / float(_boardSize.x));
		sb::Vector2f halfSize = 0.5f * size;
		sb::Vector2f delta(size.x / _boardSize.x, size.y / _boardSize.y);
		return sb::Vector2f(-halfSize.x + (boardPos.x + 0.5f) * delta.x,
			-halfSize.y + (boardPos.y + 0.5f) * delta.y);
	}

	inline int floori(float x) { return int(floor(x)); }

	sb::Vector2i worldToBoardPosition(const sb::Vector2f& worldPos) {
		sb::Vector2f size(1, _boardSize.y / float(_boardSize.x));
		sb::Vector2f delta(size.x / _boardSize.x, size.y / _boardSize.y);
		sb::Vector2f adjustedPos = worldPos + 0.5f * size;
		auto result = sb::Vector2i(floori(adjustedPos.x / delta.x), floori(adjustedPos.y / delta.y));
		return result;
	}

	sb::Vector2f getCellSize() {
		return sb::Vector2f(1.f / _boardSize.x, 1.f / _boardSize.x);
	}

	void moveBy(Tetromino& tetromino, const sb::Vector2i& translation) {
		sb::Vector2i position = worldToBoardPosition(tetromino.getPosition());
		position += translation;
		tetromino.setPosition(boardToWorldPosition(position));
	}

	void driftBy(Tetromino& tetromino, const sb::Vector2i& translation, float effectSeconds = .2f) {
		sb::Vector2i position = worldToBoardPosition(tetromino.getPosition());
		position += translation;
		tetromino.getEffects().driftTo(boardToWorldPosition(position), tetromino, effectSeconds);
	}

	bool isOccupied(const sb::Vector2i& position) {
		for (size_t i = 0; i < _blocks.size(); i++) {
			const sb::Vector2i blockPosition = worldToBoardPosition(_blocks[i].getPosition());
			bool isBlockAlive = _blocks[i].getState() == Block::State::Alive;
			if (blockPosition == position && isBlockAlive)
				return true;
		}

		return false;
	}

	bool isOutsideBoard(const sb::Vector2i& position) {
		return position.x < 0 || position.x >= _boardSize.x || 
			position.y < 0 || position.y >= _boardSize.y;
	}

	bool isInvalid(Tetromino& tetromino) {
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();

		for (std::size_t i = 0; i < blockPositions.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(blockPositions[i]);
			if (isOccupied(boardPosition) || isOutsideBoard(boardPosition))
				return true;
		}

		return false;
	}

	inline bool isValid(Tetromino& tetromino) { 
		return !isInvalid(tetromino); 
	}

	void freeze(Tetromino& tetromino) {
		char type = tetromino.getType();
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();
		_hasTetromino = false;
		tetromino.die();
		_dyingTetrominoes.push_back(_tetromino);

		for (size_t i = 0; i < blockPositions.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(blockPositions[i]);
			createBlock(type, boardPosition);
		}
	}

	bool isLineFull(size_t y) {
		for (int x = 0; x < _boardSize.x; x++) {
			if (!isOccupied(sb::Vector2i(x, y)))
				return false;
		}

		return true;
	}

	void implodeLine(size_t y) {
		bool hasImmediateExplosion = false;
		bool hasDelayedExplosion = false;
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPos = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPos.y == y) {
				bool isAboveOccupied = isOccupied(sb::Vector2i(boardPos.x, boardPos.y + 1));
				hasImmediateExplosion = hasImmediateExplosion || isAboveOccupied;
				hasDelayedExplosion = hasDelayedExplosion || !isAboveOccupied;
				float duration = isAboveOccupied ? 0.01f : 0.8f;
				_blocks[i].die(duration);
			}
		}

		if (hasImmediateExplosion)
			_explosionSound.play(0.01f);
		if (hasDelayedExplosion)
			_explosionSound.play(0.8f);
	}
	
	void dropBlocksAboveLine(int y) {
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPos = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPos.y > y) 
				bounceBy(_blocks[i], sb::Vector2i(0, -1));
		}	
	}

	void clearLine(size_t y) {
		implodeLine(y);
		dropBlocksAboveLine(y);
		_linesCleared++;
	}

	void clearLines() {
		for (int y = 0; y < _boardSize.y; y++) {
			while (isLineFull(y))
				clearLine(y);
		}
	}

	void drop(Tetromino& tetromino) {
		float effectSeconds = 0.2f;
		driftBy(tetromino, sb::Vector2i(0, -1), effectSeconds);
		if (isInFinalPosition(tetromino)) 
			tetromino.playCollisionEffect(effectSeconds * 0.7f);

		if (isInvalid(tetromino)) {
			driftBy(tetromino, sb::Vector2i(0, 1));
			freeze(tetromino);
			clearLines();
			createRandomTetromino();
		}
	}

	void step(Tetromino& tetromino, float ds) {
		_secondsSinceLastStep += ds;

		while (_secondsSinceLastStep >= _stepIntervalInSeconds) {
			if (_isAutodropEnabled)
				drop(tetromino);
			_secondsSinceLastStep -= _stepIntervalInSeconds;
		}
	}

	bool isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos, const sb::Vector2i& dir) {
		sb::Vector2i boardPosTo = worldToBoardPosition(targetPos);
		Tetromino testTetromino = tetromino;

		while (isValid(testTetromino)) {
			sb::Vector2i pos = worldToBoardPosition(testTetromino.getPosition());
			if (pos == boardPosTo)
				return true;
			moveBy(testTetromino, dir);
		}

		return false;
	}

	bool isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos) {
		static const std::vector<sb::Vector2i> directions = { sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1) };

		for (size_t i = 0; i < directions.size(); i++) {
			if (isReachable(tetromino, targetPos, directions[i]))
				return true;
		}

		return false;
	}

	static bool isGarbage(const Block& block) { 
		return block.getState() == Block::State::Garbage; 
	}

	static bool isTetrominoGarbage(Tetromino& tetromino) {
		return tetromino.getState() == Tetromino::State::Garbage;
	}

	void dispose() {
		_blocks.erase(std::remove_if(_blocks.begin(), _blocks.end(), isGarbage), _blocks.end());	
		_dyingTetrominoes.erase(std::remove_if(_dyingTetrominoes.begin(), _dyingTetrominoes.end(), isTetrominoGarbage), _dyingTetrominoes.end());
	}

	void bounceBy(Block& block, const sb::Vector2i& translation, float duration = 0.5f) {
		sb::Vector2i position = worldToBoardPosition(block.getPosition());
		position += translation;
		block.getEffects().bounceTo(boardToWorldPosition(position), block, duration);
	}

	int getLowestLineWithBlocks() {
		int lowest = -1;
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPosition.y < lowest || lowest < 0)
				lowest = boardPosition.y;
		}

		return lowest;
	}

	bool isInFinalPosition(Tetromino& tetromino) {
		if (isInvalid(tetromino))
			return false;

		Tetromino testTetromino = tetromino;
		driftBy(testTetromino, sb::Vector2i(0, -1));
		return isInvalid(testTetromino);
	}

public:
	Board(const sb::Vector2i& boardSize) 
		: _batch(1024), _boardSize(boardSize), _grid(boardSize, 0.01f), _stripes(boardSize.x, 0.03f),
		_border(0.005f, boardSize.y / (float)boardSize.x, true), _isGridEnabled(false), _hasTetromino(false), 
		_stepIntervalInSeconds(0.5f), _secondsSinceLastStep(0), _isFull(false), _linesCleared(0), _isAutodropEnabled(true)
	{
		_stripes.setScale(1, boardSize.y / (float)boardSize.x);
		_explosionSound.loadFromAssetPool("Sounds/Explosion.wav");
		_popSound.loadFromAssetPool("Sounds/Grab.wav");
		_spinSound.loadFromAssetPool("Sounds/Rotate.wav");
		_spinSound.setPlaybackInterval(0.1f);
	}

	inline bool isDead() const { return _isFull; }

	inline bool hasTetromino() const { return _hasTetromino; }

	inline Tetromino& getTetromino() { return _tetromino; }

	inline std::vector<Block>& getBlocks() { return _blocks; }

	inline const sb::Vector2i& getBoardSize() const { return _boardSize; }

	inline size_t getLinesCleared() const { return _linesCleared; }

	sb::Vector2f getSize() const {
		float inverseAspect = _boardSize.y / float(_boardSize.x);
		return sb::Vector2f(getScale().x * 1.f, getScale().y * inverseAspect);
	}

	sb::FloatRect getBounds() {
		sb::Vector2f size = getSize();
		sb::FloatRect bounds(-size.x / 2, -size.y / 2, size.x, size.y);
		
		return getTransformedBounds(bounds, getTransform());
	}

	inline void showGrid(bool enabled) { _isGridEnabled = enabled; }

	inline void enableAutodrop(bool enable) { _isAutodropEnabled = enable; }

	inline void setStepInterval(float dropIntervalInSeconds) { _stepIntervalInSeconds = dropIntervalInSeconds; }

	void die() {
		//std::cout << "You, sir, are a dead man!" << std::endl;
		_hasTetromino = false;
		_isFull = true;
	}

	void createBlock(char type, const sb::Vector2i& position) {
		Block block(type);
		block.setPosition(boardToWorldPosition(position));
		block.setScale(getCellSize());
		_blocks.push_back(block);
	}

	void createTetromino(char type, const sb::Vector2i& boardPosition) {
		Tetromino tetromino(type);
		createTetromino(tetromino, boardPosition);
	}

	void createTetromino(char type) {
		Tetromino tetromino(type);
		sb::Vector2i boardPosition(_boardSize.x / 2 - (_boardSize.x % 2 ? 0 : 1), 
			_boardSize.y - tetromino.getBlockBounds().top());
		createTetromino(tetromino, boardPosition);
	}

	void driftTetrominoTo(sb::Vector2f pos, float effectSeconds = 0.2f) {
		sb::Vector2f previousPos = _tetromino.getPosition();
		sb::Vector2f newPos = getTransform() * pos;
		newPos.y = std::min(previousPos.y, newPos.y);

		if (isReachable(_tetromino, newPos)) {
			sb::Vector2i boardPos = worldToBoardPosition(newPos);
			sb::Vector2f worldPos = boardToWorldPosition(boardPos);
			_tetromino.getEffects().driftTo(worldPos, _tetromino, effectSeconds);
		}
	}

	void driftTetrominoBy(const sb::Vector2i& translation) {
		sb::Vector2f cellSize = getCellSize();
		sb::Vector2f worldTranslation(translation.x * cellSize.x, translation.y * cellSize.y);
		sb::Vector2f end = _tetromino.getPosition() + worldTranslation;
		float effectSeconds = 0.2f;
		driftTetrominoTo(end, effectSeconds);

		if (isInFinalPosition(_tetromino))
			_tetromino.playCollisionEffect(effectSeconds * 0.7f);
	}

	inline void driftTetrominoBy(int x, int y) { driftTetrominoBy(sb::Vector2i(x, y)); }

	void spinTetromino() {
		_tetromino.getEffects().spinBy(-90 * sb::ToRadian, _tetromino, 0.75f);

		if (isInvalid(_tetromino))
			_tetromino.getEffects().spinBy(90 * sb::ToRadian, _tetromino);
		else
			_spinSound.play();
	}

	void popTetromino() {
		_tetromino.getEffects().pop(_tetromino);
		_popSound.play();
	}

	void dropTetromino() {
		drop(_tetromino);
	}

	void hardDropTetromino() {
		Tetromino projection = computeProjection();
		_secondsSinceLastStep = 0;
		float effectSeconds = 0.5f;
		_tetromino.getEffects().bounceTo(projection.getPosition(), _tetromino, effectSeconds);
		_tetromino.playCollisionEffect(effectSeconds * 0.6f);
	}

	void dropBlocks() {
		for (size_t i = 0; i < _blocks.size(); i++)
			bounceBy(_blocks[i], sb::Vector2i(0, -1));
	}

	void implodeBlocks(float duration = 0.8f) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].die(duration);
	}

	void clearLowestLineWithBlocks() {
		int y = getLowestLineWithBlocks();
		if (y >= 0)
			clearLine(y);
	}

	Tetromino computeProjection() {
		Tetromino projection(_tetromino);
		projection.getEffects().stop();
		while (!isInvalid(projection))
			moveBy(projection, sb::Vector2i(0, -1));
		moveBy(projection, sb::Vector2i(0, 1));

		return projection;
	}

	void updateEntities(float ds) {
		dispose();

		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].update(ds);

		if (_hasTetromino)
			_tetromino.update(ds);
		else
			createRandomTetromino();

		for (size_t i = 0; i < _dyingTetrominoes.size(); i++)
			_dyingTetrominoes[i].update(ds);

		//std::cout << _dyingTetrominoes.size() << std::endl;
	}

	void updateSounds(float ds) {
		_explosionSound.update(ds);
		_popSound.update(ds);
		_spinSound.update(ds);
	}

	void update(float ds) {
		updateEntities(ds);
		updateSounds(ds);

		if (!_isFull) {
			step(_tetromino, ds);
		}
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();

		target.draw(_stripes, states);

		if (_isGridEnabled)
			target.draw(_grid, states);

		for (size_t i = 0; i < _blocks.size(); i++) 
			_batch.draw(_blocks[i], states);
		target.draw(_batch);
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].drawExplosion(target, states);

		if (_hasTetromino) {
			Tetromino projection = computeProjection();
			projection.setColor(sb::Color(1, 1, 1, 0.25f));
			_batch.draw(projection, states);
			_batch.draw(_tetromino, states);
			target.draw(_batch);
			_tetromino.drawCollisionEffect(target, states);
			_tetromino.drawBubbleEffect(target, states);
		}

		for (size_t i = 0; i < _dyingTetrominoes.size(); i++) {
			_dyingTetrominoes[i].drawCollisionEffect(target, states);
			_dyingTetrominoes[i].drawBubbleEffect(target, states);
		}

		target.draw(_border, states);
	}
};

void demo14() {
	sb::Window window(400, getWindowHeight(400));
	Board board(sb::Vector2i(10, 15));

	board.createBlock('j', sb::Vector2i(5, 5));
	board.createTetromino('i');

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

sb::Vector2i getWindowSize(size_t width, float inverseAspect) {
	return sb::Vector2i(width, int(inverseAspect * width));
}

void adjustCameraToBoard(sb::Camera& camera, const Board& board) {
	float cameraAspect = camera.getAspectRatio();
	const sb::Vector2f& boardSize = board.getSize();
	float boardAspect = boardSize.x / boardSize.y;

	if (boardAspect > cameraAspect)
		camera.setWidth(board.getSize().x);
	else
		camera.setHeight(board.getSize().y);	
}

void demo15() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);
	board.createBlock('j', sb::Vector2i(5, 5));
	board.createTetromino('i');

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo16() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);
	board.createBlock('j', sb::Vector2i(5, 10));
	board.createTetromino('i');

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo17() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 5));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo18() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Grid grid(sb::Vector2i(2, 2), 0.01f);
	Tetromino tetromino('m');

	tetromino.setPosition(-0.25f, 0.25f);
	tetromino.setScale(0.5f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			tetromino.rotate(-90 * sb::ToRadian);
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();
		std::cout << blockPositions[1].x << " " << blockPositions[1].y << std::endl;
		
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(grid);
		window.draw(tetromino);
		window.display();
	}
}

bool isTetrominoTouchGoingDown(sb::Window&window, Board& board) {
	if (board.hasTetromino())
		return isTouchGoingDown(window, board.getTetromino());

	return false;
}

void demo19() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);
	board.createTetromino('i', sb::Vector2i(4, 2));
	board.createBlock('j', sb::Vector2i(2, 2));

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r) || 
			isTetrominoTouchGoingDown(window, board))
			board.spinTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void input20(sb::Window& window, Board& board) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::r) ||
		isTetrominoTouchGoingDown(window, board))
		board.spinTetromino();
}

void demo20() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input20(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void addBlocks(Board& board) {
	for (int i = 0; i < board.getBoardSize().y; i++)
		board.createBlock('m', sb::Vector2i(1, i));
}

bool isTouchDown(sb::Window& window, Board& board) {
	if (sb::Input::isTouchDown(1)) 
		return board.getBounds().contains(sb::Input::getTouchPosition(window)); 
	
	return false;
}

void input21(sb::Window& window, Board& board) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
		board.driftTetrominoBy(-1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
		board.driftTetrominoBy(1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
		board.driftTetrominoBy(0, -1);
	if (isTouchDown(window, board))
	board.driftTetrominoTo(sb::Input::getTouchPosition(window));
}

void demo21() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);
	addBlocks(board);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input21(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		drawOutline(window, board.getBounds(), 0.02f, sb::Color(0, 1, 0, 1));
		window.display();
	}
}

void input22(sb::Window& window, Board& board) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
		board.driftTetrominoBy(-1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
		board.driftTetrominoBy(1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
		board.driftTetrominoBy(0, -1);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
		board.spinTetromino();
	if (isTouchDown(window, board))
		board.driftTetrominoTo(sb::Input::getTouchPosition(window));
}

void demo22() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input22(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void draw23(sb::DrawTarget& target, Board& board) {
	Tetromino projection = board.computeProjection();
	drawOutline(target, projection.getBounds(), 0.01f, sb::Color(0, 1, 0, 1));
}

void input23(sb::Window& window, Board& board) {
	bool quickdrop = false;

	if (sb::Input::isTouchDown(1)) {
		Tetromino projection = board.computeProjection();
		const sb::Vector2f touch = sb::Input::getTouchPosition(window);
		if (projection.getBounds().contains(touch)) {
			board.hardDropTetromino();
			quickdrop = true;
		}
	}

	if (sb::Input::isKeyGoingDown(sb::KeyCode::Space)) {
		board.hardDropTetromino();
		quickdrop = true;
	}
		
	if (!quickdrop)
		input22(window, board);
}

void demo23() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input23(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		draw23(window, board);
		window.display();
	}
}

void demo24() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(4, 4));

	adjustCameraToBoard(window.getCamera(), board);
	board.createBlock('m', sb::Vector2i(0, 0));
	board.createBlock('m', sb::Vector2i(1, 0));
	board.createBlock('m', sb::Vector2i(2, 0));
	board.createBlock('m', sb::Vector2i(1, 1));
	board.createBlock('m', sb::Vector2i(0, 2));
	board.createBlock('m', sb::Vector2i(1, 2));
	board.createBlock('m', sb::Vector2i(2, 2));
	board.createBlock('m', sb::Vector2i(1, 3));
	board.createTetromino('i', sb::Vector2i(3, 2));
	board.spinTetromino();
	board.showGrid(true);
	board.setStepInterval(1);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo25() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input22(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

bool isProjectionTouchGoingDown(sb::Window& window, Board& board) {
	if (sb::Input::isTouchGoingDown(1)) {
		Tetromino projection = board.computeProjection();
		const sb::Vector2f touch = sb::Input::getTouchPosition(window);
		return projection.getBounds().contains(touch);
	}

	return false;
}

sb::Vector2f getSwipe(sb::Window& window, float ds) {
	static sb::Vector2f start;
	static float secondsSinceStart;
	static bool swiping = false;
	const float maxSwipeSeconds = 0.75f;

	bool swipeStarting = sb::Input::isTouchGoingDown(1);
	bool swipeEnding = swiping && !sb::Input::isTouchGoingDown(1) && !sb::Input::isTouchDown(1);

	if (swipeStarting) {
		start = sb::Input::getTouchPosition(window);
		swiping = true;
		secondsSinceStart = 0;
		return sb::Vector2f(0, 0);
	}
	else if (swipeEnding) {
		swiping = false;
		if (secondsSinceStart <= maxSwipeSeconds)
			return sb::Input::getTouchPosition(window) - start;
	}

	secondsSinceStart += ds;
	return sb::Vector2f(0, 0);
}

namespace {
	class Game : public sb::Drawable {
		Board _board;
		size_t _linesCleared;
		size_t _score;
		sb::Vector2f touchOffset;

	protected:
		void updateScore() {
			size_t previousLinesCleared = _linesCleared;
			_linesCleared = _board.getLinesCleared();

			size_t newLinesCleared = _linesCleared - previousLinesCleared;
			if (newLinesCleared > 0) {
				_score += (size_t)pow(10, newLinesCleared);
				SB_MESSAGE("Score: " << _score);
			}
		}

		void updateStepInterval() {
			size_t steps = _board.getLinesCleared() / 10;
			float stepInterval = std::max(0.5f - 0.1f * steps, 0.1f);
			_board.setStepInterval(stepInterval);
		}

	public:
		Game(const sb::Vector2i& boardSize) : _board(boardSize), _linesCleared(0), _score(0)
		{ }

		inline Board& getBoard() { return _board; }

		void input(sb::Window& window, float ds) {
			if (sb::Input::isTouchGoingDown(1))
				touchOffset = _board.getTetromino().getPosition() - sb::Input::getTouchPosition(window);
			if (sb::Input::isTouchDown(1))
				_board.driftTetrominoTo(sb::Input::getTouchPosition(window) + touchOffset);
			if (getSwipe(window, ds).y > 0.05f * window.getInverseAspect())
				_board.spinTetromino();
			if (isProjectionTouchGoingDown(window, _board))
				_board.hardDropTetromino();

			if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
				_board.driftTetrominoBy(-1, 0);
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
				_board.driftTetrominoBy(1, 0);
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
				_board.driftTetrominoBy(0, -1);
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
				_board.spinTetromino();
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Space))
				_board.hardDropTetromino();
		}

		void update(float ds) {
			_board.update(ds);

			updateScore();
			updateStepInterval();
		}

		virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
			target.draw(_board, states);
		}
	};
}

void demo26() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Game game(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), game.getBoard());
	game.getBoard().showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		game.update(ds);
		game.input(window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(game);
		window.display();
	}
}

void demo27() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Game game(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), game.getBoard());
	game.getBoard().showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		game.update(ds);
		game.input(window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(game);
		window.display();
	}
}

void demo28() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Quad quad;
	std::vector<sb::Tween2f> tweens(32);
	size_t currentTween = 0;
	float time = 0;

	quad.setScale(0.1f);

	tweens[0] = sb::Tween2f().bounceIn(sb::Vector2f(-0.4f), sb::Vector2f(0), 0.3f).wait(0.5f).circInOut(sb::Vector2f(0), sb::Vector2f(-0.3f, 0.5f), 0.2f);
	tweens[1] = sb::Tween2f().linear(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[2] = sb::Tween2f().sineIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[3] = sb::Tween2f().sineOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[4] = sb::Tween2f().sineInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[5] = sb::Tween2f().quadIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[6] = sb::Tween2f().quadOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[7] = sb::Tween2f().quadInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[8] = sb::Tween2f().cubicIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[9] = sb::Tween2f().cubicOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[10] = sb::Tween2f().cubicInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[11] = sb::Tween2f().quartIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[12] = sb::Tween2f().quartOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[13] = sb::Tween2f().quartInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[14] = sb::Tween2f().quintIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[15] = sb::Tween2f().quintOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[16] = sb::Tween2f().quintInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[17] = sb::Tween2f().expoIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[18] = sb::Tween2f().expoOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[19] = sb::Tween2f().expoInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[20] = sb::Tween2f().circIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[21] = sb::Tween2f().circOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[22] = sb::Tween2f().circInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[23] = sb::Tween2f().backIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[24] = sb::Tween2f().backOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[25] = sb::Tween2f().backInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[26] = sb::Tween2f().elasticIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[27] = sb::Tween2f().elasticOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[28] = sb::Tween2f().elasticInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[29] = sb::Tween2f().bounceIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[30] = sb::Tween2f().bounceOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[31] = sb::Tween2f().bounceInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);

	while (window.isOpen()) {
		time += getDeltaSeconds();
		sb::Input::update();
		window.update();
		quad.setPosition(tweens[currentTween].value(time));
		if (sb::Input::isTouchGoingDown(1)) {
			currentTween = (currentTween + 1) % tweens.size();
			time = 0;
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);

		window.display();
	}
}

void demo29() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Quad quad;
	Animation2f animation;

	quad.setScale(0.1f);
	animation.tween = sb::Tween2f().bounceIn(sb::Vector2f(-0.4f), sb::Vector2f(0), 0.3f).wait(0.5f).circInOut(sb::Vector2f(0), sb::Vector2f(-0.3f, 0.5f), 0.2f);	
	
	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		if (sb::Input::isTouchGoingDown(1) && !animation.isPlaying())
			animation.start();
		std::cout << (animation.isPlaying() ? "playing" : "not playing") << std::endl;

		animation.update(ds);
		quad.setPosition(animation.value());

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);

		window.display();
	}
}

class MyQuad : public sb::Drawable, public sb::Transformable {
	sb::Quad _quad;
	TransformEffects2 _effects;

public:
	MyQuad() : _effects(*this)
	{ }

	void update(float ds) {
		_effects.update(ds);
	}

	inline TransformEffects2& getEffects() { return _effects; }

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_quad, states);
	}
};

void demo30() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	MyQuad quad;

	quad.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		quad.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			quad.getEffects().pop();
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

void demo31() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	MyQuad quad;

	quad.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		quad.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			quad.getEffects().driftOld(touch);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

void print32(TransformEffects2& effects) {
	if (effects.isMoving())
		std::cout << "driftOld: " << effects.getTargetPosition().x << " " << effects.getTargetPosition().y << std::endl;

	if (effects.isRotating())
	std::cout << "spinOld: " << effects.getTargetRotation() * sb::ToDegrees << std::endl;

	if (effects.isScaling())
		std::cout << "pop: " << effects.getTargetScale().x << std::endl;
}

sb::Vector2f discretize(const sb::Vector2f& v, const sb::Vector2f& discretization) {
	float x = floor(v.x / discretization.x);
	float y = floor(v.y / discretization.y);
	return sb::Vector2f(x * discretization.x, y * discretization.y) + 0.5f * discretization;
}

void input32(sb::Window& window, TransformEffects2& effects) {
	const sb::Vector2f cellSize = sb::Vector2f(0.1f, 0.1f);

	if (sb::Input::isTouchDown(1)) {
		sb::Vector2f touch = sb::Input::getTouchPosition(window);
		effects.driftOld(discretize(touch, cellSize));
	}
	if (sb::Input::isTouchGoingDown(1))
		effects.pop();

	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left)) {
		sb::Vector2f pos = effects.getTargetPosition();
		effects.driftOld(discretize(pos - sb::Vector2f(cellSize.x, 0), cellSize));
	}
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right)) {
		sb::Vector2f pos = effects.getTargetPosition();
		effects.driftOld(discretize(pos + sb::Vector2f(cellSize.x, 0), cellSize));
	}
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
		float angle = effects.getTargetRotation();
		effects.spinOld(angle - 90 * sb::ToRadian);
	}
}

void demo32() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Tetromino tetromino('z');
	TransformEffects2 effects(tetromino);

	tetromino.setScale(0.1f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		effects.update(ds);
		input32(window, effects);
		print32(effects);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo33() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Block block('z');
	TransformEffects2 effects(block);
	
	srand(1234567891);

	block.setScale(0.1f);
	block.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		effects.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			effects.die(1.1f);
		if (sb::Input::isKeyDown(sb::KeyCode::b)) {
			sb::Vector2f discretization(0.1f, 0.1f);
			sb::Vector2f discretized = discretize(block.getPosition(), discretization);
			effects.bounce(discretized - sb::Vector2f(0, discretization.y), 0.5f);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo34() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('z');

	tetromino.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isTouchGoingDown(1))
			tetromino.getEffects2().pop();
		if (sb::Input::isTouchDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			tetromino.getEffects2().driftOld(touch);
		}
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			tetromino.getEffects2().spinOld(tetromino.getRotation() - 90 * sb::ToRadian);
			
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo35() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('z');

	block.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b))
			block.getEffects2().bounce(block.getPosition() - sb::Vector2f(0, 0.1f));

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			block.getEffects2().die();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo36() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('z');

	block.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			block.getEffects2().driftTo(touch, 1);
		}
		
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo37() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('z');

	tetromino.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			tetromino.getEffects2().driftTo(touch, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo38() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(2, 2));

	board.showGrid(true);
	board.createTetromino('m', sb::Vector2i(0, 1));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.getTetromino().update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Down)) {
			board.driftTetrominoBy(sb::Vector2i(0, -1));
		}
		
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo39() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('m');

	tetromino.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
			tetromino.getEffects2().spinBy(45 * sb::ToRadian, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo40() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('m');

	block.setScale(0.2f);
	Block block2 = block;

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block2.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
			block2.getEffects().spinBy(90 * sb::ToRadian, block2, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block2);
		window.display();
	}
}

void demo41() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('z');

	tetromino.setScale(0.2f);
	tetromino.setPosition(0.2f, 0.3f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
			tetromino.getEffects().spinBy(90 * sb::ToRadian, tetromino, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo42() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(4, 4));

	board.showGrid(true);
	board.createTetromino('m', sb::Vector2i(1, 2));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.getTetromino().update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			board.spinTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo43() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('m');

	tetromino.setScale(0.1f);
	tetromino.setPosition(0.2f);
	tetromino.setRotation(45 * sb::ToRadian);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isTouchGoingDown(1))
			tetromino.getEffects().driftTo(sb::Input::getTouchPosition(window), tetromino, 2);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			tetromino.getEffects().spinBy(-90 * sb::ToRadian, tetromino);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::p))
			tetromino.getEffects().pop(tetromino, 1);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void touchInput44(sb::Window& window, Board& board, float ds) {
	static float secondsSinceLastTouch = 1;
	static sb::Vector2f touchOffset;
	secondsSinceLastTouch += ds;

	if (sb::Input::isTouchGoingDown(1)) {
		touchOffset = board.getTetromino().getPosition() - sb::Input::getTouchPosition(window);
		if (secondsSinceLastTouch >= 1) 
			board.popTetromino();
		secondsSinceLastTouch = 0;
	}
	if (sb::Input::isTouchDown(1))
		board.driftTetrominoTo(sb::Input::getTouchPosition(window) + touchOffset);
	if (getSwipe(window, ds).y > 0.05f * window.getInverseAspect() 
		|| isTouchGoingDown(window, board.getTetromino()))
		board.spinTetromino();
}

void input44(sb::Window& window, Board& board, float ds) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
		board.spinTetromino();
	if (sb::Input::isKeyGoingDown(sb::KeyCode::p))
		board.popTetromino();
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
		board.driftTetrominoBy(sb::Vector2i(0, -1));
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
		board.driftTetrominoBy(sb::Vector2i(-1, 0));
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
		board.driftTetrominoBy(sb::Vector2i(1, 0));

	touchInput44(window, board, ds);
}

void demo44() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('m', sb::Vector2i(4, 4));
	board.createBlock('j', sb::Vector2i(4, 2));
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.getTetromino().update(ds);
		input44(window, board, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo45() {
	srand(12353859);
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Block block('j');

	block.setScale(0.5f);
	block.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::i))
			block.die();

		std::cout << (int)block.getState() << std::endl;

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo46() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createBlock('m', sb::Vector2i(4, 4));
	board.createBlock('m', sb::Vector2i(5, 5));
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::i)) 
			board.implodeBlocks();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo47() {
	srand(12353859);
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Block block('j');

	block.setScale(0.2f);
	block.setPosition(0, 0.3f);
	block.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b))
			block.getEffects().bounceBy(sb::Vector2f(0, -0.1f), block, 0.75f);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo48() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('z', sb::Vector2i(4, 8));
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		for (size_t i = 0; i < board.getBlocks().size(); i++)
			board.getBlocks()[i].update(ds);
		if (board.hasTetromino())
			board.getTetromino().update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b) && board.hasTetromino())
			board.dropTetromino();
	
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo49() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createBlock('z', sb::Vector2i(4, 8));
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		for (size_t i = 0; i < board.getBlocks().size(); i++)
			board.getBlocks()[i].update(ds);
		if (board.hasTetromino())
			board.getTetromino().update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b))
			board.dropBlocks();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo50() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();

		if (sb::Input::isKeyGoingDown(sb::KeyCode::q))
			board.hardDropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void fillLine(Board& board, int y) {
	int width = board.getBoardSize().x;
	for (int x = 0; x < width; x++)
		board.createBlock('z', sb::Vector2i(x, y));
}

void demo51() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	fillLine(board, 0);
	fillLine(board, 1);
	fillLine(board, 2);

	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			board.clearLowestLineWithBlocks();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void addBlocks(Board& board, char type, int y, std::vector<int> horizontalPositions) {
	std::vector<sb::Vector2i> positions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(2, 0), sb::Vector2i(3, 0),
		sb::Vector2i(6, 0), sb::Vector2i(7, 0), sb::Vector2i(8, 0), sb::Vector2i(9, 0),
		sb::Vector2i(0, 1), sb::Vector2i(1, 1), sb::Vector2i(2, 1), sb::Vector2i(3, 1),
		sb::Vector2i(6, 1), sb::Vector2i(7, 1), sb::Vector2i(8, 1), sb::Vector2i(9, 1),
		sb::Vector2i(0, 2), sb::Vector2i(1, 2) };

	for (size_t i = 0; i < horizontalPositions.size(); i++)
		board.createBlock(type, sb::Vector2i(horizontalPositions[i], y));
}

void addBlocks52(Board& board) {
	addBlocks(board, 'j', 0, { 0, 1, 2, 3, 6, 7, 8, 9 });
	addBlocks(board, 'j', 1, { 0, 1, 2, 3, 6, 7, 8, 9 });
	addBlocks(board, 'j', 2, { 1, 2 });
}

void demo52() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('z', sb::Vector2i(4, 4));
	addBlocks52(board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo53() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('z', sb::Vector2i(4, 4));
	addBlocks52(board);
	board.showGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		input44(window, board, ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::q))
			board.hardDropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void initColors54(std::vector<sb::Color>& colors) {
	colors.resize(7);
	int alpha = 150;
	colors[0] = createColor(  0,   0, 240, alpha);
	colors[1] = createColor(255, 242,   0, alpha);
	colors[2] = createColor(  0, 240, 240, alpha);
	colors[3] = createColor(  0, 240,   0, alpha);
	colors[4] = createColor(240, 160,   0, alpha);
	colors[5] = createColor(240,   0,   0, alpha);
	colors[6] = createColor(160,   0, 240, alpha);
}

void init54(sb::ParticleSystem& particleSystem, sb::Texture& texture, sb::Color& color) {
	particleSystem.setTexture(texture);
	particleSystem.setEmissionRatePerSecond(50);
	particleSystem.setParticleSizeRange(sb::Vector2f(0.1f, 0.13f));
	particleSystem.setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
	particleSystem.setParticleColor(color);
	particleSystem.setEmissionShape(sb::Disk(0.075f, 0.1f, 0, 2 * sb::Pi));
	particleSystem.setScale(0.3f);
}

void demo54() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	std::vector<sb::Color> colors;
	std::size_t currentColor = 0;
	sb::Texture texture;
	sb::ParticleSystem particleSystem(1024);

	initColors54(colors);
	texture.loadFromAsset("Textures/Particle1.png");
	texture.enableMipmap(true);
	init54(particleSystem, texture, colors[0]);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particleSystem);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c)) {
			currentColor = (currentColor + 1) % colors.size();
			particleSystem.setParticleColor(colors[currentColor]);
		}

		window.display();
	}
}

void init55(sb::ParticleSystem& particleSystem, sb::Texture& texture, sb::Color& color) {
	particleSystem.setTexture(texture);
	particleSystem.setEmissionRatePerSecond(0);
	particleSystem.setParticleLifetimeRange(2.f * sb::Vector2f(0.1f, 1));
	particleSystem.setParticleSpeedRange(sb::Vector2f(0.1f, 1));
	particleSystem.setEmissionType(sb::ParticleSystem::EmissionType::Random);
	particleSystem.setParticleSizeRange(sb::Vector2f(0.01f, 0.13f));
	particleSystem.setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
	particleSystem.setParticleColor(color);
}

void update55(Block& block, sb::ParticleSystem& particleSystem, float ds) {
	static bool isExploding = false;

	if (!isExploding && block.getState() == Block::State::Garbage) {
		particleSystem.reset();
		particleSystem.addBurst(0, 50);
		isExploding = true;
	}

	block.update(ds);
	particleSystem.update(ds);
}

void demo55() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Texture texture;
	sb::ParticleSystem particleSystem(1024);
	sb::Color color = createColor(0, 0, 240, 150);
	Block block('j');

	texture.loadFromAsset("Textures/Particle1.png");
	texture.enableMipmap(true);
	init55(particleSystem, texture, color);
	float factor = 0.5f;
	particleSystem.setScale(factor * 0.3f);
	block.setScale(factor * 0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		update55(block, particleSystem, ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::i))
			block.die(1);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.draw(particleSystem);

		window.display();
	}
}

void demo56() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Texture texture;
	BlockExplosion explosion(512);
	
	std::vector<char> types = { 'i', 'j', 'l', 'o', 's', 't', 'z' };
	size_t currentType = 0;
	explosion.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		explosion.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::e))
			explosion.explode();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::t)) {
			currentType = (currentType + 1) % types.size();
			char type = types[currentType];
			std::cout << type << std::endl;
			explosion.setType(type);
		}

		std::cout << explosion.isActive() << std::endl;

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(explosion);

		window.display();
	}
}

void demo57() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('z');

	block.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			block.die();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);

		window.display();
	}
}

void demo58() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('i');
	Tetromino other = tetromino;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));

		window.display();
	}

}

class MyShape {
public:
	virtual ~MyShape() { }

	virtual float value() { return 0; }

	virtual MyShape* clone() { return new MyShape(*this); }
};

class MyDisk : public MyShape {
public:
	virtual float value() { return 1; }

	virtual MyShape* clone() { return new MyDisk(*this); }
};

class MyBox : public MyShape {
public:
	virtual float value() { return 2; }

	virtual MyShape* clone() { return new MyBox(*this); }
};

class MyEmissionShape {
	MyShape* _shape;

public:
	virtual ~MyEmissionShape() {
		if (_shape)
			delete _shape;
	}

	MyEmissionShape() : _shape(NULL)
	{ }

	MyEmissionShape(const MyEmissionShape& other)
	{
		_shape = other._shape->clone();
	}

	MyEmissionShape& operator=(const MyEmissionShape& other) {
		_shape = other._shape->clone();
		return *this;
	}

	template <class T>
	inline void setShape(const T& shape) {  
		if (_shape)
			delete _shape;
		_shape = new T(shape);
	}

	inline float getValue() { return _shape->value(); }
};

class MyParticleSystem {
	MyEmissionShape _emissionShape;

public:
	template <class T>
	inline void setEmissionShape(const T& shape) { _emissionShape.setShape(shape); }

	float getEmissionShapeValue() {
		return _emissionShape.getValue();
	}
};

void demo59() {
	MyParticleSystem system1;
	system1.setEmissionShape(MyDisk());
	MyParticleSystem system2;
	system2 = system1;
	std::cout << system2.getEmissionShapeValue() << std::endl;
	std::cin.get();

	MyDisk disk;
	MyShape* shape;
	shape = new MyShape(disk);
	std::cout << shape->value() << std::endl;
	std::cin.get();

	MyEmissionShape shape2;
	shape2.setShape(MyDisk());
	std::cout << shape2.getValue() << std::endl;
	std::cin.get();

	MyParticleSystem particleSystem;
	particleSystem.setEmissionShape(MyDisk());
	std::cout << particleSystem.getEmissionShapeValue() << std::endl;
	std::cin.get();
}

void addBlocks(Board& board, int y, const std::vector<char>& types, const std::vector<int>& positions) {
	for (size_t i = 0; i < types.size(); i++) 
		board.createBlock(types[i], sb::Vector2i(positions[i], y));
}

void demo60() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));

	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.showGrid(true);
	board.setStepInterval(100);

	//addBlocks(board, 'j', 0, { 0, 1, 2, 3, 4, 5, 6, 7 , 8 , 9 });
	//board.createBlock()
	addBlocks(board, 0, { 'i', 'j', 'l', 'o', 's', 't' }, { 0, 1, 2, 3, 4, 5 });
	//addBlocks(board, 'z', 1, { 0, 1, 2 });
	//addBlocks(board, 'o', 2, { 0, 1 });
	//addBlocks(board, 'l', 3, { 0 });

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			board.clearLowestLineWithBlocks();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);

		std::cout << board.getBlocks().size() << std::endl;

		window.display();
	}
}

class Backdrop : public sb::Drawable, public sb::Transformable {
	sb::Mesh _mesh;

public:
	Backdrop() : _mesh(4, sb::PrimitiveType::TriangleStrip) {
	}

	void update(sb::Camera& camera) {
		static sb::Color bottomColor = createColor(252, 182, 159);
		static sb::Color topColor = createColor(255, 236, 210);
		sb::Vector2f extent(camera.getWidth() * 0.5f, camera.getWidth() * camera.getInverseAspectRatio() * 0.5f);
		_mesh[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), bottomColor);
		_mesh[1] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), bottomColor);
		_mesh[2] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), topColor);
		_mesh[3] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), topColor);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
	}
};

void touchInputComplete(sb::Window& window, Board& board, float ds) {
	static float secondsSinceLastTouch = 1;
	static sb::Vector2f touchOffset;
	secondsSinceLastTouch += ds;

	if (sb::Input::isTouchGoingDown(1)) {
		touchOffset = board.getTetromino().getPosition() - sb::Input::getTouchPosition(window);
		if (secondsSinceLastTouch >= 1)
			board.popTetromino();
		secondsSinceLastTouch = 0;
	}
	if (sb::Input::isTouchDown(1))
		board.driftTetrominoTo(sb::Input::getTouchPosition(window) + touchOffset);
	if (getSwipe(window, ds).y > 0.05f * window.getInverseAspect())
		board.spinTetromino();
}

void inputComplete(sb::Window& window, Board& board, float ds) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
		board.spinTetromino();
	if (sb::Input::isKeyGoingDown(sb::KeyCode::p))
		board.popTetromino();
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
		board.driftTetrominoBy(sb::Vector2i(0, -1));
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
		board.driftTetrominoBy(sb::Vector2i(-1, 0));
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
		board.driftTetrominoBy(sb::Vector2i(1, 0));
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Space))
		board.hardDropTetromino();

	touchInputComplete(window, board, ds);
}

void updateSceneComplete(Board& board) {
	if (board.isDead()) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You lost the game", "You lost the game, press OK to try again", NULL);
		resetDeltaSeconds();
		board = Board(board.getBoardSize());
	}
}

void complete() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Backdrop backdrop;
	Board board(sb::Vector2i(10, 14));
	sb::Music music;
		
	music.loadFromAsset("Music/BackgroundMusic.ogg");
	music.setLooping(true);
	music.play();

	adjustCameraToBoard(window.getCamera(), board);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		backdrop.update(window.getCamera());
		board.update(ds);
		updateSceneComplete(board);

		inputComplete(window, board, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(backdrop);
		sb::Transform test;
		window.draw(board, test);
		window.display();
	}
}

void init61(sb::ParticleSystem& system) {
	system.setEmissionRatePerSecond(0);
	system.setEmissionShape(sb::Box(1, 0.01f));
	system.setEmissionType(sb::ParticleSystem::EmissionType::Random);
	system.setParticleLifetimeRange(2.f * sb::Vector2f(0.1f, 1));
	system.setParticleSpeedRange(sb::Vector2f(0.1f, 1));
	system.setParticleSizeRange(0.5f * sb::Vector2f(0.01f, 0.13f));
	system.setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
	system.setParticleColor(createColor(139, 69, 19, 255));
	system.addBurst(0.5f, 20);
	system.addBurst(1.5f, 20);
	system.addBurst(2.5f, 20);
	system.addBurst(3.5f, 20);

}

void demo61() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Texture texture;
	sb::ParticleSystem particleSystem(512);

	texture.loadFromAsset("Textures/Particle1.png");
	texture.enableMipmap(true);
	particleSystem.setTexture(texture);
	particleSystem.setScale(0.2f);
	init61(particleSystem);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particleSystem);
		window.display();
	}
}

void demo62() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Texture texture;
	sb::ParticleSystem particleSystem(512);
	Block block;

	texture.loadFromAsset("Textures/Particle1.png");
	texture.enableMipmap(true);
	particleSystem.setTexture(texture);
	particleSystem.setScale(0.2f);
	init61(particleSystem);
	block.setScale(0.2f);
	particleSystem.setPosition(0, -0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);
		particleSystem.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.draw(particleSystem);
		window.display();
	}
}

void demo63() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	BlockCollisionEffect effect(128);
	Block block;

	effect.setPosition(0, -0.1f);
	effect.setScale(0.2f);
	block.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);
		effect.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			effect.play();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.draw(effect);
		window.display();
	}
}

void demo64() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	BlockCollisionEffect effect(128);
	Block block1('i');
	Block block2('j');
	bool colliding = false;

	effect.setPosition(0, -0.1f);
	effect.setScale(0.2f);
	block1.setScale(0.2f);
	block1.setPosition(0, 0.2f);
	block2.setScale(0.2f);
	block2.setPosition(0, -0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block1.update(ds);
		block2.update(ds);
		effect.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c)) {
			if (colliding)
				block1.setPosition(0, 0.2f);
			else {
				block1.setPosition(0, 0);
				effect.play();
			}

		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block1);
		window.draw(block2);
		window.draw(effect);
		window.display();
	}
}

void demo65() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	BlockCollisionEffect effect(128);
	Block block1('i');
	Block block2('j');
	enum class DropState { Top, Bottom, Dropping };
	DropState dropState = DropState::Top;
	sb::Vector2f top(0, 0.2f);
	sb::Vector2f bottom(0, 0);

	effect.setPosition(0, -0.1f);
	effect.setScale(0.2f);
	block1.setScale(0.2f);
	block1.setPosition(top);
	block2.setScale(0.2f);
	block2.setPosition(0, -0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block1.update(ds);
		block2.update(ds);
		effect.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c)) {
			if (dropState == DropState::Top) {
				block1.getEffects().driftTo(bottom, block1);
				effect.play(0.2f);
				dropState = DropState::Dropping;
			}
			else if (dropState == DropState::Bottom) {
				block1.setPosition(top);
				dropState = DropState::Top;
			}
		}

		if (dropState == DropState::Dropping) {
			if (!block1.getEffects().isPlaying()) 
				dropState = DropState::Bottom;
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block1);
		window.draw(block2);
		window.draw(effect);
		window.display();
	}
}

void demo66() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block1('i');

	block1.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block1.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			block1.getCollisionEffect().play(0.2f);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block1);
		block1.drawCollisionEffect(window);
		window.display();
	}
}

void demo67() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block1('i');
	Block block2('j');
	enum class DropState { Top, Bottom, Dropping };
	DropState dropState = DropState::Top;
	sb::Vector2f top(0, 0.2f);
	sb::Vector2f bottom(0, 0);

	block1.setScale(0.2f);
	block1.setPosition(top);
	block2.setScale(0.2f);
	block2.setPosition(0, -0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block1.update(ds);
		block2.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c)) {
			if (dropState == DropState::Top) {
				block1.getEffects().driftTo(bottom, block1);
				block2.getCollisionEffect().play(0.2f);
				dropState = DropState::Dropping;
			}
			else if (dropState == DropState::Bottom) {
				block1.setPosition(top);
				dropState = DropState::Top;
			}
		}

		if (dropState == DropState::Dropping) {
			if (!block1.getEffects().isPlaying())
				dropState = DropState::Bottom;
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block1);
		window.draw(block2);
		block2.drawCollisionEffect(window);
		window.display();
	}
}

void demo68() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('m');

	block.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
			block.getCollisionEffect().play(0.5f, BlockCollisionEffect::Position::Left);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			block.getCollisionEffect().play(0.5f, BlockCollisionEffect::Position::Top);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
			block.getCollisionEffect().play(0.5f, BlockCollisionEffect::Position::Right);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
			block.getCollisionEffect().play(0.5f, BlockCollisionEffect::Position::Bottom);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		block.drawCollisionEffect(window);
		window.display();
	}
}

void demo69() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Tetromino tetromino('m');
	size_t currentType = 0;

	tetromino.setScale(0.2f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			tetromino.playCollisionEffect(0.5f);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::i))
			tetromino.rotate(90 * sb::ToRadian);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			tetromino.rotate(-90 * sb::ToRadian);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::t)) {
			currentType = (currentType + 1) % Tetromino::getTypes().size();
			tetromino.setType(Tetromino::getTypes()[currentType]);
			tetromino.setLight(light);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		tetromino.drawCollisionEffect(window);

		window.display();
	}
}

void demo70() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	window.getCamera().setWidth(1.3f);
	board.createBlock('j', sb::Vector2i(2, 1));
	board.createTetromino('m', sb::Vector2i(2, 7));
	board.showGrid(true);
	board.enableAutodrop(false);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::h))
			board.hardDropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);

		window.display();
	}
}

void demo71() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(1, 3));

	adjustCameraToBoard(window.getCamera(), board);
	window.getCamera().setWidth(3);
	//board.createBlock('j', sb::Vector2i(2, 1));
	board.createTetromino('m', sb::Vector2i(0, 1));
	board.showGrid(true);
	board.enableAutodrop(false);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
			board.driftTetrominoBy(sb::Vector2i(0, -1));

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);

		window.display();
	}
}

void demo72() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	window.getCamera().setWidth(1.5);
	board.createBlock('j', sb::Vector2i(2, 1));
	board.createTetromino('m', sb::Vector2i(2, 4));
	board.showGrid(true);
	board.enableAutodrop(false);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
			board.driftTetrominoBy(sb::Vector2i(0, -1));
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			board.spinTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);

		window.display();
	}
}

void demo73() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(1, 3));

	adjustCameraToBoard(window.getCamera(), board);
	window.getCamera().setWidth(3);
	board.createTetromino('m', sb::Vector2i(0, 1));
	board.showGrid(true);
	board.enableAutodrop(false);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);

		window.display();
	}
}

void demo74() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	window.getCamera().setWidth(1.5f);
	board.createBlock('j', sb::Vector2i(2, 1));
	board.createTetromino('m', sb::Vector2i(2, 4));
	board.showGrid(true);
	board.enableAutodrop(false);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			board.spinTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);

		window.display();
	}
}

void demo75() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Backdrop background;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		background.update(window.getCamera());

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(background);

		window.display();
	}
}

sb::Color shade(const sb::Color& color, float factor) {
	return sb::Color(color.r * factor, color.g * factor, color.b * factor, color.a);
}

void demo76() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Backdrop Backdrop;
	StripedQuad stripes(5);

	stripes.setScale(1, 2);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		Backdrop.update(window.getCamera());

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(Backdrop);
		window.draw(stripes);
		window.display();
	}
}

void demo77() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Backdrop backdrop; 
	Border border(0.01f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		backdrop.update(window.getCamera());

		window.clear();
		window.draw(backdrop);
		window.draw(border);
		window.display();
	}
}

void updateScene(Board& board) {
	if (board.isDead()) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You lost the game", "You lost the game, press OK to try again", NULL);
		resetDeltaSeconds();
		board = Board(board.getBoardSize());
	}
}

void demo78() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		updateScene(board);
	
		if (sb::Input::isKeyGoingDown(sb::KeyCode::d)) {
			board.die();
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);

		window.display();
	}
}

void init79(sb::ParticleSystem& particleSystem, sb::Texture& texture, const sb::Color& color) {
	particleSystem.setTexture(texture);
	particleSystem.setEmissionRatePerSecond(5);
	particleSystem.setParticleSizeRange(0.8f * sb::Vector2f(0.1f, 0.13f));
	particleSystem.setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
	particleSystem.setParticleColor(color);
	particleSystem.setEmissionShape(sb::Box(0.5f, 0.01f));
	particleSystem.setEmissionType(sb::ParticleSystem::EmissionType::Directional);
	particleSystem.setEmissionDirection(sb::Vector2f(0, 1));
	particleSystem.setScale(0.3f);
}

void demo79() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Texture texture;
	sb::ParticleSystem particleSystem(1024);

	texture.loadFromAsset("Textures/Particle2.png");
	texture.enableMipmap(true);
	init79(particleSystem, texture, createColor(255, 0, 0));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);
		if (sb::Input::isTouchDown(1)) 
			particleSystem.setPosition(sb::Input::getTouchPosition(window));

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particleSystem);

		window.display();
	}
}

void attachEmitter(Tetromino& tetromino, sb::ParticleSystem& emitter) {
	sb::FloatRect bounds = tetromino.getBounds();
	emitter.setPosition(bounds.left + 0.5f * bounds.width, bounds.top());
	float emitterWidth = bounds.width / 0.3f;
	emitter.setEmissionShape(sb::Box(emitterWidth, 0.001f));
	emitter.setEmissionRatePerSecond(emitterWidth * 5);
}

void demo80() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Texture texture;
	sb::ParticleSystem emitter(1024);
	Tetromino tetromino('i');

	texture.loadFromAsset("Textures/Particle2.png");
	texture.enableMipmap(true);
	init79(emitter, texture, tetromino.getColor());
	emitter.setEmissionShape(sb::Box(1, 0.001f));
	tetromino.setScale(0.1f);
	attachEmitter(tetromino, emitter);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		emitter.update(ds);
		tetromino.update(ds);

		if (sb::Input::isTouchDown(1)) {
			tetromino.setPosition(sb::Input::getTouchPosition(window));
			attachEmitter(tetromino, emitter);
		}
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
			tetromino.getEffects().spinBy(-90 * sb::ToRadian, tetromino);
			attachEmitter(tetromino, emitter);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.draw(emitter);
		drawOutline(window, tetromino.getBounds());

		window.display();
	}
}

void demo81() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Texture texture;
	sb::ParticleSystem emitter(1024);

	texture.loadFromAsset("Textures/Particle2.png");
	texture.enableMipmap(true);
	init79(emitter, texture, sb::Color(1, 0, 0));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		emitter.update(ds);

		 if (sb::Input::isKeyGoingDown(sb::KeyCode::s))
			 emitter.die();

		//std::cout << (int)emitter.getState() << std::endl;

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(emitter);

		window.display();
	}
}

void demo82() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Tetromino tetromino('t');
	BubbleEffect emitter(1024, tetromino.getColor());

	window.getCamera().setWidth(10);
	tetromino.setScale(1);
	//tetromino.setScale(0.1f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		if (sb::Input::isTouchDown(1))
			tetromino.setPosition(sb::Input::getTouchPosition(window));
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			tetromino.getEffects().spinBy(-90 * sb::ToRadian, tetromino);
		tetromino.update(ds);
		emitter.update(tetromino, tetromino, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.draw(emitter);
		window.display();
	}
}

void demo83() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Tetromino tetromino('t');

	window.getCamera().setWidth(10);
	tetromino.setScale(1);
	//tetromino.setScale(0.1f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		if (sb::Input::isTouchDown(1))
			tetromino.setPosition(sb::Input::getTouchPosition(window));
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			tetromino.getEffects().spinBy(-90 * sb::ToRadian, tetromino);
		tetromino.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		tetromino.drawBubbleEffect(window);
		window.display();
	}
}

void demo84() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Tetromino tetromino('t');

	window.getCamera().setWidth(10);
	tetromino.setScale(1);
	//tetromino.setScale(0.1f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		if (sb::Input::isTouchDown(1))
			tetromino.setPosition(sb::Input::getTouchPosition(window));
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			tetromino.getEffects().spinBy(-90 * sb::ToRadian, tetromino);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			tetromino.die();
		tetromino.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		tetromino.drawBubbleEffect(window);
		std::cout << (int)tetromino.getState() << std::endl;
		window.display();
	}
}

void demo85() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	BlockCollisionEffect effect(512);

	window.getCamera().setWidth(10);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			effect.play(0);
		effect.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(effect);
		//std::cout << (int)tetromino.getState() << std::endl;
		window.display();
	}
}

void demo86() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Music music;

	music.loadFromAsset("Music/BackgroundMusic.ogg");
	music.setLooping(true);
	music.play();

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

void demo87() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Sound collision;
	sb::Sound explosion;
	sb::Sound grab;
	sb::Sound rotate;
	sb::Music music;

	collision.loadFromAsset("Sounds/Collision.wav");
	explosion.loadFromAsset("Sounds/Explosion.wav");
	grab.loadFromAsset("Sounds/Grab.wav");
	rotate.loadFromAsset("Sounds/Rotate.wav");
	music.loadFromAsset("Music/BackgroundMusic.ogg");
	music.setLooping(true);
	music.play();

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			collision.play();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::e))
			explosion.play();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::g))
			grab.play();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			rotate.play();

		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

void demo88() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	SoundEffect rotate;

	rotate.loadFromAssetPool("Sounds/Rotate.wav");

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		rotate.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			rotate.play(1);

		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

void demo89() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(2, 5));
	
	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('o');
	board.enableAutodrop(false);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
			board.dropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo90() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	SoundEffect collision;
	SoundEffect explosion;
	SoundEffect grab;
	SoundEffect rotate;
	sb::Music music;

	collision.loadFromAssetPool("Sounds/Collision.wav");
	explosion.loadFromAssetPool("Sounds/Explosion.wav");
	grab.loadFromAssetPool("Sounds/Grab.wav");
	rotate.loadFromAssetPool("Sounds/Rotate.wav");
	music.loadFromAsset("Music/BackgroundMusic.ogg");
	music.setLooping(true);
	music.play();

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			collision.play();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::e))
			explosion.play();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::g))
			grab.play();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			rotate.play();

		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

void demo91() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('z');

	tetromino.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c)) {
			tetromino.playCollisionEffect(0.5f);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		tetromino.drawCollisionEffect(window);

		window.display();
	}
}

void demo() {
	complete();

	//demo91();

	//demo90();

	//demo89();

	//demo88();

	//demo87();

	//demo86();

	//demo85();

	//demo84();

	//demo83();

	//demo82();

	//demo81();

	//demo80();

	//demo79();

	//demo78();

	//demo77();

	//demo76();

	//demo75();

	//demo74();

	//demo73();

	//demo72();

	//demo71();

	//demo70();

	//demo69();

	//demo68();

	//demo67();

	//demo66();

	//demo65();

	//demo64();

	//demo63();

	//demo62();

	//demo61();

	//demo60();

	//demo59();

	//demo58();

	//demo57();

	//demo56();

	//demo55();

	//demo54();

	//demo53();
	
	//demo52();

	//demo51();

	//demo49();

	//demo48();

	//demo47();

	//demo46();

	//demo45();

	//demo44();

	//demo43();

	//demo42();

	//demo41();

	//demo40();

	//demo39();

	//demo38();

	//demo37();

	//demo36();

	//demo35();

	//demo34();

	//demo33();

	//demo32();

	//demo31();

	//demo30();

	//demo29();

	//demo28();

	//demo27();

	//demo26();
	
	//demo25();

	//demo24();

	//demo23();

	//demo22();

	//demo21();

	//demo20();

	//demo19();

	//demo18();

	//demo17();

	//demo16();

	//demo15();

	//demo14();

	//demo13();

	//demo12();

	//demo11();

    //demo10();

    //demo9();

    //demo8();

    //demo7();

    //demo6();

    //demo5();

    //demo4();

    //demo3();

    //demo2();

    //demo1();

    //demo0();
}
