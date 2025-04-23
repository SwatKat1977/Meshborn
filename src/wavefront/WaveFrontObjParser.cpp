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
#include <format>
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

/**
 * Parses a Wavefront .obj file and fills the provided model object with data.
 *
 * Reads the specified .obj file, parsing vertex positions, normals,
 * texture coordinates, faces, groups, and objects. Meshes are created
 * based on object/group/material changes. Material libraries may also
 * be parsed if specified. This parser supports triangle, quad, and
 * n-gon polygonal faces.
 *
 * @param filename The path to the .obj file to be parsed.
 * @param model Pointer to the Model object to populate.
 * @return true if parsing succeeds, false if any error occurs.
 * @throws std::runtime_error if the file cannot be read.
 */
bool WaveFrontObjParser::ParseObj(std::string filename,
                                  Model* model) {
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

    std::string currentObjectName = "default";
    std::string currentGroupName = "default";
    std::string currentMaterial = "";
    std::string currentMeshName = "default:default";
    Mesh* currentMesh = nullptr;

    for (const auto& line : rawLines) {
        std::string_view view(line);

        if (view.starts_with(KEYWORD_GROUP)) {
            std::string groupName;
            if (!ParseGroupElement(view, &groupName)) {
                return false;
            }

            currentGroupName = groupName;
            currentMeshName = currentObjectName + ":" + currentGroupName;
            LOG(Logger::LogLevel::Debug,
                std::format("GROUP => {}", currentGroupName));

        } else if (view.starts_with(KEYWORD_OBJECT)) {
            std::string objectName;
            if (!ParseObjectElement(view, &objectName)) {
                return false;
            }

            currentObjectName = objectName;
            currentMeshName = currentObjectName + ":" + currentGroupName;
            LOG(Logger::LogLevel::Debug,
                std::format("OBJECT => {}", currentObjectName));

        // Polygonal face
        } else if (view.starts_with(KEYWORD_POLYGONAL_FACE)) {

            if (!currentMesh ||
                currentMesh->name != currentMeshName ||
                currentMesh->material != currentMaterial) {

                if (currentMesh) {
                    LOG(Logger::LogLevel::Debug, "Moving along ....");

                    if (!FinaliseVertices(currentMesh,
                        vertexPositions,
                        vertexNormals,
                        textureCoordinates)) {
                        printf("failed to finalise\n");
                        return false;
                    }
                }

                // Create an instance of Mesh class for object/group/material
                // change.
                Mesh newMesh;
                newMesh.name = currentMeshName;
                newMesh.material = currentMaterial;

                // Add the new meshes to the the model and the set the current
                // mesh to it by getting the last element in the array.
                model->meshes.push_back(std::move(newMesh));
                currentMesh = &model->meshes.back();

                LOG(Logger::LogLevel::Debug,
                    std::format("NEW MESH => name: {}, material: {}",
                        currentMesh->name,
                        currentMesh->material));
            }

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

            currentMesh->faces.push_back(face);

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

            std::string useMaterialName;
            if (!ParseUseMaterial(view, &useMaterialName)) {
                return false;
            }

            currentMaterial = useMaterialName;

            LOG(Logger::LogLevel::Debug, std::format(
                "USE MATERIAL => {}", currentMaterial));

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

    if (currentMesh) {
        FinaliseVertices(currentMesh, vertexPositions, vertexNormals,
                         textureCoordinates);
    }

    return true;
}

/**
 * Parses a group name element from a string and stores the result.
 *
 * Expects at least two space-separated words: keyword and group name.
 * Extracts the name and assigns it to the provided output string.
 *
 * @param element The input string containing the group definition.
 * @param groupName Output pointer for the parsed group name.
 * @return true on success, false if input format is invalid.
 */
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

/**
 * Parses an object name element from a string and stores the result.
 *
 * Expects at least two space-separated words: keyword and object name.
 * Extracts the name and assigns it to the provided output string.
 *
 * @param element The input string containing the object definition.
 * @param objectName Output pointer for the parsed object name.
 * @return true on success, false if input format is invalid.
 */
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
            faceElement.texture = -1;
            faceElement.normal = -1;

        // Format: v/vt
        } else if (secondSlash == std::string::npos) {
            faceElement.vertex = std::stoi(rawFaceelement.substr(
                0, firstSlash));
            faceElement.texture = std::stoi(rawFaceelement.substr(
                firstSlash + 1));
            faceElement.normal = -1;

        // Format: v//vn
        } else if (secondSlash == firstSlash + 1) {
            faceElement.vertex = std::stoi(rawFaceelement.substr(
                0, firstSlash));
            faceElement.texture = -1;
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

/**
 * Parses a 3D or 4D vector string and stores it in a Point4D structure.
 *
 * Expects 4 or 5 space-separated values: keyword, x, y, z, and optional w.
 * Converts the numeric values and stores them in the output vector element.
 *
 * @param element The input string containing the vector definition.
 * @param vectorElement Output pointer for the parsed 4D vector.
 * @return true on success, false if parsing fails or input is invalid.
 */
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

/**
 * Parses a vertex normal string and stores it in a Point3D structure.
 *
 * Accepts a string with 4 or 5 space-separated elements: keyword and the
 * x, y, z components. Parses the components and assigns them to the output.
 *
 * @param element The input string containing the normal vector.
 * @param vectorNormalElement Output pointer for the parsed normal vector.
 * @return true on success, false if parsing fails or input is invalid.
 */
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

/**
 * Parses a texture coordinate string and stores the result.
 *
 * Expects a string with four space-separated elements: keyword, u, v, and w.
 * Extracts the float values and writes them to the given coordinates struct.
 *
 * @param element The input string containing the texture coordinate data.
 * @param coordinates Output pointer for parsed texture coordinate values.
 * @return true on successful parse, false on format or parse error.
 */
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

bool WaveFrontObjParser::ParseUseMaterial(std::string_view element,
                                          std::string* material) {
    auto words = SplitElementString(std::string(element));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, "User Material entry is invalid");
        return false;
    }

    *material = words[1];

    return true;
}

