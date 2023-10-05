#pragma once
#ifndef  TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <glad/glad.h>
#include <string>
#include <iostream>

//A class to make it easier to import textures
class Texture
{
public:
    //Constructor
	Texture(std::string imagePath = "")
		:imagePath(imagePath)
	{

        //Generate Texture and bind it for configuration
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        //Set wrapping and mipmap configurations
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //Load texture data
        stbi_set_flip_vertically_on_load(true);
        imageData = stbi_load(imagePath.c_str(), &width, &height, &numberOfChannels, 0);

        //Use default texture if given texture is not found.
        if (imageData == NULL)
        {
            std::cout << "Failed to load image texture \"" + imagePath + "\"! Resorting to default texture" << std::endl;
            imageData = stbi_load("Textures/TextureNotFound.png", &width, &height, &numberOfChannels, 0);
            
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        //Free data
        stbi_image_free(imageData);
	}

    ~Texture() { std::cout << "Deleting Texture" << std::endl; }

    std::string filePath() { return imagePath; }
    unsigned int TextureID() { return textureID; }
    int Height() { return height; }
    int Width() { return width; }

protected:
	std::string imagePath;
    unsigned int textureID;
	unsigned char* imageData;
	int width, height, numberOfChannels;
};

#endif // ! TEXTURE_H