#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec3 aPosition;
out vec3 aNormal;
out vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;


void main()
{
    aPosition = inPosition;
    aNormal = normalMatrix * inNormal;
    aTexCoord = inTexCoord;
    //gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);
    gl_Position = modelMatrix * vec4(aPosition, 1.0);
}