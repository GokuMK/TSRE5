#version 130

varying vec2 vTextureCoord;
varying float fogFactor;
varying vec3 vNormal;
varying vec4 shadowPos;

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
uniform sampler2DShadow shadow1;
uniform float secondTexEnabled;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;
uniform float enableNormals;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main() {
        //gl_FragColor  =vec4(1.0,0.0,0.0,1.0);
        if(textureEnabled == 0) {
            gl_FragColor = shapeColor;
        } else {
            gl_FragColor = texture(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
            gl_FragColor.a = max(gl_FragColor.a, isAlpha);  

            if(gl_FragColor.a < alphaTest)
                discard;    
                
            float bias = 0.005;
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
                bias = 0.01*tan(acos(cosTheta));
                bias = clamp(bias, 0, 0.01);
            }
            vec4 shadowPos2 = shadowPos*0.5+0.5;
            float visibility2 = shadowPos2.z;
            //float visibility1 = texture( shadow1, vec2(shadowPos2.x, shadowPos2.y) ).z;
            //float visibility1 = texture( shadow1, vec3(shadowPos2.x, shadowPos2.y, shadowPos2.z - 0.01) );
            //gl_FragColor.g = 0.0;
            //if(visibility2 > 0.5 && visibility2 < 0.7 )
            //    gl_FragColor.g = 1.0;
            //gl_FragColor.r = 0.0;
            //if(visibility1 > 0.0)
            //    gl_FragColor.r = 1.0;
            //if(visibility1 > 0.5 && visibility1 < 0.7 )
            //    gl_FragColor.r = 1.0;
            //gl_FragColor.b = 0.0;
            float visibility = 1.0;
            //float bias = 0.01;
            if(shadowPos2.x > 0.0 && shadowPos2.x < 1.0 && shadowPos2.y > 0.0 && shadowPos2.y < 1.0)
            for (int i=0;i<4;i++){
		int index = i;
		visibility -= 0.17*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[index]/3000.0, (shadowPos2.z-bias)) ));
            }
            //if (visibility1 < visibility2 - 0.01)
            //if (visibility1 > 0.5)
                //gl_FragColor.b = 1.0;
            //    visibility = visibility1;
            gl_FragColor.xyz *= visibility;
                
            vec4 FragColor2 = gl_FragColor + fogFactor*skyColor;

            if(FragColor2.r > skyColor.x ) 
                FragColor2.r = max(skyColor.x, gl_FragColor.r);

            if(FragColor2.g > skyColor.y ) 
                FragColor2.g = max(skyColor.y, gl_FragColor.g);

            if(FragColor2.b > skyColor.z ) 
                FragColor2.b = max(skyColor.z, gl_FragColor.b);

            gl_FragColor = FragColor2;
        }
}
