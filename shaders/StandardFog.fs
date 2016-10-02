varying vec2 vTextureCoord;
varying float fogFactor;
varying vec3 vNormal;

uniform float textureEnabled;
uniform vec4 shapeColor;
uniform float isAlpha;
uniform float alphaTest;
uniform vec4 skyColor;
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec4 specularColor;
uniform vec3 lightDirection;
uniform sampler2D uSampler;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;
uniform float enableNormals;

void main() {
        //gl_FragColor  =vec4(1.0,0.0,0.0,1.0);
        if(textureEnabled == 0) {
            gl_FragColor = shapeColor;
        } else {
            gl_FragColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
            gl_FragColor.a = max(gl_FragColor.a, isAlpha);  

            if(gl_FragColor.a < alphaTest)
                discard;    

            if(enableNormals > 0.0){
                vec3 normal = normalize(mat3(uMVMatrix) * mat3(uMSMatrix) * vNormal);
                vec3 lights = normalize(lightDirection);
                float cosTheta = dot( normal, lights );
                cosTheta = clamp(cosTheta, 0, 1);
                vec4 color = diffuseColor;
                color *= cosTheta;
                color += ambientColor;
                color.a = 1.0;
                gl_FragColor *= color;
            }

            gl_FragColor = mix(gl_FragColor, skyColor, fogFactor);
        }
}
