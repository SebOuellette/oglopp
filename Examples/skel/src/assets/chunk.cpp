#include "assets/chunk.h"

#define RENDER_DIMENSIONS 3

/* @brief Create a new chunk sub pos object
* @param[in] newX	The x position of the chunk block
* @param[in] newY	The y position of the chunk block
* @param[in] newZ	The z position of the chunk block
*/
ChunkSubPos::ChunkSubPos(int16_t newX, int16_t newY, int16_t newZ) : x(newX), y(newY), z(newZ) {}

/* @brief Subtract one chunk position from another
 * @param[in] posB  The position to subtract from the position stored in this object
 * @return          The difference
*/
ChunkSubPos ChunkSubPos::operator-(ChunkSubPos const& posB) const {
    ChunkSubPos result(this->x, this->y, this->z);

    result.x -= posB.x;
    result.y -= posB.y;
    result.z -= posB.z;

    return result;
}

/* @brief Add one chunk position to another
* @param[in] posB  The position to add to the position stored in this object
* @return          The sum as a new object
*/
ChunkSubPos ChunkSubPos::operator+(ChunkSubPos const& posB) const {
	ChunkSubPos result(this->x, this->y, this->z);

	result.x += posB.x;
	result.y += posB.y;
	result.z += posB.z;

	return result;
}

 /* @brief Convert the chunk sub pos into a floating point local pos, where 0 -> 0.0 and CHUNK_SIZE -> 1.0
* @return The local position of this chunkSubPos used for vertices
*/
glm::vec3 ChunkSubPos::toLocal() {
	// For now... try just returning the actual position.. We'd have to scale it later otherwise.. maybe I can just make the shape big.
	return glm::vec3(this->x, this->y, this->z);
}

/* @brief True if this chunk sub pos is within a chunk, false otherwise
* @return 	true if within the chunk, false otherwise
*/
bool ChunkSubPos::isInRange() const {
	bool res = this->x >= 0 && this->y >= 0 && this->z >= 0 &&
	this->x < CHUNK_SIZE && this->y < CHUNK_SIZE && this->z < CHUNK_SIZE;

	return res;
}

/* @brief Create a new chunk mesh based on a 3d array of chunk blocks
* @param[in] chunkData     A 3d array of chunk data. Indexed as [z][y][x]
*/
Chunk::Chunk(ChunkData const& chunkData) : savedChunkData(chunkData) {
	this->worldGen = nullptr;
	this->thisChunkPos = ChunkPos();

	this->drawCubes(chunkData);
}

/* @brief The chunk position. Will set the opengl shape position as well as generate with WorldGen
 * @param[in] chunkPos	The position of the chunk
 * @param[in] worldGen	A reference to the world generation object
*/
Chunk::Chunk(ChunkPos const& chunkPos, WorldGen& worldGen) {
	this->worldGen = &worldGen;
	this->thisChunkPos = chunkPos;

	worldGen.generateChunk(chunkPos, &this->savedChunkData);

	this->drawCubes(this->savedChunkData);

	this->setPosition(glm::vec3(CHUNK_SIZE * chunkPos.getX(), CHUNK_SIZE * chunkPos.getY(), CHUNK_SIZE * chunkPos.getZ()));
}

/* @brief Get a reference to the saved chunk data
 * @return the chunk data
*/
ChunkData const& Chunk::getChunkData() {
	return this->savedChunkData;
}

/* @brief Get this chunk position
 * @return A constant reference to this chunk position
*/
ChunkPos const& Chunk::getChunkPos() {
	return this->thisChunkPos;
}


