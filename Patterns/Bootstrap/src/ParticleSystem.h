#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include "Body.h"
#include "Color.h"
#include "Vector2.h"
#include "Mesh.h"
#include "Texture.h"
#include "Tween.h"
#include "Shape.h"
#include "Disk.h"
#include "Math.h"
#include "Memory.h"
#include <vector>
#include <algorithm>

namespace sb
{
	class ParticleSystem : public Drawable, public Body {
		struct Particle : public Body {
			std::vector<Color> startVertexColors = std::vector<Color>(4);
			std::vector<Color> vertexColors = std::vector<Color>(4);
			float secondsSinceBirth = 0;
			float lifetime = 0;
			Vector2f startScale;
			bool isActive = false;
		};

		struct Burst {
			std::size_t numParticles;
			float emissionTime;
			bool emitted = false;

			Burst(float emissionTime_, std::size_t numParticles_)
				: numParticles(numParticles_), emissionTime(emissionTime_)
			{ }
		};

		struct Emission {
			Shape* _shape;

		public:
			Emission() : _shape(new Disk(0)) 
			{ }

			Emission(const Emission& other) {
				_shape = other._shape->clone();
			}

			Emission& operator=(const Emission& other) {
				_shape = other._shape->clone();
				return *this;
			}

			virtual ~Emission() {
				delete _shape;
			}

			inline Shape& getShape() { return *_shape; }

			template <class T>
			inline void setShape(const T& shape) {
				delete _shape;
				_shape = new T(shape);
			}
		};

		class Pool {
		public:
			struct Item { 
				Item() {}
				Item(const Item& other);
				Item& operator=(const Item& other);

				ParticleSystem* particleSystem; 
				bool isActive;
			};

			Pool();
			Pool(const Pool& other);
			Pool& operator=(const Pool& other);
			virtual ~Pool();
			inline std::vector<Item>& getAllItems() { return _items; }
			inline static bool isItemActive(const Item& item) { return item.isActive; }
			inline std::size_t getNumItems() const { return _items.size(); }
			inline std::size_t getNumActiveItems() const { return _numActiveItems; }
			inline bool hasPrototype() const { return _prototype != NULL; }
			inline ParticleSystem* getPrototype() { return _prototype; };
			void setPrototype(const ParticleSystem& prototype);
			Item& getAvailableItem();
			void update();
			void clear();

		protected:
			void reset(ParticleSystem& particleSystem);
			void activate(Item& item);
			void shrink();
			void expand();

		private:
			ParticleSystem* _prototype;
			std::vector<Item> _items;
			std::size_t _numActiveItems;
		};

	public:
		enum class State { Alive, Dying, Garbage };
		enum class EmissionType { Concentric, Random, Directional };

		ParticleSystem(std::size_t maxNumParticles)
			: _mesh(maxNumParticles * 6, PrimitiveType::TriangleStrip), _texture(NULL),
			_particles(maxNumParticles), _numActiveParticles(0),
			_secondsSinceLastEmission(1), _secondsSinceBirth(0),
			_hasLifetime(false), _lifetime(1), _emissionRatePerSecond(1), _drag(0), _angularDrag(0),
			_particleDrag(0), _angularParticleDrag(0), _particleIntertia(1), _particleLifetimeRange(1, 1),
			_particleSizeRange(0.1f, 0.1f), _particleRotationRange(0, 0), _particleSpeedRange(1, 1),
			_particleVertexColors(4, Color(1, 1, 1, 1)), _hasParticleColorChannelsOverLifetime(4, false), 
			_particleColorChannelsOverLifetime(4), _hasParticleScaleOverLifetime(false), 
			_emissionType(EmissionType::Concentric), _emissionDirection(1, 0), _state(State::Alive)
		{ }

		inline float getEmissionRatePerSecond() const { return _emissionRatePerSecond; }

		inline ParticleSystem* getSubSystemOnParticleDeath() { return _pool.getPrototype(); }

		inline void setTexture(Texture& texture) { _texture = &texture; }

		inline void setDrag(float drag) { _drag = drag; }

		inline void setAngularDrag(float angularDrag) { _angularDrag = angularDrag; }

		inline void setParticleDrag(float drag) { _particleDrag = drag; }

		inline void setAngularParticleDrag(float angularDrag) { _angularParticleDrag = angularDrag; }

		inline void setParticleInertia(float intertia) { _particleIntertia = intertia; }

