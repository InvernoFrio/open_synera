#version 450

layout(binding = 0) uniform sampler2D uSceneColor;

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

void main() {
    /*
        关键点：
        采样器已经设置为 VK_FILTER_NEAREST，
        所以这里普通 texture 采样就会产生像素放大效果。
    */
    vec4 color =
        texture(uSceneColor, inUV);

    outColor = color;
}