#version 130

varying vec2 vTextureCoord;
varying float fogFactor;
varying vec3 vNormal;
varying vec4 shadowPos;
varying vec4 shadow2Pos;
varying float vAlpha;

uniform float textureEnabled;
uniform int shadowsEnabled;
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
uniform sampler2DShadow shadow2;
uniform float secondTexEnabled;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;
uniform float enableNormals;
uniform float colorBrightness;

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

float insideBox(vec2 v, vec2 bottomLeft, vec2 topRight) {
    vec2 s = step(bottomLeft, v) - step(topRight, v);
    return s.x * s.y;   
}

void main() {
        if(textureEnabled == 0) {
            gl_FragColor = shapeColor;
        } else {
            gl_FragColor = texture(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
            vec4 tex2 = texture(uSampler2, vec2(vTextureCoord.s*secondTexEnabled, vTextureCoord.t*secondTexEnabled));
            //if(secondTexEnabled > 0){
            //    gl_FragColor *= tex2*2.0;
            //}
            float isSecondTexEnabled = sign(secondTexEnabled);
            gl_FragColor = gl_FragColor*(1-isSecondTexEnabled) + gl_FragColor*tex2*2.0*isSecondTexEnabled;
            //gl_FragColor.a = max(gl_FragColor.a, isAlpha);  

            // discard if transparent
            //if(gl_FragColor.a < alphaTest)
            //    discard;    
            gl_FragColor.a = max(gl_FragColor.a, vAlpha);  
            // discard if transparent 
            if(gl_FragColor.a < -vAlpha)
                discard;    
            //gl_FragColor.a = 1.0;

            // calculate normals
            vec3 normal = normalize(mat3(uMVMatrix) * mat3(uMSMatrix) * vNormal);
            vec3 lights = normalize(lightDirection);
            float cosTheta = clamp(dot( normal, lights ), 0, 1);
            float visibility = (1.0-enableNormals) + cosTheta*enableNormals;
            float shadowIntensity = 0.1;
            float shadow1Res = 2000.0;
            float bias = 0.0025*tan(acos(cosTheta))*enableNormals + 0.0025*(1.0-enableNormals);
            //float shadow1Res = 5000.0;
            //float bias = 0.0005*tan(acos(cosTheta))*enableNormals + 0.0025*(1.0-enableNormals);
            bias = clamp(bias, 0, 0.01);

            // calculate shadows
            vec4 shadowPos2 = shadowPos*0.5+0.5;
            vec4 shadow2Pos2 = shadow2Pos*0.5+0.5;
            float camdist = length(shadowPos.xyz);
            camdist = clamp(camdist, 0, 1.0);
            float camdist2 = length(shadow2Pos.xyz);
            camdist2 = clamp(camdist2, 0, 1.0);
            float t = 1.0 - floor(camdist);
            float t2 = 1.0 - floor(camdist2);

            float shadowsEnabled2 = shadowsEnabled;
            float bias2 = 0.002;
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[0]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*(1.0-t)*t2*0.4*(1.0-texture( shadow2, vec3(shadow2Pos2.xy + poissonDisk[0]/4000.0, (shadow2Pos2.z-bias2)) ));

            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[1]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*(1.0-t)*t2*0.4*(1.0-texture( shadow2, vec3(shadow2Pos2.xy + poissonDisk[1]/4000.0, (shadow2Pos2.z-bias2)) ));
            
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[2]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[3]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[4]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[5]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[6]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[7]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[8]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[9]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[10]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[11]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[12]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[13]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[14]/shadow1Res, (shadowPos2.z-bias)) ));
            visibility -= shadowsEnabled2*t*shadowIntensity*(1.0-texture( shadow1, vec3(shadowPos2.xy + poissonDisk[15]/shadow1Res, (shadowPos2.z-bias)) ));

            // calculate light color
            vec3 color = diffuseColor.xyz;
            color *= clamp(visibility, 0.0, 1.0);
            color += ambientColor.xyz;
            gl_FragColor.xyz *= color*colorBrightness;
            
            // calculate bloom fog
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
