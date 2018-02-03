#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 img;

uniform vec3 myPlan;
uniform vec3 myOffset;
uniform vec3 myRatio;
uniform vec3 myRotation;

out vec2 p_image;

vec2 pos;
vec2 angle;

void main(){
     p_image = img;
     
     pos = position * myRatio.xy;

     angle = vec2(cos(myRotation.x), sin(myRotation.x));
     pos = pos
           * mat2(angle.x ,-angle.y,
     	      	  angle.y , angle.x);
     pos -= (vec2(1,1) - myRatio.xy);

     gl_Position = vec4(pos + (myOffset.xy * 2) , myPlan.x, 1.f);
}
