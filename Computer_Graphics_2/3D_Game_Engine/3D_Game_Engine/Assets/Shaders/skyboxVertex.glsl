#version 120
attribute vec3 position;
attribute vec3 textureCoords;
attribute vec3 normal;

uniform mat4 camera;

void main()
{
    vec4 pos = camera * vec4(position, 1.0);
    gl_Position = pos.xyww;
    textureCoords = position;
} 