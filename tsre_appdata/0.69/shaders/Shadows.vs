#version 330 core

attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 aTextureCoord;

uniform float lod;
uniform mat4 uPMatrix;
uniform mat4 uShadowPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;

out vec2 vTextureCoord;

void main() {
    gl_Position = uShadowPMatrix * uMVMatrix * uMSMatrix * vertex;
    vTextureCoord = aTextureCoord;
}
