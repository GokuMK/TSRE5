attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 aTextureCoord;

uniform float lod;
uniform float sun;
uniform float brightness;
uniform mat4 uPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;

varying vec2 vTextureCoord;
varying float aaa;
varying vec4 color;
varying vec4 sLight;
varying vec3 vnormal;

void main() {
    //vTextureCoord = aTextureCoord;
    //gl_Position = uPMatrix * uMVMatrix * uMSMatrix * vertex;
    gl_Position = uPMatrix * uMVMatrix * uMSMatrix * vertex;
    vTextureCoord = aTextureCoord;

    aaa = sqrt((gl_Position.x)*(gl_Position.x) + (gl_Position.z)*(gl_Position.z))/(lod*1.5);
    if(aaa<0.0) aaa = 0.0;
    if(aaa>1.0) aaa = 1.0;
    float slight = sun;
    slight = slight*(1.0 - brightness) + brightness;
    sLight = vec4(slight,slight,slight,1.0);

    color = vec4(1.0,1.0,1.0,1.0);
    vnormal = normal;
    /*vec3 lights = vec3(0.707,0.707,0.0);
    normal = mat3(uMVMatrix) * mat3(uMSMatrix) * normal;
    normal = normalize(normal);
    lights = normalize(lights);
    float cosTheta = dot( normal, lights );
    cosTheta = clamp(cosTheta, 0, 1);
    color *= cosTheta;
    color.a = 1.0;*/
}
