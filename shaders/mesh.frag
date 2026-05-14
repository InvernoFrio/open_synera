#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec3 fragWorldNormal;
layout(location = 2) in vec3 fragWorldPosition;
layout(location = 3) in vec4 fragShadingParams;
layout(location = 4) in vec4 fragOutlineParams;

layout(location = 0) out vec4 outColor;

void main() {
    float renderMode =
        fragOutlineParams.x;

    /*
        描边模式：
        直接输出描边颜色，不参与光照。
    */
    if (renderMode > 0.5) {
        outColor = fragColor;
        return;
    }

    vec3 baseColor =
        fragColor.rgb;

    vec3 normal =
        normalize(fragWorldNormal);

    vec3 lightDir =
        normalize(vec3(-0.4, -1.0, -0.3));

    float ndotl =
        max(dot(normal, -lightDir), 0.0);

    float shadeSteps =
        max(fragShadingParams.x, 1.0);

    float shadowStrength =
        clamp(fragShadingParams.y, 0.0, 1.0);

    float ambient =
        clamp(fragShadingParams.z, 0.0, 1.0);

    float stepped =
        floor(ndotl * shadeSteps) /
        max(shadeSteps - 1.0, 1.0);

    stepped =
        clamp(stepped, 0.0, 1.0);

    float lightFactor =
        mix(
            ambient,
            1.0,
            stepped
        );

    lightFactor =
        mix(
            1.0,
            lightFactor,
            shadowStrength
        );

    vec3 finalColor =
        baseColor * lightFactor;

    outColor =
        vec4(finalColor, fragColor.a);
}