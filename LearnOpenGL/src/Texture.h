#pragma once

#include "Renderer.h"


class Texture
{
private:
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, bpp;		//bpp = bits per pixel

public:
	//Constructor & Destructor
	Texture(const std::string& file_path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return width; };
	inline int GetHeight() const { return height; };
};