/* @brief Check if a face should be drawn between thisBlockPos and compareBlockPos, given the chunk data chunkData
* @param[in] thisBlockPos      The position of the current block. This will be the "inner" face
* @param[in] compareBlockPos   The position of the next block. This will be the "outer" face (draw in clockwise)
* @return                      True if a face should be drawn. False if a face should not be drawn and/or one of the positions is out of range.
*/
const bool Chunk::shouldDrawFace(ChunkSubPos const& thisBlockPos, ChunkSubPos const& compareBlockPos, ChunkData const& chunkData) {
    ChunkSubPos offset = compareBlockPos - thisBlockPos;

	if (!thisBlockPos.isInRange()) {
		return false;
	}

	//if (!compareBlockPos.isInRange()) {
	//	return false; // true
	//}

    // zerocount must be equal to the number of dimensions minus one. in order to draw. No more, no less (for 3d, and 2d at least)
    uint8_t zeroCount = ((offset.x == 0) ? 1 : 0) + ((offset.y == 0) ? 1 : 0) + ((offset.z == 0) ? 1 : 0);
    if (zeroCount != RENDER_DIMENSIONS - 1) {

		//std::cout << "Not the right number of 0s - " << static_cast<int>(zeroCount) << " pos: [" << offset.x << ", " << offset.y << ", " << offset.z << "]" << std::endl;
        return false;
    }

    bool res = false;
    BlockType typeA = chunkData.at(thisBlockPos.x, thisBlockPos.y, thisBlockPos.z); // this->worldGen->blockAt(WorldPos(this->thisChunkPos, thisBlockPos.x, thisBlockPos.y, thisBlockPos.z)); //
    BlockType typeB = AIR;
    if (compareBlockPos.isInRange()) {
    	typeB = chunkData.at(compareBlockPos.x, compareBlockPos.y, compareBlockPos.z);
    } else if (this->worldGen != nullptr) {
		WorldPos comparePos(this->thisChunkPos, compareBlockPos.x, compareBlockPos.y, compareBlockPos.z);

		/*std::cout << 	"Chunk:[" << this->thisChunkPos.getX() << "," << this->thisChunkPos.getY() << "," << this->thisChunkPos.getZ() << "] " <<
						"Block:[" << compareBlockPos.x << "," << compareBlockPos.y << "," << compareBlockPos.z << "] " <<
						"-> Chunk:[" << comparePos.getChunkPos().getX() << "," << comparePos.getChunkPos().getY() << "," << comparePos.getChunkPos().getZ() << "] " <<
						"Block:[" << comparePos.getX() << "," << comparePos.getY() << "," << comparePos.getZ() << "] "
						"(abs)[" << comparePos.absX() << "," << comparePos.absY() << "," << comparePos.absZ() << "]" << std::endl;*/


		typeB = this->worldGen->blockAt(comparePos);
		//std::cout << "Type is " << typeB << std::endl;
	} else {
		std::cout << this->worldGen << std::endl;
	}

	// COMMENTED - Don't sort anymore. That's not what we want, cause we could draw double the faces
	// Only draw on blocks, not on air.
    // Sort the block types so we don't have to compare each option twice
    //if (typeA > typeB) {
    //    BlockType tmp = typeA;
    //    typeA = typeB;
    //    typeB = tmp;
    //}

    // Compare. The lessermost enum must be typeA
    switch (typeB) {
        // If blockA is not Air, but typeB IS Air
        case BlockType::AIR:
            res = typeA != BlockType::AIR; // And the other is not air.. then draw
            break;

        default:
            res = false;
            break;
    }

    return res;
}

/* @brief Draw a face between Block A and block B. Check if the face should be drawn first.
* @param[in] thisBlockPos      The position of the current block. This will be the "inner" face
* @param[in] compareBlockPos   The position of the next block. This will be the "outer" face (draw in clockwise)
* @return                      A reference to this chunk object
*/
Chunk& Chunk::drawFace(ChunkSubPos const& thisBlockPos, ChunkSubPos const& compareBlockPos, ChunkData const& chunkData) {
    if (false == this->shouldDrawFace(thisBlockPos, compareBlockPos, chunkData)) {
        // Return. Do not draw anything. No face should be drawn.
        return *this;
    }

	// Find an offset, which indicates which face we draw
	// This is also the normal of the face
    ChunkSubPos normal = compareBlockPos - thisBlockPos;

    // Find the block type of this block
    BlockType thisBlock = chunkData.at(thisBlockPos.x, thisBlockPos.y, thisBlockPos.z); //this->worldGen->blockAt(WorldPos(this->thisChunkPos, thisBlockPos.x, thisBlockPos.y, thisBlockPos.z));

	// Based on the offset/normal, draw the desired face
	if (normal.x < 0) {
		this->drawLeftFace(thisBlockPos, thisBlock);
	} else if (normal.x > 0) {
		this->drawRightFace(thisBlockPos, thisBlock);
	} else if (normal.y > 0) {
		this->drawTopFace(thisBlockPos, thisBlock);
	} else if (normal.y < 0) {
		this->drawBottomFace(thisBlockPos, thisBlock);
	} else if (normal.z > 0) {
		this->drawBackFace(thisBlockPos, thisBlock);
	} else if (normal.z < 0) {
		this->drawFrontFace(thisBlockPos, thisBlock);
	}

    // Need to push the vertices in clockwise order, no matter what face.
    return *this;
}

