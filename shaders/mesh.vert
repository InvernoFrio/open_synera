#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(binding = 0) uniform CameraUBO {
    mat4 viewProjection;
} camera;

layout(push_constant) uniform PushConstants {
    mat4 model;
    vec4 color;
    vec4 shadingParams;
    vec4 outlineParams;
} pushConstants;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 fragWorldNormal;
layout(location = 2) out vec3 fragWorldPosition;
layout(location = 3) out vec4 fragShadingParams;
layout(location = 4) out vec4 fragOutlineParams;

void main() {
    vec4 worldPosition =
        pushConstants.model * vec4(inPosition, 1.0);

    mat3 normalMatrix =
        transpose(inverse(mat3(pushConstants.model)));

    vec3 worldNormal =
        normalize(normalMatrix * inNormal);

    float renderMode =
        pushConstants.outlineParams.x;

    float outlineWidth =
        pushConstants.outlineParams.y;

    /*
        renderMode > 0.5 表示描边模式。
        在世界空间沿法线膨胀，避免非等比缩放导致描边厚度完全失控。
    */
    if (renderMode > 0.5) {
        worldPosition.xyz +=
            worldNormal * outlineWidth;
    }

    fragColor =
        pushConstants.color;

    fragWorldNormal =
        worldNormal;

    fragWorldPosition =
        worldPosition.xyz;

    fragShadingParams =
        pushConstants.shadingParams;

    fragOutlineParams =
        pushConstants.outlineParams;

    gl_Position =
        camera.viewProjection * worldPosition;
}