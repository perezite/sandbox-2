#pragma once
#include "Vector2.h"
#include "Transform.h"

namespace sb 
{
    class Window;

	class Camera
	{
	public:
        Camera(Window& window);

		Transform& getTransform();

		inline const sb::Vector2f& getCenter() const { return m_center; }

		inline float getRotation() const { return m_rotation; }

        inline const sb::Vector2f& getSize() const { return m_size; }

        inline const sb::Vector2f& getRequestedSize() const { return m_requestedSize; }

        inline float getWidth() const { return getSize().x; }

        inline float getHeight() const { return getSize().y; }
        
        inline float getAspectRatio() const { return getWidth() / getHeight(); }

        inline float getInverseAspectRatio() const { return getHeight() / getWidth(); }

        inline void setCenter(float x, float y) { setCenter(sb::Vector2f(x, y)); }

		void setCenter(const sb::Vector2f& center);

		void setRotation(float rotation);
	
        void requestSize(float width, float height);

        inline void requestSize(const sb::Vector2f& size) { requestSize(size.x, size.y); }

        void refreshSize();

		inline void translate(const sb::Vector2f& translation) { setCenter(getCenter() + translation); }

		inline void rotate(float angle) { setRotation(getRotation() + angle); }

	protected:
		void updateTransform();

	private:
        Window& m_parentWindow;

		sb::Vector2f m_center;

        sb::Vector2f m_size;

        sb::Vector2f m_requestedSize;

		float m_rotation;

		Transform m_transform;

		bool m_transformNeedsUpdate;
	};
}