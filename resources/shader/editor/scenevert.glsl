#version 330 core
layout(location = 0) in vec2 aPos;
out vec2 pos;
uniform mat4 projection;

void main()
{
    vec2 few = aPos;
    if(few.x == -1.0) few.x = 0.0;
    if(few.y == -1.0) few.y = 0.0;
    gl_Position = projection * vec4(few,0.0,1.0);
    pos = few;
}