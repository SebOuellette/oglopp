#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <cmath>
#include <oglopp.h>

using namespace oglopp;

int main() {
	Window::Settings sets;
	sets.doFaceCulling = false; // Leave this as false for a cool effect when you go behind the mirror
	sets.doStencilBuffer = true;

	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Stencil Example", sets);


	const std::string VERTEX = \
		"#version 330 core\n"\
		"layout (location = 0) in vec3 pos;\n"\
		"layout (location = 1) in vec3 normal;\n"\
		"layout (location = 2) in vec2 tex;\n"\
		\
		"uniform mat4 rotation;\n"\
		"uniform mat4 model;\n"\
		"uniform mat4 view;\n"\
		"uniform mat4 projection;"\
		\
		"out vec4 vNormal;\n"\
		"out vec2 vTex;\n"\
		\
		"void main() {\n"\
			"gl_Position = projection * view * model * vec4(pos, 1.0);\n"\
			"vNormal = rotation * vec4(normal, 1.0);\n"\
			"vTex = tex;\n"\
		"}\n";

	// Shaders
	Shader cubeShader(
		// Vertex Shader
		VERTEX.c_str(),

		// Fragment Shader
		"#version 330 core\n"\
		"out vec4 FragColor;\n"\
		"in vec4 vNormal;\n"\
		"in vec2 vTex;\n"\
		\
		"uniform sampler2D texture0;\n"\
		"uniform bool reflection;"
		\
		"void main() {\n"\
			"FragColor = abs(vNormal) * texture(texture0, vTex);\n"\
			"if (reflection) {\n"\
				"FragColor = vec4((FragColor.xyz * 0.5 + vec3(0.5)), FragColor.a);\n"\
			"}\n"\
		"}\n",

		ShaderType::RAW);


	Shader floorShader {
		VERTEX.c_str(),

		"#version 330 core\n"\
		"out vec4 FragColor;\n"\
		\
		"void main() {\n"\
			"FragColor = vec4(1);\n"\
		"}\n",


		ShaderType::RAW
	};

	// Camera
	window.getCam().setPos(glm::vec3(7,3,2));
	window.getCam().setAngle(glm::vec3(25,14,0));	


	// Load a 3d model and texture
	Cube cube;
	Texture tex("Examples/assets/container.jpg", oglopp::Texture::FileType::JPG);
	cube.pushTexture(&tex);
	cube.setPosition(glm::vec3(0, 1, 0));

	// One way to create a reflection is just by drawing the object twice.
	// We really don't need two separate Cubes here, we could just draw cube twice rather than cube and reflection.
	Cube reflection;
	reflection.pushTexture(&tex);


	// Now the floor for reflections
	Rectangle floor;
	floor.rotate(glm::dvec3(M_PI/2, 0, 0)); // Lay it flat
	floor.scale(glm::dvec3(4));

	// ----- Render Loop -----
	double time = 0;
	while (!window.shouldClose()) {
		// Builtin simple WASD and mouse control
		window.handleNoclip();

		// Make the cube spin
		time = glfwGetTime();
		cube.setAngle(glm::dvec3(sin(time) * M_PI, cos(time), 0));

		// Set the reflection's angle to the mirror of the main cube
		reflection.setAngle(glm::dvec3(-cube.getAngle().x, cube.getAngle().y, cube.getAngle().z));
		reflection.setPosition(-cube.getPosition());

		// Clear color, depth, and stencil
		window.clear();

		// First, we'll just draw the normal shape, not the reflection
		{
			window.stencilFunc(Window::DepthPass::ALWAYS);

			cubeShader.use();
			cubeShader.setBool("reflection", false);
			cube.draw(window, &cubeShader);
		}

		// Now we'll draw the mirror we'll use for reflecting. In this case, it's just a stencil
		// used to mask another object on the other side.
		{
			// Set the stencil to 0 so we can just check if our new content is greater (than 0)
			window.writeStencil(); // Must enable writing before clearing
			window.clear(Window::ClearMask::STENCIL);

			// "If my shape is drawing to this pixel, replace the stencil pixel with 1. Otherwise set to zero"
			window.stencilOp(Window::REPLACE, Window::ZERO, Window::ZERO);
			window.stencilFunc(Window::DepthPass::GEQUAL, 1);
					
			floor.draw(window, &floorShader);

			// Stop writing to the stencil
			window.readStencil();
		}

		// Reflection
		{
			// "If the stencil was previously set to 1 on this pixel, then draw my shape."
			window.stencilFunc(Window::DepthPass::EQUAL, 1);
			// Ignore depth since it's accounted for in the stencil. It will be 0 if we don't want to reflect.
			// Also, we want to draw the reflection on top of the mirror.
			window.clear(Window::DEPTH);

			cubeShader.use();
			cubeShader.setBool("reflection", true);
			reflection.draw(window, &cubeShader);
		}


		
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
