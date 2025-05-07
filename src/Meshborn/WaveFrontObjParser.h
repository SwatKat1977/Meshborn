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
#ifndef WAVEFRONTOBJPARSER_H_
#define WAVEFRONTOBJPARSER_H_
#include <memory>
#include <string>
#include <vector>
#include "BaseWavefrontParser.h"
#include "Structures.h"
#include "Mesh.h"
#include "Model.h"

namespace Meshborn {

class WaveFrontObjParser : public BaseWavefrontParser {
 public:
    WaveFrontObjParser();

    std::unique_ptr<Model> ParseObj(std::string filename);

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

    bool ParseSmoothShading(std::string_view element,
                            int *shadingGroup);

    bool ParseTextureCoordinate(std::string_view element,
                                TextureCoordinates *coordinates);

    bool ParseUseMaterial(std::string_view element,
                          std::string *material);

    bool FinaliseVertices(Mesh *mesh,
                          const Point4DList& positions,
                          const Point3DList& normals,
                          const TextureCoordinatesList& textureCoordinates);
};

}   // namespace Meshborn

#endif  // WAVEFRONTOBJPARSER_H_
