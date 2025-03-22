#version 330 core
uniform vec2 size;
uniform float radius;
uniform vec4 color;
uniform vec2 position;

float roundedBoxSDF(vec2 p, vec2 b, float r) {
    return length(max(abs(p) - b + r, 0.0)) - r;
}
void main() {
    vec2 center = vec2(position.x + size.x / 2.0, position.y - size.y / 2.0);
    float distance = roundedBoxSDF(gl_FragCoord.xy - center, size / 2.0, radius * 100.0);
    float alpha = (1.0 - smoothstep(0.0f, 2.0f, distance)) * color.w;
    vec4 mixed_color = vec4(color.xyz, alpha);
    gl_FragColor = mix(mixed_color, color, alpha / color.w);
}
