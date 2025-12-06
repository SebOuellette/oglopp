#include "chunkdata.h"
#include <cstring>


/* @brief Convert X, Y, Z to an index
 * @param[in] x	The x position
 * @param[in] y The y position
 * @param[in] z	The z position
 * @return		The converted index
*/
uint64_t ChunkData::xyzToIndex(uint64_t x, uint64_t y, uint64_t z) {
	return x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z;
}

/* @brief Default constructor. Fill with air
*/
ChunkData::ChunkData() {
	std::memset(this->rawData, 0, sizeof(this->rawData));
}

/* @brief Create a new chunkdata object
 * @param[in] inData	The input data as an initializer list. Indexed as [z],[y],[x]
*/
ChunkData::ChunkData(std::array<std::array<std::array<uint16_t, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> inData) {
	uint16_t blockType = 0;

	for (uint64_t x=0;x<CHUNK_SIZE;x++) {
		for (uint64_t y=0;y<CHUNK_SIZE;y++) {
			for (uint64_t z=0;z<CHUNK_SIZE;z++) {
				blockType = inData[z][y][x];

				this->rawData[ChunkData::xyzToIndex(x, y, z)] = (blockType > BlockType::LAST_BLOCK) ? BlockType::AIR : static_cast<BlockType>(blockType);
			}
		}
	}
}

/* @brief Lookup the block type at some X Y Z coordiante
 * @param[in] x	The x position to lookup
 * @param[in] y	The y position to lookup
 * @param[in] z	The z position to lookup
 * @return		the block type found at the position.
*/
BlockType ChunkData::at(uint64_t x, uint64_t y, uint64_t z) const {
	return this->rawData[ChunkData::xyzToIndex(x, y, z)];
}

/* @brief Lookup the block type at some X Y Z coordiante
 * @param[in] x			The x position to lookup
 * @param[in] y			The y position to lookup
 * @param[in] z			The z position to lookup
 * @param[in] newBlock	The new block to set at the position
 * @return		the block type found at the position.
*/
ChunkData& ChunkData::set(uint64_t x, uint64_t y, uint64_t z, BlockType newBlock) {
	this->rawData[ChunkData::xyzToIndex(x, y, z)] = newBlock;

	return *this;
}

/* @brief Get the raw buffer. Update the pointer to be equal to a pointer to the current blocks
 * @param[out] outBlocks	This pointer will be set to a pointer to the raw data in this chunk data object
 * @return					A reference to this chunk data object
*/
ChunkData& ChunkData::getRawBuffer(BlockType* outBlocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE]) {
	*outBlocks = this->rawData;

	return *this;
}
