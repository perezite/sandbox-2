#pragma once

#include "Drawable.h"
#include "Renderer.h"
#include "DrawTarget.h"
#include "Camera.h"
#include "Stopwatch.h"
#include "SDL.h"

namespace sb
{
	class Window : public DrawTarget
	{
	public:
		Window(sb::Vector2i size) : Window(size.x, size.y)
		{ }

		Window(int width = 400, int height = 400);

		virtual ~Window();

		inline const sb::Vector2i& getSize() const { return m_size; }

		inline bool isOpen() { return m_isOpen; }

		inline const Camera& getCamera() const { return m_camera; }

		inline Camera& getCamera() { return m_camera; }

        Uint32 getWindowId() const;

        void setSize(int width, int height);

        inline void setSize(sb::Vector2i& size) { setSize(size.x, size.y); }

        void setFramerateLimit(int framerateLimit);

		void update();

		void clear(const Color& clearColor = Color(0, 0, 0, 1));
		
		using DrawTarget::draw;
		virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType = PrimitiveType::Triangles, 
			const DrawStates& states = DrawStates::getDefault());

		void display();

    protected:
        void onResize();

        void limitFramerate();

	private:
		bool m_isOpen;

		SDL_Window* m_sdlWindow;

		SDL_GLContext m_glContext;

		Renderer* m_renderer;

		Vector2i m_size;

        Stopwatch m_stopwatch;

        float m_minimumMsPerFrame;

		Camera m_camera;

	};
}