#version 120

varying vec3 textureCoords;
uniform samplerCube skybox;

void main()
{    
    gl_FragColor = texture(S_skybox, texCoord);
}