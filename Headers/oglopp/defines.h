#ifndef OGLOPP_DEFINES_H
#define OGLOPP_DEFINES_H

#include "oglopp/glad/gl.h"
#include <GLFW/glfw3.h>

// Cross-Platform Linking for Windows
#if defined(_WIN32) || defined(_WIN64)
	#ifdef OGL_BUILD_DLL
		#define OGL_API __declspec(dllexport)
	#else
		#define OGL_API __declspec(dllimport)
	#endif
#else
	#define OGL_API
#endif

// Helpful defines
#define HLGL_WORLD_UP 		glm::dvec3(0.0f, 1.0f, 0.0f)

#define HLGL_VEC_COMPONENTS 	3
#define HLGL_EBO_COMPONENTS		HLGL_VEC_COMPONENTS // One EBO index is composed of this many integers
#define HLGL_COL_COMPONENTS 	3
#define HLGL_TEX_COMPONENTS		2
#define HLGL_OPT_COMPONENTS		1

#define HLGL_RENDER_FAR			(800.f)
#define HLGL_RENDER_NEAR		(0.01f)

#ifdef GL_MAX_TEXTURE_UNITS
	#define HLGL_SHAPE_MAX_TEXTURES	(GL_MAX_TEXTURE_UNITS - GL_TEXTURE0)
#else
	#define HLGL_SHAPE_MAX_TEXTURES 	32
#endif

#define HLGL_DEFAULT_FOV		65.f
#define HLGL_SENSITIVITY		0.03f

#define HLGL_SCROLL_CB_TYPE ()
#define HLGL_SCROLL_CALLBACK ()

#define HLGL_NOCLIP_SPEED (0.05)



#endif
