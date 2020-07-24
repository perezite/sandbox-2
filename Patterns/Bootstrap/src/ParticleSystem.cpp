#include "ParticleSystem.h"
#include "DrawTarget.h"
#include "Logger.h"
#include <algorithm>

namespace sb
{
	bool ParticleSystem::isPlaying()
	{
		return !_hasLifetime || _secondsSinceBirth < _lifetime || _pool.getNumActiveItems() > 0
			|| _numActiveParticles > 0 || hasUnemittedBursts();
	}

	void ParticleSystem::setEmissionDirection(const sb::Vector2f& emissionDirection)
	{
		SB_ERROR_IF(_emissionType != EmissionType::Directional, "Emission type must be directional to specify an emission direction");
		_emissionDirection = emissionDirection.normalized();
	}

	void ParticleSystem::setEmissionRatePerSecond(float rate)
	{
		//_secondsSinceLastEmission = rate == 0 ? 0 : 1 / rate;
		_emissionRatePerSecond = rate;
	}

	void ParticleSystem::addBurst(float emissionTime, std::size_t _numParticles) 
	{
		_bursts.emplace_back(emissionTime, _numParticles);
	}

	void ParticleSystem::clearBursts()
	{
		_bursts.clear();
	}

	void ParticleSystem::setParticleVertexColor(std::size_t index, const Color& color)
	{
		SB_ERROR_IF(index > 4, "Vertex index out of range");
		_particleVertexColors[index] = color;
	}

	void ParticleSystem::setParticleColor(const Color& color)
	{
		std::fill(_particleVertexColors.begin(), _particleVertexColors.end(), color);
	}

	void ParticleSystem::setParticleColorChannelOverLifetime(std::size_t channelIndex, const Tweenf& particleColorChannelOverLifetime) 
	{
		SB_ERROR_IF(channelIndex > 4, "Color channel index out of range");
		SB_ERROR_IF(particleColorChannelOverLifetime.getDuration() > 1, "Tween duration out of range");
		_particleColorChannelsOverLifetime[channelIndex] = particleColorChannelOverLifetime;
		_hasParticleColorChannelsOverLifetime[channelIndex] = true;
	}

	void ParticleSystem::setParticleScaleOverLifetime(const Tweenf& particleScaleOverLifetime) 
	{
		SB_ERROR_IF(particleScaleOverLifetime.getDuration() > 1, "Tween duration out of range");
		_particleScaleOverLifetime = particleScaleOverLifetime;
		_hasParticleScaleOverLifetime = true;
	}

	void ParticleSystem::setLifetime(float lifetime)
	{
		_hasLifetime = true;
		_lifetime = lifetime;
	}

	void ParticleSystem::setSubSystemOnParticleDeath(const ParticleSystem& subSystem)
	{
		SB_ERROR_IF(!subSystem._hasLifetime, "sub particle systems must have a lifetime to prevent zombie systems");
		_pool.setPrototype(subSystem);
	}

	bool ParticleSystem::hasUnemittedBursts() 
	{
		bool found = false;
		for (size_t i = 0; i < _bursts.size(); i++) {
			if (_bursts[i].emitted == false)
				found = true;
		}

		return found;
	}

	void ParticleSystem::die()
	{
		setLifetime(0);
		setEmissionRatePerSecond(0);
		clearBursts();
		_state = State::Dying;
	}

	void ParticleSystem::reset()
	{
		_secondsSinceLastEmission = _emissionRatePerSecond == 0 ? 0 : 1 / _emissionRatePerSecond;
		_numActiveParticles = 0;
		_secondsSinceBirth = 0;

		for (std::size_t i = 0; i < _particles.size(); i++)
			_particles[i].isActive = false;

		for (std::size_t i = 0; i < _bursts.size(); i++)
			_bursts[i].emitted = false;

		std::vector<Pool::Item>& poolItems = _pool.getAllItems();
		for (std::size_t i = 0; i < poolItems.size(); i++)
			poolItems[i].particleSystem->reset();

		std::vector<Vertex>& vertices = _mesh.getVertices();
		std::fill(vertices.begin(), vertices.end(), sb::Vertex(sb::Vector2f(0, 0), sb::Color(0, 0, 0, 0)));
	}

	void ParticleSystem::update(float ds)
	{
		_secondsSinceBirth += ds;
	
		updateState();
		updateParticleSystem(ds);
		updateParticles(ds);
		updateSubSystems(ds);

		SB_DEBUG_IF(id == "main", "num pool items: " << _pool.getNumItems() << " num active pool items: " << _pool.getNumActiveItems());
	}

