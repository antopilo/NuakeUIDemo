#version 460 core


in sample vec2 v_UV;
out vec4 FragColor;

uniform sampler2D u_Atlas;
uniform sample vec2 u_TexturePos;
uniform vec2 u_TextureScale;

uniform vec4 u_FontColor;

uniform float u_PxRange;

float screenPxRange(vec2 coord) {
    vec2 unitRange = vec2(u_PxRange) / vec2(textureSize(u_Atlas, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(coord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec2 textSize = textureSize(u_Atlas, 0);

    vec2 uv = vec2(mix(u_TexturePos.x / textSize.x, u_TextureScale.x / textSize.x, v_UV.x),
                   mix(u_TextureScale.y / textSize.y, u_TexturePos.y / textSize.y, 1.0 - v_UV.y));

    vec3 msd = texture(u_Atlas, uv).rgb;
    float sd = median(msd.r, msd.g, msd.b);

    float r = msd.r;
    float g = msd.g;
    float b = msd.b;
    float median = max(min(r, g), min(max(r, g), b));
    float signed_dist = median - 0.5;
    float d = fwidth(signed_dist);
    float opacity = smoothstep(-d, d, signed_dist);
    FragColor = mix(vec4(0), u_FontColor, opacity);
}