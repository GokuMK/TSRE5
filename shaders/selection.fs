precision mediump float;

varying vec2 vTextureCoord;
varying float aaa;
varying float slight;
varying vec4 color;

uniform sampler2D uSampler;
            
void main(void) {
    gl_FragColor = color;
    gl_FragColor.a = 1.0;
}