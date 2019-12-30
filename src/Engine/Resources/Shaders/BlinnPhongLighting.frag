#version 330 core

const int kMaxPointLights = 16;
const int kMaxSpotLights = 16;
const int kMaxDirectionalLights = 2;
const int kMaxTextures = 4;

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

struct Attenuation {
    float constant;
    float linear;
    float exponent;
};

struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float range;
    float intensity;
    float cutOff;
    Attenuation attenuation;
};

struct PointLight {
    vec3 color;
    vec3 position;
    float range;
    float intensity;
    Attenuation attenuation;
};

struct Lighting {
    float ambientCoeff;
    float diffuseCoeff;
    float specularCoeff;

    AmbientLight     ambient;
    DirectionalLight[kMaxDirectionalLights] directionalLights;
    SpotLight[kMaxSpotLights]               spotLights;
    PointLight[kMaxPointLights]             pointLights;
};

struct _textures {
    int num;
    sampler2D[kMaxTextures] textures;
};

struct LightColor {
    vec3 diffuse;
    vec3 specular;
};

struct Textures {
    _textures diffuse;
    _textures specular;
    _textures ambient;
    _textures emissive;
    _textures height;
    _textures normals;
    _textures shininess;
    _textures opacity;
    _textures lightmap;
    _textures reflection;
};

uniform Material material;
uniform Lighting lighting;
uniform Textures textures;

uniform int spotLightsNum;
uniform int pointLightsNum;
uniform int directionalLightsNum;
uniform vec3 eyePosition;

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec4 pixelColor;

LightColor calcPointLight(PointLight light, vec3 normal, vec3 position, vec3 eye);

LightColor calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 position, vec3 eye);

LightColor calcSpotLight(SpotLight light, vec3 normal, vec3 position, vec3 eye);

vec3 calcDiffuse(vec3 color, vec3 direction, float intensity, vec3 normal);
vec3 calcSpecular(vec3 color, vec3 direction, float intensity, vec3 normal, vec3 position, vec3 eye);

void main()
{
    vec4 baseColor = texture2D(textures.diffuse.textures[0], aTexCoord.st);

    // Ambient
    vec3 ambientColor = lighting.ambient.intensity * lighting.ambient.color;

    // Diffuse and Specular
    LightColor spot = LightColor(vec3(0, 0, 0), vec3(0, 0, 0));
    for (int i = 0; i < min(spotLightsNum, kMaxSpotLights); ++i) {
        LightColor tmp = calcSpotLight(lighting.spotLights[i], aNormal, aPosition, eyePosition);
        spot.diffuse += tmp.diffuse;
        spot.specular += tmp.specular;
    }

    LightColor point = LightColor(vec3(0, 0, 0), vec3(0, 0, 0));
    for (int i = 0; i < min(pointLightsNum, kMaxPointLights); ++i) {
        LightColor tmp = calcPointLight(lighting.pointLights[i], aNormal, aPosition, eyePosition);
        point.diffuse += tmp.diffuse;
        point.specular += tmp.specular;
    }

    LightColor directional = LightColor(vec3(0, 0, 0), vec3(0, 0, 0));
    for (int i = 0; i < min(directionalLightsNum, kMaxDirectionalLights); ++i){
        LightColor tmp = calcDirectionalLight(lighting.directionalLights[i], aNormal, aPosition, eyePosition);
        directional.diffuse += tmp.diffuse;
        directional.specular += tmp.specular;
    }

    vec3 lightColor = lighting.ambientCoeff * ambientColor +
                      lighting.diffuseCoeff * (point.diffuse + spot.diffuse + directional.diffuse)  +
                      lighting.specularCoeff * (point.specular + spot.specular + directional.specular);

    pixelColor = vec4(baseColor.rgb * lightColor, 1.0f);
}

LightColor calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 position, vec3 eye)
{
    vec3 diffuse = calcDiffuse(light.color, -light.direction, light.intensity, normal);
    vec3 specular = calcSpecular(light.color, light.direction, light.intensity, normal, position, eye);

    return LightColor(diffuse, specular);
}

LightColor calcSpotLight(SpotLight light, vec3 normal, vec3 position, vec3 eye)
{
    vec3 light_to_pixel = normalize(position - light.position);
    float spot_factor = dot(light_to_pixel, light.direction);

    if (spot_factor > light.cutOff) {
        PointLight p = PointLight(light.color,
                                  light.position,
                                  light.range,
                                  light.intensity,
                                  Attenuation(light.attenuation.constant, light.attenuation.linear, light.attenuation.exponent));
        LightColor color = calcPointLight(p, normal, position, eye);
        color.diffuse * (1.0 - (1.0 - spot_factor) * 1.0 / (1.0 - light.cutOff));
        color.specular * (1.0 - (1.0 - spot_factor) * 1.0 / (1.0 - light.cutOff));

        return color;
    } else {
        return LightColor(vec3(0, 0, 0), vec3(0, 0, 0));
    }
}

LightColor calcPointLight(PointLight light, vec3 normal, vec3 position, vec3 eye)
{
    vec3 direction = position - light.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec3 diffuse = calcDiffuse(light.color, -direction, light.intensity, normal);
    vec3 specular = calcSpecular(light.color, direction, light.intensity, normal, position, eye);

    float attenuation = light.attenuation.constant +
                        light.attenuation.linear * distance +
                        light.attenuation.exponent * distance;

    return LightColor(diffuse / attenuation, specular / attenuation);
}

vec3 calcDiffuse(vec3 color, vec3 direction, float intensity, vec3 normal)
{
    float diffuse_factor = max(dot(normal, direction), 0);
    return color * intensity * diffuse_factor;
}

vec3 calcSpecular(vec3 color, vec3 direction, float intensity, vec3 normal, vec3 position, vec3 eye)
{
    vec3 view_direction = normalize(eye - position);
    vec3 reflect_direction = reflect(direction, normal);
    float specular_factor = pow(max(dot(view_direction, reflect_direction), 0), 32);
    return specular_factor * color * intensity;
}