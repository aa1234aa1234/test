#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texCoord;

out vec3 frag_pos;
out vec3 vnormal;
out vec2 tex_coord;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    frag_pos = vec3(model * vec4(position, 1.0));
    vnormal = mat3(transpose(inverse(model))) * normal;
    tex_coord = texCoord;
    gl_Position = projection * view * vec4(frag_pos,1.0);
}

#shader fragment
#version 330 core

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess; //specular factor

    bool useDiffuse;
    bool useSpecular;
    bool useNormal;

    sampler2D diffuseMap;
    sampler2D normalMap;
    sampler2D specularMap;
};

struct PBRMaterial {
    vec3 albedo;
    float roughness;
    float metallic;
    float ao;

    bool useAlbedo;
    bool useRoughness;
    bool useNormal;
    bool useMetallic;
    bool useAO;
};

struct BaseLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 frag_pos;
in vec3 vnormal;
in vec2 tex_coord;

uniform Material material;
uniform PBRMaterial pbr_material;
//uniform BaseLight light; apparently this isnt really used at all
uniform vec3 ambientLight;
uniform bool use_pbr;


void main() {

    if(use_pbr) {
        return;
    }

    if(material.useDiffuse) {
        FragColor = texture(material.diffuseMap, tex_coord) * vec4(ambientLight, 1.0);
    }
    else {
        FragColor = vec4(ambientLight, 1.0) * material.diffuse;
    }
}