/* @brief Draw the cubes for the chunk
* @param[in] chunkData A reference to the chunk data
*/
Chunk& Chunk::drawCubes(ChunkData const& chunkData) {
	for (int16_t z=0;z<CHUNK_SIZE;z++) {
        for (int16_t y=0;y<CHUNK_SIZE;y++) {
            for (int16_t x=0;x<CHUNK_SIZE;x++) {
				this->drawFace(ChunkSubPos(x, y, z), ChunkSubPos(x + 1, y, z), chunkData);
				this->drawFace(ChunkSubPos(x, y, z), ChunkSubPos(x - 1, y, z), chunkData);

				this->drawFace(ChunkSubPos(x, y, z), ChunkSubPos(x, y + 1, z), chunkData);
				this->drawFace(ChunkSubPos(x, y, z), ChunkSubPos(x, y - 1, z), chunkData);

				this->drawFace(ChunkSubPos(x, y, z), ChunkSubPos(x, y, z + 1), chunkData);
				this->drawFace(ChunkSubPos(x, y, z), ChunkSubPos(x, y, z - 1), chunkData);
            }
        }
	}

	this->updateVAO(true, true, true);
    return *this;
}

/* @brief Push the vertices to draw the left face in clockwise order
* @param[in] pos	The position of this chunk block
* @param[in] block	The block type to draw
* @return          A reference to this chunk object
*/
Chunk& Chunk::drawLeftFace(ChunkSubPos const& pos, BlockType const& block) { // blue
	glm::vec3 fragA = (pos + ChunkSubPos(0, 0, 1)).toLocal();
	glm::vec3 fragB = (pos + ChunkSubPos(0, 1, 0)).toLocal();
	glm::vec3 normal = glm::vec3(-1.0, 0.0, 0.0);

	this->pushPoint(fragA, normal, {0.25, 0.25}, block);
	this->pushPoint({fragA.x, fragB.y, fragA.z}, normal, {0, 0.25}, block);
	this->pushPoint(fragB, normal, {0, 0.5}, block);

	this->pushPoint(fragB, normal, {0, 0.5}, block);
	this->pushPoint({fragA.x, fragA.y, fragB.z}, normal, {0.25, 0.5}, block);
	this->pushPoint(fragA, normal, {0.25, 0.25}, block);

	return *this;
}

/* @brief Push the vertices to draw the right face in clockwise order
* @param[in] pos	The position of this chunk block
* @param[in] block	The block type to draw
* @return          A reference to this chunk object
*/
Chunk& Chunk::drawRightFace(ChunkSubPos const& pos, BlockType const& block) { // yellow
	glm::vec3 fragA = (pos + ChunkSubPos(1, 0, 0)).toLocal();
	glm::vec3 fragB = (pos + ChunkSubPos(1, 1, 1)).toLocal();
	glm::vec3 normal = glm::vec3(1.0, 0.0, 0.0);

	this->pushPoint(fragA, normal, {0.5, 0.5}, block);
	this->pushPoint({fragA.x, fragB.y, fragA.z}, normal, {0.75, 0.5}, block);
	this->pushPoint(fragB, normal, {0.75, 0.25}, block);

	this->pushPoint(fragB, normal, {0.75, 0.25}, block);
	this->pushPoint({fragA.x, fragA.y, fragB.z}, normal, {0.5, 0.25}, block);
	this->pushPoint(fragA, normal, {0.5, 0.5}, block);

	return *this;
}

