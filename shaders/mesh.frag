#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec3 fragWorldNormal;
layout(location = 2) in vec3 fragWorldPosition;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 baseColor =
        fragColor.rgb;

    vec3 normal =
        normalize(fragWorldNormal);

    /*
        方向光方向：
        这里表示光从右上前方照向场景。
    */
    vec3 lightDir =
        normalize(vec3(-0.4, -1.0, -0.3));

    /*
        Lambert:
        表面朝向光源时更亮，背光面更暗。

        注意：
        lightDir 表示光线传播方向。
        计算时常用 -lightDir 表示指向光源的方向。
    */
    float ndotl =
        max(dot(normal, -lightDir), 0.0);

    vec3 ambient =
        baseColor * 0.25;

    vec3 diffuse =
        baseColor * ndotl * 0.85;

    vec3 finalColor =
        ambient + diffuse;

    outColor =
        vec4(finalColor, fragColor.a);
}