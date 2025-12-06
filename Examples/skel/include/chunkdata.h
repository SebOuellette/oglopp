#ifndef CHUNKDATA_H
#define CHUNKDATA_H

#include <cstdint>
#include <array>

#include "defines.h"

enum BlockType : uint16_t {
    AIR 	= 0x00,
    STONE	= 0x01,
    DIRT	= 0x02,
    GRASS	= 0x03,
    WATER	= 0x04,
    GLASS	= 0x05,
    WOOD	= 0x06,
    LEAVES	= 0x07,

    LAST_BLOCK // Stores the highest block value that's currently implemented
};

class ChunkData {
private:
	BlockType rawData[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

	/* @brief Convert X, Y, Z to an index
	 * @param[in] x	The x position
	 * @param[in] y The y position
	 * @param[in] z	The z position
	 * @return		The converted index
	*/
	static uint64_t xyzToIndex(uint64_t x, uint64_t y, uint64_t z);

public:
	/* @brief Default constructor. Fill with air
	*/
	ChunkData();

	/* @brief Create a new chunkdata object
	 * @param[in] inData	The input data as an initializer list. Indexed as [z],[y],[x]
	*/
	ChunkData(std::array<std::array<std::array<uint16_t, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> inData);

	/* @brief Lookup the block type at some X Y Z coordiante
	 * @param[in] x	The x position to lookup
	 * @param[in] y	The y position to lookup
	 * @param[in] z	The z position to lookup
	 * @return		the block type found at the position.
	*/
	BlockType at(uint64_t x, uint64_t y, uint64_t z) const;

	/* @brief Lookup the block type at some X Y Z coordiante
	 * @param[in] x			The x position to lookup
	 * @param[in] y			The y position to lookup
	 * @param[in] z			The z position to lookup
	 * @param[in] newBlock	The new block to set at the position
	 * @return		the block type found at the position.
	*/
	ChunkData& set(uint64_t x, uint64_t y, uint64_t z, BlockType newBlock);

	/* @brief Get the raw buffer. Update the pointer to be equal to a pointer to the current blocks
	 * @param[out] outBlocks	This pointer will be set to a pointer to the raw data in this chunk data object
	 * @return					A reference to this chunk data object
	*/
	ChunkData& getRawBuffer(BlockType* outBlocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE]);
};

//typedef std::array<std::array<std::array<uint16_t, CHUNK_SIZE + 2>, CHUNK_SIZE + 2>, CHUNK_SIZE + 2> ChunkData;

#endif
