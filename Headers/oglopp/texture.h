#ifndef OGLOPP_TEXTURE_H
#define OGLOPP_TEXTURE_H

#include "oglopp/glad/gl.h"
#include <stdint.h>
#include <vector>

namespace oglopp {
	/** @brief Texture
	*/
	class Texture {
	public:
		enum FileType {
			JPG,
			PNG
		};

	private:
		int width = -1;
		int height = -1;
		int channels = -1;

		unsigned int TID = 0;
		unsigned int TBO = 0; // Texture Buffer Object, optional

		/** @brief Get color register that corresponds with some type. GL_RGB for jpg. GL_RGBA for png
		 * @return The register defining how to read the color
		*/
		uint16_t getTypeRegister(FileType type);

		/** @brief Setup a texture before we load from a file or memory
	 	*/
		void setupTex(bool nearest, FileType type, uint8_t* data);

	public:
		template <typename T>
		static uint16_t glTypeRegFromType() {
			if (std::is_same<T, uint16_t>::value) {
				return GL_R16UI;
			} else if (std::is_same<T, int16_t>::value) {
				return GL_R16I;
			} else if (std::is_same<T, uint8_t>::value || std::is_same<T, char>::value) {
				return GL_R8UI;
			} else if (std::is_same<T, int8_t>::value) {
				return GL_R8I;
			} else if (std::is_same<T, int32_t>::value) {
				return GL_R32I;
			} else if (std::is_same<T, uint32_t>::value) {
				return GL_R32UI;
			} else {
				return GL_R8UI;
			}
		}

		/** @brief Texture default constructor
		*/
		Texture();

		/** @brief Create a texture buffer object
	 	*/
		template <typename T>
		Texture(T data[], uint64_t elements) {
			this->loadTBO(data, elements);
		}

		/** @brief Texture constructor. Load texture
		*/
		Texture(const char* path, FileType type = FileType::JPG, bool nearest = false);

		/** @brief Texture destructor
		*/
		~Texture();

		/** @brief Load an image path into the texture
		*  @param[in]	path	The filepath to load
		*  @param[in]	type	The type of the texture file
		*  @param[in]	nearest	Use nearest-neighbour texture filtering
		*  @return				A reference to this texture object
		*/
		Texture& load(const char* path, FileType type = FileType::JPG, bool nearest = false);

		/** @brief Load a buffer from memory into a texture
		*  @param[in]	path	The filepath to load
		*  @param[in]	type	The type of the texture file
		*  @param[in]	nearest	Use nearest-neighbour texture filtering
		*  @return				A reference to this texture object
		*/
		Texture& loadMem(const uint8_t* buffer, size_t bufferSize, FileType type = FileType::JPG, bool nearest = false);

		/** @brief Load a data buffer into a Texture Buffer Object
		*  @param[in]	path	The filepath to load
		*  @return				A reference to this texture object
		*/
		template <typename T>
		Texture& loadTBO(T data[], uint64_t elements) {
			// Create and bind the TBO
			glGenBuffers(1, &this->TBO);
			glBindBuffer(GL_TEXTURE_BUFFER, this->TBO);
			glBufferData(GL_TEXTURE_BUFFER, elements * sizeof(T), data, GL_STATIC_DRAW);

			// Create a texture to associate with the TBO. We use the texture to pass to the shader
			glGenTextures(1, &this->TID);
			glBindTexture(GL_TEXTURE_BUFFER, this->TID);
			glTexBuffer(GL_TEXTURE_BUFFER, Texture::glTypeRegFromType<T>(), this->TBO);

			return *this;
		}

		/** @brief Destroy the image. Called on destructor.
		*  @return A reference to this texture object
		*/
		Texture& destroy();

		/** @brief Check if this texture object has loaded some image yet. Checks if the size is -1x-1x-1
		 * @return True if this object has loaded an image, false if this is empty
		*/
		bool isDefined();

		/** @brief Get the size of this texture. Will be -1x-1x-1 if this texture has not been defined
		 * @param[out] getWidth		A pointer to an integer where a width will be saved
		 * @param[out] getHeight	A pointer to an integer where the height will be saved
		 * @param[out] getChannels	Optionally get the channels. If nullptr, do not get the channels.
		*/
		Texture& getSize(int* getWidth, int* getHeight, int* getChannels = nullptr);

		/** @brief Get texture ID
		*  @return	The opengl texture ID of this texture object
		*/
		unsigned int getTexture();

		/** @brief Bind this texture to some texture ID. Called before drawing each shape
		 * @param[in] id	The texture ID to bind to.
	 	*/
		Texture& bind(uint16_t id);
	};
}

#endif
