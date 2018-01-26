#version 330 core

in vec2 vTextureCoord;
uniform sampler2D uSampler4;
uniform vec3 iMapShadow;

//layout(location = 0) out float fragmentdepth;

void main() {
        vec4 z = texture(uSampler4, vec2(vTextureCoord.s, vTextureCoord.t));
        z = z*0.5+0.5;
        gl_FragDepth = iMapShadow.x*z.r + iMapShadow.y*z.g + iMapShadow.z*z.b;
}
