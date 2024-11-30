#include "oglopp/shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "oglopp/defines.h"
#include "oglopp/glad/gl.h"

namespace oglopp {
	/* @brief Load the shader file
	 * @param[in] shader		The path to the shader, or the contents of the shader itself
	 * @param[in] type			The type of the shader variable, either path or raw.
	 * @param[in] step			The shader step: vertex, geometry, or fragment
	 * @return 					The loaded shader index
	*/
	uint32_t Shader::loadShaderFile(const char* shader, ShaderType type, ShaderStep step) {
		// 1. retrieve the vertex/fragment source code from filePath
		std::string shaderCode;
		int success;
		char infoLog[512];
		uint32_t shaderIndex = 0;

		if (type == FILE) {
			std::ifstream shaderFile;

			// ensure ifstream objects can throw exceptions:
			shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

			try {
				std::cout << "Opening file path: " << shader << std::endl;
				shaderFile.open(shader, std::ios::in);

			} catch(const std::ios_base::failure& fail) {
				std::cerr << "File failed to open." << std::endl;
				return 0;
			}

			std::stringstream shaderStream;
			// read file's buffer contents into streams
			shaderStream << shaderFile.rdbuf();

			// close file handlers
			shaderFile.close();

			// convert stream into string
			shaderCode = shaderStream.str();
		} else {
			shaderCode = shader;
		}

		const char* shaderPtr = shaderCode.c_str();

		// Do the opengl part now
		shaderIndex = glCreateShader(step);
		glShaderSource(shaderIndex, 1, &shaderPtr, NULL);
		glCompileShader(shaderIndex);

		// print compile errors if any
		glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shaderIndex, 512, NULL, infoLog);
			std::cout << "[Oglopp] " << ((step == VERTEX) ? "Vertex" : ((step == GEOMETRY) ? "Geometry" : ((step == COMPUTE) ? "Compute" :"Fragment"))) << " compilation failed!\n" << infoLog << std::endl;
			std::cout << shaderPtr << std::endl;
		};

		return shaderIndex;
	}

	/* @brief Load a list of shaders into this shader object
	 * @param[in] vertexShader		The vertex shader path or file contents
	 * @param[in] geoShader			The geometry shader path or file contents. Nullptr for no geometry shader
	 * @param[in] fragmentShader	The fragment shader path or file contents
	 * @param[in] type				The shader type.  File or raw.
 	*/
	void Shader::load(const char* vertexShader, const char* geoShader, const char* fragmentShader, ShaderType type) {
		int success;
		char infoLog[512];

		uint32_t vertexIndex;
		uint32_t geoIndex;
		uint32_t fragmentIndex;

		if (vertexShader != nullptr) {
			vertexIndex = Shader::loadShaderFile(vertexShader, type, VERTEX);
		}
		if (geoShader != nullptr) {
			geoIndex = Shader::loadShaderFile(geoShader, type, GEOMETRY);
		}
		if (fragmentShader != nullptr) {
			fragmentIndex = Shader::loadShaderFile(fragmentShader, type, FRAGMENT);
		}

		// shader Program
		this->ID = glCreateProgram();
		if (vertexShader != nullptr) {
			glAttachShader(this->ID, vertexIndex);
		}
		if (geoShader != nullptr) {
			glAttachShader(this->ID, geoIndex);
		}
		if (fragmentShader != nullptr) {
			glAttachShader(this->ID, fragmentIndex);
		}
		glLinkProgram(this->ID);

		// print linking errors if any
		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "[Oglopp] Shader linking failed!\n" << infoLog << std::endl;
		}

		// delete the shaders as they're linked into our program now and no longer necessary
		if (vertexShader != nullptr) {
			glDeleteShader(vertexIndex);
		}
		if (geoShader != nullptr) {
			glDeleteShader(geoIndex);
		}
		if (fragmentShader != nullptr) {
			glDeleteShader(fragmentIndex);
		}
	}

	/* @brief Get the texture uniform string for use in fragment shaders
	 * @param[in] textureId	The texture number from 0 to 32
	*/
	std::string Shader::getTextureUniform(uint8_t textureId) {
		if (textureId >= HLGL_SHAPE_MAX_TEXTURES) {
			return "";
		}

		return "uniform sampler2D texture" + std::to_string(textureId) + ";\n";
	}

	Shader::Shader(const char* vertex, const char* fragment, ShaderType type) : ID(0) {
		this->drawType = TRIANGLES;

		this->load(vertex, nullptr, fragment, type);
	}

	Shader::Shader(const char* vertex, const char* geometry, const char* fragment, ShaderType type) : ID(0) {
		this->drawType = TRIANGLES;

		this->load(vertex, geometry, fragment, type);
	}

	void Shader::use() {
		glUseProgram(this->ID);
	}

	void Shader::setBool(const std::string &name, bool value) const {
	    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string &name, int value) const {
	    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string &name, float value) const {
	    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
	}

	void Shader::setVec2(const std::string &name, glm::vec2 const& value) const {
		glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value[0], value[1]);
	}

	void Shader::setVec3(const std::string &name, glm::vec3 const& value) const {
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value[0], value[1], value[2]);
	}

	void Shader::setVec4(const std::string &name, glm::vec4 const& value) const {
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value[0], value[1], value[2], value[3]);
	}

	void Shader::setMat4(const std::string &name, glm::mat4 const& matrix) const {
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(*const_cast<glm::mat4*>(&matrix)));
	}

	/* @brief Get a constant reference to the current draw type
	 * @return A constant reference
 	*/
	DrawType const& Shader::getDrawType() {
		return this->drawType;
	}

	/* @brief Set the draw type
	 * @param[in] type	The new type to set the draw type to
	 * @return			A reference to this shape object
 	*/
	Shader& Shader::setDrawType(DrawType const& type) {
		this->drawType = type;

		return *this;
	}
}
