// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "oglopp.h"
#include "oglopp/shader.h"
#include <iostream>
#include <cmath>


using namespace oglopp;

#define CAMSPEED (0.05f)

void handleInput(Window& window) {
	// When escape is pressed...
	if (window.keyPressed(GLFW_KEY_ESCAPE)) {
		// Release the cursor
		window.cursorCapture();

		// If control is also pressed.. Destroy the window
		if (window.keyPressed(GLFW_KEY_LEFT_CONTROL)) {
			window.destroy();
		}
	}

	bool eventRecevied = false;
	{ // Keyboard
		if (window.keyPressed(GLFW_KEY_W)) {
			window.getCam().translate(window.getCam().getBack() * -CAMSPEED);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_A)) {
			window.getCam().translate(window.getCam().getRight() * -CAMSPEED);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_S)) {
			window.getCam().translate(window.getCam().getBack() * CAMSPEED);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_D)) {
			window.getCam().translate(window.getCam().getRight() * CAMSPEED);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_LEFT_CONTROL)) {
			window.getCam().translate(window.getCam().getUp() * -CAMSPEED);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_SPACE)) {
			window.getCam().translate(window.getCam().getUp() * CAMSPEED);
			eventRecevied = true;
		}
	}

	// The cursor is trapped. So we can do window stuff now
	if (window.isCursorCaptured()) {
		// Get the cursor position then set it to the middle of the window... or bottom left idk but it works
		glm::dvec2 cursor = window.getCursorPos();
		window.setCursorPos({0.0, 0.0});

		window.getCam().aimBy(cursor.y, cursor.x);
	}

	if (eventRecevied) {
		window.cursorCapture();
		window.setCursorPos({0.0, 0.0});
	}

	if (window.keyPressed(GLFW_KEY_R)) {
		window.cursorRelease();
	}
}

class InputBuffer {
public:
	static Window* windowPtr;

	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		InputBuffer::windowPtr->getCam().setFov(InputBuffer::windowPtr->getCam().getFov() - yoffset);
	}
};

Window* InputBuffer::windowPtr  = nullptr;

