#include "Transform.h"

namespace sb
{
	Transform Transform::Identity = Transform();

	Transform::Transform()
	{
		m_matrix[0] = 1; m_matrix[3] = 0; m_matrix[6] = 0;
		m_matrix[1] = 0; m_matrix[4] = 1; m_matrix[7] = 0;
		m_matrix[2] = 0; m_matrix[5] = 0; m_matrix[8] = 1;
	}

	Transform::Transform(Vector2f position, Vector2f scale, float rotation)
	{
		float c = cosf(rotation);
		float s = sinf(rotation);

		m_matrix[0] = c * scale.x;	m_matrix[3] = -s * scale.y;	m_matrix[6] = position.x;
		m_matrix[1] = s * scale.x;	m_matrix[4] = c * scale.y;	m_matrix[7] = position.y;
		m_matrix[2] = 0;			m_matrix[5] = 0;			m_matrix[8] = 1;
	}

	Transform::Transform( 
		float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22)
	{
		m_matrix[0] = a00; m_matrix[3] = a01; m_matrix[6] = a02;
		m_matrix[1] = a10; m_matrix[4] = a11; m_matrix[7] = a12;
		m_matrix[2] = a20; m_matrix[5] = a21; m_matrix[8] = a22;
	}

	const Transform& Transform::translate(const Vector2f& position)
	{
		Transform translation(
			1, 0, position.x,
			0, 1, position.y,
			0, 0, 1);

		return apply(translation);
	}

	const Transform& Transform::scale(const Vector2f& scale)
	{
		Transform scaling(
			scale.x, 0, 0,
			0, scale.y, 0,
			0, 0, 1);

		return apply(scaling);
	}

	const Transform& Transform::rotate(const float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);

		Transform rotation(
			c, -s, 0,
			s,  c, 0,
			0,  0, 1);

		return apply(rotation);
	}

	const Transform& Transform::apply(const Transform& transform)
	{
		const float* a = m_matrix;
		const float* b = transform.getMatrix();

		*this = Transform(
			a[0] * b[0] + a[3] * b[1] + a[6] * b[2],
			a[0] * b[3] + a[3] * b[4] + a[6] * b[5],
			a[0] * b[6] + a[3] * b[7] + a[6] * b[8],
			a[1] * b[0] + a[4] * b[1] + a[7] * b[2],
			a[1] * b[3] + a[4] * b[4] + a[7] * b[5],
			a[1] * b[6] + a[4] * b[7] + a[7] * b[8],
			a[2] * b[0] + a[5] * b[1] + a[8] * b[2],
			a[2] * b[3] + a[5] * b[4] + a[8] * b[5],
			a[2] * b[6] + a[5] * b[7] + a[8] * b[8]);

		return *this;
	}

	const Vector2f& Transform::transform(Vector2f& vector) const
	{
		vector = Vector2f(
			m_matrix[0] * vector.x + m_matrix[3] * vector.y + m_matrix[6],
			m_matrix[1] * vector.x + m_matrix[4] * vector.y + m_matrix[7]);

		return vector;
	}

	const Mesh& Transform::transform(Mesh& mesh) const
	{
		for (std::size_t i = 0; i < mesh.getVertexCount(); i++) {
			Vector2f& pos = mesh[i].position;
			pos *= *this;
		}

		return mesh;
	}

	Transform& operator *=(Transform& left, const Transform& right)
	{
		left.apply(right);
		return left;
	}

	Transform operator*(const Transform& left, const Transform& right)
	{
		return Transform(left).apply(right);
	}

	const Vector2f& operator*=(Vector2f& vector, const Transform& transform)
	{
		return transform.transform(vector);
	}

	Vector2f operator*(const Transform& transform, const Vector2f& vector)
	{
		sb::Vector2f transformed = vector;
		return transform.transform(transformed);
	}

	Mesh operator*(const Transform& transform, const Mesh& mesh)
	{
		Mesh result(mesh);
		return transform.transform(result);
	}
}