	void ParticleSystem::draw(DrawTarget& target, DrawStates states) {
		if (isPlaying()) {
			states.texture = _texture;
			target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
			drawSubSystems(target, states);
		}
	}



	void ParticleSystem::updateState()
	{
		if (_state == State::Dying)
			updateDying();
	}

	void ParticleSystem::updateParticleSystem(float ds)
	{
		sb::Vector2f force = -_drag * velocity;
		float torque = -_angularDrag * angularVelocity;
		velocity += force;
		angularVelocity += torque;

		translate(ds * velocity);
		rotate(ds * angularVelocity);
	}

	bool ParticleSystem::isParticleDead(const Particle& particle)
	{
		return particle.secondsSinceBirth > particle.lifetime;
	}
	
	void ParticleSystem::deactivateParticleInMesh(std::size_t meshIndex) 
	{
		_mesh[meshIndex * 6 + 0].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 1].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 2].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 3].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 4].position = Vector2f(0, 0);
		_mesh[meshIndex * 6 + 5].position = Vector2f(0, 0);
	}

	void ParticleSystem::emitSubSystem(const Particle& emittingParticle) 
	{
		ParticleSystem* subSystem = _pool.getAvailableItem().particleSystem;
		sb::Vector2f parentScale = getScale();
		sb::Vector2f scale = subSystem->getScale();
		subSystem->setScale(parentScale.x * scale.x, parentScale.y * scale.y);
		subSystem->setPosition(emittingParticle.getPosition());
		subSystem->setRotation(emittingParticle.getRotation());
		subSystem->velocity = emittingParticle.velocity;
		subSystem->angularVelocity = emittingParticle.angularVelocity;
	}

	void ParticleSystem::removeDeadParticles() 
	{
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive && isParticleDead(_particles[i])) {
				_particles[i].isActive = false;
				_numActiveParticles--;
				deactivateParticleInMesh(i);
				if (_pool.hasPrototype())
					emitSubSystem(_particles[i]);
			}
		}
	}

	bool ParticleSystem::isParticleInactive(const Particle& particle) {
		return !particle.isActive;
	}

	std::size_t ParticleSystem::findAvailableIndex() 
	{
		std::vector<Particle>::iterator it =
			std::find_if(_particles.begin(), _particles.end(), isParticleInactive);
		return std::distance(_particles.begin(), it);
	}

	Vector2f ParticleSystem::getDirection(Particle& particle) 
	{
		sb::Vector2f srection;
		bool randomDirection = _emissionType == EmissionType::Random || _emission.getShape().getBoundingRadius() < 0.0001f;
		if (randomDirection)
			return randomOnCircle(1);
		else if (_emissionType == EmissionType::Concentric)
			return (particle.getPosition() - getPosition()).normalized();
		else
			return _emissionDirection;
	}

	void ParticleSystem::initParticle(Particle& particle) 
	{
		const sb::Vector2f& scale = getScale();

		particle.setPosition(getTransform() * _emission.getShape().random());
		float size = random(_particleSizeRange.x * scale.x, _particleSizeRange.y * scale.y);
		particle.setScale(size, size);
		particle.startScale = Vector2f(size, size);
		particle.setRotation(random(_particleRotationRange.x, _particleRotationRange.y));
		Vector2f direction = getDirection(particle);
		Vector2f scaledDirection(scale.x * direction.x, scale.y * direction.y);
		particle.velocity = random(_particleSpeedRange.x, _particleSpeedRange.y) * scaledDirection + _particleIntertia * velocity;
		particle.angularVelocity = random(_particleAngularVelocityRange.x, _particleAngularVelocityRange.y);
		particle.lifetime = random(_particleLifetimeRange.x, _particleLifetimeRange.y);
		particle.vertexColors = _particleVertexColors;
		particle.startVertexColors = _particleVertexColors;
		particle.isActive = true;
	}

	void ParticleSystem::emitParticle() 
	{
		if (_numActiveParticles == _particles.size())
			return;

		std::size_t availableIndex = findAvailableIndex();
		Particle particle;
		initParticle(particle);
		_particles[availableIndex] = particle;
		_numActiveParticles++;
	}

	void ParticleSystem::emitParticles(float ds) 
	{
		if (_emissionRatePerSecond == 0)
			return;

		_secondsSinceLastEmission += ds;
		float emissionInterval = 1 / _emissionRatePerSecond;
		while (_secondsSinceLastEmission > emissionInterval) {
			emitParticle();
			_secondsSinceLastEmission -= emissionInterval;
		}
	}

	void ParticleSystem::emitBurst(Burst& burst) 
	{
		for (std::size_t i = 0; i < burst.numParticles; i++)
			emitParticle();

		burst.emitted = true;
	}

	void ParticleSystem::emitBursts(float ds) 
	{
		for (std::size_t i = 0; i < _bursts.size(); i++) {
			if (!_bursts[i].emitted && _secondsSinceBirth >= _bursts[i].emissionTime)
				emitBurst(_bursts[i]);
		}
	}

	Vector2f ParticleSystem::computeParticleForce(Particle& particle) 
	{
		Vector2f dragForce = -_particleDrag * particle.velocity;
		return dragForce;
	}

	float ParticleSystem::computeParticleTorque(Particle& particle)
	{
		float dragTorque = -_angularParticleDrag * particle.angularVelocity;
		return dragTorque;
	}

	void ParticleSystem::updateScale(Particle& particle) 
	{
		if (_hasParticleScaleOverLifetime) {
			float t = getNormalizedSecondsSinceBirth(particle);
			particle.setScale(_particleScaleOverLifetime.value(t) * particle.startScale);
		}
	}

	float ParticleSystem::getNormalizedSecondsSinceBirth(const Particle & particle)
	{
		return particle.secondsSinceBirth / particle.lifetime;
	}

	void ParticleSystem::updateVertexColorChannel(std::size_t channelIndex, float& colorChannel, const float& startColorChannel, float t)
	{
		if (_hasParticleColorChannelsOverLifetime[channelIndex])
			colorChannel = startColorChannel * _particleColorChannelsOverLifetime[channelIndex].value(t);
	}

	void ParticleSystem::updateVertexColor(Color& color, const Color& startColor, Particle& particle) 
	{
		float t = getNormalizedSecondsSinceBirth(particle);

		updateVertexColorChannel(0, color.r, startColor.r, t);
		updateVertexColorChannel(1, color.g, startColor.g, t);
		updateVertexColorChannel(2, color.b, startColor.b, t);
		updateVertexColorChannel(3, color.a, startColor.a, t);
	}

	void ParticleSystem::updateVertexColors(Particle& particle) 
	{
		for (std::size_t i = 0; i < particle.vertexColors.size(); i++)
			updateVertexColor(particle.vertexColors[i], particle.startVertexColors[i], particle);
	}

	void ParticleSystem::transformParticle(Particle& particle, float ds) 
	{
		particle.secondsSinceBirth += ds;

		particle.velocity += ds * computeParticleForce(particle);
		particle.angularVelocity += ds * computeParticleTorque(particle);

		particle.translate(ds * particle.velocity);
		particle.rotate(ds * particle.angularVelocity);

		updateScale(particle);
		updateVertexColors(particle);
	}

	void ParticleSystem::transformParticles(float ds) 
	{
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive)
				transformParticle(_particles[i], ds);
		}
	}

	void ParticleSystem::updateParticleVertices(Particle& particle, std::size_t index) 
	{
		std::vector<Vector2f> edges(4);
		edges[0] = particle.getTransform() * Vector2f(-0.5f, -0.5f);
		edges[1] = particle.getTransform() * Vector2f(0.5f, -0.5f);
		edges[2] = particle.getTransform() * Vector2f(-0.5f, 0.5f);
		edges[3] = particle.getTransform() * Vector2f(0.5f, 0.5f);

		_mesh[index * 6 + 0] = Vertex(edges[0], particle.vertexColors[0], Vector2f(0, 0));
		_mesh[index * 6 + 1] = Vertex(edges[0], particle.vertexColors[0], Vector2f(0, 0));
		_mesh[index * 6 + 2] = Vertex(edges[1], particle.vertexColors[1], Vector2f(1, 0));
		_mesh[index * 6 + 3] = Vertex(edges[2], particle.vertexColors[2], Vector2f(0, 1));
		_mesh[index * 6 + 4] = Vertex(edges[3], particle.vertexColors[3], Vector2f(1, 1));
		_mesh[index * 6 + 5] = Vertex(edges[3], particle.vertexColors[3], Vector2f(1, 1));
	}

	void ParticleSystem::updateMesh(float ds) 
	{
		for (std::size_t i = 0; i < _particles.size(); i++) {
			if (_particles[i].isActive)
				updateParticleVertices(_particles[i], i);
		}
	}

	void ParticleSystem::updateParticles(float ds)
	{
		removeDeadParticles();
		if (isPlaying()) {
			emitParticles(ds);
			emitBursts(ds);
			transformParticles(ds);
			updateMesh(ds);
		}
	}

	void ParticleSystem::updateSubSystems(float ds)
	{
		_pool.update();

		std::vector<Pool::Item>& poolItems = _pool.getAllItems();
		for (std::size_t i = 0; i < poolItems.size(); i++) {
			if (poolItems[i].isActive)
				poolItems[i].particleSystem->update(ds);
		}
	}

	void ParticleSystem::updateDying()
	{
		if (!isPlaying())
			_state = State::Garbage;
	}

	void ParticleSystem::drawSubSystems(DrawTarget& target, DrawStates& states)
	{
		std::vector<Pool::Item>& poolItems = _pool.getAllItems();
		for (std::size_t i = 0; i < poolItems.size(); i++) {
			if (poolItems[i].isActive)
				target.draw(*poolItems[i].particleSystem, states);
		}
	}

	ParticleSystem::Pool::Pool()
		: _prototype(NULL), _numActiveItems(0)
	{
	}

	ParticleSystem::Pool::Pool(const Pool& other) : _prototype(NULL), _items(other._items), _numActiveItems(other._numActiveItems) {
		if (other.hasPrototype())
			copy(_prototype, other._prototype); 
	}

	ParticleSystem::Pool & ParticleSystem::Pool::operator=(const Pool & other)
	{
		_items = other._items;
		_numActiveItems = other._numActiveItems;
		if (hasPrototype())
			copy(_prototype, other._prototype);
		return *this;
	}

	ParticleSystem::Pool::~Pool()
	{
		clear();
		delete _prototype;
	}

	void ParticleSystem::Pool::setPrototype(const ParticleSystem& prototype)
	{
		if (_prototype)
			delete _prototype;
		_prototype = new ParticleSystem(prototype);
	}

	ParticleSystem::Pool::Item& ParticleSystem::Pool::getAvailableItem()
	{
		for (std::size_t i = 0; i < _items.size(); i++) {
			if (!_items[i].isActive) {
				activate(_items[i]);
				return _items[i];
			}
		}

		std::size_t oldSize = _items.size();
		expand();
		activate(_items[oldSize]);
		return _items[oldSize];
	}

	void ParticleSystem::Pool::update()
	{
		for (std::size_t i = 0; i < _items.size(); i++) {
			if (_items[i].isActive && !_items[i].particleSystem->isPlaying()) {
				_items[i].isActive = false;
				_numActiveItems--;
			}
		}

		if (_numActiveItems < _items.size() / 2)
			shrink();
	}

	void ParticleSystem::Pool::shrink()
	{
		std::size_t currentSize = _items.size();
		std::size_t newSize = _numActiveItems + (currentSize - _numActiveItems) / 2;

		std::vector<Item> _shrinked;
		_shrinked.reserve(newSize);

		for (std::size_t i = 0; i < _items.size(); i++) {
			if (_items[i].isActive)
				_shrinked.push_back(_items[i]);
			else
				delete _items[i].particleSystem;
		}

		_items = _shrinked;
	}

	void ParticleSystem::Pool::expand()
	{
		std::size_t oldSize = _items.size();
		std::size_t newSize = oldSize == 0 ? 1 : oldSize * 2;
		std::size_t numNewElement = newSize - oldSize;

		for (std::size_t i = 0; i < numNewElement; i++) {
			Item item;
			item.particleSystem = new ParticleSystem(*_prototype);
			item.isActive = false;
			_items.push_back(item);
		}
	}

	void ParticleSystem::Pool::clear()
	{
		for (std::size_t i = 0; i < _items.size(); i++)
			delete _items[i].particleSystem;
		_items.clear();
	}

	void ParticleSystem::Pool::reset(ParticleSystem& particleSystem)
	{
		particleSystem.reset();
		particleSystem.setPosition(_prototype->getPosition());
		particleSystem.setScale(_prototype->getScale());
		particleSystem.setRotation(_prototype->getRotation());
	}

	void ParticleSystem::Pool::activate(Item& item)
	{
		item.isActive = true;
		reset(*item.particleSystem);
		_numActiveItems++;
	}

	ParticleSystem::Pool::Item::Item(const Item &other) : isActive(other.isActive)
	{
		particleSystem = new ParticleSystem(*other.particleSystem);
	}

	ParticleSystem::Pool::Item& ParticleSystem::Pool::Item::operator=(const Item& other)
	{
		isActive = other.isActive;
		particleSystem = new ParticleSystem(*other.particleSystem);
		return *this;
	}
}