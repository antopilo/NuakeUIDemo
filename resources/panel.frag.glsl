#version 460 core

in vec2 v_UV;
out vec4 FragColor;

uniform vec2 u_Size;
uniform vec4 u_Color;
uniform vec4 u_BorderColor;
uniform float u_Border;
uniform float u_BorderRadius;

float ShouldDiscard(vec2 coords, float border, vec2 dimensions, float radius)
{
    vec2 circle_center = vec2(radius + border, radius + border);
    float dst = length(coords - circle_center);
    float delta = fwidth(dst);
    if (coords.x < circle_center.x && coords.y < circle_center.y) 
        return 1.0 - smoothstep(radius - delta, radius, dst); //first circle

    circle_center.x += dimensions.x - 2 * (radius + border) ;
    dst = length(coords - circle_center);
    delta = fwidth(dst);
    if (coords.x > circle_center.x && coords.y < circle_center.y) 
        return 1.0 - smoothstep(radius - delta, radius, dst); //first circle

    circle_center.y += dimensions.y - 2 * (radius + border);
    dst = length(coords - circle_center);
    delta = fwidth(dst);
    if (coords.x > circle_center.x && coords.y > circle_center.y) 
        return 1.0 - smoothstep(radius - delta, radius, dst); //first circle

    circle_center.x -= dimensions.x - 2 * (radius + border);
    dst = length(coords - circle_center);
    delta = fwidth(dst);
    if (coords.x < circle_center.x && coords.y > circle_center.y) 
        return 1.0 - smoothstep(radius - delta, radius, dst); //first circle

    return 1.f;

}

void main() { 
	vec2 coords = v_UV * u_Size;
	vec2 center = u_Size / 2.0;
	vec4 finalColor = u_Color;

    // Border rounding
    float a = finalColor.a;
    if(u_BorderRadius > 0.f)
    {
        a = min(ShouldDiscard(coords, 0, u_Size, u_BorderRadius + 2), finalColor.a);
        if(a == 0.f) 
            discard;
    }
    
    if(u_Border > 0.f)
    {
        float borderAlpha = 0.f;
        if(u_BorderRadius > 0.f)
            borderAlpha = ShouldDiscard(coords, u_Border, u_Size, u_BorderRadius);

        if(a == 1.f)
        { 
            finalColor = mix(u_BorderColor, u_Color, borderAlpha);
            if(coords.x < u_Border || coords.x > u_Size.x - u_Border ||
                coords.y < u_Border || coords.y > u_Size.y - u_Border)
            {
                 finalColor = u_BorderColor;
            }
            FragColor = finalColor; 
            return;
        }
        else
        {
            finalColor = u_BorderColor;
            finalColor.a = min(a, u_BorderColor.a);
            FragColor = finalColor;
            return;
        }
    }
    
    finalColor.a = a;
	FragColor = finalColor; 
}