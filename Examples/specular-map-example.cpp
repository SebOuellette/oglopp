// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "oglopp.h"
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
	window.create(800, 800, "HoneyLib OpenGL - Specular Map Example");
	InputBuffer::windowPtr = &window;
	glfwSetScrollCallback(window.getWindow(), InputBuffer::scrollCallback);

	// // Initialize our shader object
	Shader shader(
		// Vertex
		(std::string(oglopp::Shader::GLSL_VERSION_STRING) +
		"layout (location = 0) in vec3 aPos;\n"\
		"layout (location = 1) in vec3 aNormal;\n"\
		"layout (location = 2) in vec2 aTexCoord;\n" +
		oglopp::Shader::MODEL_VIEW_PROJECTION_MATRICES +
		"uniform vec3 lightPos;\n"\
		\
		"out vec3 FragPos;\n"\
		"out vec3 Normal;\n"\
		"out vec2 texCoord;\n"\
		\
		"void main() {\n"\
			"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"\
			"FragPos = vec3(model * vec4(aPos, 1.0));\n"\
			"texCoord = aTexCoord;\n"\
			"Normal = vec3(rotation * vec4(aNormal, 1.0));\n"\
		"}\n").c_str(), // End of vertex

		// Fragment
		(std::string(oglopp::Shader::GLSL_VERSION_STRING) +
		"struct Material {sampler2D diffuse;sampler2D specular;float shininess;};\n"\
		"struct Light {vec3 diffuse; vec3 ambient; vec3 specular; vec3 position;};\n"\
		"uniform Material material;\n"\
		"uniform Light light;\n"\
		"uniform vec3 viewPos;\n"\
		\
		"in vec3 FragPos;\n"\
		"in vec3 Normal;\n"\
		"in vec2 texCoord;\n"\
		\
		"out vec4 FragColor;\n"\
		\
		"void main() {\n"\
			"vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));\n"\
			\
			"vec3 norm = normalize(Normal);\n"\
			"vec3 lightDir = normalize(light.position - FragPos);\n"\
			"float diff = max(dot(norm, lightDir), 0.0);\n"\
			"vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));\n"\
			\
			"vec3 viewDir = normalize(viewPos - FragPos);\n"\
			"vec3 reflectDir = reflect(-lightDir, norm);\n"\
			"float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"\
			"vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));\n"\
			\
			"vec3 result = ambient + diffuse + specular;\n"\
			"FragColor = vec4(result, 1.0);\n"\
			\
			"FragColor = vec4(ambient + diffuse + specular, 1.0);\n"\
		"}\n").c_str(), // End of fragment

		ShaderType::RAW);

	int width = 0;
	int height = 0;
	window.getSize(&width, &height);
	std::cout << "Window size: [" << width << ", " << height << "]" << std::endl;


	Cube coob;
	coob.scale(glm::vec3(3.0));

	//Cube floor;
	//floor.scale(glm::vec3(100.f, 0.5, 100.f));
	//floor.setPosition(glm::vec3(0.f, -4.f, 0.f));

	// Load texture and specular map
	Texture diffuse("/network/Programming/opengl/Examples/assets/container2.png", Texture::FileType::PNG);
	Texture specular("/network/Programming/opengl/Examples/assets/container2_specular.png", Texture::FileType::PNG);

	coob.pushTexture(diffuse);
	coob.pushTexture(specular);

	// Camera cam;
	float angle = 0;
	window.getCam().setPos(glm::vec3(6.0, 5.0, -6.0)).setAngle(glm::vec3(20, -45, 0));
	window.getCam().setFov(65);

	shader.use();
	shader.setFloat("material.shininess", 64.f);
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);
	shader.setVec3("light.ambient", glm::vec3(0.03));
	shader.setVec3("light.diffuse", glm::vec3(1.0));
	shader.setVec3("light.position", glm::vec3(0.0, 4.0, 0.0));
	shader.setVec3("light.specular", glm::vec3(1.0, 0.0, 0.0));


	// ----- Render Loop -----
	while (!window.shouldClose()) {
		// Process events
		handleInput(window);


		angle += 0.02;

		//window.getCam().setAngle(glm::vec3(angle * 10, 0, 0.0));

		// Update the projection and view matrices for all the shapes to be drawn
		int width, height;
		window.getSize(&width, &height);
		window.getCam().updateProjectionView(width, height);

		// Uniforms
		//coob.setPosition(glm::vec3(sin(angle / 10) * 4, cos(angle / 10) * 4, cos(angle / 10) * sin(angle / 10) * 4));
		//coob.rotate(glm::vec3(angle / 10));
		//coob2.translate(glm::vec3(0, 1.0, 0.0));

		//window.getCam().setPos(glm::vec3(sin(angle) * 4, 3.0, cos(angle) * 4));

		// Prepare render layer

		shader.use();
		shader.setVec3("viewPos", window.getCam().getPos());
		shader.setVec3("light.position", glm::vec3(sin(angle) * 10.0, 4.0, cos(angle) * 10.0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		coob.draw(window, &shader);


		//shader.setVec3("material.color.diffuse", glm::vec3(0.2, 0.2, 1.0));
		//floor.draw(window, &shader);

		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
