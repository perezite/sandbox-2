#include "Transformable.h"

namespace sb 
{
	const Transform& Transformable::getTransform()
	{
		if (m_transformNeedsUpdate)
			updateTransform();

		return m_transform;
	}

	void Transformable::setPosition(const Vector2f& position)
	{
		m_position = position;
		m_transformNeedsUpdate = true;
	}

	void Transformable::setScale(const sb::Vector2f& scale)
	{
		m_scale = scale;
		m_transformNeedsUpdate = true;
	}

	void Transformable::setRotation(float radians)
	{
		m_rotation = radians;
		m_transformNeedsUpdate = true;
	}

	void Transformable::updateTransform()
	{
		m_transform = Transform(m_position, m_scale, m_rotation);
		m_transformNeedsUpdate = false;
	}
}