int main() {


	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Materials");
	InputBuffer::windowPtr = &window;
	glfwSetScrollCallback(window.getWindow(), InputBuffer::scrollCallback);

	// // Initialize our shader object
	Shader shader(
		// Vertex
		"#version 330 core\n"\
		"layout (location = 0) in vec3 aPos;\n"\
		"layout (location = 1) in vec3 aNormal;\n"\
		"layout (location = 2) in vec2 aTexCoord;\n"\
		\
		"out VS_OUT {\n"\
			"vec3 vNormal;\n"\
			"vec2 vTexCoord;\n"\
		"} vs_out;\n"\
		\
		"void main() {\n"\
			"gl_Position = vec4(aPos, 1.0);\n"\
			"gl_PointSize = 2000.0;\n"\
			"vs_out.vTexCoord = aTexCoord;\n"\
			"vs_out.vNormal = aNormal;\n"\
		"}\n", // End of vertex

		// Geometry
		"#version 330 core\n"\
		"layout (triangles) in;\n"\
		"layout (triangle_strip, max_vertices = 3) out;\n"\
		\
		"uniform mat4 projection;\n"\
		"uniform mat4 view;\n"\
		"uniform mat4 model;\n"\
		"uniform mat4 rotation;\n"\
		\
		"in VS_OUT {\n"\
			"vec3 vNormal;\n"\
			"vec2 vTexCoord;\n"\
		"} gs_in[];\n"\
		\
		"out vec3 FragPos;\n"\
		"out vec3 Normal;\n"\
		"out vec2 texCoord;\n"\
		\
		"void main() {\n"\
			"for (int i=0;i<3;i++) {\n"\
				"vec4 scale = vec4(1.0, 1.0, 1.0, 1.0);\n"\
				"vec4 newPos = (scale * gl_in[i].gl_Position);\n"\
				"vec4 newNormal = (rotation * scale * vec4(gs_in[i].vNormal, 1.0));\n"\
				\
				"gl_PointSize = 2000.0;\n"\
				"gl_Position = projection * view * model * newPos;\n"\
				"Normal = newNormal.xyz;\n"\
				"texCoord = gs_in[i].vTexCoord;\n"\
				"FragPos = vec3(model * gl_in[i].gl_Position);\n"\
				"EmitVertex();\n"\
			"}\n"\
			"EndPrimitive();\n"\
		"}\n",

		// Fragment
		(std::string(oglopp::Shader::GLSL_VERSION_STRING) +
		oglopp::Shader::COLOR_STRUCT +
		oglopp::Shader::MATERIAL_STRUCT +
		"uniform Material material;\n"\
		"uniform vec3 lightColor;\n"\
		"uniform vec3 lightPos;\n"\
		"uniform vec3 viewPos;\n"\
		\
		"in vec3 FragPos;\n"\
		"in vec3 Normal;\n"\
		"in vec2 texCoord;\n"\
		\
		"out vec4 FragColor;\n"\
		\
		"void main() {\n"\
			"vec3 ambient = lightColor * material.color.ambient;\n"\
			\
			"vec3 norm = normalize(Normal);\n"\
			"vec3 lightDir = normalize(lightPos - FragPos);\n"\
			"float diff = max(dot(norm, lightDir), 0.0);\n"\
			"vec3 diffuse = lightColor * (diff * material.color.diffuse);\n"\
			\
			"vec3 viewDir = normalize(viewPos - FragPos);\n"\
			"vec3 reflectDir = reflect(-lightDir, norm);\n"\
			"float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"\
			"vec3 specular = lightColor * (spec * material.color.specular);\n"\
			\
			"vec3 result = ambient + diffuse + specular;\n"\
			"FragColor = vec4(result, 1.0);\n"\
		"}\n").c_str(), // End of fragment

		ShaderType::RAW);

	int width = 0;
	int height = 0;
	window.getSize(&width, &height);
	std::cout << "Window size: [" << width << ", " << height << "]" << std::endl;

	// Initialize the shape we want to draw
	Cube coob;
	coob.scale(glm::vec3(0.5));

	Cube coob2;
	coob2.scale(glm::vec3(0.5, 0.5, 0.6));

	Cube coob3;
	coob3.scale(glm::vec3(3.0));

	Cube floor;
	floor.scale(glm::vec3(100.f, 0.5, 100.f));
	floor.setPosition(glm::vec3(0.f, -4.f, 0.f));

	// Camera cam;
	float angle = 0;

	window.getCam().setPos(glm::vec3(0.0, 0.0, -4.0)).setAngle(glm::vec3(00, -90, 0));
	window.getCam().setFov(65);

	shader.use();
	shader.setFloat("material.shininess", 64.f);
	shader.setVec3("material.color.ambient", glm::vec3(0.05, 0.03, 0.01));
	shader.setVec3("material.color.specular", glm::vec3(0.8));
	shader.setVec3("lightColor", glm::vec3(1.0));
	shader.setVec3("lightPos", glm::vec3(0.0, 4.0, 0.0));
	//shader.setDrawType(POINTS);

	// ----- Render Loop -----
	while (!window.shouldClose()) {
		// Process events
		handleInput(window);


		angle += 0.02;

		//window.getCam().setAngle(glm::vec3(angle * 10, 0, 0.0));

		// Uniforms
		coob.rotate(glm::vec3(0.01));
		coob2.setPosition(glm::vec3(sin(angle), cos(angle), 0.0));
		coob3.setPosition(glm::vec3(sin(angle / 10) * 4, cos(angle / 10) * 4, cos(angle / 10) * sin(angle / 10) * 4));
		//coob2.translate(glm::vec3(0, 1.0, 0.0));

		//window.getCam().setPos(glm::vec3(sin(angle) * 4, 3.0, cos(angle) * 4));

		// Prepare render layer

		shader.use();
		shader.setVec3("viewPos", window.getCam().getPos());
		shader.setVec3("lightPos", glm::vec3(sin(angle) * 10.0, 3.0, cos(angle) * 10.0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.setVec3("material.color.diffuse", glm::vec3(1.0, 0.5, 0.0));
		coob.draw(window, &shader);

		shader.setVec3("material.color.diffuse", glm::vec3(1.0));
		shader.setDrawType(POINTS);
		coob2.draw(window, &shader);
		shader.setDrawType(TRIANGLES);


		coob3.draw(window, &shader);


		shader.setVec3("material.color.diffuse", glm::vec3(0.2, 0.2, 1.0));
		floor.draw(window, &shader);

		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
