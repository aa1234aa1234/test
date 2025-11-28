//
// Created by sw_306 on 2025-09-01.
//

#ifndef PROJECT_ICONRENDERSYSTEM_H
#define PROJECT_ICONRENDERSYSTEM_H
#include "IconTextures.h"
#include "Shader.h"
#include "Components.hpp"
#include <stb/stb_image.h>

class IconRenderSystem : public System {
    int width, height, textureWidth, textureHeight;
    unsigned int textureAtlas;
    unsigned int vao,vbo,instancevbo;
    Shader* shader;

	struct Element {
		glm::vec4 textureCoords;
		glm::vec4 renderBox;
	};

	std::vector<Element> instanceData;
public:
    IconRenderSystem() {};
    ~IconRenderSystem() {
        delete shader;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &instancevbo);
        glDeleteTextures(1, &textureAtlas);
    };

    void Initialize(int& width, int& height) {
        this->width = width; this->height = height;
        shader = new Shader("resources/shader/editor/iconvert.glsl","resources/shader/editor/iconfrag.glsl");

        float ndc[] = {
            1.0, 1.0 ,
            1.0,-1.0,
            -1.0,1.0,
            -1.0,-1.0,
            1.0,-1.0,
            -1.0,1.0,
        };
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &instancevbo);
        glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
        glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(Element), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ndc), ndc, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Element), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Element), (void*)(offsetof(Element, renderBox)));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<RenderableIcon>(), true);

        SystemCoordinator::getInstance()->SetSystemSignature<IconRenderSystem>(signature);

    	ImportAtlas("resources/textures/minesweeperatlas.png");
    	instanceData.reserve(5000);
    	//shader->use();

    }

    void Update() {

    	shader->use();
    	glm::mat4 mat = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
    	glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(mat));
    	int elementCount = 0;
    	instanceData.clear();
    	auto* sc = SystemCoordinator::getInstance();
    	for (auto& p : entities) {
    		auto& icon = sc->GetComponent<RenderableIcon>(p);
    		if (!icon.visible) continue;
    		auto renderRect = icon.renderRect;
    		if (renderRect.y < 0 || renderRect.y > height) continue;

    		EntityID currentEntity = p;
    		while (sc->EntityHasComponent<ParentComponent>(currentEntity)) {
    			EntityID parent = sc->GetComponent<ParentComponent>(currentEntity).parent;
    			if (sc->EntityHasComponent<ScrollableComponent>(parent)) {
    				renderRect.y -= sc->GetComponent<ScrollableComponent>(parent).offset;
    			}

    			currentEntity = parent;
    		}
    		instanceData.emplace_back(Element{GetIcon(icon.uvRect,glm::vec2(textureWidth,textureHeight)), renderRect});
    		elementCount++;
    	}


    	glBindTexture(GL_TEXTURE_2D, textureAtlas);

    	glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
    	glBufferData(GL_ARRAY_BUFFER, elementCount * sizeof(Element), nullptr, GL_STREAM_DRAW);
    	glBufferSubData(GL_ARRAY_BUFFER, 0, elementCount * sizeof(Element), instanceData.data());

    	glBindVertexArray(vao);

    	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceData.size());
    	glBindTexture(GL_TEXTURE_2D, 0);
    }

    void ImportAtlas(const char* path) {
		glGenTextures(1, &textureAtlas);
		int components;
		unsigned char* data = stbi_load(path, &textureWidth, &textureHeight, &components, 0);
		if (data) {
			GLenum format;
			switch (components) {
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			}
			glBindTexture(GL_TEXTURE_2D, textureAtlas);
			glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			std::cout << "texture failed to load " << path << std::endl;
			stbi_image_free(data);
		}
	}
};

#endif //PROJECT_ICONRENDERSYSTEM_H
