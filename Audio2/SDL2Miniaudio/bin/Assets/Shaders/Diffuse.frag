#version 100
precision mediump float;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform sampler2D texture;	

void main()
{
	gl_FragColor = texture2D(texture, v_texCoords) * v_color;
}