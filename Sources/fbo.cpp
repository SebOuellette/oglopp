#include "oglopp/fbo.h"
#include "oglopp/glad/gl.h"

namespace oglopp {
	/** @brief Create a new SSBO object. Default constructor
	*/
	FBO::FBO(unsigned int rboWidth, unsigned int rboHeight) {
		glGenFramebuffers(1, &this->fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

		glGenRenderbuffers(1, &this->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, rboWidth, rboHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
	}

	FBO::~FBO() {
		glDeleteFramebuffers(1, &this->fbo);
		glDeleteRenderbuffers(1, &this->rbo);
	}

	/** @brief Bind the SSBO to some binding number. Default is 0 if not specified
	 * @return				A reference to the SSBO 0
 	*/
	FBO& FBO::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		return *this;
	}

	/** @brief Unbind the bound FBO
 	*/
	void FBO::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
	}


	/**
	 * @brief Check if the framebuffer is complete after loading
	 * @return True if complete, false otherwise
	 */
	bool FBO::isComplete() const {
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	/**
	 * @brief Resize the rbo
	 * @param[in] rboWidth	The initial width of the rbo
	 * @param[in] rboHeight The initial height of the rbo
	 */
	void FBO::resize(unsigned int rboWidth, unsigned int rboHeight) {
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, rboWidth, rboHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}


	unsigned int FBO::getFbo() const {
		return this->fbo;
	}

	unsigned int FBO::getRbo() const {
		return this->rbo;
	}
}
