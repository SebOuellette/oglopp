#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iostream>

#include "oglopp/obj/loader.h"

//#define VERBOSE

namespace oglopp {
	/**
	 * @brief Push a set of indices. If negative, create a new 0 element.
	 * @param[in] vbo	The VBO to push vertices to
	 * @param[in] vertIndex	Vertex index
	 * @param[in] texIndex	Texture index
	 * @param[in] normIndex	Normal index
	 * @return 		A reference to this Loader object
	 */
	Loader& Loader::pushFace(VBO& vbo, int64_t vertIndex, int64_t texIndex, int64_t normIndex) {
		// If the index is in the list, copy it and push
		//  Otherwise, push 0,0,0
		vbo.push(
			(vertIndex > 0 && vertIndex <= this->cVertices.size()) ? 
				this->cVertices[vertIndex - 1] : 
				CVertex(0, 0, 0),

			(normIndex > 0 && normIndex <= this->cNormals.size()) ? 
				this->cNormals[normIndex - 1] : 
				CNormal(0, 0, 0),

			(texIndex > 0 && texIndex <= this->cTexCoords.size()) ?
				this->cTexCoords[texIndex - 1] : 
				CTexCoord(0, 0)
		);

#ifdef VERBOSE
		std::cout << vertIndex << "/" << this->cVertices.size() << "," << texIndex << "/" << this->cTexCoords.size() << "," << normIndex << "/" << this->cNormals.size() << "] ";
#endif

		return *this;
	}

	/**
	 * @brief Push a triangle of indices to the EBO
	 * @param[in] ebo	The EBO to push indices to
	 * @param[in] start	The start index of the triangle fan
	 * @param[in] offset	The number of points so far in this frame
	 */
	Loader& Loader::pushTriangleIndices(EBO& ebo, uint32_t start, uint32_t offset) {
		if (offset < 3) return *this;

		ebo.push(
			start, 
			start + offset - 2, 
			start + offset - 1);
#ifdef VERBOSE
		std::cout << "<" << 
			start << ", " << 
			start + offset - 2 << ", " << 
			start + offset - 1 << "> ";
#endif

		return *this; 
	}


	Loader::Loader() {}
	
	/**
	 * @brief Construct a new shape from an obj file
	 * @param[in] filename	The path of the obj file
	 * @param[in] vao	The VAO object to build
	 * @return A reference to this objloader object
	 */
	Loader& Loader::construct(std::string const& filename, VAO& vao) {
		// Push the file
		std::ifstream file(filename);
		
		// Read every line
		std::string line = "";
		std::string type = "";
		std::stringstream stream;

		// Temp
		float x, y, z;
		int64_t indices[3] = {0, 0, 0};
		uint32_t indexItem; 
		std::string tmp;

		uint32_t totalPoints = 0;
		uint32_t facePointCnt = 0; // 0-n

		while (file.good()) {
			file >> type;
			file.get();
#ifdef VERBOSE
			std::cout << "[" << type << "] [";
#endif
			if (!file.good()) break;
			
			x = y = z = 0;

			if ("v" == type) {
				// Read and parse a vertex
				file >> x >> y >> z;
				this->cVertices.push_back(CVertex(x, y, z));

#ifdef VERBOSE
				std::cout << x << "," << y << "," << z;
#endif
			} else if ("vt" == type) {
				// Read and parse a texture coordinate
				file >> x >> y;
				this->cTexCoords.push_back(CTexCoord(x, y));

#ifdef VERBOSE
				std::cout << x << "," << y;
#endif
			} else if ("vn" == type) {
				// Read and parse a normal vector
				file >> x >> y >> z;
				this->cNormals.push_back(CNormal(x, y, z));
#ifdef VERBOSE
				std::cout << x << "," << y << "," << z;
#endif
			} else if ("f" == type) {
				totalPoints += facePointCnt; 
				facePointCnt = 0;
				
				// Read and parse a face
				//  (set of points)
				std::getline(file, line);
				line += " "; // Extra space to register the last point
				tmp = "";
				std::memset(indices, -1, sizeof(indices));
				indexItem = 0;

				for (char& c : line) {
					if (indexItem >= sizeof(indices)) 
						throw std::runtime_error("OBJ Pushing too many items for one point!");

					// End of single index
					if (!tmp.empty() && (' ' == c || '/' == c)) {
						indices[indexItem] = std::atol(tmp.c_str());
						
						indexItem++;
						tmp = "";
					} else {
						tmp += c;
					}

					// End of point in face
					if (' ' == c && indexItem > 0) {
						// Push the face points to the VBO
						pushFace(vao.getVBO(), indices[0], indices[1], indices[2]);
						facePointCnt++;

						// :) 
						//
						// To future developers:
						//
						// VBO indices != EBO indices
						//
						// VBO indices are just used in OBJ files for face points to reference the cVertices, cNormals, cTexCoords
						// EBO indices are used by opengl to re-use entire points (calls to pushFace)

						// Use this loop to push the EBO indices as well
						// (As long as enough are provided to create a triangle
						Loader::pushTriangleIndices(vao.getEBO(), totalPoints, facePointCnt);
#ifdef VERBOSE
						std::cout << "[";
#endif

						std::memset(indices, -1, sizeof(indices));
						indexItem = 0;
					}
				}

				// If one or two points were provided, no indices will be specified
				if (facePointCnt < 3) 
					throw std::runtime_error("OBJ Cannot generate a face with less than 3 points");
			} else {
				// Read full line to re-sync the file pointer
				std::getline(file, line);
#ifdef VERBOSE
				std::cout << line;
#endif
			}
#ifdef VERBOSE
			std::cout << "]" << std::endl;
#endif
		}

		file.close();


		// Clear local buffers
		this->cVertices.clear();
		this->cTexCoords.clear();
		this->cNormals.clear();

		// Now let's update the VAO
		vao.update<CVertex, CNormal, CTexCoord>();

		return *this;
	}
}
