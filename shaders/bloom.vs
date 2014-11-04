attribute vec3 aVertexPosition;
attribute vec4 lightValue;
attribute vec2 aTextureCoord;

uniform float lod;
uniform float sun;
uniform float brightness;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;
uniform mat4 uPMatrix;

varying vec2 vTextureCoord;
varying float aaa;
varying vec4 color;
varying vec4 sLight;

void main(void) {
     gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);
     vTextureCoord = aTextureCoord;

    aaa = sqrt((gl_Position.x)*(gl_Position.x) + (gl_Position.z)*(gl_Position.z))/(lod*13.5)-0.30;
    if(aaa<0.0) aaa = 0.0;
    if(aaa>1.0) aaa = 1.0;
    float skylight = floor(lightValue.x/100.0);
    float blocklight = lightValue.x - skylight*100.0;
    float slight = ((skylight*sun)/15.0 + blocklight/15.0);//*lightValue.z;
    if(slight > 1.0) slight = 1.0;
    slight = slight*(1.0 - brightness) + brightness;
    slight *= lightValue.z;
    sLight = vec4(slight,slight,slight,1.0);
    if(lightValue.a != 0.0) {
        float m5 = floor(lightValue.a/(256.0*256.0));
        float m6 = floor((lightValue.a - m5*256.0*256.0)/(256.0));
        float m7 = lightValue.a - m5*256.0*256.0 - m6*256.0;

        color = vec4(m5/255.0, m6/255.0, m7/255.0, 1.0);
    }
    else color = vec4(1.0,1.0,1.0,1.0);
}
