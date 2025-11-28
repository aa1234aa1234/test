//
// Created by sw_306 on 2025-10-13.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "ErrorHandler.h"

Texture::Texture() : renderId(0), target(GL_NONE) {}

Texture::Texture(std::string filePath) : target(GL_TEXTURE_2D) {
    GLCall(glGenTextures(1, &renderId));
    stbi_set_flip_vertically_on_load(true);
    int channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (data) {
        bind();
        GLenum format;
        if (channels == 4) format = GL_RGBA;
        else if (channels == 3) format = GL_RGB;
        GLCall(glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(target));
        setFiltering(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);
        setWrapping(GL_REPEAT);
        stbi_image_free(data);
    }
    else {
        stbi_image_free(data);
    }

    unbind();

}

Texture::Texture(const std::vector<std::string> &faces) : renderId(0), width(0), height(0), target(GL_TEXTURE_CUBE_MAP) {
    stbi_set_flip_vertically_on_load(0);
    GLCall(glGenTextures(1, &renderId));
    bind();
    int channels;
    for (int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (data) {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        }
        else {
            std::cout << "error when loading cubemap";
            stbi_image_free(data);
        }
    }
    setFiltering(GL_LINEAR,GL_LINEAR);
    setWrapping(GL_CLAMP_TO_EDGE);
    unbind();
    stbi_set_flip_vertically_on_load(1);
}

Texture::~Texture() {}

void Texture::initCubeMap(int width, int height) {
    if (!renderId) glGenTextures(1, &renderId);
    bind();
    this->width = width;
    this->height = height;
    for (int i = 0; i<6; i++) {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    }
    setFiltering(GL_LINEAR,GL_LINEAR);
    setWrapping(GL_CLAMP_TO_EDGE);
    unbind();
}

void Texture::bind(const int& slot) {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(target, renderId));
}

void Texture::unbind() {
    GLCall(glBindTexture(target, 0));
}

void Texture::setWrapping(GLenum mode) {
    GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, mode));
    GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, mode));
    if (target == GL_TEXTURE_CUBE_MAP) GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_R, mode));
}

void Texture::setFiltering(GLenum minfilter, GLenum magfilter) {
    GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minfilter));
    GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magfilter));
}

void Texture::setType(const int& type) {
    this->type = type;
}
