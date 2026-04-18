// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include "../Headers/oglopp.h"
#include <iostream>
#include <cmath>


using namespace oglopp;

#define CAMSPEED (0.05)

void scrollCallback(double horiz, double vert, void* data) {
	if (nullptr == data) return; // Data could be null

	Camera* camera = static_cast<Camera*>(data);
	camera->setFov(camera->getFov() - vert);
}

int main() {
	// Create the window
	Window window;
	window.create(800, 800, "HoneyLib OpenGL - Shading");

	// Set the scroll callback data pointer to the window's camera
	// This lets us update the FOV easily. We can change this if we need to do more. 
	window.setScrollCallback(scrollCallback, &window.getCam());

	int width = 0;
	int height = 0;
	window.getSize(&width, &height);
	std::cout << "Window size: [" << width << ", " << height << "]" << std::endl;

	// Initialize the shape we want to draw
	Cube coob;
	Cube coob2;
	Cube coob3;
	Cube floor;
	Sphere sphere;

	coob.scale(glm::vec3(0.5));
	coob2.scale(glm::vec3(0.5, 0.5, 0.6));
	coob3.scale(glm::vec3(3.0));
	floor.scale(glm::vec3(100.f, 0.5, 100.f));
	floor.setPosition(glm::vec3(0.f, -2.f, 0.f));
	sphere.setPosition(glm::vec3(3.f, 3.f, 6.f));
	sphere.scale(glm::vec3(3.0));

	// Initialize our shader object
	/*
	 	Oglopp provides some information to the shaders behind the scenes about the shape we're drawing. 
		It's important to know what data is available, and how to access it. 

		Graphical shaders are performed when the draw function is called on a shape. 
		 myshape.draw(window, &shader);

		Based on the following, you can imagine how one may create a class to inherit Shader, and automatically
		perform 
	
	MESH:
	 layout (location = 0) in vec3 pos
	 layout (location = 1) in vec3 normal
	 layout (location = 2) in vec2 tex
		Your mesh is accessible from the vertex shader using the layout qualifier. 
		 layout (location = <index>) in <type> <variable_name>;

		The indices and types must match what was used to construct myshape. Oglopp builtin shapes
		are composed of three components: CV, CN, and CT: Vertex positions, Normals, and Texture coordinates 
		respectively. If you are using a shape with custom components, you would use a layout
		qualifier to index each component in your shape.

	PROJECTION:
	 uniform mat4 model
	 uniform mat4 view
	 uniform mat4 projection
	 uniform mat4 rotation
		Projection variables are exposed to the vertex shader as uniforms. They are used to 
		draw the shape as a 3D object. This list of variables is composed of the model, view, 
		and projection matrices, as well as a rotation matrix. The rotation matrix is needed to apply
		the final transformation to the mesh's normal vectors, otherwise shading remains static as the
		object rotates.

		The model matrix is composed of a translation to the shape's position, a rotation in the x, y, and z axes, 
		followed by a scaling of the object. This seems backwards but it gives the right result idk. 
	*/
	Shader shader(
		// Vertex
		"#version 330 core\n"\
		"layout (location = 0) in vec3 POS;\n"\
		"layout (location = 1) in vec3 NORMAL;\n"\
		"layout (location = 2) in vec2 TEX;\n"\
		\
		"uniform mat4 model;\n"\
		"uniform mat4 view;\n"\
		"uniform mat4 projection;\n"\
		"uniform mat4 rotation;\n"\
		"uniform vec3 lightPos;\n"\
		\
		"out vec3 FragPos;\n"\
		"out vec3 Normal;\n"\
		"out vec2 TexCoord;\n"\
		\
		"void main() {\n"\
			"gl_Position = projection * view * model * vec4(POS, 1.0);\n"\
			"FragPos = vec3(model * vec4(POS, 1.0));\n"\
			"TexCoord = TEX;\n"\
			"Normal = vec3(rotation * vec4(NORMAL, 1.0));\n"\
		"}\n", // End of vertex

		// Fragment
		"#version 330 core\n"\
		"uniform sampler2D texture1;\n"\
		"uniform sampler2D texture2;\n"\
		"uniform vec3 albedo;\n"\
		"uniform vec3 lightColor;\n"\
		"uniform vec3 lightPos;\n"\
		"uniform vec3 viewPos;\n"\
		\
		"in vec3 FragPos;\n"\
		"in vec3 Normal;\n"\
		"in vec2 TexCoord;\n"\
		\
		"out vec4 FragColor;\n"\
		\
		"void main() {\n"\
			"float ambientStrength = 0.1;\n"\
			"vec3 ambient = ambientStrength * lightColor;\n"\
			\
			"vec3 norm = normalize(Normal);\n"\
			"vec3 lightDir = normalize(lightPos - FragPos);\n"\
			\
			"float diff = max(dot(norm, lightDir), 0.0);\n"\
			"vec3 diffuse = diff * lightColor;\n"\
			\
			"float specularStrength = 0.3;\n"\
			"vec3 viewDir = normalize(viewPos - FragPos);\n"\
			"vec3 reflectDir = reflect(-lightDir, norm);\n"\
			\
			"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"\
			"vec3 specular = specularStrength * spec * lightColor;\n"\
			\
			"vec3 result = (ambient + diffuse + specular) * albedo;\n"\
			"FragColor = vec4(result, 1.0);\n"\
		"}\n", // End of fragment

		ShaderType::RAW);

	// Camera cam;
	float angle = 0;

	glEnable(GL_DEPTH_TEST);

	window.getCam().setPos(glm::vec3(0.0, 0.0, -4.0)).setAngle(glm::vec3(00, -90, 0));
	window.getCam().setFov(65);



	// ----- Render Loop -----
	while (!window.shouldClose()) {
		// Process events
		window.handleNoclip();


		angle += 0.02;

		// Uniforms
		coob.rotate(glm::vec3(0.01));
		coob2.setPosition(glm::vec3(sin(angle), cos(angle), 0.0));
		coob3.setPosition(glm::vec3(sin(angle / 10) * 4, cos(angle / 10) * 4, cos(angle / 10) * sin(angle / 10) * 4));

		// Update the projection and view matrices for all the shapes to be drawn
		int width, height;
		window.getSize(&width, &height);
		window.getCam().updateProjectionView(width, height);

		// Prepare render layer
		shader.use();
		shader.setVec3("viewPos", window.getCam().getPos());
		shader.setVec3("lightColor", glm::vec3(1.0));
		shader.setVec3("lightPos", glm::vec3(0.0, 4.0, 0.0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.setVec3("albedo", glm::vec3(1.0, 0.5, 0.0));
		coob.draw(window, &shader);

		shader.setVec3("albedo", glm::vec3(1.0));
		coob2.draw(window, &shader);

		coob3.draw(window, &shader);

		shader.setVec3("albedo", glm::vec3(0.2, 0.2, 1.0));
		floor.draw(window, &shader);

		shader.setVec3("albedo", glm::vec3(1.0, 0.2, 0.1));
		sphere.draw(window, &shader);

		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();
	}

	return 0;
}
