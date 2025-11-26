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
    vec2 invert = vec2(TexCoord.x,1.0-TexCoord.y);
    float border = borderWidth * (1.0/width);
    if (TexCoord.x < border || TexCoord.x > (1.0 - border) || TexCoord.y < border || TexCoord.y > (1.0 - border)) {
        FragColor = vec4(1.0,1.0,1.0,0.0);
    }
    else {
        if(a == 1) FragColor = texture(tex,invert);
        else FragColor = vec4(colors,1.0);
    }
    if(FragColor.a == 0.0) discard;
}