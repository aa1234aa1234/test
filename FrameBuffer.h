#pragma once

class FrameBuffer
{
	unsigned int fbo, texture, rbo;
public:
	FrameBuffer(float width, float height);
	FrameBuffer();
	~FrameBuffer();
	unsigned int getFrameTexture();
	unsigned int getFrameBuffer();
	void bind();
	void unbind();
	void init(int& width, int& height);
	void resize(int& width, int& height);
};
