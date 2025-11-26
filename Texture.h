//
// Created by sw_306 on 2025-10-13.
//

#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H
#include "header.h"


class Texture {

public:
    enum TextureType {
        ALBEDO,
        ROUGHNESS,
        NORMAL,
        SPECULAR,
        AO,
        METALLIC,
        DIFFUSE
    };

    Texture();
    Texture(std::string filePath);
    //for cube maps
    Texture(const std::vector<std::string> &faces);
    ~Texture();

    void bind(const int& slot = 0);
    void unbind();

    void initCubeMap(int width, int height);
    void setWrapping(GLenum mode);
    void setFiltering(GLenum minfilter, GLenum magfilter);
    void setType(const int& type);

private:
    GLuint target;
    std::string filePath;
    unsigned int renderId;
    int width, height;
    int type;
};


#endif //PROJECT_TEXTURE_H