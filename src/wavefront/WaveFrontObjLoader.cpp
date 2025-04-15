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
#include <fstream>
#include <iostream>         /// TEMPORARY - TO BE DELETED!!!
#include <sstream>
#include <utility>
#include "glm/glm.hpp"
#include "LoggerManager.h"
#include "WaveFrontObjLoader.h"
#include "MaterialLibraryParser.h"

namespace Meshborn {
namespace WaveFront {

const char MATERIAL_LIBRARY_ELEMENT[] = "mtllib ";
const char POLYGONAL_FACE_ELEMENT[] = "f ";
const char TEXTURE_COORDINATE_ELEMENT[] = "vt ";
const char USE_MATERIAL_ELEMENT[] = "usemtl ";
const char VECTOR_ELEMENT[] = "v ";
const char VECTOR_NORMAL_ELEMENT[] = "vn ";

struct PolygonalFaceElement {
    PolygonalFaceElement() : vertex(-1), texture(-1), normal(-1) {}

    int vertex;
    int texture;
    int normal;
};

struct PolygonalFace {
    PolygonalFaceElement elements[3];
};

std::vector<std::string> ReadObjFile(const std::string& filename);
bool ParseVectorElement(std::string_view element, glm::vec4* vectorElement);
bool ParseVertexNormalElement(std::string_view element,
    glm::vec3* vectorNormalElement);
bool ParsePolygonalFaceElement(std::string_view element,
    PolygonalFace* face);
bool ParseObjFile(std::vector<std::string> lines);

std::vector<std::string> SplitElementString(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

void LoadFromFile(std::string filename) {
    std::vector<std::string> rawLines;

    try {
        rawLines = ReadObjFile(filename);
    }
    catch (std::runtime_error ex) {
        throw std::runtime_error(ex.what());
    }

    ParseObjFile(rawLines);
}

std::vector<std::string> ReadObjFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Only keep the line if it's not empty or not a comment.
        if (!line.empty() && !line.starts_with("#")) {
            // Strip Windows carriage return
            if (line.back() == '\r') {
                line.pop_back();
            }

            lines.push_back(std::move(line));
        }
    }

    return lines;
}

bool ParseVectorElement(std::string_view element, glm::vec4* vectorElement) {
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
            return false;
        }
        catch (std::out_of_range) {
            return false;
        }

        *vectorElement = glm::vec4(x, y, z, w);
    } else {
        return false;
    }

    return true;
}

bool ParseVertexNormalElement(std::string_view element,
                              glm::vec3* vectorNormalElement) {
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

        *vectorNormalElement = glm::vec3(x, y, z);
    } else {
        return false;
    }

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
bool ParsePolygonalFaceElement(std::string_view element,
                               PolygonalFace* face) {
    auto words = SplitElementString(std::string(element));
    if (words.size() != 4) {
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        PolygonalFaceElement faceElement;

        std::string rawFaceelement = words[i + 1];
        // std::string rawFaceelement = "1//3";

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

        face->elements[i] = faceElement;
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
bool ParseMaterials(std::string_view element, std::string &materialLibrary) {
    auto words = SplitElementString(std::string(element));

    // Requires 2 works (keyword and material_file)
    if (words.size() != 2) {
        return false;
    }

    std::ifstream file(words[1]);

    if (!file.good()) {
        LOG(Logger::LogLevel::Warning, std::format(
            "Materials library '{}' is missing/inaccessible",
            words[1]));
    } else {
        materialLibrary = words[1];
    }

    return true;
}

bool ParseObjFile(std::vector<std::string> lines) {
    std::vector<glm::vec4> vertexPositions;
    std::vector<glm::vec3> vertexNormals;
    std::vector<PolygonalFace> faces;

    for (const auto& line : lines) {
        std::string_view view(line);

        // Polygonal face
        if (view.starts_with(POLYGONAL_FACE_ELEMENT)) {
            PolygonalFace face;
            if (!ParsePolygonalFaceElement(view, &face)) {
                return false;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "POLYGONAL FACE => 1 = {}/{}/{} | 2 = {}/{}/{} | 3 = {}/{}/{}",
                face.elements[0].vertex,
                face.elements[0].texture,
                face.elements[0].normal,
                face.elements[1].vertex,
                face.elements[1].texture,
                face.elements[1].normal,
                face.elements[2].vertex,
                face.elements[2].texture,
                face.elements[2].normal));
            faces.push_back(face);

        // Vertex position
        } else if (view.starts_with(VECTOR_ELEMENT)) {
            glm::vec4 vertexPosition;

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
        } else if (view.starts_with(VECTOR_NORMAL_ELEMENT)) {
            glm::vec3 vertexNormal;

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
        } else if (view.starts_with(TEXTURE_COORDINATE_ELEMENT)) {
            LOG(Logger::LogLevel::Debug, std::format(
                "Found texture coordinate: {}", view));

        // Use material [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(USE_MATERIAL_ELEMENT)) {
            LOG(Logger::LogLevel::Debug, std::format(
                "Use material element: {}", view));

        // Material library [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(MATERIAL_LIBRARY_ELEMENT)) {
            std::string materialLibrary;

            if (!ParseMaterials(view, materialLibrary)) {
                LOG(Logger::LogLevel::Critical, std::format(
                    "Materials library line '{}' is invalid",
                    view));
                return false;
            }

            if (!materialLibrary.empty()) {
                printf("Parsing material library....\n");
                MaterialLibraryParser().ParseLibrary(materialLibrary);
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIALS LIBRARY => {}", view));

        } else {
            std::cout << line << '\n';
        }
    }

    return true;
}

}   // namespace WaveFront
}   // namespace Meshborn
