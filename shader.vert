#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform float windowWidth;
uniform float windowHeight;
uniform float baseRuler;
void main(){  gl_Position.xyz = vec3(vertexPosition_modelspace.x*2*baseRuler/windowWidth,vertexPosition_modelspace.y*2*baseRuler/windowHeight,vertexPosition_modelspace.z);
  gl_Position.w = 1.0;
}
