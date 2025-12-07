#ifndef OGLOPP_FBO_H
#define OGLOPP_FBO_H

#include "defines.h"

namespace oglopp {
	class FBO {
		public:

			/**
			 * @brief Create a new SSBO object. Default constructor
			 * @param[in] rboWidth	The initial width of the rbo
			 * @param[in] rboHeight The initial height of the rbo
	 		*/
			FBO(unsigned int rboWidth = 800, unsigned int rboHeight = 600);
			~FBO();

			/** @brief Bind the FBO to some binding number. Default is 0 if not specified
			 * @return	A reference to the FBO object
		 	*/
			FBO& bind();

			/** @brief Unbind the bound FBO
		 	*/
			static void unbind();

			/**
			 * @brief Check if the framebuffer is complete after loading
			 * @return True if complete, false otherwise
			 */
			bool isComplete() const;

			/**
			 * @brief Resize the rbo
			 * @param[in] rboWidth	The initial width of the rbo
			 * @param[in] rboHeight The initial height of the rbo
			 */
			void resize(unsigned int rboWidth, unsigned int rboHeight);


			unsigned int getFbo() const;
		 	unsigned int getRbo() const;
			unsigned int getWidth() const;
		 	unsigned int getHeight() const;


		private:
			GLuint fbo;
			GLuint rbo;

			int width;
			int height;
	};
}

#endif
