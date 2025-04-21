#ifndef OGLOPP_INIT_H
#define OGLOPP_INIT_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <fstream>

namespace oglopp {
	/** @brief Hold a single instance of an object to safely initialize and destroy opengl resources
	*/
	class _HoneyLib_InitGL {
	public:
		_HoneyLib_InitGL();
		~_HoneyLib_InitGL();
	};

	extern _HoneyLib_InitGL _instance;

	std::ostream& operator<<(std::ostream& os, glm::mat4 const& obj);
	std::ostream& operator<<(std::ostream& os, glm::vec4 const& obj);
	std::ostream& operator<<(std::ostream& os, glm::vec3 const& obj);
}

#endif
