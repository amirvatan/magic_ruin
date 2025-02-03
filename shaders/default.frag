#version 330 core
out vec4 frag_color;

in vec2 textureCoordinate;
uniform sampler2D ntexture;

void main() {
	frag_color = texture(ntexture,textureCoordinate);
}
