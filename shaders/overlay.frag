#version 450

layout(location = 0) in vec2 fragUV;
layout(location = 1) in vec4 fragColor;
layout(location = 2) in vec4 fragParams;
layout(location = 3) in vec4 fragExtra;

layout(location = 0) out vec4 outColor;

float rectMask(
    vec2 uv,
    vec2 center,
    vec2 halfSize
) {
    vec2 d =
        abs(uv - center) - halfSize;

    float outside =
        length(max(d, vec2(0.0)));

    float inside =
        min(max(d.x, d.y), 0.0);

    float dist =
        outside + inside;

    /*
        非像素化跳字：
        这里故意保留 smooth edge，不做硬阈值。
    */
    return
        1.0 - smoothstep(0.0, 0.025, dist);
}

bool digitSegmentOn(
    int digit,
    int seg
) {
    /*
        7 段数字：
             0
          5     1
             6
          4     2
             3
    */

    if (digit == 0) {
        return seg == 0 || seg == 1 || seg == 2 ||
               seg == 3 || seg == 4 || seg == 5;
    }

    if (digit == 1) {
        return seg == 1 || seg == 2;
    }

    if (digit == 2) {
        return seg == 0 || seg == 1 || seg == 6 ||
               seg == 4 || seg == 3;
    }

    if (digit == 3) {
        return seg == 0 || seg == 1 || seg == 6 ||
               seg == 2 || seg == 3;
    }

    if (digit == 4) {
        return seg == 5 || seg == 6 || seg == 1 ||
               seg == 2;
    }

    if (digit == 5) {
        return seg == 0 || seg == 5 || seg == 6 ||
               seg == 2 || seg == 3;
    }

    if (digit == 6) {
        return seg == 0 || seg == 5 || seg == 6 ||
               seg == 4 || seg == 2 || seg == 3;
    }

    if (digit == 7) {
        return seg == 0 || seg == 1 || seg == 2;
    }

    if (digit == 8) {
        return true;
    }

    if (digit == 9) {
        return seg == 0 || seg == 1 || seg == 2 ||
               seg == 3 || seg == 5 || seg == 6;
    }

    return false;
}

float digitMask(
    vec2 uv,
    int digit
) {
    float mask = 0.0;

    /*
        uv: 0..1
        预留一点边距。
    */

    vec2 centers[7] = vec2[](
        vec2(0.5, 0.88), // top
        vec2(0.82, 0.68), // upper right
        vec2(0.82, 0.32), // lower right
        vec2(0.5, 0.12), // bottom
        vec2(0.18, 0.32), // lower left
        vec2(0.18, 0.68), // upper left
        vec2(0.5, 0.50)  // middle
    );

    vec2 sizes[7] = vec2[](
        vec2(0.26, 0.055),
        vec2(0.055, 0.22),
        vec2(0.055, 0.22),
        vec2(0.26, 0.055),
        vec2(0.055, 0.22),
        vec2(0.055, 0.22),
        vec2(0.26, 0.055)
    );

    for (int i = 0; i < 7; i++) {
        if (digitSegmentOn(digit, i)) {
            mask =
                max(
                    mask,
                    rectMask(
                        uv,
                        centers[i],
                        sizes[i]
                    )
                );
        }
    }

    return mask;
}

void main() {
    float shapeType =
        fragParams.x;

    vec4 color =
        fragColor;

    float alpha = 1.0;

    if (shapeType > 0.5 && shapeType < 1.5) {
        int digit =
            int(floor(fragExtra.x + 0.5));

        alpha =
            digitMask(
                fragUV,
                digit
            );
    } else {
        /*
            SolidRect 默认直接显示。
        */
        alpha = 1.0;
    }

    if (alpha <= 0.001) {
        discard;
    }

    outColor =
        vec4(
            color.rgb,
            color.a * alpha
        );
}