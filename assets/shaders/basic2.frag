#version 330 core

smooth in vec4 smoothColor;

out vec4 fragColor;

void main() {
  fragColor = smoothColor;
}
