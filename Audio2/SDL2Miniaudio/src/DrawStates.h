#pragma once
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"

namespace sb
{
	struct DrawStates
	{
		Transform transform = Transform::Identity;

		Transform textureTransform = Transform::Identity; 

		const Texture* texture = NULL;

		Shader* shader = NULL;

		DrawStates()
		{ }

		DrawStates(const Transform& transform_)
			: transform(transform_)
		{ }

		DrawStates(const Texture& texture_)
			: texture(&texture_)
		{ }

		DrawStates(Shader& shader_)
			: shader(&shader_)
		{ }

		DrawStates(const Transform& transform_, const Texture& texture_, Shader& shader_)
			: transform(Transform::Identity), texture(&texture_), shader(&shader_)
		{ }

		static DrawStates& getDefault();
	};

	const bool operator ==(const DrawStates& left, const DrawStates& right);

	inline const bool operator !=(const DrawStates& left, const DrawStates& right) { return !(left == right); }
}
