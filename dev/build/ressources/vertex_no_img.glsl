#version 330 core

layout (location = 0) in vec2 position;

uniform vec3 myPlan;
uniform vec3 myOffset;
uniform vec3 mySize;

void main(){
     gl_Position = vec4((position.x * mySize.x + myOffset.x) * 2 - 1, (position.y * mySize.y + myOffset.y) * 2 - 1, myPlan.x, 1.f);
}