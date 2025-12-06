#include "worldpos.h"
#include "defines.h"
#include <cmath>

/* @brief Create a new chunk position
* @param[in] x The x position of the chunk
* @param[in] y The y position of the chunk
* @param[in] z The z position of the chunk
*/
ChunkPos::ChunkPos(int64_t x, int64_t y, int64_t z) {
    this->setX(x).setY(y).setZ(z);
}

/* @brief Get the (X/Y/Z) position of the chunk
* @return  The position coordinate of the chunk
*/
int64_t ChunkPos::getX() const {
    return this->chunkX;
}

int64_t ChunkPos::getY() const {
    return this->chunkY;
}

int64_t ChunkPos::getZ() const {
    return this->chunkZ;
}

/* @brief Set the (X/Y/Z) position of the chunk
* @return  The new position coordinate of the chunk
*/
ChunkPos& ChunkPos::setX(int64_t newX) {
    this->chunkX = newX;

    return *this;
}

ChunkPos& ChunkPos::setY(int64_t newY) {
    this->chunkY = newY;

    return *this;
}

ChunkPos& ChunkPos::setZ(int64_t newZ) {
    this->chunkZ = newZ;

    return *this;
}

/* @brief Assign this chunk pos to some other chunk position
* @param[in] newPos    The new chunk position
* @return              A reference to this chunk position
*/
ChunkPos& ChunkPos::operator=(ChunkPos const& newPos) {
    this->chunkX = newPos.chunkX;
    this->chunkY = newPos.chunkY;
    this->chunkZ = newPos.chunkZ;

    return *this;
}

/* @brief Add some other chunk position to this chunk position
* @param[in] posB	The position to add to this one
* @return			A new chunk position with the sum
*/
ChunkPos ChunkPos::operator+(ChunkPos const& posB) const {
    ChunkPos result = *this;

    result.chunkX += posB.chunkX;
    result.chunkY += posB.chunkY;
    result.chunkZ += posB.chunkZ;

    return result;
}

/* @brief Subtract some other chunk position from this chunk position
* @param[in] posB	The position to subtract from this one
* @return			A new chunk position with the difference
*/
ChunkPos ChunkPos::operator-(ChunkPos const& posB) const {
    ChunkPos result = *this;

    result.setX(result.getX() - posB.getX());
    result.setY(result.getY() - posB.getY());
    result.setZ(result.getZ() - posB.getZ());

    return result;
}

/* @brief Multiply by a sclalar
 * @param[in] scalar	The scalar to multiply by
 * @return				The product
*/
ChunkPos ChunkPos::operator*(int64_t const& scalar) const {
	ChunkPos result = *this;

    result.setX(result.getX() * scalar);
    result.setY(result.getY() * scalar);
    result.setZ(result.getZ() * scalar);

    return result;
}

/* Compare two chunk positions
 * @param[in] posB	The position to subtract from this one
 * @return			A new chunk position with the difference
*/
bool ChunkPos::operator==(ChunkPos const& posB) const {
	return this->chunkX == posB.chunkX && this->chunkY == posB.chunkY && this->chunkZ == posB.chunkZ;
}

/* Compare two chunk positions
 * @param[in] posB	The position to compare
 * @return			True of greater, false otherwise
*/
bool ChunkPos::operator>(ChunkPos const& posB) const {
	if (this->chunkX == posB.chunkX) {
		if (this->chunkY == posB.chunkY) {
			if (this->chunkZ == posB.chunkZ) {
				return false;
			}

			return this->chunkZ > posB.chunkZ;
		}

		return this->chunkY > posB.chunkY;
	}

	return this->chunkX > posB.chunkX;
}

/* Compare two chunk positions
 * @param[in] posB	The position to compare
 * @return			True of less, false otherwise
*/
bool ChunkPos::operator<(ChunkPos const& posB) const {
	if (this->chunkX == posB.chunkX) {
		if (this->chunkY == posB.chunkY) {
			if (this->chunkZ == posB.chunkZ) {
				return false;
			}

			return this->chunkZ < posB.chunkZ;
		}

		return this->chunkY < posB.chunkY;
	}

	return this->chunkX < posB.chunkX;
}

