#include "Texture.h"
#include "stb_image/stb_image.h"


//Constructor
Texture::Texture(const std::string& file_path)
	: rendererID(0), filePath(file_path), localBuffer(nullptr),
	width(0), height(0), bpp(0)
{
	//Loading image
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(file_path.c_str(), &width, &height, &bpp, 4);

	//Binding texture
	glErrorCall( glGenTextures(1, &rendererID) );
	glErrorCall( glBindTexture(GL_TEXTURE_2D, rendererID) );

	//Setting texture parameters
	glErrorCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );	//Resampling texture down if it needs to be rendered small
	glErrorCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );	//Resampling texture up if it needs to be rendered large
	glErrorCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );	//Clamping horizontally
	glErrorCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );	//Clamping vertically

	//Giving opengl the data of the loaded image
	glErrorCall( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer) );
	glErrorCall( glBindTexture(GL_TEXTURE_2D, 0) );	//Unbinding once the data is given

	//Freeing the local buffer
	if (localBuffer)
		stbi_image_free(localBuffer);
}

//Destructor
Texture::~Texture()
{
	glErrorCall(glDeleteTextures(1, &rendererID));
}


void Texture::Bind(unsigned int slot) const
{
	//Binding texture to the proper slot
	glErrorCall(glActiveTexture(GL_TEXTURE0 + slot));
	glErrorCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}


void Texture::Unbind() const
{
	glErrorCall(glBindTexture(GL_TEXTURE_2D, 0));
}
