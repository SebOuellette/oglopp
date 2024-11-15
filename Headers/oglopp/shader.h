#ifndef OGLOPP_SHADER_H
#define OGLOPP_SHADER_H

#include <stdint.h>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "oglopp/defines.h"
#include "oglopp/glad/gl.h"

namespace oglopp {
	enum ShaderType : uint8_t {
		FILE,	// const char* vertex and const char* fragment represent a path to the respective shaders.
		RAW 	// const char* vertex and const char* fragment represent the fragment shader contents as a string.
	};

	enum ShaderStep : uint16_t {
		VERTEX 		= GL_VERTEX_SHADER,
		GEOMETRY	= GL_GEOMETRY_SHADER,
		FRAGMENT	= GL_FRAGMENT_SHADER,
	};

	/* @brief Shader object
	*/
	class Shader {
	private:
		unsigned int ID;

		/* @brief Load the shader file
		 * @param[in] shader		The path to the shader, or the contents of the shader itself
		 * @param[in] type			The type of the shader variable, either path or raw.
		 * @param[in] step			The shader step: vertex, geometry, or fragment
		 * @return 					The loaded shader index
		*/
		static uint32_t loadShaderFile(const char* shader, ShaderType type, ShaderStep step);

		/* @brief Load a list of shaders into this shader object
		 * @param[in] vertexShader		The vertex shader path or file contents
		 * @param[in] geoShader			The geometry shader path or file contents. Nullptr for no geometry shader
		 * @param[in] fragmentShader	The fragment shader path or file contents
		 * @param[in] type				The shader type.  File or raw.
	 	*/
		void load(const char* vertexShader, const char* geoShader, const char* fragmentShader, ShaderType type);

	public:
		static constexpr const char* GLSL_VERSION_STRING = "#version 330 core\n";

		static constexpr const char* COLOR_STRUCT = // struct Color
		"struct Color {"\
			"vec3 ambient;"\
			"vec3 diffuse;"\
			"vec3 specular;"\
		"};\n";

		static constexpr const char* TEXTURE_COLOR_STRUCT = // struct Texture
		"struct Color {"\
			"sampler2D diffuse;"\
			"sampler2D specualar;"\
		"};\n";

		static constexpr const char* MATERIAL_STRUCT =  // struct Material
		"struct Material {"\
		 	"Color color;"\
		 	"float shininess;"\
		"};\n";

		static constexpr const char* LIGHT_DIR_STRUCT = // struct LightD
		"struct LightD {"\
			"Color color;"\
			"vec3 direction;"\
		"};\n";

		static constexpr const char* LIGHT_POINT_STRUCT = // struct LightD
		"struct LightP {"\
			"Color color;"\
			"vec3 position;"\
			"float constant;"\
			"float linear;"\
			"float quadratic;"\
		"};\n";

		static constexpr const char* MODEL_VIEW_PROJECTION_MATRICES = //
		"uniform mat4 model;\n"\
		"uniform mat4 view;\n"\
		"uniform mat4 projection;\n"\
		"uniform mat4 rotation;\n";

		static std::string getTextureUniform(uint8_t textureId);

		/* @brief Create a new shader
		*/
		Shader(const char* vertex, const char* fragment, ShaderType type);
		Shader(const char* vertex, const char* geometry, const char* fragment, ShaderType type);
		// use / activate the shader
		void use();
		// Utility uniform functions
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int vlaue) const;
		void setFloat(const std::string &name, float value) const;
		void setVec2(const std::string &name, glm::vec2 const& value) const;
		void setVec3(const std::string &name, glm::vec3 const& value) const;
		void setVec4(const std::string &name, glm::vec4 const& vector) const;
		void setMat4(const std::string &name, glm::mat4 const& matrix) const;
	};
}

#endif
