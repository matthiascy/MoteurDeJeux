#version 330 core

const int MAX_BONES = 128;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;
layout (location = 5) in ivec4 inBoneIds0;
layout (location = 6) in ivec4 inBoneIds1;
layout (location = 7) in vec4 inBoneWeights0;
layout (location = 8) in vec4 inBoneWeights1;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform mat4 uBoneTransforms[MAX_BONES];
uniform bool hasAnimation;

out vec3 aPosition;
out vec3 aNormal;
out vec2 aTexCoord;

mat4 calcBoneTransformMatrix() {
    mat4 mat = mat4(0);
    for (int i = 0; i < 4; ++i) {
        mat += uBoneTransforms[inBoneIds0[i]] * inBoneWeights0[i];
        mat += uBoneTransforms[inBoneIds1[i]] * inBoneWeights1[i];
    }
    return mat;
}

void main()
{
    mat4 boneTransform = mat4(1.0);
    if (hasAnimation) {
        boneTransform = calcBoneTransformMatrix();
    }

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * boneTransform * vec4(inPosition, 1.0);

    aPosition = (modelMatrix * boneTransform * vec4(inPosition, 1.0)).xyz;

    aNormal = normalMatrix * (boneTransform * vec4(inNormal, 0.0)).xyz;

    aTexCoord = inTexCoord;
}
