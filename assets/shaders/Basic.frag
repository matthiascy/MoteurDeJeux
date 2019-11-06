#version 330

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec4 pixelColor;

uniform vec3 lightPos;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform bool isTerrain;

void main(void)
{
    vec3 L = normalize(lightPos - aPosition);
    float NL = max(dot(normalize(aNormal), L), 0.0);
    vec3 color = vec3(0.39, 1.0f, 0.0);
    vec3 col   = clamp(color* 0.2 + color * 0.8 * NL, 0.0, 1.0f);
    if (isTerrain) {
        if (aPosition.z < -90) {
            if (aPosition.z > -110) {
                pixelColor = (1 + aPosition.z / 90.0) * texture2D(texture3, aTexCoord)
                +(-aPosition.z / 90.0) * texture2D(texture0, aTexCoord);
            } else {
                pixelColor = texture2D(texture3, aTexCoord);
            }
        } else if (aPosition.z < -60) {
            pixelColor = texture2D(texture0, aTexCoord);
        } else if (aPosition.z < 10) {
            pixelColor = texture2D(texture1, aTexCoord);
        } else {
            pixelColor = texture2D(texture2, aTexCoord);
        }
    } else {
        pixelColor = texture2D(texture0, aTexCoord);
    }
}