/* @brief Compare two chunk positions
 * @param[in] posB	The position to compare
 * @return			True if not equal, false if equal
*/
bool ChunkPos::operator!=(ChunkPos const& posB) const {
	return this->chunkX != posB.chunkX || this->chunkY != posB.chunkY || this->chunkZ != posB.chunkZ;
}

/* @brief Create a world position for a block. Initialize the chunk position to 0
 * @param[in] blockX		The X position of the block or sub-block relative to the origin of the chunk
 * @param[in] blockY		The Y position of the block or sub-block relative to the origin of the chunk
 * @param[in] blockZ		The Z position of the block or sub-block relative to the origin of the chunk
*/
WorldPos::WorldPos(int64_t blockX, int64_t blockY, int64_t blockZ) {
	this->setChunkPos(ChunkPos(0, 0, 0)).setX(blockX).setY(blockY).setZ(blockZ);
}

/* @brief Create a world position for a block within a chunk. They're separated for precision and extremely high values. Huuuuugue ass worlds. Huuuuuge ass chunks.
 * @param[in] newChunkPos	The chunk position
 * @param[in] blockX		The X position of the block or sub-block relative to the origin of the chunk
 * @param[in] blockY		The Y position of the block or sub-block relative to the origin of the chunk
 * @param[in] blockZ		The Z position of the block or sub-block relative to the origin of the chunk
*/
WorldPos::WorldPos(ChunkPos const& newChunkPos, int64_t blockX, int64_t blockY, int64_t blockZ) {
	this->setChunkPos(newChunkPos).setX(blockX).setY(blockY).setZ(blockZ);
}


/* @brief Get a reference to the chunk position
 * @return	A reference to the chunk position object
*/
ChunkPos const& WorldPos::getChunkPos() const {
	return this->chunkPos;
}

/* @brief Set the chunk position
 * @param[in] newPos	The new chunk position to set to
 * @return				A reference to this world position object
*/
WorldPos& WorldPos::setChunkPos(ChunkPos const& newPos) {
	this->chunkPos = newPos;

	return *this;
}

/* @brief Get the (X/Y/Z) position of the block
 * @return  The position coordinate of the block
*/
int64_t WorldPos::getX() const {
	return this->subX;
}
int64_t WorldPos::getY() const {
	return this->subY;
}
int64_t WorldPos::getZ() const {
	return this->subZ;
}

/* @brief Get the absolute position of some coordinate (combined chunk and sub_chunk)
 * @return	The combined value. Currently a double, should be switched to some kind of bignum to take advantage of the full int64 size for chunks
*/
int64_t WorldPos::absX() const {
	return this->chunkPos.getX() * CHUNK_SIZE + this->subX;
}

int64_t WorldPos::absY() const {
	return this->chunkPos.getY() * CHUNK_SIZE + this->subY;
}

int64_t WorldPos::absZ() const {
	return this->chunkPos.getZ() * CHUNK_SIZE + this->subZ;
}


/* @brief Set the (X/Y/Z) position of the block
 * @return  The new position coordinate of the block
*/
WorldPos& WorldPos::setX(int64_t newX) {
	this->subX = (newX + CHUNK_SIZE) % CHUNK_SIZE;
	int64_t chunkXOffset = newX / CHUNK_SIZE - ((newX < 0) ? 1 : 0);
	this->chunkPos.setX(this->chunkPos.getX() + chunkXOffset);
	return *this;
}
WorldPos& WorldPos::setY(int64_t newY) {
	this->subY = (newY + CHUNK_SIZE) % CHUNK_SIZE;
	int64_t chunkYOffset = newY / CHUNK_SIZE - ((newY < 0) ? 1 : 0);
	this->chunkPos.setY(this->chunkPos.getY() + chunkYOffset);
	return *this;
}
WorldPos& WorldPos::setZ(int64_t newZ) {
	this->subZ = (newZ + CHUNK_SIZE) % CHUNK_SIZE;
	int64_t chunkZOffset = newZ / CHUNK_SIZE - ((newZ < 0) ? 1 : 0);
	this->chunkPos.setZ(this->chunkPos.getZ() + chunkZOffset);
	return *this;
}
