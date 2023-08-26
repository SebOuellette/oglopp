#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader {
private:
	unsigned int ID;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	// use / activate the shader
	void use();
	// Utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int vlaue) const;
	void setFloat(const std::string &name, float value) const;
	void setVec4(const std::string &name, std::vector<float> value) const;
};

#endif // SHADER_HPP
