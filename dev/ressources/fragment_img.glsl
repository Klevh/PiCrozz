#version 330 core

uniform sampler2D sampler;
in vec2 p_image;

out vec4 color;

void main(){
     color = texture(sampler,p_image);
}
