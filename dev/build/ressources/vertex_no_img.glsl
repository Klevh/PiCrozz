#version 330 core

layout (location = 0) in vec2 position;

uniform vec3 myPlan;
uniform vec3 myOffset;
uniform vec3 myRatio;
uniform vec3 myColor;
uniform vec3 myRotation;

out vec3 colorIn;

vec2 pos;
vec2 angle;

void main(){
     colorIn = myColor;
     
     pos = position * myRatio.xy;

     angle = vec2(cos(myRotation.x), sin(myRotation.x));
     pos = pos
           * mat2(angle.x ,-angle.y,
     	      	  angle.y , angle.x);
     pos -= (vec2(1,1) - myRatio.xy);

     gl_Position = vec4(pos + (myOffset.xy * 2) , myPlan.x, 1.f);
}