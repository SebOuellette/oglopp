#ifndef CHUNK_H
#define CHUNK_H

#include <oglopp/shape.h>
#include <iostream>

#include "defines.h"
#include "chunkdata.h"
#include "worldpos.h"
#include "worldgen.h"

class ChunkSubPos {
public:
    int16_t x;
    int16_t y;
    int16_t z;

	/* @brief Create a new chunk sub pos object
	 * @param[in] newX	The x position of the chunk block
	 * @param[in] newY	The y position of the chunk block
	 * @param[in] newZ	The z position of the chunk block
	*/
	ChunkSubPos(int16_t newX = 0, int16_t newY = 0, int16_t newZ = 0);

    /* @brief Subtract one chunk position from another
    * @param[in] posB  The position to subtract from the position stored in this object
    * @return          The difference as a new object
    */
    ChunkSubPos operator-(ChunkSubPos const& posB) const;

 	/* @brief Add one chunk position to another
    * @param[in] posB  The position to add to the position stored in this object
    * @return          The sum as a new object
    */
	ChunkSubPos operator+(ChunkSubPos const& posB) const;

    /* @brief Convert the chunk sub pos into a floating point local pos, where 0 -> 0.0 and CHUNK_SIZE -> 1.0
     * @return The local position of this chunkSubPos used for vertices
    */
    glm::vec3 toLocal();

	/* @brief True if this chunk sub pos is within a chunk, false otherwise
	 * @return 	true if within the chunk, false otherwise
	*/
	bool isInRange() const;
};

class Chunk : public oglopp::Shape {
public:
    /* @brief Create a new chunk mesh based on a 3d array of chunk blocks
     * @param[in] chunkData     A 3d array of chunk data. Indexed as [z][y][x], plus the surrounding squares from the next chunks. (1 on each side)
    */
    Chunk(ChunkData const& chunkData);

    /* @brief The chunk position. Will set the opengl shape position as well as generate with WorldGen
     * @param[in] chunkPos	The position of the chunk
     * @param[in] worldGen	A reference to the world generation object
    */
    Chunk(ChunkPos const& chunkPos, WorldGen& worldGen);

    /* @brief Get a reference to the saved chunk data
     * @return the chunk data
    */
    ChunkData const& getChunkData();

    /* @brief Get this chunk position
     * @return A constant reference to this chunk position
    */
    ChunkPos const& getChunkPos();

private:
	ChunkPos thisChunkPos;
	WorldGen* worldGen;
	ChunkData savedChunkData;

    /* @brief Check if a face should be drawn between thisBlockPos and compareBlockPos, given the chunk data chunkData
     * @param[in] thisBlockPos      The position of the current block. This will be the "inner" face
     * @param[in] compareBlockPos   The position of the next block. This will be the "outer" face (draw in clockwise)
     * @return                      True if a face should be drawn. False if a face should not be drawn and/or one of the positions is out of range.
     */
    const bool shouldDrawFace(ChunkSubPos const& thisBlockPos, ChunkSubPos const& compareBlockPos, ChunkData const& chunkData);

    /* @brief Draw a face between Block A and block B. Check if the face should be drawn first.
     * @param[in] thisBlockPos      The position of the current block. This will be the "inner" face
     * @param[in] compareBlockPos   The position of the next block. This will be the "outer" face (draw in clockwise)
     * @return                      A reference to this chunk object
     */
    Chunk& drawFace(ChunkSubPos const& thisBlockPos, ChunkSubPos const& compareBlockPos, ChunkData const& chunkData);

    /* @brief Draw the cubes for the chunk
    * @param[in] chunkData A reference to the chunk data
    */
    Chunk& drawCubes(ChunkData const& chunkData);


    /* @brief Push the vertices to draw the left face in clockwise order
     * @param[in] pos	The position of this chunk block
     * @param[in] block	The block type to draw
     * @return          A reference to this chunk object
    */
    Chunk& drawLeftFace(ChunkSubPos const& pos, BlockType const& block);

	/* @brief Push the vertices to draw the right face in clockwise order
     * @param[in] pos	The position of this chunk block
     * @param[in] block	The block type to draw
     * @return          A reference to this chunk object
    */
    Chunk& drawRightFace(ChunkSubPos const& pos, BlockType const& block);

	/* @brief Push the vertices to draw the top face in clockwise order
     * @param[in] pos	The position of this chunk block
     * @param[in] block	The block type to draw
     * @return          A reference to this chunk object
    */
    Chunk& drawTopFace(ChunkSubPos const& pos, BlockType const& block);

	/* @brief Push the vertices to draw the bottom face in clockwise order
     * @param[in] pos	The position of this chunk block
     * @param[in] block	The block type to draw
     * @return          A reference to this chunk object
    */
    Chunk& drawBottomFace(ChunkSubPos const& pos, BlockType const& block);

	/* @brief Push the vertices to draw the front face in clockwise order
     * @param[in] pos	The position of this chunk block
     * @param[in] block	The block type to draw
     * @return          A reference to this chunk object
    */
    Chunk& drawFrontFace(ChunkSubPos const& pos, BlockType const& block);

	/* @brief Push the vertices to draw the back face in clockwise order
     * @param[in] pos	The position of this chunk block
     * @param[in] block	The block type to draw
     * @return          A reference to this chunk object
    */
    Chunk& drawBackFace(ChunkSubPos const& pos, BlockType const& block);
};

#endif
