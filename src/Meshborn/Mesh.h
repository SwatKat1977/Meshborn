/*
Meshborn
Copyright (C) 2025 SwatKat1977

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MESH_H_
#define MESH_H_
#include <string>
#include <vector>
#include "Structures.h"

namespace Meshborn {
namespace WaveFront {

struct PolygonalFaceElement {
    PolygonalFaceElement() : vertex(-1), texture(-1), normal(-1) {}

    int vertex;
    int texture;
    int normal;
};

enum class PolygonalFaceType {
    //  Triangle (3 vertices)
    TRIANGE,

    //  Quad (4 vertices)
    QUAD,

    // N-gon (5+ vertices):
    // Technically allowed by the .obj format, though many parsers or game
    // engines don’t support them directly.
    N_GON
};

/**
 * Represents a single polygonal face in a 3D mesh.
 *
 * A face consists of one or more elements, each of which typically references
 * a vertex, texture coordinate, and/or normal index.
 */
struct PolygonalFace {
    PolygonalFaceType faceType;
    std::vector<PolygonalFaceElement> elements;
};

/**
 * Represents a 3D mesh consisting of vertices and polygonal faces.
 *
 * A mesh may also be associated with a name and material identifier.
 */
class Mesh {
 public:
    std::string name;
    std::string material;
    std::vector<PolygonalFace> faces;
    std::vector<Vertex> vertices;
};

}   // namespace WaveFront
}   // namespace Meshborn

#endif  // MESH_H_
