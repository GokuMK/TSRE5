#version 330 core

in vec2 vTextureCoord;
in float fogFactor;
in vec3 vNormal;

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
uniform sampler2D uSampler2;
uniform float secondTexEnabled;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;
uniform float enableNormals;

//layout(location = 0) out float fragmentdepth;

void main() {
      //color = vec4(1.0, 1.0, 0.5, 1.0);
        if(textureEnabled != 0){
            vec4 color = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
            color.a = max(color.a, isAlpha);  
            if(color.a < alphaTest)
                discard;    
        }
}
