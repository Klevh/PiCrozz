#version 330 core

layout (location = 0) in vec2 position;

uniform vec3 plan;
uniform vec3 offset;
uniform vec3 size;

void main(){
     gl_Position = vec4(position.x * size.x + offset.x, position.y * size.y + offset.y, plan.x, 1.f);
}