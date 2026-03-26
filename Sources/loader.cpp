#include "oglopp/obj/loader.h"

namespace oglopp {
	/**
	 * @brief Create a new point object using their indices in the component lists
	 * @param[in] vertIdx	The vertex component index
	 * @param[in] texIdx	The texture component index
	 * @param[in] normIdx	The normal component index
	 * @param[in] optIdx	The option component index
	 */
	Point ObjLoader::buildPoint(int64_t vertIdx, int64_t texIdx, int64_t normIdx, int64_t optIdx) {
		Point point;


		// Verts
		if (vertIdx >= 0 && vertIdx < cVertices.size()) {
			vert = cVertices[vertIdx];
		}

		// Texture coords
		if (texIdx >= 0 && texIdx < cTexCoords.size()) {
			tex = cTexCoords[texIdx];
		}

		// Normal
		if (normIdx >= 0 && normIdx < cNormals.size()) {
			normal = cNormals[normIdx];
		}

		// Option index
		if (optIdx >= 0 && optIdx < cOptions.size()) {
			opt = cOptions[optIdx];
		}

		return point;
	}
}
