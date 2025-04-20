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
#include <fstream>
#include <iostream>         /// TEMPORARY - TO BE DELETED!!!
#include <sstream>
#include <utility>
#include "LoggerManager.h"
#include "WaveFrontObjParser.h"
#include "MaterialLibraryParser.h"

namespace Meshborn {
namespace WaveFront {

const char KEYWORD_GROUP[] = "g ";
const char KEYWORD_MATERIAL_LIBRARY[] = "mtllib ";
const char KEYWORD_OBJECT[] = "o ";
const char KEYWORD_POLYGONAL_FACE[] = "f ";
const char KEYWORD_TEXTURE_COORDINATE[] = "vt ";
const char KEYWORD_USE_MATERIAL[] = "usemtl ";
const char KEYWORD_VECTOR[] = "v ";
const char KEYWORD_VECTOR_NORMAL[] = "vn ";

WaveFrontObjParser::WaveFrontObjParser() {
}

bool WaveFrontObjParser::ParseObj(std::string filename) {
    std::vector<std::string> rawLines;

    try {
        rawLines = ReadFile(filename);
    }
    catch (std::runtime_error ex) {
        throw std::runtime_error(ex.what());
    }

    std::vector<Point4D> vertexPositions;
    std::vector<Point3D> vertexNormals;
    std::vector<TextureCoordinates> textureCoordinates;
    std::vector<PolygonalFace> faces;

    for (const auto& line : rawLines) {
        std::string_view view(line);

        if (view.starts_with(KEYWORD_GROUP)) {
            std::cout << "GROUP " << view << "\n";
            std::string groupName;
            if (!ParseGroupElement(view, &groupName)) {
            }

        //         } else if (view.starts_with(KEYWORD_VECTOR)) {
        } else if (view.starts_with(KEYWORD_OBJECT)) {
            std::cout << "OBJECT " << view << "\n";
            std::string objectName;
            if (!ParseObjectElement(view, &objectName)) {

            }

        // Polygonal face
        } else if (view.starts_with(KEYWORD_POLYGONAL_FACE)) {
            PolygonalFace face;
            if (!ParsePolygonalFaceElement(view, &face)) {
                return false;
            }

            if (face.faceType == PolygonalFaceType::TRIANGE) {
                LOG(Logger::LogLevel::Debug, std::format(
                    "POLYGONAL FACE [triangle] => 1 = {}/{}/{} | 2 = {}/{}/{} "
                    "| 3 = {}/{}/{}",
                    face.elements[0].vertex,
                    face.elements[0].texture,
                    face.elements[0].normal,
                    face.elements[1].vertex,
                    face.elements[1].texture,
                    face.elements[1].normal,
                    face.elements[2].vertex,
                    face.elements[2].texture,
                    face.elements[2].normal));
            } else {
                std::string faceType;
                if (face.faceType == PolygonalFaceType::QUAD) {
                    faceType = "Quad";
                } else {
                    faceType = "N-Gon";
                }

                LOG(Logger::LogLevel::Debug,
                     std::format("POLYGONAL FACE ({}) =>", faceType));

                for (size_t i = 0; i < face .elements.size(); ++i) {
                    LOG(Logger::LogLevel::Debug, std::format(
                        "    {} = {}/{}/{}",
                        i,
                        face.elements[i].vertex,
                        face.elements[i].texture,
                        face.elements[i].normal));
                }
            }
            faces.push_back(face);

        // Vertex position
        } else if (view.starts_with(KEYWORD_VECTOR)) {
            Point4D vertexPosition;

            if (!ParseVectorElement(view, &vertexPosition)) {
                return false;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "VERTEX => X: {} | Y: {} | Z: {} | W: {}",
                vertexPosition.x,
                vertexPosition.y,
                vertexPosition.z,
                vertexPosition.w));
            vertexPositions.push_back(vertexPosition);

        // Vertex normal
        } else if (view.starts_with(KEYWORD_VECTOR_NORMAL)) {
            Point3D vertexNormal;

            if (!ParseVertexNormalElement(view, &vertexNormal)) {
                return false;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "VERTEX NORMAL => X: {} | Y: {} | Z: {}",
                vertexNormal.x,
                vertexNormal.y,
                vertexNormal.z));
            vertexNormals.push_back(vertexNormal);

        // Texture coordinate [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(KEYWORD_TEXTURE_COORDINATE)) {
            TextureCoordinates coordinates;
            if (!ParseTextureCoordinate(view, &coordinates)) {
                return false;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "TEXTURE COORDINATE => U: {} | V: {} | W: {}",
                coordinates.u,
                coordinates.v,
                coordinates.w));
            textureCoordinates.push_back(coordinates);

