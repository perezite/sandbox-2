#include "Vector2.h"
#include "Transformable.h"
#include "Body.h"
#include "Window.h"

namespace sb
{
	class Physics 
	{
	public:
		Physics(float aspect)
			: _dragCoefficient(10), _fixedDeltaSeconds(0.02f)
		{ }

		inline void setDragCoefficient(float drag) { _dragCoefficient = drag; }

		void addBody(Body& body);

		void simulate(float ds, sb::Window& window);

	protected:
		void prepare();

		const Vector2f computeCollisionForce(const Body& current, const Body& other);

		void computeCollisionForces();

		sb::Vector2f computeBoundaryForce(const Body& fruit, sb::Window& window);

		void computeBoundaryForces(sb::Window& window);

		void computeDragForces();

		void computeForces(sb::Window& window);

		void moveBodies(Body& body, const sb::Vector2f& force, float ds);

		void moveBodies(float ds);

		void step(float ds, sb::Window& window);

	private:
		std::vector<Body*> _bodies;

		std::vector<sb::Vector2f> _forces;

		float _dragCoefficient;

		float _fixedDeltaSeconds;
	};
}
