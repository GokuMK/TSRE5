precision mediump float;

varying vec2 vTextureCoord;
varying float aaa;
varying vec4 sLight;
varying vec4 color;
varying vec3 sky;

uniform vec4 skyColor;
uniform sampler2D uSampler;
            
void main(void) {

    gl_FragColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
    if(gl_FragColor.a < 0.3)
       discard;    
    gl_FragColor *= color;

    gl_FragColor = gl_FragColor*sLight;
    gl_FragColor = mix(gl_FragColor, skyColor, aaa);
}