        // Use material [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(KEYWORD_USE_MATERIAL)) {
            LOG(Logger::LogLevel::Debug, std::format(
                "Use material element: {}", view));

        // Material library [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(KEYWORD_MATERIAL_LIBRARY)) {
            std::string materialLibrary;

            if (!ParseMaterials(view, &materialLibrary)) {
                LOG(Logger::LogLevel::Critical, std::format(
                    "Materials library line '{}' is invalid",
                    view));
                return false;
            }

            if (!materialLibrary.empty()) {
                printf("Parsing material library....\n");
                MaterialMap materials;
                if (!MaterialLibraryParser().ParseLibrary(materialLibrary,
                                                          &materials)) {
                    std::cout << "ERR parsing material library\n";
                    return false;
                }
            }

            LOG(Logger::LogLevel::Debug,
                std::format("MATERIALS LIBRARY => {}", view));

        } else {
            LOG(Logger::LogLevel::Debug,
                std::format("Unknown obj tag: '{}'", line));
        }
    }

    return true;
}

bool WaveFrontObjParser::ParseGroupElement(std::string_view element,
                                           std::string* groupName) {
    auto words = SplitElementString(std::string(element));
    if (words.size() < 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Group '{}' is invalid", element));
        return false;
    }

    *groupName = words[1];
    return true;
}

bool WaveFrontObjParser::ParseObjectElement(std::string_view element,
                                            std::string* objectName) {
    auto words = SplitElementString(std::string(element));
    if (words.size() < 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Object '{}' is invalid", element));
        return false;
    }

    *objectName = words[1];
    return true;
}

/**
 * @brief Parses a polygonal face element from a string.
 *
 * This function processes a face definition from an input string, typically
 * formatted according to the Wavefront OBJ specification. It supports face
 * element formats such as:
 *
 * - v          (vertex only)
 *
 * - v/vt       (vertex and texture)
 *
 * - v//vn      (vertex and normal)
 *
 * - v/vt/vn    (vertex, texture, and normal)
 *
 * The function extracts up to three face elements and assigns them to the
 * provided PolygonalFace object.
 *
 * @param element The input string representing a polygonal face element,
 *                usually in the format: "f v1 v2 v3".
 * @param face    Pointer to a PolygonalFace struct where the parsed elements
 *                will be stored.
 *
 * @return true if the face string is valid and was successfully parsed;
 *         false otherwise.
 */
bool WaveFrontObjParser::ParsePolygonalFaceElement(std::string_view element,
                                                   PolygonalFace* face) {
    auto words = SplitElementString(std::string(element));
    if (words.size() < 4) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Polygonal face '{}' is invalid", element));
        return false;
    }

    for (auto it = std::next(words.begin()); it != words.end(); ++it) {
        PolygonalFaceElement faceElement;

        std::string rawFaceelement = *it;
        size_t firstSlash = rawFaceelement.find('/');
        size_t secondSlash = rawFaceelement.find('/', firstSlash + 1);

        // Format: v
        if (firstSlash == std::string::npos) {
            faceElement.vertex = std::stoi(rawFaceelement);

        // Format: v/vt
        } else if (secondSlash == std::string::npos) {
            faceElement.vertex = std::stoi(rawFaceelement.substr(
                0, firstSlash));
            faceElement.texture = std::stoi(rawFaceelement.substr(
                firstSlash + 1));

        // Format: v//vn
        } else if (secondSlash == firstSlash + 1) {
            faceElement.vertex = std::stoi(rawFaceelement.substr(
                0, firstSlash));
            faceElement.normal = std::stoi(rawFaceelement.substr(
                secondSlash + 1));

        // Format: v/vt/vn
        } else {
            faceElement.vertex = std::stoi(rawFaceelement.substr(
                0, firstSlash));
            faceElement.texture = std::stoi(rawFaceelement.substr(
                firstSlash + 1, secondSlash - firstSlash - 1));
            faceElement.normal = std::stoi(rawFaceelement.substr(
                secondSlash + 1));
        }

        face->elements.push_back(faceElement);
    }

    switch ((words.size() -1)) {
        // 3 vertex - triangle
        case 3:
            face->faceType = PolygonalFaceType::TRIANGE;
            break;

        // 4 vertex - Quad
        case 4:
            face->faceType = PolygonalFaceType::QUAD;
            break;

        // N-gon (5+ vertices)
        default:
            face->faceType = PolygonalFaceType::N_GON;
    }

    return true;
}

