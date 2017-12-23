#version 330 core

layout (location = 0) in vec2 position;

uniform vec3 myPlan;
uniform vec3 myOffset;
uniform vec3 mySize;
uniform vec3 myColor;

out vec3 colorIn;

void main(){
     colorIn = myColor;
     gl_Position = vec4((position.x * mySize.x + myOffset.x - .5) * 2, (position.y * mySize.y + myOffset.y - .5) * 2, myPlan.x, 1.f);
}