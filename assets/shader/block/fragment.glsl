#version 330 core
out vec4 fragColor;

in vec2 position;
in vec2 size;
in float radius;
in vec4 color;

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius) {
    return length(max(abs(CenterPosition) - Size + Radius, 0.0)) - Radius;
}
void main() {
    float edgeSoftness = 1.0f;
    float r = radius * 30.0f;
    float distance = roundedBoxSDF(gl_FragCoord.xy - location - (size / 2.0f), size / 2.0f, r);
    float smoothedAlpha = 1.0f - smoothstep(0.0f, edgeSoftness * 2.0f, distance);

    vec4 quadColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), color, smoothedAlpha);

    fragColor = quadColor;
}
