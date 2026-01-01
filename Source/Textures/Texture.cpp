#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../External/stb_image.h" // public domain image loader

// takes in filepath and returns an ID for the texture that was created from that texture file
unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = directory + "/" + std::string(path);
	int textureWidth;
	int textureHeight;
	int textureChannels;
	unsigned char* textureData;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	// channels:
	// 1	grayscale	GL_RED
	// 3	RGB			GL_RGB
	// 4	RGBA		GL_RGBA

	// load the texture data
	textureData = stbi_load(filename.c_str(), &textureWidth, &textureHeight, &textureChannels, 0);

	// could use a function here
	if (textureData)
	{
		switch (textureChannels)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		}
	}
	else
	{
		std::cout << "Failed to load texture!" << std::endl;
	}
	stbi_image_free(textureData);

	return textureID;
}