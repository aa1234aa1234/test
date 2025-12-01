#version 330 core
out vec4 FragColor;
in vec3 colors;
in vec2 pos;
in vec2 size;
flat in int typeId;
in vec4 clipRect;
flat in int fstate;
uniform sampler2D tex;
uniform float width;
uniform float height;
uniform float borderWidth=5.0;
vec3 NormalizeRGB(vec3 color) {
    return color.xyz/255.0;
}

void idk(vec3 color) {
    float borderx = borderWidth * (1.0/width), bordery = borderWidth * (1.0/height);
    switch(typeId) {
    case 0:
        FragColor = vec4(color,1.0);
        break;
    case 1:
        if (pos.x < borderx || pos.x > (1.0 - borderx) || pos.y < bordery || pos.y > (1.0 - bordery)) {
            FragColor = vec4(1.0,1.0,1.0,0.0);
        }
        else FragColor = vec4(color,1.0);
        break;
    case 2:
        FragColor = texture(tex,vec2(pos.x,1.0-pos.y));
        break;
    default:
        FragColor = vec4(color,1.0);
        break;
    }
    if(fstate == 1) {
        FragColor.a = 0.5;
        if (pos.x < borderx || pos.x > (1.0 - borderx) || pos.y < bordery || pos.y > (1.0 - bordery)) {
            FragColor = vec4(NormalizeRGB(vec3(120,90,125)),1.0);
        }
    }
    else FragColor.a = 1.0;
    if(clipRect.x != -1 && clipRect.y != -1 && clipRect.z != -1 && clipRect.w != -1);
}

void main()
{
    vec2 invert = vec2(pos.x,1.0-pos.y);
    vec3 color = NormalizeRGB(colors);
    
    //if (pos.x < border || pos.x > (1.0 - border) || pos.y < border || pos.y > (1.0 - border)) {
        //FragColor = vec4(1.0,1.0,1.0,0.0);
    //}
    //else {
        //if(a == 1) FragColor = texture(tex,invert);
        //else FragColor = vec4(color,1.0);
    //}
    
    //FragColor = vec4(color,1.0);
    idk(color);
    //FragColor = vec4(0.0,1.0,0.0,1.0);
}