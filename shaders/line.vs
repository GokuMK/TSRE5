attribute vec3 aVertexPosition;
attribute vec2 aTextureCoord;
attribute vec4 lightValue;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;

varying vec4 color;

void main(void) {
    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);
    color = vec4(1.0,1.0,1.0,1.0);
    color.r = aTextureCoord.x;
    color.g = lightValue.x;
    color = vec4(0.0,0.0,0.0,1.0);
    //slight += lightValue.z;
}
