#include "../Headers/oglopp.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

namespace oglopp {
	Shader::Shader(const char* vertex, const char* fragment, ShaderType type) {
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;

		if (type == FILE) {
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;

			// ensure ifstream objects can throw exceptions:
			vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

			try {
				std::cout << "Opening file path: " << vertex << std::endl;
				vShaderFile.open(vertex, std::ios::in);

				std::cout << "Opening file path: " << fragment << std::endl;
				fShaderFile.open(fragment, std::ios::in);

			} catch(const std::ios_base::failure& fail) {
				std::cerr << "File failed to open." << std::endl;
				return;
			}

			std::stringstream vShaderStream;
			std::stringstream fShaderStream;
			// read file's buffer contents into streams
			fShaderStream << fShaderFile.rdbuf();
			vShaderStream << vShaderFile.rdbuf();

			// close file handlers
			fShaderFile.close();
			vShaderFile.close();

			// convert stream into string
			fragmentCode = fShaderStream.str();
			vertexCode   = vShaderStream.str();
		} else {
			vertexCode = vertex;
			fragmentCode = fragment;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		unsigned int vertexi, fragmenti;
		int success;
		char infoLog[512];

		// vertex Shader
		vertexi = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexi, 1, &vShaderCode, NULL);
		glCompileShader(vertexi);

		// print compile errors if any
		glGetShaderiv(vertexi, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(vertexi, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			std::cout << vertexCode << std::endl;
		};

		// similiar for Fragment Shader
		fragmenti = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmenti, 1, &fShaderCode, NULL);
		glCompileShader(fragmenti);

		// print compile errors if any
		glGetShaderiv(fragmenti, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(fragmenti, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			std::cout << fragmentCode << std::endl;
		};

		// shader Program
		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertexi);
		glAttachShader(this->ID, fragmenti);
		glLinkProgram(this->ID);

		// print linking errors if any
		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertexi);
		glDeleteShader(fragmenti);
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
}
