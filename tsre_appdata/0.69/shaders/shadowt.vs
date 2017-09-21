#version 330 core

attribute vec4 vertex;

out vec2 vTextureCoord;

void main() {
    gl_Position = vertex;
    vTextureCoord = 0.5*vertex.xy + 0.5;
}
