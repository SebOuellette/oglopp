#ifndef OGLOPP_TEXTURE_H
#define OGLOPP_TEXTURE_H

#include <stdint.h>

namespace oglopp {
	/* @brief Texture
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

		/* @brief Get color register that corresponds with some type. GL_RGB for jpg. GL_RGBA for png
		 * @return The register defining how to read the color
		*/
		uint16_t getTypeRegister(FileType type);

	public:

		/* @brief Texture default constructor
		*/
		Texture();

		/* @brief Texture constructor. Load texture
		*/
		Texture(const char* path, FileType type = FileType::JPG);

		/* @brief Texture destructor
		*/
		~Texture();

		/* @brief Load an image path into the texture
		*  @param[in]	path	The filepath to load
		*  @return				A reference to this texture object
		*/
		Texture& load(const char* path, FileType type = FileType::JPG);

		/* @brief Destroy the image. Called on destructor.
		*  @return A reference to this texture object
		*/
		Texture& destroy();

		/* @brief Check if this texture object has loaded some image yet. Checks if the size is -1x-1x-1
		 * @return True if this object has loaded an image, false if this is empty
		*/
		bool isDefined();

		/* @brief Get the size of this texture. Will be -1x-1x-1 if this texture has not been defined
		 * @param[out] getWidth		A pointer to an integer where a width will be saved
		 * @param[out] getHeight	A pointer to an integer where the height will be saved
		 * @param[out] getChannels	Optionally get the channels. If nullptr, do not get the channels.
		*/
		Texture& getSize(int* getWidth, int* getHeight, int* getChannels = nullptr);

		/* @brief Get texture ID
		*  @return	The opengl texture ID of this texture object
		*/
		unsigned int getTexture();
	};
}

#endif
