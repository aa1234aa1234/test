#version 330 core
out vec4 FragColor;
in vec2 pos;
in vec2 size;
flat in int isSelected;
uniform float width;
uniform float height;
vec3 NormalizeRGB(vec3 color) {
    return color.xyz/255.0;
}


void main()
{
    FragColor = vec4(NormalizeRGB((isSelected == 1 ? vec3(46,67,110) : vec3(40,40,40))), 1.0);
    //if(isSelected == 0) FragColor = vec4(NormalizeRGB(vec3(40,40,40)),1.0);
    //else FragColor = vec4(NormalizeRGB(vec3(46,67,110)),1.0);
}