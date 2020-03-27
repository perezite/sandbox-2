#pragma once
#include "DrawTarget.h"
#include "Drawable.h"
#include "Window.h"
#include "DrawStates.h"
#include <map>
#include <tuple>

namespace sb
{
	class DrawBatch : public Drawable
	{
	private:
		struct DrawCommand {
			DrawCommand(Drawable& drawable_, DrawStates states_)
				: drawable(drawable_), states(states_)
			{ }

			Drawable& drawable;
			DrawStates states;
		};

		class Buffer : public DrawTarget {
		public:
			Buffer(std::size_t capacity)
			{
				m_vertices.reserve(capacity);
			}

			inline void setTarget(DrawTarget& target) { m_target = &target; }

			using DrawTarget::draw;
			virtual void draw(const std::vector<Vertex>& vertices,
				const PrimitiveType& primitiveType, const DrawStates& states = DrawStates::getDefault());

			void flush();

		protected:
			void assertBufferCapacity(const std::vector<Vertex>& vertices);

			bool mustFlush(const std::vector<Vertex>& vertices, const PrimitiveType primitiveType, const DrawStates& states);

			void insert(const std::vector<Vertex>& vertices, 
				const PrimitiveType& primitiveType, const DrawStates& states);

			inline void transformVertices(std::vector<Vertex>& vertices, const DrawStates& states);

			inline void insertTriangles(const std::vector<Vertex>& vertices);

			inline void insertTriangleStrip(const std::vector<Vertex>& vertices);

		private:
			static std::size_t BatchingThreshold;

			DrawTarget* m_target;

			std::vector<Vertex> m_vertices;

			DrawStates m_currentStates;

			PrimitiveType m_currentPrimitiveType;
		};

	public:
		DrawBatch(std::size_t bufferCapacity = 512)
			: m_buffer(bufferCapacity)
		{
			m_drawCommands.reserve(bufferCapacity / 4);
		}

		void draw(Drawable& drawable, const DrawStates& states = DrawStates::getDefault());

		virtual void draw(DrawTarget& target, DrawStates states);

	private:
		Buffer m_buffer;

		std::vector<DrawCommand> m_drawCommands;
	};
}