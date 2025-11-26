#version 330 core
out vec4 FragColor;
in vec2 pos;

uniform sampler2D tex;

vec3 NormalizeRGB(vec3 color) {
    return color.xyz/255.0;
}


void main()
{
    vec2 invert = vec2(pos.x,1.0-pos.y);
    FragColor = texture(tex,invert);
}