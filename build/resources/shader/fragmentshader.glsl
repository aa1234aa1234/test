#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
//uniform sampler2D texture_diffuse[1];
uniform sampler2D texture_diffuse[8];
uniform sampler2D texture_specular[8];
uniform int diffusecnt;
uniform int specularcnt;
void main()
{
    //FragColor = texture(texture_diffuse[0],TexCoord);
    vec4 diffusecolor = vec4(0.0);
    vec4 specularcolor = vec4(0.0);
    for(int i = 0; i<diffusecnt; i++) {
        diffusecolor += texture(texture_diffuse[i],TexCoord)/float(diffusecnt);
    }
    for(int i = 0; i<specularcnt; i++) {
        specularcolor += texture(texture_specular[i],TexCoord)/float(specularcnt);
    }
    FragColor = diffusecolor+specularcolor;
}