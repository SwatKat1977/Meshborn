/*
Meshborn
Copyright (C) 2025  SwatKat1977

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
#ifndef WAVEFRONT_WAVEFRONTOBJPARSER_H_
#define WAVEFRONT_WAVEFRONTOBJPARSER_H_
#include <string>
#include <vector>
#include "BaseWavefrontParser.h"
#include "../Structures.h"

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

struct PolygonalFace {
    PolygonalFaceType faceType;
    std::vector<PolygonalFaceElement> elements;
};

class WaveFrontObjParser : public BaseWavefrontParser {
 public:
    WaveFrontObjParser();

    bool ParseObj(std::string filename);

 private:

     bool ParseGroupElement(std::string_view element,
                            std::string* face);

    bool ParseObjectElement(std::string_view element,
                            std::string* face);

    bool ParseVectorElement(std::string_view element,
                            Point4D* vectorElement);

    bool ParsePolygonalFaceElement(std::string_view element,
                                   PolygonalFace* face);

    bool ParseVertexNormalElement(std::string_view element,
                                  Point3D* vectorNormalElement);

    bool ParseMaterials(std::string_view element,
                        std::string *materialLibrary);

    bool ParseTextureCoordinate(std::string_view element,
                                TextureCoordinates *coordinates);
};

}   // namespace WaveFront
}   // namespace Meshborn

#endif  // WAVEFRONT_WAVEFRONTOBJPARSER_H_
