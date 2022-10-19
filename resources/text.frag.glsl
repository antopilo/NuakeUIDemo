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

float opacity(vec4 sdfSample)
{
    float r = sdfSample.r;
    float g = sdfSample.g;
    float b = sdfSample.b;
    float signed_dist = median(r, g, b) - 0.5;
    float d = fwidth(signed_dist);
    float opacity = smoothstep(-d, d, signed_dist);
    return opacity;
}

void main() {
    vec2 textSize = textureSize(u_Atlas, 0);

    vec2 uv = vec2(mix(u_TexturePos.x / textSize.x, u_TextureScale.x / textSize.x, v_UV.x),
                   mix(u_TextureScale.y / textSize.y, u_TexturePos.y / textSize.y, 1.0 - v_UV.y));

    const float subPixelAmount = 1.333;
    vec2 unitRange = 1.0 / textSize;
    vec2 uvLeft = uv - vec2(unitRange.x * subPixelAmount, 0);
    vec2 uvRight = uv + vec2(unitRange.x * subPixelAmount, 0);
    vec2 uvUp = uv + vec2(0, unitRange.y * subPixelAmount);
    float middle = opacity(texture(u_Atlas, uv));
    float left = opacity(texture(u_Atlas, uvLeft));
    float right = opacity(texture(u_Atlas, uvRight));
    float up = opacity(texture(u_Atlas, uvUp));

    vec3 color = u_FontColor.rgb;
    bool direction = false;
   
    const float ratio = 1.6666;
    const float curve_tolerance = 1.0;
    const float h_tolerance = 0;
    
    const bool subpixel = false;
    const float subpixel_threshold = 1;
    float curveAmount = up - middle;
    bool mid = middle < (subpixel_threshold);
    bool leftt = left < subpixel_threshold;
    bool rightt = right < subpixel_threshold;
    if(subpixel &&(middle < subpixel_threshold) && curveAmount < curve_tolerance)
    {
        color.r = left;
        color.g = middle;
        color.b = right;
    }
    
    FragColor = vec4(color.rgb, middle);
}