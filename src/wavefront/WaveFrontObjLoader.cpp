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
#include "WaveFrontObjLoader.h"

namespace Meshborn {
namespace WaveFront {

const char MATERIAL_LIBRARY_FACE_ELEMENT[] = "mtllib ";
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

            std::cout << "[POLYGONAL FACE]"
                      << " 1 = " << face.elements[0].vertex << "/"
                                 << face.elements[0].texture << "/"
                                 << face.elements[0].normal
                      << " | 2 = " << face.elements[1].vertex << "/"
                                   << face.elements[1].texture << "/"
                                   << face.elements[1].normal
                      << " | 3 = " << face.elements[2].vertex << "/"
                                   << face.elements[2].texture << "/"
                                   << face.elements[2].normal
                      << "\n";

            faces.push_back(face);

        // Vertex position
        } else if (view.starts_with(VECTOR_ELEMENT)) {
            glm::vec4 vertexPosition;

            if (!ParseVectorElement(view, &vertexPosition)) {
                return false;
            }

            std::cout << "[VERTEX] "
                      << "X: " << vertexPosition.x << " | "
                      << "Y: " << vertexPosition.y << " | "
                      << "Z: " << vertexPosition.z << " | "
                      << "W: " << vertexPosition.w << "\n";
            vertexPositions.push_back(vertexPosition);

        // Vertex normal
        } else if (view.starts_with(VECTOR_NORMAL_ELEMENT)) {
            glm::vec3 vertexNormal;

            if (!ParseVertexNormalElement(view, &vertexNormal)) {
                return false;
            }

            std::cout << "[VERTEX NORMAL] "
                << "X: " << vertexNormal.x << " | "
                << "Y: " << vertexNormal.y << " | "
                << "Z: " << vertexNormal.z << "\n";

            vertexNormals.push_back(vertexNormal);

        // Texture coordinate [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(TEXTURE_COORDINATE_ELEMENT)) {
            std::cout << "Found texture coordinate: " << view << '\n';

        // Use material [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(USE_MATERIAL_ELEMENT)) {
            std::cout << "[USE_MATERIAL_ELEMENT] " << view << '\n';

        // Material library [NOT IMPLEMENTED YET!]
        } else if (view.starts_with(MATERIAL_LIBRARY_FACE_ELEMENT)) {
            std::cout << "MATERIAL LIBRARY] " << view << '\n';

        } else {
            std::cout << line << '\n';
        }
    }

    return true;
}

}   // namespace WaveFront
}   // namespace Meshborn
