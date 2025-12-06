#ifndef WORLDPOS_H
#define WORLDPOS_H

#include <cstdint>


class ChunkPos {
public:
    /* @brief Create a new chunk position
     * @param[in] x The x position of the chunk
     * @param[in] y The y position of the chunk
     * @param[in] z The z position of the chunk
    */
    ChunkPos(int64_t x = 0, int64_t y = 0, int64_t z = 0);

    /* @brief Get the (X/Y/Z) position of the chunk
     * @return  The position coordinate of the chunk
    */
    int64_t getX() const;
    int64_t getY() const;
    int64_t getZ() const;

    /* @brief Set the (X/Y/Z) position of the chunk
     * @return  The new position coordinate of the chunk
    */
    ChunkPos& setX(int64_t newX);
    ChunkPos& setY(int64_t newY);
    ChunkPos& setZ(int64_t newZ);

    /* @brief Assign this chunk pos to some other chunk position
     * @param[in] newPos    The new chunk position
     * @return              A reference to this chunk position
    */
    ChunkPos& operator=(ChunkPos const& newPos);

    /* @brief Add some other chunk position to this chunk position
     * @param[in] posB	The position to add to this one
	 * @return			A new chunk position with the sum
    */
    ChunkPos operator+(ChunkPos const& posB) const;

	/* @brief Subtract some other chunk position from this chunk position
     * @param[in] posB	The position to subtract from this one
	 * @return			A new chunk position with the difference
    */
    ChunkPos operator-(ChunkPos const& posB) const;

    /* @brief Multiply by a sclalar
     * @param[in] scalar	The scalar to multiply by
     * @return				The product
    */
    ChunkPos operator*(int64_t const& scalar) const;

    /* Compare two chunk positions
     * @param[in] posB	The position to compare
	 * @return			True of equal, false otherwise
   	*/
    bool operator==(ChunkPos const& posB) const;

    /* Compare two chunk positions
     * @param[in] posB	The position to compare
	 * @return			True of greater, false otherwise
   	*/
    bool operator>(ChunkPos const& posB) const;

    /* Compare two chunk positions
    * @param[in] posB	The position to compare
	 * @return			True of less, false otherwise
   	*/
    bool operator<(ChunkPos const& posB) const;

    /* @brief Compare two chunk positions
     * @param[in] posB	The position to compare
     * @return			True if not equal, false if equal
    */
    bool operator!=(ChunkPos const& posB) const;

private:
    int64_t chunkX;
    int64_t chunkY;
    int64_t chunkZ;
};


class WorldPos {
public:
	/* @brief Create a world position for a block. Initialize the chunk position to 0
	 * @param[in] blockX		The X position of the block or sub-block relative to the origin of the chunk
	 * @param[in] blockY		The Y position of the block or sub-block relative to the origin of the chunk
	 * @param[in] blockZ		The Z position of the block or sub-block relative to the origin of the chunk
	*/
	WorldPos(int64_t blockX = 0, int64_t blockY = 0, int64_t blockZ = 0);

	/* @brief Create a world position for a block within a chunk. They're separated for precision and extremely high values. Huuuuugue ass worlds. Huuuuuge ass chunks.
	 * @param[in] newChunkPos	The chunk position
	 * @param[in] blockX		The X position of the block or sub-block relative to the origin of the chunk
	 * @param[in] blockY		The Y position of the block or sub-block relative to the origin of the chunk
	 * @param[in] blockZ		The Z position of the block or sub-block relative to the origin of the chunk
	*/
    WorldPos(ChunkPos const& newChunkPos, int64_t blockX = 0, int64_t blockY = 0, int64_t blockZ = 0);

    /* @brief Get a reference to the chunk position
     * @return	A reference to the chunk position object
    */
    ChunkPos const& getChunkPos() const;

    /* @brief Set the chunk position
     * @param[in] newPos	The new chunk position to set to
     * @return				A reference to this world position object
    */
    WorldPos& setChunkPos(ChunkPos const& newPos);

    /* @brief Get the (X/Y/Z) position of the block
     * @return  The position coordinate of the block
    */
    int64_t getX() const;
    int64_t getY() const;
    int64_t getZ() const;

    /* @brief Get the absolute position of some coordinate (combined chunk and sub_chunk)
     * @return	The combined value. Currently a double, should be switched to some kind of bignum to take advantage of the full int64 size for chunks
    */
    int64_t absX() const;
    int64_t absY() const;
    int64_t absZ() const;


    /* @brief Set the (X/Y/Z) position of the block
     * @return  The new position coordinate of the block
    */
    WorldPos& setX(int64_t newX);
    WorldPos& setY(int64_t newY);
    WorldPos& setZ(int64_t newZ);

private:
    // Chunk Position
    ChunkPos chunkPos;

    // Position within the chunk.
    int64_t subX;
    int64_t subY;
    int64_t subZ;
};


#endif
