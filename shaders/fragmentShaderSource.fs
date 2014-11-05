varying vec2 vTextureCoord;
varying float aaa;
varying vec4 sLight;
varying vec4 color;
varying vec3 sky;

uniform float textureEnabled;
uniform vec4 shapeColor;
uniform float isAlpha;
uniform float alphaTest;
uniform vec4 skyColor;
uniform sampler2D uSampler;



void main() {
        //gl_FragColor  =vec4(1.0,0.0,0.0,1.0);
        if(textureEnabled == 0) {
            gl_FragColor = shapeColor;
        } else {
            gl_FragColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
        
            gl_FragColor.a = max(gl_FragColor.a, isAlpha);  

            if(gl_FragColor.a < alphaTest)
                discard;    

            gl_FragColor *= color;

            gl_FragColor = gl_FragColor*sLight;
            vec4 FragColor2 = gl_FragColor + aaa*skyColor;

            //float a = 0.0;
            if(FragColor2.r > skyColor.x ) 
                FragColor2.r = max(skyColor.x, gl_FragColor.r);

            if(FragColor2.g > skyColor.y ) 
                FragColor2.g = max(skyColor.y, gl_FragColor.g);

            if(FragColor2.b > skyColor.z ) 
                FragColor2.b = max(skyColor.z, gl_FragColor.b);

            gl_FragColor = FragColor2;
        }
}
