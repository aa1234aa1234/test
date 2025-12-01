#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUIPos;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 asize;
layout(location = 4) in vec4 aclipRect;
out vec4 colors;
out vec2 pos;
out vec2 size;
flat out int fstate;
flat out int typeId;
out vec4 clipRect;
uniform mat4 projection;
uniform int state=0;

void main()
{
    vec2 few = aPos;
    if(few.x == -1.0) few.x = 0.0;
    if(few.y == -1.0) few.y = 0.0;
    vec2 pos1 = aUIPos + (few*asize);
    if(fstate == 1) gl_Position = projection * vec4(pos1, 1.0, 1.0);
    else gl_Position = projection * vec4(pos1,0.0,1.0);
    pos = few;
    size = asize;
    colors = aColor;
    typeId = 0;
    clipRect = aclipRect;
    fstate = state;
}