#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

smooth out vec4 smoothColor;

uniform mat4 MVP;
uniform float alpha;

void main() {
  smoothColor = vec4(color, alpha);
  gl_Position = MVP * vec4(position, 1);
}
