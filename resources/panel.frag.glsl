#version 460 core

in vec2 v_UV;
out vec4 FragColor;

uniform vec2 u_Size;
uniform vec4 u_Color;
uniform vec4 u_BorderColor;
uniform float u_Border;
uniform float u_BorderRadius;

bool ShouldDiscard(vec2 coords, float border, vec2 dimensions, float radius)
{
    vec2 circle_center = vec2(radius + border, radius + border);

    if (length(coords - circle_center) >= radius
        && coords.x < circle_center.x && coords.y < circle_center.y) return true; //first circle

    circle_center.x += dimensions.x - 2 * (radius + border) ;

    if (length(coords - circle_center) >= radius
        && coords.x > circle_center.x && coords.y < circle_center.y) return true; //second circle

    circle_center.y += dimensions.y - 2 * (radius + border);

    if (length(coords - circle_center) >= radius
        && coords.x > circle_center.x && coords.y > circle_center.y) return true; //third circle

    circle_center.x -= dimensions.x - 2 * (radius + border);

    if (length(coords - circle_center) >= radius
        && coords.x < circle_center.x && coords.y > circle_center.y) return true; //fourth circle

    return false;

}

void main() { 
	vec2 coords = v_UV * u_Size;
	vec2 center = u_Size / 2.0;
	vec4 finalColor = u_Color;

    // Border rounding
    if (ShouldDiscard(coords, 0, u_Size, u_BorderRadius))
        discard;

	//if(coords.x < u_Border || coords.x > u_Size.x - u_Border ||
     //   coords.y < u_Border || coords.y > u_Size.y - u_Border)
	//	finalColor = u_BorderColor;
	if(ShouldDiscard(coords, u_Border, u_Size, u_BorderRadius) ||
        (coords.x < u_Border || coords.x > u_Size.x - u_Border ||
        coords.y < u_Border || coords.y > u_Size.y - u_Border))
        finalColor = u_BorderColor;

	FragColor = finalColor; 
	
}