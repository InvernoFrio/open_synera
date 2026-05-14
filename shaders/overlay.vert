#version 450

layout(push_constant) uniform OverlayPushConstants {
    vec4 rect;    // x, y, width, height in pixels
    vec4 color;   // rgba
    vec4 params;  // x shapeType, y rotation, z targetW, w targetH
    vec4 extra;   // x digit, y/z/w reserved
} pushConstants;

layout(location = 0) out vec2 fragUV;
layout(location = 1) out vec4 fragColor;
layout(location = 2) out vec4 fragParams;
layout(location = 3) out vec4 fragExtra;

void main() {
    vec2 corners[6] = vec2[](
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(1.0, 1.0),

        vec2(1.0, 1.0),
        vec2(0.0, 1.0),
        vec2(0.0, 0.0)
    );

    vec2 uv =
        corners[gl_VertexIndex];

    vec2 local =
        uv - vec2(0.5);

    float rotation =
        pushConstants.params.y;

    float c =
        cos(rotation);

    float s =
        sin(rotation);

    vec2 rotated;
    rotated.x =
        local.x * c - local.y * s;
    rotated.y =
        local.x * s + local.y * c;

    vec2 pixelPosition =
        pushConstants.rect.xy +
        (rotated + vec2(0.5)) * pushConstants.rect.zw;

    float targetWidth =
        pushConstants.params.z;

    float targetHeight =
        pushConstants.params.w;

    vec2 ndc;
    ndc.x =
        pixelPosition.x / targetWidth * 2.0 - 1.0;

    /*
        Overlay 使用左上角为原点的屏幕坐标。
        Vulkan NDC Y 向下为正时，这里转成 clip 坐标。
    */
    ndc.y =
        pixelPosition.y / targetHeight * 2.0 - 1.0;

    gl_Position =
        vec4(ndc, 0.0, 1.0);

    fragUV =
        uv;

    fragColor =
        pushConstants.color;

    fragParams =
        pushConstants.params;

    fragExtra =
        pushConstants.extra;
}