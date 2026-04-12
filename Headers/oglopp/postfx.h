#ifndef OGLOPP_POSTFX_H
#define OGLOPP_POSTFX_H

#include "shape.h"
#include "fbo.h"

namespace oglopp {
	/** @brief Drawable object
	*/
	class PostFx : public Shape {
	private:
		FBO fbo;
		Texture fboTex; 


	public:	
		PostFx(Window& window);

		/** @brief Draw this shape to the specified window using an optional shader
		 * @param[in] window	A reference to the window object
		 * @param[in] pShader	An optional pointer to the shader object
		 * @return 		A reference to this shape
		 */
		virtual PostFx& draw(Window& window, Shader* pShader = nullptr) override;
	};
}

#endif
