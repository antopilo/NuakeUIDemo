#version 460 core

in vec2 v_UV;
out vec4 FragColor;

uniform vec2 u_Size;
uniform vec4 u_Color;
uniform vec4 u_BorderColor;
uniform float u_Border;

bool ShouldDiscard(vec2 coords, vec2 dimensions, float radius)
{
    vec2 circle_center = vec2(radius, radius);

    if (length(coords - circle_center) >= radius
        && coords.x < circle_center.x && coords.y < circle_center.y) return true; //first circle

    circle_center.x += dimensions.x - 2 * radius;

    if (length(coords - circle_center) >= radius
        && coords.x > circle_center.x && coords.y < circle_center.y) return true; //second circle

    circle_center.y += dimensions.y - 2 * radius;

    if (length(coords - circle_center) >= radius
        && coords.x > circle_center.x && coords.y > circle_center.y) return true; //third circle

    circle_center.x -= dimensions.x - 2 * radius;

    if (length(coords - circle_center) >= radius
        && coords.x < circle_center.x && coords.y > circle_center.y) return true; //fourth circle

    return false;

}

void main() { 
	vec2 coords = v_UV * u_Size;
	vec2 center = u_Size / 2.0;
	vec4 finalColor = u_Color;

    // Border rounding
    if (ShouldDiscard(coords, u_Size, 8.0))
        discard;

	if(coords.x < u_Border || coords.x > u_Size.x - u_Border ||
        coords.y < u_Border || coords.y > u_Size.y - u_Border)
		finalColor = u_BorderColor;
	
	FragColor = finalColor; 
	
}