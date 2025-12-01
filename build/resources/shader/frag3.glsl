#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 colors;
uniform sampler2D tex;
uniform vec3 color;
uniform int a=0;
uniform float width;
uniform float borderWidth=1.0;
void main()
{
    //vec4 bg = vec4(1.0-gl_FrontColor.r,1.0-gl_FrontColor.g,1.0-gl_FrontColor.b,1.0);
    vec2 invert = vec2(TexCoord.x,1.0-TexCoord.y);
    //if(texture(tex,invert).r < 0.1) discard;
    float smoothing = 0.1;
    float alpha = smoothstep(0.5-smoothing, 0.5 + smoothing,texture(tex,invert).r);
    FragColor = vec4(colors,alpha);
    if(alpha == 0.01) discard;
}