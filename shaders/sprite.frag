#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in vec4 fragParams;

layout(location = 0) out vec4 outColor;

void main() {
    /*
        程序化像素 Sprite：
        先做一个菱形/圆点混合的小闪光。
        后续接 TextureAtlas 后这里会改成采样贴图。
    */

    vec2 uv =
        fragUV * 2.0 - 1.0;

    float dist =
        length(uv);

    float diamond =
        abs(uv.x) + abs(uv.y);

    float alphaCircle =
        1.0 - smoothstep(0.55, 0.95, dist);

    float alphaDiamond =
        1.0 - smoothstep(0.75, 1.1, diamond);

    float alpha =
        max(alphaCircle, alphaDiamond);

    /*
        低分辨率下 smoothstep 仍然会有一点软边。
        为了更像像素风，做一次硬阈值。
    */
    alpha =
        alpha > 0.25 ? 1.0 : 0.0;

    if (alpha <= 0.0) {
        discard;
    }

    outColor =
        vec4(
            fragColor.rgb,
            fragColor.a * alpha
        );
}