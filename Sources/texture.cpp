#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

#include "oglopp/texture.h"
#include "oglopp/defines.h"

namespace oglopp {

	/* @brief Get color register that corresponds with some type. GL_RGB for jpg. GL_RGBA for png
	* @return The register defining how to read the color
	*/
	uint16_t Texture::getTypeRegister(FileType type) {
		switch (type) {
			case FileType::JPG:
				return GL_RGB;
			case FileType::PNG:
				return GL_RGBA;
		}

		// Should not get down here
		return GL_RGB;
	}

	/* @brief Texture default constructor
	*/
	Texture::Texture() {

	}

	/* @brief Texture constructor. Load texture
	*/
	Texture::Texture(const char* path, FileType type) {
		this->load(path, type);
	}

	/* @brief Texture destructor
	*/
	Texture::~Texture() {
		this->destroy();
	}

	/* @brief Load an image path into the texture
	*  @param[in]	path	The filepath to load
	*  @return				A reference to this texture object
	*/
	Texture& Texture::load(const char* path, FileType type) {
		glGenTextures(1, &this->TID);
		glBindTexture(GL_TEXTURE_2D, this->TID);

		// Set texture filtering and wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Vertical flip first
		stbi_set_flip_vertically_on_load(true);

		// Load and generate texture
		uint8_t* data = stbi_load(path, &this->width, &this->height, &this->channels, 0);
		if (data == nullptr) {
			std::cout << "Failed to load texture" << std::endl;
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, Texture::getTypeRegister(type), GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}

		return *this;
	}

	/* @brief Destroy the image. Called on destructor.
	*  @return A reference to this texture object
	*/
	Texture& Texture::destroy() {
		return *this;
	}

	/* @brief Check if this texture object has loaded some image yet. Checks if the size is -1x-1x-1
	* @return True if this object has loaded an image, false if this is empty
	*/
	bool Texture::isDefined() {
		return (this->channels < 0 || this->height < 0 || this->width < 0);
	}

	/* @brief Get the size of this texture. Will be -1x-1x-1 if this texture has not been defined
	* @param[out] getWidth		A pointer to an integer where a width will be saved
	* @param[out] getHeight	A pointer to an integer where the height will be saved
	* @param[out] getChannels	Optionally get the channels. If nullptr, do not get the channels.
	*/
	Texture& Texture::getSize(int* getWidth, int* getHeight, int* getChannels) {
		*getWidth = this->width;
		*getHeight = this->height;
		*getChannels = this->channels;

		return *this;
	}

	/* @brief Get texture ID
	*  @return	The opengl texture ID of this texture object
	*/
	unsigned int Texture::getTexture() {
		return this->TID;
	}
}