#version 330 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec4 pixelColor;

void main()
{
    pixelColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}