#include "worldgen.h"

#include <cmath>
#include <iostream>

/* @brief Create a new worldgen object, with a new optional seed.
 * @param[in] worldSeed	The world seed. If not provided, then it is equal to time(NULL)
*/
WorldGen::WorldGen(uint64_t worldSeed) : seed(worldSeed) {}

/* @brief Get the world seed
 * @return The world seed
*/
uint64_t const& WorldGen::getSeed() {
    return this->seed;
}

/* @brief Generate a chunk and populate chunkData.
*/
WorldGen& WorldGen::generateChunk(ChunkPos const& chunkAt, ChunkData* dataOut) {
    if (dataOut == nullptr) {
        return *this;
    }

    for (uint64_t z = 0; z<CHUNK_SIZE; z++) {
        for (uint64_t y = 0; y<CHUNK_SIZE; y++) {
            for (uint64_t x = 0; x<CHUNK_SIZE; x++) {
                dataOut->set(x, y, z, this->blockAt(WorldPos(chunkAt, x, y, z)));
            }
        }
    }

    return *this;
}

/* @brief Get the blocktype at some world position
 * @param[in] blockPos	The block position
 * @return				The block type of the block at that position
*/
BlockType WorldGen::blockAt(WorldPos const& blockPos, bool doRecurse) {
	BlockType blockRes = BlockType::AIR;

	int64_t absx = blockPos.absX();
	int64_t absy = blockPos.absY();
	int64_t absz = blockPos.absZ();

	double stonefunc = (std::sinf(absx / 10.0) + std::cosf(absz / 10.0)) * 5.0 + 0.0;
	double function2 = (std::sinf(absx / 3.0) + std::cosf(absz / 3.0)) * 5.0 + 0.0;
	double function3 = (std::sinf(absx / 20.0) + std::cosf(absz / 20.0)) * 7.0 + 0.0;

	double func3d = (std::sinf(absx / 2.0) + std::cosf(absz / 2.0) + std::sinf(absy / 2.0));

	// Ground
	if (absy < stonefunc) {
		blockRes = BlockType::STONE;
		//	} else if (absy < function2) {
		//blockRes = BlockType::DIRT;
	} else if (func3d > 2.5) {
		blockRes = BlockType::GRASS;

		// If a grass block has a block on top, set it to dirt.
		if (doRecurse) {
			BlockType type = this->blockAt(WorldPos(blockPos.getChunkPos(), blockPos.getX(), blockPos.getY() + 1, blockPos.getZ()), false);
			if (type != BlockType::AIR) {
				blockRes = BlockType::DIRT;
			}
		}
	}

	// Extra stuff
	if (absy == 20 && absx == 20 && blockPos.absZ() == 20) {
		blockRes = BlockType::STONE;
	}

	return blockRes;
}
