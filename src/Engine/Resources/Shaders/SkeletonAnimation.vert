#version 330 core

const int MAX_BONES = 50;
const int MAX_WEIGHTS = 4;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform mat4 uBoneTransforms[MAX_JOINTS];
uniform uint uBoneIndices[MAX_BONES];
uniform float uBoneWeights[MAX_WEIGHTS];

out vec3 aPosition;
out vec3 aNormal;
out vec2 aTexCoord;

mat4 calcBoneTransformMatrix() {
    mat4 mat = mat4(0);
    for (int i = 0; i < MAX_BONES; ++i) {
        mat += uBoneTransforms[uBoneIndices[i]] * uBoneWeights[i];
    }
    return mat;
}

void main()
{
    mat4 boneTransform = calcBoneTransformMatrix();

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * boneTransform * vec4(inPosition, 1.0);

    aPosition = (modelMatrix * boneTransform * vec4(inPosition, 1.0)).xyz;

    aNormal = normalMatrix * (boneTransform * vec4(inNormal, 0.0)).xyz;

    aTexCoord = inTexCoord;
}
