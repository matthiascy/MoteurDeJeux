#version 330 core

in vec3 aColor;

out vec4 pixelColor;

void main()
{
    pixelColor = vec4(aColor, 1.0);
}