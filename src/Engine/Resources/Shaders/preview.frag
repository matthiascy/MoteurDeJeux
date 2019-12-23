#version 330 core

out vec4 fragColor;

uniform sampler2D tex;
smooth in vec4 smoothColor;
in vec2 texCoord;

void main() {
  fragColor = smoothColor;
  //fragColor = texture(tex, texCoord);
}
