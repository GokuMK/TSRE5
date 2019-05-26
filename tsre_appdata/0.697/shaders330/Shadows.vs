#version 330 core

in vec4 vertex;
in vec3 normal;
in vec2 aTextureCoord;

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
