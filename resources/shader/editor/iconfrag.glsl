#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D tex;
void main()
{
    vec2 invert = vec2(TexCoords.x,1.0-TexCoords.y);
    FragColor = texture(tex,invert);
}