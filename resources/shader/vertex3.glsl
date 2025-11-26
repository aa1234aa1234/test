#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 atexcoord;
out vec2 TexCoord;
out vec3 colors;
uniform float width;
uniform float height;
uniform mat4 projection;
void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    TexCoord = atexcoord;
    colors = vec3(1.0,1.0,1.0);
}