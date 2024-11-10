#include "../Headers/oglopp.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace oglopp {
	/* @brief Texture default constructor
	*/
	Texture::Texture() {

	}

	/* @brief Texture constructor. Load texture
	*/
	Texture::Texture(const char* path) {
		this->load(path);
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
	Texture& Texture::load(const char* path) {
		glGenTextures(1, &this->TID);
		glBindTexture(GL_TEXTURE_2D, this->TID);

		// Set texture filtering and wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load and generate texture
		uint8_t* data = stbi_load(path, &this->width, &this->height, &this->channels, 0);
		if (data == nullptr) {
			std::cout << "Failed to load texture" << std::endl;
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

	/* @brief Get texture ID
	*  @return	The opengl texture ID of this texture object
	*/
	unsigned int Texture::getTexture() {
		return this->TID;
	}
}