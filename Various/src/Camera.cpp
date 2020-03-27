#include "Camera.h"

namespace sb 
{
	Transform& Camera::getTransform()
	{
		if (m_transformNeedsUpdate)
			updateTransform();

		return m_transform;
	}

	void Camera::setPosition(const sb::Vector2f & position)
	{
		m_position = position;
		m_transformNeedsUpdate = true;
	}

	void Camera::setWidth(float width)
	{
		m_width = width;
		m_transformNeedsUpdate = true;
	}

	void Camera::setAspectRatio(float aspect)
	{
		m_aspectRatio = aspect;
		m_inverseAspectRatio = 1 / aspect;
		m_transformNeedsUpdate = true;
	}

	void Camera::setRotation(float rotation)
	{
		m_rotation = rotation;
		m_transformNeedsUpdate = true;
	}

	void Camera::updateTransform()
	{
		float* m = m_transform.getMatrix();
		float cf = cosf(-m_rotation);
		float sf = sinf(-m_rotation);

		sb::Vector2f inverseScale(2 / m_width, m_aspectRatio * 2 / m_width);
		float a = cf * inverseScale.x;
		float b = -sf * inverseScale.x;
		float c = sf * inverseScale.y;
		float d = cf * inverseScale.y;
		float tx = -m_position.x;
		float ty = -m_position.y;

		m[0] = a;	m[3] = b;	m[6] = a * tx + b * ty;
		m[1] = c;	m[4] = d;	m[7] = c * tx + d * ty;
		m[2] = 0;	m[5] = 0;	m[8] = 1;
	}
}