#version 330 core


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct AmbientLight {
    vec3 color;
    float intensity;
};


struct Lighting {
    float ambientCoeff;
    float diffuseCoeff;
    float specularCoeff;

    AmbientLight     ambient;
    DirectionalLight directional;
};


uniform Material material;
uniform Lighting lighting;
uniform vec3 eyePosition;

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec4 pixelColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main()
{
    vec4 baseColor = texture2D(texture0, aTexCoord.st);

    // Ambient
    vec3 ambient = lighting.ambient.intensity * lighting.ambient.color;

    // Diffuse
    // Angle between incident light and normal of the surface
    float diffuseFactor = max(dot(aNormal, lighting.directional.direction), 0);
    vec3 diffuse = vec3(lighting.directional.color * lighting.directional.intensity * diffuseFactor);

    // Specular
    vec3 viewDirection = normalize(eyePosition - aPosition);
    vec3 reflectDirction = reflect(-lighting.directional.direction, aNormal);
    float spec = pow(max(dot(viewDirection, reflectDirction), 0.0), 32);
    vec3 specular = spec * lighting.directional.color * lighting.directional.intensity;

    vec3 lightColor = ambient * lighting.ambientCoeff +
                      diffuse * lighting.diffuseCoeff +
                      specular * lighting.specularCoeff;

    pixelColor = vec4(baseColor.rgb * lightColor, 1.0f);
}
