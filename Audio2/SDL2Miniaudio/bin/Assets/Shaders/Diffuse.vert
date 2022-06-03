attribute vec2 position;
attribute vec4 color;
attribute vec2 texCoords;

uniform mat3 transform;

varying vec4 v_color;
varying vec2 v_texCoords;

vec3 transformedPosition;

void main()
{					
	transformedPosition = transform * vec3(position.x, position.y, 1);
	gl_Position = vec4(transformedPosition.x, transformedPosition.y, 0, 1 );
	
	v_color = color;
	v_texCoords = texCoords;
}