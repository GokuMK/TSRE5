attribute vec3 aVertexPosition;
attribute vec4 lightValue;
attribute vec2 aTextureCoord;


uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;
uniform mat4 uPMatrix;

varying vec2 vTextureCoord;
varying float aaa;
varying vec4 color;
varying float slight;

void main(void) {
     gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);
     vTextureCoord = aTextureCoord;

     color = vec4(0.0,0.0,0.0,1.0);
     float yy = floor(lightValue.y/(256.0*256.0));
     float zx = floor((lightValue.y - yy*256.0*256.0)/(256.0));
     float cv = lightValue.y - yy*256.0*256.0 - zx*256.0;
     color.r = yy/255.0;
     color.g = zx/255.0;
     color.b = cv/255.0;
     slight = 1.0;
}
