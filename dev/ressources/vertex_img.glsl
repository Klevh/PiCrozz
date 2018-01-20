#version 330 core

layout (location = 0) in vec2 position;

uniform vec3 myPlan;
uniform vec3 myOffset;
uniform vec3 myRatio;
uniform vec3 myRotation;
uniform vec3 myImageCoord;

out vec2 p_image;

vec2 pos;
vec2 angle;

void main(){
     p_image = myImageCoord.xy;
     
     pos = position * myRatio.xy;

     angle = vec2(cos(myRotation.x), sin(myRotation.x));
     pos = pos
           * mat2(angle.x ,-angle.y,
     	      	  angle.y , angle.x);
     pos -= (vec2(1,1) - myRatio.xy);

     gl_Position = vec4(pos + (myOffset.xy * 2) , myPlan.x, 1.f);
}