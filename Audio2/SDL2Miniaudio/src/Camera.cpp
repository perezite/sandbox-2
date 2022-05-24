#include "Camera.h"
#include "Window.h"

namespace sb 
{
    Camera::Camera(Window & window)
        : m_parentWindow(window), m_rotation(0), m_transformNeedsUpdate(true)
    {
        requestSize(1, 1);
    }

    Transform& Camera::getTransform()
	{
        if (m_transformNeedsUpdate) {
			updateTransform();
            m_transformNeedsUpdate = false;
        }

		return m_transform;
	}
	
    void Camera::setCenter(const sb::Vector2f& center)
	{
		m_center = center;
		m_transformNeedsUpdate = true;
	}

	void Camera::setRotation(float rotation)
	{
		m_rotation = rotation;
		m_transformNeedsUpdate = true;
	}

    void Camera::requestSize(float width, float height)
    {
        const Vector2i& windowSize = m_parentWindow.getSize();
        const float windowAspect = float(windowSize.x) / float(windowSize.y);
        const float cameraAspect = width / height;
        m_requestedSize = Vector2f(width, height);

        if (windowAspect > cameraAspect) {
            float widthScale = windowAspect / cameraAspect;
            m_size = Vector2f(widthScale * width, height);
        } else {
            float heightScale = cameraAspect / windowAspect;
            m_size = Vector2f(width, heightScale * height);
        }
    }

    void Camera::refreshSize() 
    {
        requestSize(m_requestedSize);
    }

	void Camera::updateTransform()
	{
		float* m = m_transform.getMatrix();
		float cf = cosf(-m_rotation);
		float sf = sinf(-m_rotation);

        sb::Vector2f inverseScale(2 / m_size.x, 2 / m_size.y);
		float a = cf * inverseScale.x;
		float b = -sf * inverseScale.x;
		float c = sf * inverseScale.y;
		float d = cf * inverseScale.y;
		float tx = -m_center.x;
		float ty = -m_center.y;

		m[0] = a;	m[3] = b;	m[6] = a * tx + b * ty;
		m[1] = c;	m[4] = d;	m[7] = c * tx + d * ty;
		m[2] = 0;	m[5] = 0;	m[8] = 1;
	}
}