#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 mvp;
smooth out vec4 smoothColor;
out vec2 texCoord;

void main() {
  texCoord    = inTexCoord;
  smoothColor = vec4(inColor, 1.0f);
  gl_Position = mvp * vec4(inPosition, 1.0f);
}
