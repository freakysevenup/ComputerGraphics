#version 120

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec3 normal;

uniform mat4 camera, model, normalMat;

varying vec3 normalInterp;
varying vec3 vertPos;
varying vec2 textureCoords0;

void main()
{
	textureCoords0 = textureCoords;
    gl_Position = camera * model * vec4(position, 1.0);
    vec4 vertPos4 = model * vec4(position, 1.0);
    vertPos = vec3(vertPos4) / vertPos4.w;
    normalInterp = vec3(normalMat * vec4(normal, 0.0));
}