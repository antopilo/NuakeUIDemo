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

    vec2 uv = vec2(mix(int(u_TexturePos.x) / textSize.x, u_TextureScale.x / int(textSize.x), v_UV.x),
                   mix(u_TextureScale.y / int(textSize.y), int(u_TexturePos.y) / textSize.y, 1.0 - v_UV.y));

    const float subPixelAmount = 0.5;
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
   
    const float ratio = 0.6666;
    const float curve_tolerance = 0.07;
    const float h_tolerance = 0.6666;
    
    const bool subpixel = true;
    if(subpixel)
    {
        const float h_diff = abs(middle - right);
        const float h_diff_r = abs(middle - left);

        float curveAmount = abs(up - middle);
        if(h_diff > h_tolerance && curveAmount < curve_tolerance)
        {
            color.b *= right;
            color.g *= mix(right, middle, ratio);
            color.r *= middle;
            direction = true;
        }

        if(h_diff_r > h_tolerance && curveAmount < curve_tolerance)
        {
            color.r *= left;
            color.g *= mix(left, middle, ratio);
            color.b *= middle;
            direction = true;
        }
    }
    
    FragColor = vec4( color.rgb, middle);
}