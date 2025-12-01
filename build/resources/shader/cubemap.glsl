#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = position;
    gl_Position = projection * view * vec4(position,1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube tex;

void main() {
    FragColor = texture(tex,TexCoords);
}