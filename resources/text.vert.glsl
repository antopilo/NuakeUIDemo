#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 UV;

uniform mat4 u_Model;
uniform mat4 u_View;

out vec2 v_UV;

void main() {
	v_UV = UV;
	int x = int(Position.x);
	int y = int(Position.y);
	int z = int(Position.z);

	gl_Position = u_View * u_Model * vec4(x, y, z, 1.0f);
}