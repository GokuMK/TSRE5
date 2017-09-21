#version 130

attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 aTextureCoord;
attribute float alpha;

uniform float lod;
uniform mat4 uShadowPMatrix;
uniform mat4 uShadow2PMatrix;
uniform mat4 uPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;

varying vec2 vTextureCoord;
varying float fogFactor;
varying vec3 vNormal;
varying vec4 shadowPos;
varying vec4 shadow2Pos;
varying float vAlpha;

void main() {
    shadowPos = uShadowPMatrix * uMVMatrix * uMSMatrix * vertex;
    shadow2Pos = uShadow2PMatrix * uMVMatrix * uMSMatrix * vertex;
    gl_Position = uPMatrix * uMVMatrix * uMSMatrix * vertex;
    vTextureCoord = aTextureCoord;

    fogFactor = sqrt((gl_Position.x)*(gl_Position.x) + (gl_Position.z)*(gl_Position.z))/(lod*1.4);
    if(fogFactor<0.0) fogFactor = 0.0;
    if(fogFactor>1.0) fogFactor = 1.0;

    vNormal = normal;
    vAlpha = alpha;

}
