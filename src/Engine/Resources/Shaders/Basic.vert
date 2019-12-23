#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec3 aPosition;
out vec3 aNormal;
out vec2 aTexCoord;

//uniform mat4 modelMatrix;
//uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

void main(void)
{
    aPosition = inPosition;
    aNormal = normalMatrix * inNormal;
    aTexCoord = inTexCoord;
    gl_Position = projMatrix * modelViewMatrix * vec4(inPosition, 1.0);
}