/**
 * Finalises the mesh by converting face data into vertex attributes.
 *
 * This function populates the mesh's vertex list using face indices and the
 * provided lists of positions, normals, and texture coordinates. Returns
 * false if any indices are out of bounds or the mesh is null.
 *
 * @param mesh Pointer to the mesh to populate.
 * @param positions List of 4D vertex positions.
 * @param normals List of 3D vertex normals.
 * @param textureCoordinates List of texture coordinate vectors.
 * @return true on success, false if input data is invalid.
 */
bool WaveFrontObjParser::FinaliseVertices(
    Mesh *mesh,
    const Point4DList& positions,
    const Point3DList& normals,
    const TextureCoordinatesList& textureCoordinates) {

    LOG(Logger::LogLevel::Debug, std::format("Finalizing mesh '{}'",
                                             mesh->name));

    if (!mesh) {
        LOG(Logger::LogLevel::Critical,
            "Invalid mesh passed to FinaliseVertices");
        return false;
    }

    mesh->vertices.clear();

    for (const auto& face : mesh->faces) {
        for (const auto& elem : face.elements) {
            Vertex vertex;

            if (elem.vertex < 1 ||
                elem.vertex > static_cast<int>(positions.size())) {
                return false;
            }

            vertex.position = {
                positions[elem.vertex - 1].x,
                positions[elem.vertex - 1].y,
                positions[elem.vertex - 1].z,
                positions[elem.vertex - 1].w
            };

            vertex.normal = { 0.0f, 0.0f, 0.0f };
            if (elem.normal >= 1 &&
                elem.normal <= static_cast<int>(normals.size())) {
                    vertex.normal = normals[elem.normal - 1];
            }

            vertex.textureCoordinates = { 0.0f, 0.0f, 0.0f };
            if (elem.texture >= 1 &&
                elem.texture <= static_cast<int>(textureCoordinates.size())) {
                vertex.textureCoordinates =
                    textureCoordinates[elem.texture - 1];
            }

            mesh->vertices.push_back(vertex);

        }
    }

    return true;
}

}   // namespace WaveFront
}   // namespace Meshborn
