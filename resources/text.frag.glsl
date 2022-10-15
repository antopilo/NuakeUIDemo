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

vec3 sdf_triplet_alpha( vec3 sdf, float horz_scale, float vert_scale, float vgrad ) {
    float doffset = 1.0 - textureSize(u_Atlas, 0).x;
    float hint_amount = 0.5f;
    float hdoffset = mix( doffset * horz_scale, doffset * vert_scale, vgrad );
    float rdoffset = mix( doffset, hdoffset, hint_amount );
    vec3 alpha = smoothstep( vec3( 0.5 - rdoffset ), vec3( 0.5 + rdoffset ), sdf );
    alpha = pow( alpha, vec3( 1.0 + 0.2 * vgrad * hint_amount ) );
    return alpha;
}

void main() {
    vec2 textSize = textureSize(u_Atlas, 0);

    vec2 uv = vec2(mix(int(u_TexturePos.x) / textSize.x, u_TextureScale.x / textSize.x, v_UV.x),
                   mix(int(u_TextureScale.y) / textSize.y, u_TexturePos.y / textSize.y, 1.0 - v_UV.y));

    vec2 unitRange = 1.0 / textSize;

    vec4 msd = texture(u_Atlas, uv + vec2( unitRange.x, 0.0 )).rgba;
    float sdf_north = texture( u_Atlas, uv + vec2( 0.0, unitRange.y ) ).r;
    float sdf_east  = texture( u_Atlas, uv + vec2( unitRange.x, 0.0 ) ).r;

    float sd = median(msd.r, msd.g, msd.b);

    float r = msd.r;
    float g = msd.g;
    float b = msd.b;
    float median = max(min(r, g), min(max(r, g), b));
    float signed_dist = median - 0.5;
    float d = fwidth(signed_dist);
    float opacity = smoothstep(-d, d, signed_dist);

    float horz_scale  = 0.33333; // Should be 0.33333, a subpixel size, but that is too colorful
    float vert_scale  = 0.6;

    vec2  sgrad     = vec2( sdf_east - msd.a, sdf_north - msd.a );
    float sgrad_len = max( length( sgrad ), 1.0 / 128.0 );
    vec2  grad      = sgrad / vec2( sgrad_len );
    float vgrad = abs( grad.y ); // 0.0 - vertical stroke, 1.0 - horizontal one

    // Subpixel SDF samples
    vec2  subpixel = vec2( 0.3333 * unitRange.x, 0 );

    // For displays with vertical subpixel placement:
    // vec2 subpixel = vec2( 0.0, subpixel_offset );

    float sdf_sp_n  = texture( u_Atlas, uv - subpixel ).r;
    float sdf_sp_p  = texture( u_Atlas, uv + subpixel ).r;
    vec3 triplet_alpha = sdf_triplet_alpha( vec3( sdf_sp_n, msd.a, sdf_sp_p ), horz_scale, vert_scale, vgrad );

    // For BGR subpixels:
    // triplet_alpha = triplet.bgr
    //FragColor = mix(vec4(0), u_FontColor, opacity);
    FragColor = vec4( u_FontColor.rgb , opacity);

}