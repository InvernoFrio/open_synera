#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(binding = 0) uniform CameraUBO {
    mat4 viewProjection;
} camera;

layout(push_constant) uniform PushConstants {
    mat4 model;
    vec4 color;
} pushConstants;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 fragWorldNormal;
layout(location = 2) out vec3 fragWorldPosition;

void main() {
    vec4 worldPosition =
        pushConstants.model * vec4(inPosition, 1.0);

    /*
        normalMatrix 用于把模型空间法线转换到世界空间。
        如果模型存在非等比缩放，用 transpose(inverse(mat3(model))) 更正确。
    */
    mat3 normalMatrix =
        transpose(inverse(mat3(pushConstants.model)));

    fragColor =
        pushConstants.color;

    fragWorldNormal =
        normalize(normalMatrix * inNormal);

    fragWorldPosition =
        worldPosition.xyz;

    gl_Position =
        camera.viewProjection * worldPosition;
}