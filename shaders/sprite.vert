#version 450

layout(binding = 0) uniform CameraUBO {
    mat4 viewProjection;
    vec4 cameraRight;
    vec4 cameraUp;
} camera;

layout(push_constant) uniform SpritePushConstants {
    vec4 positionSize;   // xyz = world position, w = unused
    vec4 color;          // rgba
    vec4 params;         // x = width, y = height, z = rotation, w = spriteType
} pushConstants;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragUV;
layout(location = 2) out vec4 fragParams;

void main() {
    vec2 corners[6] = vec2[](
        vec2(-0.5, -0.5),
        vec2( 0.5, -0.5),
        vec2( 0.5,  0.5),

        vec2( 0.5,  0.5),
        vec2(-0.5,  0.5),
        vec2(-0.5, -0.5)
    );

    vec2 uvs[6] = vec2[](
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(1.0, 1.0),

        vec2(1.0, 1.0),
        vec2(0.0, 1.0),
        vec2(0.0, 0.0)
    );

    vec2 corner =
        corners[gl_VertexIndex];

    float width =
        pushConstants.params.x;

    float height =
        pushConstants.params.y;

    float rotation =
        pushConstants.params.z;

    float c =
        cos(rotation);

    float s =
        sin(rotation);

    vec2 rotated;
    rotated.x = corner.x * c - corner.y * s;
    rotated.y = corner.x * s + corner.y * c;

    vec3 worldPosition =
        pushConstants.positionSize.xyz +
        camera.cameraRight.xyz * rotated.x * width +
        camera.cameraUp.xyz * rotated.y * height;

    gl_Position =
        camera.viewProjection * vec4(worldPosition, 1.0);

    fragColor =
        pushConstants.color;

    fragUV =
        uvs[gl_VertexIndex];

    fragParams =
        pushConstants.params;
}