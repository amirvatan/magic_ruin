#version 330 core
out vec4 o_color;

in vec2 v_uvs;

uniform sampler2D texture_slot;

void main() {
	//o_color = vec4(1, 0, 0, 1);
	o_color = texture(texture_slot, v_uvs);
}