		inline void setParticleLifetimeRange(const Vector2f& lifetimeRange) { _particleLifetimeRange = lifetimeRange; }

		inline void setParticleSizeRange(const Vector2f& sizeRange) { _particleSizeRange = sizeRange; }

		inline void setParticleRotationRange(const Vector2f& rotationRange) { _particleRotationRange = rotationRange; }

		inline void setParticleSpeedRange(const Vector2f& speedRange) { _particleSpeedRange = speedRange; }

		inline void setParticleAngularVelocityRange(const Vector2f& range) { _particleAngularVelocityRange = range; }

		inline void hasLifetime(bool hasLifetime) { _hasLifetime = hasLifetime; }

		inline void setEmissionType(EmissionType type) { _emissionType = type; }

		inline State getState() const { return _state; }

		bool isPlaying();

		void setEmissionDirection(const sb::Vector2f& emissionDirection);

		template <class T>
		inline void setEmissionShape(const T& shape) { _emission.setShape(shape); }

		void setEmissionRatePerSecond(float rate);

		void addBurst(float emissionTime, std::size_t _numParticles);

		void clearBursts();

		void setParticleVertexColor(std::size_t index, const Color& color);

		void setParticleColor(const Color& color);

		void setParticleColorChannelOverLifetime(std::size_t channelIndex, const Tweenf& particleColorChannelOverLifetime);

		void setParticleScaleOverLifetime(const Tweenf& particleScaleOverLifetime);

		void setLifetime(float lifetime);

		void setSubSystemOnParticleDeath(const ParticleSystem& subSystem);

		bool hasUnemittedBursts();

		void die();

		void reset();

		void update(float ds);

		virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault());

		std::string id;

	protected:
		void updateState();

		void updateParticleSystem(float ds);

		inline static bool isParticleSystemDead(ParticleSystem* particleSystem) { return !particleSystem->isPlaying(); }
		
		static bool isParticleDead(const Particle& particle);

		void deactivateParticleInMesh(std::size_t meshIndex);

		void emitSubSystem(const Particle& particle);

		void removeDeadParticles();

		static bool isParticleInactive(const Particle& particle);

		std::size_t findAvailableIndex();

		Vector2f getDirection(Particle& particle);

		void initParticle(Particle& particle);

		void emitParticle();

		void emitParticles(float ds);

		void emitBurst(Burst& burst);

		void emitBursts(float ds);

		Vector2f computeParticleForce(Particle& particle);

		float computeParticleTorque(Particle& particle);

		void updateScale(Particle& particle);

		float getNormalizedSecondsSinceBirth(const Particle& particle);

		void updateVertexColorChannel(std::size_t channelIndex, float& colorChannel, const float& startColorChannel, float t);

		void updateVertexColor(Color& color, const Color& startColor, Particle& particle);

		void updateVertexColors(Particle& particle);

		void transformParticle(Particle& particle, float ds);

		void transformParticles(float ds);

		void updateParticleVertices(Particle& particle, std::size_t index);

		void updateMesh(float ds);

		void updateParticles(float ds);

		void updateSubSystems(float ds);

		void updateDying();

		void drawSubSystems(DrawTarget& target, DrawStates& states);

	private:
		Mesh _mesh;
		Texture* _texture;

		std::vector<Particle> _particles;
		std::vector<Burst> _bursts;
		std::size_t _numActiveParticles;
		float _secondsSinceLastEmission;
		float _secondsSinceBirth;

		bool _hasLifetime;
		float _lifetime;
		float _emissionRatePerSecond;
		float _drag;
		float _angularDrag;
		float _particleDrag;
		float _angularParticleDrag;
		float _particleIntertia;
		Vector2f _particleLifetimeRange;
		Vector2f _particleSizeRange;
		Vector2f _particleRotationRange;
		Vector2f _particleSpeedRange;
		Vector2f _particleAngularVelocityRange;
		std::vector<Color> _particleVertexColors;
		std::vector<bool> _hasParticleColorChannelsOverLifetime;
		std::vector<Tweenf> _particleColorChannelsOverLifetime;
		bool _hasParticleScaleOverLifetime;
		Tweenf _particleScaleOverLifetime;
		Emission _emission;
		EmissionType _emissionType;
		sb::Vector2f _emissionDirection;
		State _state;

		Pool _pool;
	};
}