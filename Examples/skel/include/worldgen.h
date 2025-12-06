#ifndef WORLDGEN_H
#define WORLDGEN_H

#include <ctime>

#include "chunkdata.h"
#include "worldpos.h"

/* @brief Generate chunk data for a world.
*/
class WorldGen {
public:
    /* @brief Create a new worldgen object, with a new optional seed.
     * @param[in] worldSeed	The world seed. If not provided, then it is equal to time(NULL)
    */
    WorldGen(uint64_t worldSeed = time(NULL));

    /* @brief Get the world seed
     * @return The world seed
    */
    uint64_t const& getSeed();

	/* @brief Generate a chunk and populate chunkData.
	*/
	WorldGen& generateChunk(ChunkPos const& chunkAt, ChunkData* dataOut);

	/* @brief Get the blocktype at some world position
	 * @param[in] blockPos	The block position
	 * @return				The block type of the block at that position
	*/
	BlockType blockAt(WorldPos const& blockPos, bool doRecurse = true);

private:
    const uint64_t seed;
};

#endif
