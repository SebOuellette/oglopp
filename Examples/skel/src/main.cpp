// For basic texture stuff
// https://learnopengl.com/Getting-started/Hello-Triangle

// For 3d
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include <cstddef>
#include <filesystem>
#include <oglopp.h>
#include <iostream>
#include <cmath>
#include <cstring>

#include "defines.h"
#include "oglopp/texture.h"
#include "worldgen.h"
#include "assets/chunk.h"
//#include "pooler.h"

using namespace oglopp;

#define CAMSPEED (0.05)

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
		double speed = 1.0;


		if (window.keyPressed(GLFW_KEY_LEFT_SHIFT)) {
			speed = 8.0;
			eventRecevied = true;
		} else {
			speed = 1.0;
		}

		if (window.keyPressed(GLFW_KEY_LEFT_CONTROL)) {
			window.getCam().translate(window.getCam().getUp() * -CAMSPEED);
			eventRecevied = true;
		}

		if (window.keyPressed(GLFW_KEY_W)) {
			window.getCam().translate(window.getCam().getBack() * -CAMSPEED * speed);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_A)) {
			window.getCam().translate(window.getCam().getRight() * -CAMSPEED * speed);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_S)) {
			window.getCam().translate(window.getCam().getBack() * CAMSPEED * speed);
			eventRecevied = true;
		}
		if (window.keyPressed(GLFW_KEY_D)) {
			window.getCam().translate(window.getCam().getRight() * CAMSPEED * speed);
			eventRecevied = true;
		}
		if(window.keyPressed(GLFW_KEY_SPACE)) {
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

#define CHUNK_LOOKUPS_PER_FRAME 1

/* @brief Lookup the index of some chunk using binary search on X, then Y, then Z
 * @return -1 if not found, the index otherwise
*/
int64_t lookupIndex(std::vector<Chunk*>& chunks, ChunkPos const& lookupAt) {
	int64_t min = 0;
	int64_t max = chunks.size() - 1; // max is max index + 1
	int64_t middle;

	ChunkPos iterPos;

	while (min <= max) {
		middle = min + (max - min) / 2;
		iterPos = chunks[middle]->getChunkPos();

		if (iterPos == lookupAt) {
			return middle;
		} else if (iterPos < lookupAt) {
			min = middle + 1;
		} else {
			max = middle - 1;
		}

		//std::cout << min << " " << middle << " " << max << std::endl;
	}

	//std::cout << "Chunk not found. Chunk at middle was: " << iterPos.getX() << "," <<  iterPos.getY() << "," << iterPos.getZ()  << std::endl;

	return -1;
}

/* @brief Insert a chunk into the list in a sorted manner
*/
int8_t insertChunk(std::vector<Chunk*>& chunks, ChunkPos chunkPos, WorldGen& world) {
	//ChunkPos givenChunkPos = chunk->getChunkPos();
	ChunkPos iterChunkPos;
	Chunk* allocation = nullptr;

	if (chunks.size() == 0) {
		allocation = new Chunk(chunkPos, world);
		if (allocation == nullptr)
			return -1;

		chunks.push_back(allocation);
		return 1;
	}

	int64_t min = 0;
	int64_t max = chunks.size() - 1; // max is max index + 1
	int64_t middle = 0;
	ChunkPos iterPos;

	while (min <= max) {
		middle = min + (max - min) / 2;
		//std::cout << "loop - " << middle << ", max=" << max << ", min=" << min << std::endl;

		iterPos = chunks[middle]->getChunkPos();

		if (iterPos == chunkPos) {
			//std::cout << "Chunk found at " << chunkPos.getX() << ", " << chunkPos.getY() << ", " << chunkPos.getZ() << std::endl;
			return 0;
		} else if (iterPos < chunkPos) {
			min = middle + 1;
		} else {
			max = middle - 1; //(middle <= 0) ? 0 : (middle - 1);
		}
	}

	// Insert into index min
	//std::cout << "Inserted chunk at " << chunkPos.getX() << ", " << chunkPos.getY() << ", " << chunkPos.getZ() << std::endl;

	allocation = new Chunk(chunkPos, world);
	if (allocation == nullptr) {
		return -1;
	}

	chunks.insert(chunks.begin() + min, allocation);

	//chunks.push_back(chunk);
	return 1;
}



int main() {
	// Create the window
	Window window;
	InputBuffer::windowPtr = &window;

	window.create(800, 800, "Cubegame", {.clearColor = glm::vec4(0.3, 0.7, 1.0, 0.9)});
	glfwSetScrollCallback(window.getWindow(), InputBuffer::scrollCallback);

	// Get the path to this executable
	std::string MY_PATH = std::filesystem::canonical("/proc/self/exe");
	MY_PATH = MY_PATH.substr(0, MY_PATH.find_last_of('/') + 1); // This is not safe and will crash if the path doesn't have a / for some reason... idc that shouldn't happen and this is a test

	// Load shader
	Shader shader((MY_PATH + "shaders/vertex.glsl").c_str(), (MY_PATH + "shaders/geometry.glsl").c_str(), (MY_PATH + "shaders/fragment.glsl").c_str(), ShaderType::FILE);

	// Create a test texture
	Texture testTexture((MY_PATH + "assets/textures/TEST_BLOCK.png").c_str(), Texture::PNG, true);
	Texture stoneTexture((MY_PATH + "assets/textures/stone.png").c_str(), Texture::PNG, true);
	Texture grassTexture((MY_PATH + "assets/textures/grass.png").c_str(), Texture::PNG, true);
	Texture dirtTexture((MY_PATH + "assets/textures/dirt.png").c_str(), Texture::PNG, true);

	WorldGen world;
	// Store pointers cause we don't want the constructors to run...There is no default constructor case nor will there be :)
	std::vector<Chunk*> chunks;
	chunks.resize(0);
	const uint64_t LOAD_DIST = 10;
	const uint64_t RENDER_DISTANCE = (LOAD_DIST * CHUNK_SIZE); // divide by 2 to be radius instead of diameter
	// Chunks will only be unloaded if they are a distance of RENDER_DISTANCE + LOAD_UNLOAD_CHUNK_BUFFER away from the player.
	const uint64_t LOAD_UNLOAD_CHUNK_BUFFER	= 2;

	/*
	std::cout << "Generating chunks ";
	for (uint64_t x=0;x<LOAD_DIST;x++) {
		std::cout << "[" << std::setfill(' ') << std::setw(3) << static_cast<int64_t>(static_cast<double>(x + 1) / LOAD_DIST * 100) << "%]" << std::flush;

		for (uint64_t z=0;z<LOAD_DIST;z++) {
			for (uint64_t y=0;y<LOAD_DIST;y++) {
				//chunks.push_back(new Chunk(ChunkPos(x - LOAD_DIST / 2, y - LOAD_DIST / 2, z - LOAD_DIST / 2), world));
				//insertChunk(chunks, new Chunk(ChunkPos(x - LOAD_DIST / 2, y - LOAD_DIST / 2, z - LOAD_DIST / 2), world));
				insertChunk(chunks, ChunkPos(x - LOAD_DIST / 2, y - LOAD_DIST / 2, z - LOAD_DIST / 2), world);
			}
		}
		std::cout << "\b\b\b\b\b\b" << std::flush;
	}
	std::cout << "Done!" << std::endl; */

	// Debug
	// for (Chunk* pChunk : chunks) {
	// 	ChunkPos pos = pChunk->getChunkPos();

	// 	std::cout << pos.getX() << ", " << pos.getY()  << ", " << pos.getZ() << std::endl;
	// }

	// Camera cam;
	float angle = 0;

	window.getCam().setPos(glm::vec3(0.0, 20.0, -4.0)).setAngle(glm::vec3(00, -90, 0));
	window.getCam().setFov(65);

	shader.use();
	shader.setFloat("material.shininess", 12.f);
	shader.setVec3("material.color.ambient", glm::vec3(0.02, 0.01, 0.01));
	shader.setVec3("material.color.specular", glm::vec3(0.1));
	shader.setVec3("lightColor", glm::vec3(1.0));
	shader.setVec3("lightPos", glm::vec3(0.0, 20.0, 0.0));

	testTexture.bind(GL_TEXTURE0);
	shader.setInt("blockTex", 0);

	stoneTexture.bind(GL_TEXTURE1);
	shader.setInt("stoneTex", 1);

	grassTexture.bind(GL_TEXTURE2);
	shader.setInt("grassTex", 2);

	dirtTexture.bind(GL_TEXTURE3);
	shader.setInt("dirtTex", 3);

	int width, height;

	double dist;
	Chunk* pChunk;

	Chunk* pActiveChunk = nullptr;
	ChunkPos currentChunk;
	ChunkPos lastChunk;

	ChunkPos genChunk;

	size_t chunkGenIterator = 0;
	const size_t CHUNKDIST_CUBED = LOAD_DIST * LOAD_DIST * LOAD_DIST;

	// ----- Render Loop -----
	while (!window.shouldClose()) {

		// Process events
		handleInput(window);
		angle += 0.02;

		// Store last chunk to calc difference for generating chunks.
		lastChunk = currentChunk;
		// Find the current chunk position
		currentChunk.setX(window.getCam().getPos().x / CHUNK_SIZE);
		currentChunk.setY(window.getCam().getPos().y / CHUNK_SIZE);
		currentChunk.setZ(window.getCam().getPos().z / CHUNK_SIZE);

		// My chunk is updating
		if (lastChunk != currentChunk) {
			int64_t index = lookupIndex(chunks, currentChunk);

			if (index >= 0) { // Only update the chunk point if it was found in the loaded chunk list
				std::cout << "Moved chunk" << std::endl;
				pActiveChunk = chunks[index];
			}
		}

		if (pActiveChunk != nullptr) {
			//std::cout << "Currently in block: " << pActiveChunk->getChunkData().at(window.getCam().getPos().x, uint64_t y, uint64_t z)
		}

		// Update the projection and view matrices for all the shapes to be drawn
		window.getSize(&width, &height);
		window.getCam().updateProjectionView(width, height);

		// Prepare render layer
		shader.use();
		shader.setFloat("time", angle);
		shader.setVec3("viewPos", window.getCam().getPos());
		//shader.setVec3("lightPos", window.getCam().getPos());
		//shader.setVec3("lightPos", glm::vec3(sin(angle / 3) * 100.0 + 2.0, cos(angle / 3) * 100.0 + 5.0, 0.0));
		window.clear();

		shader.setVec3("material.color.diffuse", glm::vec3(1.0, 0.5, 0.0));



		// Generate new chunks
		for (size_t i=0;i<CHUNK_LOOKUPS_PER_FRAME;i++) {
			chunkGenIterator = (chunkGenIterator + 1) % CHUNKDIST_CUBED;
			genChunk.setX(chunkGenIterator / LOAD_DIST / LOAD_DIST - (LOAD_DIST >> 1)).setY((chunkGenIterator / LOAD_DIST) % LOAD_DIST - (LOAD_DIST >> 1)).setZ(chunkGenIterator % LOAD_DIST - (LOAD_DIST >> 1));

			genChunk = genChunk + currentChunk;
			insertChunk(chunks, genChunk, world);

			//std::cout << "Generating chunk at: " << genChunk.getX() << ", " << genChunk.getY() << ", " << genChunk.getZ() << " - " <<  << std::endl; //  (int64_t)lookupIndex(chunks, genChunk) << std::endl;
		}

		// x = n % 4
		// y = floor(n / 4) % 4
		// z = floor(n / 4 / 4)

		// Draw and delete old chunks
		for (size_t i=0; i<chunks.size();i++) {
			pChunk = chunks[i];

			dist = glm::distance(pChunk->getPosition(), window.getCam().getPos());
			if (dist > RENDER_DISTANCE + LOAD_UNLOAD_CHUNK_BUFFER) { // If I'm unloading a chunk on one side, I can load one on the other side. (this is probably not good but idk , it might work with a bit of a buffer (more chunks than render distance))
				//std::cout << "Unloading Chunk: distance from player was " << dist << std::endl;
				ChunkPos chunkPos = pChunk->getChunkPos();
				delete pChunk;

				chunks.erase(chunks.begin() + i);
				continue;
			}

			pChunk->draw(window, &shader);
		}

		// Swap buffers since we always draw on the back buffer isntead of the front buffer
		// When drawing on the front buffer, aka the actual pixels on the screen, you can get screen tearing and watch the pixels draw
		// We draw on the back buffer then swap it to the front to update the screen (v-sync?)
		window.bufferSwap();
		window.pollEvents();


	}

	// Free the memory
	for (std::size_t i=0;i<chunks.size();i++) {
		Chunk* lastChunk = chunks.back();

		if (lastChunk != nullptr) {
			delete lastChunk;
		}

		chunks.pop_back();
	}

	return 0;
}