bool WaveFrontObjParser::ParseVectorElement(std::string_view element,
                                            Point4D* vectorElement) {
    auto words = SplitElementString(std::string(element));
    if ((words.size() == 4) || (words.size() == 5)) {
        float x;
        float y;
        float z;
        float w = 1.0f;

        try {
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);

            if (words.size() == 5) {
                w = std::stof(words[4]);
            }
        }
        catch (std::invalid_argument) {
            LOG(Logger::LogLevel::Critical, std::format(
                "Vector '{}' is invalid (invalid argument)", element));
            return false;
        }
        catch (std::out_of_range) {
            LOG(Logger::LogLevel::Critical, std::format(
                "Vector '{}' is invalid (out of range)", element));
            return false;
        }

        *vectorElement = Point4D(x, y, z, w);
    } else {
        return false;
    }

    return true;
}

bool WaveFrontObjParser::ParseVertexNormalElement(
    std::string_view element, Point3D* vectorNormalElement) {
    float x;
    float y;
    float z;

    auto words = SplitElementString(std::string(element));
    if ((words.size() == 4) || (words.size() == 5)) {
        try {
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
        }
        catch (std::invalid_argument) {
            return false;
        }
        catch (std::out_of_range) {
            return false;
        }

        *vectorNormalElement = Point3D(x, y, z);
    } else {
        return false;
    }

    return true;
}

/**
 * @brief Parses a material library reference from a given element string.
 *
 * This function expects an input string that represents a material reference
 * in the format: `keyword material_file`. It splits the string into parts
 * and, if exactly two parts are found, assigns the second part to the
 * output parameter `materialLibrary`.
 *
 * @param element The input string view representing a single line or element
 *                that includes a material keyword and the material file name.
 * @param materialLibrary A reference to a string where the parsed material
 *                        file name will be stored if parsing is successful.
 *
 * @return true if the input string is correctly formatted and parsing
 *         succeeds; false otherwise.
 */
bool WaveFrontObjParser::ParseMaterials(std::string_view element,
                                        std::string *materialLibrary) {
    auto words = SplitElementString(std::string(element));

    // Requires 2 words (keyword and material_file)
    if (words.size() != 2) {
        return false;
    }

    std::ifstream file(words[1]);

    if (!file.good()) {
        LOG(Logger::LogLevel::Warning, std::format(
            "Materials library '{}' is missing/inaccessible",
            words[1]));
    } else {
        *materialLibrary = words[1];
    }

    return true;
}

bool WaveFrontObjParser::ParseTextureCoordinate(
    std::string_view element, TextureCoordinates *coordinates) {
    auto words = SplitElementString(std::string(element));

    // Requires 4 words (keyword, u, v, w)
    if (words.size() != 4) {
        LOG(Logger::LogLevel::Critical, "Texture coordinate is invalid");
        return false;
    }

    float coordinateU;
    float coordinateV;
    float coordinateW;

    if (!ParseFloat(words[1].c_str(), &coordinateU)) return false;
    if (!ParseFloat(words[2].c_str(), &coordinateV)) return false;
    if (!ParseFloat(words[3].c_str(), &coordinateW)) return false;

    coordinates->u  = coordinateU;
    coordinates->v  = coordinateV;
    coordinates->w  = coordinateW;

    return true;
}

}   // namespace WaveFront
}   // namespace Meshborn
