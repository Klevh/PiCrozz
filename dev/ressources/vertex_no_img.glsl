#version 330 core

layout (location = 0) in vec2 position;

uniform vec3 myPlan;
uniform vec3 myOffset;
uniform vec3 myRatio;
uniform vec3 myColor;
uniform vec3 myRotation;

out vec3 colorIn;

vec2 pos;

void main(){
     colorIn = myColor;
     
     pos = (position * myRatio.xy - vec2(.5, .5)) * 2;

     pos = pos
           * mat2(cos(myRotation.x) ,-sin(myRotation.x),
     	      	  sin(myRotation.x) , cos(myRotation.x));

     gl_Position = vec4(pos + myOffset.xy * 2 , myPlan.x, 1.f);
}