#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 coordiante;

out vec2 textureCoordinate;
uniform mat4 model;
uniform mat4 projection;

void main() {
 	gl_Position = projection * model * vec4(a_pos, 1.0);
	textureCoordinate = coordiante;
}
