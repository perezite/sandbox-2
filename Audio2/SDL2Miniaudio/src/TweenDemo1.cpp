#include "TweenDemo1.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "TweenDev.h"
#include "TweenChainDev.h"
#include "TweenerDev.h"
#include "ParticleSystem.h"

using namespace std;
using namespace sb;

namespace tweenDemo1
{
	using namespace v1::tweenFunctions;

	namespace d0 {
		void demo()
		{
			Window window(400, 400);
			Quad quad;

			quad.setScale(40, 40);
			quad.setPosition(-100);
			v1::Tween<float> tween(quad.getPosition().x, 100, 2, smoothstep);

			while (window.isOpen())
			{
				Input::update();
				window.update();
				window.clear(Color(1, 1, 1, 1));
				quad.setPosition(tween.getValue());
				window.draw(quad);
				window.display();
			}
		}
	}

	namespace d1 {
		v1::TweenFunction getRandomTweenFunction()
		{
			int val = rand() % 3;
			if (val == 0) {
				cout << "linear" << endl;
				return linear;
			}
			else if (val == 1) {
				cout << "smoothstep" << endl;
				return smoothstep;
			}
			else {
				cout << "bounce out" << endl;
				return bounceOut;
			}
		}

		void update(Window& window, Quad& quad, v1::Tween<Vector2f>& tween)
		{
			if (Input::isTouchGoingDown(1)) {
				Vector2f touchPosition = Input::getTouchPosition(window);
				float duration = random(1, 3);
				tween = v1::Tween<Vector2f>(quad.getPosition(), touchPosition, duration, getRandomTweenFunction());
			}
		}

		void demo()
		{
			Window window(400, 400);
			Quad quad;

			quad.setScale(40, 40);
			quad.setPosition(-100);

			v1::Tween2f tween(quad.getPosition(), quad.getPosition());

			while (window.isOpen())
			{
				Input::update();
				window.update();
				window.clear(Color(1, 1, 1, 1));
				update(window, quad, tween);
				quad.setPosition(tween.getValue());
				window.draw(quad);
				window.display();
			}
		}
	}

	namespace d100 {
		float getDeltaSeconds() {
			static Stopwatch watch;
			static float lastSeconds = watch.getElapsedSeconds();
			float currentSeconds = watch.getElapsedSeconds();
			float deltaSeconds = currentSeconds - lastSeconds;
			lastSeconds = currentSeconds;
			return deltaSeconds;
		}

		void demo()
		{
			Window window(400, 400);
			window.setFramerateLimit(120);
			Quad quad;
			Quad quad2;
			Quad quad3;

			quad.setScale(10);
			quad.setPosition(-50);

			quad2.setScale(20);
			quad2.setPosition(-100);

			quad3.setScale(30);
			quad3.setPosition(-150);

			v2::Tween2f tween = v2::Tween2f(quad.getPosition())
				.to(Vector2f(50, -50), 1, linear)
				.to(50, 3, smoothstep);
			Stopwatch watch;

			v2::Tween2f tween2 = v2::Tween2f(quad2.getPosition())
				.to(Vector2f(100, -100), 2, smoothstep)
				.to(100, 2, bounceOut);
			v1::Tweener2f tweener(tween2);

			v1::Tweener2f tweener2 = v1::Tweener2f(quad3.getPosition())
				.to(Vector2f(150, -150), 3, smoothstep)
				.to(150, 1, bounceOut);

			while (window.isOpen())
			{
				float ds = getDeltaSeconds();
				Input::update();
				window.update();
				tweener.update(ds);
				tweener2.update(ds);
				window.clear(Color(1, 1, 1, 1));
				quad.setPosition(tween.getValue(watch.getElapsedSeconds()));
				quad2.setPosition(tweener.getValue());
				quad3.setPosition(tweener2.getValue());
				window.draw(quad);
				window.draw(quad2);
				window.draw(quad3);
				window.display();
			}
		}
	}

	namespace my {
		float getDeltaSeconds() {
			static Stopwatch watch;
			static float lastSeconds = watch.getElapsedSeconds();
			float currentSeconds = watch.getElapsedSeconds();
			float deltaSeconds = currentSeconds - lastSeconds;
			lastSeconds = currentSeconds;
			return deltaSeconds;
		}
	}

	namespace d200 {
		void demo() {
			Window window(400, 400);
			window.setFramerateLimit(100);
			Texture texture;
			ParticleSystem emitter(1024);

			texture.loadFromAsset("Textures/LightGreenPropulsion.png");
			float halfAngle = 35;
			emitter.setScale(100);
			emitter.setTexture(texture);
			emitter.setParticleSpeed(1);
			emitter.setParticleLifetime(1);
			emitter.setParticleInertia(.5f);
			emitter.setEmissionRatePerSecond(100);
			emitter.setParticleColor(Color(1, 1, 1, .3f));
			emitter.setParticleSize(.8f * Vector2f(.175f, .75f));
			emitter.setEmissionShape(Disk(0, .6f, (270 - halfAngle) * ToRadians, (270 + halfAngle) * ToRadians));
			emitter.setParticleColorChannel(3, 1).to(.3f, 1, smoothstep);
			emitter.setParticleScale(0).to(1, .1f, bounceOut).to(.3f, .9f, quadInOut);

			while (window.isOpen()) 
			{
				float ds = my::getDeltaSeconds();
				Input::update();
				window.update();
				emitter.update(ds);
				window.clear(Color(1, 1, 1, 1));
				window.draw(emitter);
				window.display();
			}
		}
	}

	void run()
	{
		d200::demo();
		//d100::demo();
		//d1::demo();
		//d0::demo();
	}
}