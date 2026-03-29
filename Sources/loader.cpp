#include <fstream>
#include <string>
#include <iostream>

#include "oglopp/obj/loader.h"

namespace oglopp {
	Loader::Loader() {
		this->attached = nullptr;
	}
	
	/**
	 * @brief Construct a new shape from an obj file
	 * @param[in] filename	The path of the obj file
	 * @param[in] vao	The VAO object to build
	 * @return A reference to this objloader object
	 */
	Loader& Loader::construct(std::string const& filename, VAO& vao) {
		// Set the attached VAO object
		this->attached = &vao;
		// Push the file
		std::ifstream file(filename);
		
		// Read every line
		std::string line = "";
		std::string type = "";
		while (file.good()) {
			std::getline(file, type, ' ');
			std::getline(file, line);

			std::cout << "[" << type << "] [" << line << "]" << std::endl;
		}

		file.close();
		return *this;
	}
}
