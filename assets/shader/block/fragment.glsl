#version 330 core

in vec2 tex_coord;

uniform vec2 size;
uniform float radius;
uniform vec4 color;
uniform vec2 position;
uniform vec4 border_color;
uniform float border_thickness;

uniform sampler2D tex;
uniform bool texture_enabled;

float roundedBoxSDF(vec2 p, vec2 b, float r) {
    return length(max(abs(p) - b + r, 0.0)) - r;
}
void main() {
    vec4 tex_color = texture(tex, tex_coord);
    vec4 color = tex_color;

    vec2 center = vec2(position.x + size.x / 2.0, position.y - size.y / 2.0);
    float distance = roundedBoxSDF(gl_FragCoord.xy - vec2(0.5, 0.5) - center, size / 2.0 - border_thickness * 15.0, radius * 50.0 - border_thickness * 15.0);
    float distance_border = roundedBoxSDF(gl_FragCoord.xy - vec2(0.5, 0.5) - center, size / 2.0, radius * 50.0);
    if (distance < 0.0) {
        gl_FragColor = color;
    } else if (border_thickness <= 0.0) {
        float alpha = 1.0 - smoothstep(0.0f, 2.0f, distance);
        vec4 mixed_color = vec4(color.xyz, alpha);
        gl_FragColor = mix(mixed_color, color, alpha);
        return;
    } else if (distance_border < 0.0) {
        float alpha = 1.0 - smoothstep(0.0f, 2.0f, distance);
        gl_FragColor = mix(border_color, color, alpha);
    } else {
        float alpha = 1.0 - smoothstep(0.0f, 2.0f, distance_border);
        vec4 mixed_color = vec4(border_color.xyz, alpha);
        gl_FragColor = mix(mixed_color, border_color, alpha);
    }
}
