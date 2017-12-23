#version 330 core

layout (location = 0) in vec2 position;

uniform vec3 myPlan;
uniform vec3 myOffset;
uniform vec3 mySize;
uniform vec3 myColor;
uniform vec3 myRotation;

out vec3 colorIn;

vec2 pos;
vec2 center;

void main(){
     colorIn = myColor;
     
     pos = position;
     
     center = vec2(pos.x - myOffset.x - mySize.x / 2,
     	           pos.y - myOffset.y - mySize.x / 2);

     pos = pos - center;
     pos = pos
           * mat2(cos(myRotation.x) ,-sin(myRotation.x),
     	      	  sin(myRotation.x) , cos(myRotation.x));
     pos = pos + center;

     gl_Position = vec4((pos.x * mySize.x +myOffset.x - .5) * 2, (pos.y * mySize.y + myOffset.y - .5) * 2, myPlan.x, 1.f);
}