/* @brief Push the vertices to draw the top face in clockwise order
* @param[in] pos	The position of this chunk block
* @param[in] block	The block type to draw
* @return          A reference to this chunk object
*/
Chunk& Chunk::drawTopFace(ChunkSubPos const& pos, BlockType const& block) { // red
	glm::vec3 fragA = (pos + ChunkSubPos(0, 1, 0)).toLocal();
	glm::vec3 fragB = (pos + ChunkSubPos(1, 1, 1)).toLocal(); // fragB can be moved around if we want to in theory.. so we can search greedily to save vertices
	glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);

	this->pushPoint(fragA, normal, {0.25, 0.75}, block);
	this->pushPoint({fragA.x, fragA.y, fragB.z}, normal, {0.25, 1}, block);
	this->pushPoint(fragB, normal, {0.5, 1}, block);

	this->pushPoint(fragB, normal, {0.5, 1}, block);
	this->pushPoint({fragB.x, fragA.y, fragA.z}, normal, {0.5, 0.75}, block);
	this->pushPoint(fragA, normal, {0.25, 0.75}, block);

	return *this;
}

/* @brief Push the vertices to draw the bottom face in clockwise order
* @param[in] pos	The position of this chunk block
* @param[in] block	The block type to draw
* @return          A reference to this chunk object
*/
Chunk& Chunk::drawBottomFace(ChunkSubPos const& pos, BlockType const& block) { // orange
	glm::vec3 fragA = (pos + ChunkSubPos(0, 0, 1)).toLocal();
	glm::vec3 fragB = (pos + ChunkSubPos(1, 0, 0)).toLocal();
	glm::vec3 normal = glm::vec3(0.0, -1.0, 0.0);

	this->pushPoint(fragA, normal, {0.25, 0.25}, block);
	this->pushPoint({fragA.x, fragA.y, fragB.z}, normal, {0.25, 0.5}, block);
	this->pushPoint(fragB, normal, {0.5, 0.5}, block);

	this->pushPoint(fragB, normal, {0.5, 0.5}, block);
	this->pushPoint({fragB.x, fragA.y, fragA.z}, normal, {0.5, 0.25}, block);
	this->pushPoint(fragA, normal, {0.25, 0.25}, block);

	return *this;
}

/* @brief Push the vertices to draw the front face in clockwise order
* @param[in] pos	The position of this chunk block
* @param[in] block	The block type to draw
* @return          A reference to this chunk object
*/
Chunk& Chunk::drawFrontFace(ChunkSubPos const& pos, BlockType const& block) { // green
	glm::vec3 fragA = (pos + ChunkSubPos(0, 0, 0)).toLocal();
	glm::vec3 fragB = (pos + ChunkSubPos(1, 1, 0)).toLocal();
	glm::vec3 normal = glm::vec3(0.0, 0.0, -1.0);

	this->pushPoint(fragA, normal, {0.25, 0.5}, block);
	this->pushPoint({fragA.x, fragB.y, fragA.z}, normal, {0.25, 0.75}, block);
	this->pushPoint(fragB, normal, {0.5, 0.75}, block);

	this->pushPoint(fragB, normal, {0.5, 0.75}, block);
	this->pushPoint({fragB.x, fragA.y, fragA.z}, normal, {0.5, 0.5}, block);
	this->pushPoint(fragA, normal, {0.25, 0.5}, block);

	return *this;
}

/* @brief Push the vertices to draw the back face in clockwise order
* @param[in] pos	The position of this chunk block
* @param[in] block	The block type to draw
* @return          A reference to this chunk object
*/
Chunk& Chunk::drawBackFace(ChunkSubPos const& pos, BlockType const& block) { // purple
	glm::vec3 fragA = (pos + ChunkSubPos(1, 0, 1)).toLocal();
	glm::vec3 fragB = (pos + ChunkSubPos(0, 1, 1)).toLocal();
	glm::vec3 normal = glm::vec3(0.0, 0.0, 1.0);

	this->pushPoint(fragA, normal, {0.25, 0.25}, block);
	this->pushPoint({fragA.x, fragB.y, fragA.z}, normal, {0.25, 0}, block);
	this->pushPoint(fragB, normal, {0.5, 0}, block);

	this->pushPoint(fragB, normal, {0.5, 0}, block);
	this->pushPoint({fragB.x, fragA.y, fragA.z}, normal, {0.5, 0.25}, block);
	this->pushPoint(fragA, normal, {0.25, 0.25}, block);

	return *this;
}
