#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

out vec4 color;
out vec2 texCoord;

uniform mat4 MVP;

void main() {
  gl_Position = MVP * vec4(inPosition, 1.0f);
  color = vec4(inColor, 1.0f);
  texCoord = inTexCoord;
}
