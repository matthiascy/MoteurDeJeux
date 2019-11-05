#version 330

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec4 pixelColor;

uniform vec3 lightPos;
uniform sampler2D texGrass;
uniform sampler2D texRock;
uniform sampler2D texSnowRock;
uniform sampler2D texOcean;

void main(void)
{
    vec3 L = normalize(lightPos - aPosition);
    float NL = max(dot(normalize(aNormal), L), 0.0);
    vec3 color = vec3(0.39, 1.0f, 0.0);
    vec3 col   = clamp(color* 0.2 + color * 0.8 * NL, 0.0, 1.0f);
    if (aPosition.z < -90) {
      if (aPosition.z > -70) {
        pixelColor = (1 + aPosition.z / 90.0) * texture2D(texOcean, aTexCoord)
                    +(-aPosition.z / 90.0) * texture2D(texGrass, aTexCoord);
      } else {
        pixelColor = texture2D(texOcean, aTexCoord);
      }
    } else if (aPosition.z < -40) {
      pixelColor = texture2D(texGrass, aTexCoord);
    } else if (aPosition.z < 20) {
      pixelColor = texture2D(texRock, aTexCoord);
    } else {
      pixelColor = texture2D(texSnowRock, aTexCoord);
    }
}
