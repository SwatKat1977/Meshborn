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
#include <iostream>     // TEMP
#include <vector>
#include "MaterialLibraryParser.h"
#include "Material.h"
#include "LoggerManager.h"

namespace Meshborn {
namespace WaveFront {

const char KEYWORD_NEW_MATERIAL[] = "newmtl ";
const char KEYWORD_AMBIENT[] = "Ka ";
const char KEYWORD_DIFFUSE[] = "Kd ";
const char KEYWORD_EMISSIVE[] = "Ke ";
const char KEYWORD_SPECULAR[] = "Ks ";
const char KEYWORD_SPECULAR_EXPONENT[] = "Ns ";
const char KEYWORD_TRANSPARENT_DISOLVE[] = "d ";
const char KEYWORD_OPTICAL_DENSITY[] = "Ni ";
const char KEYWORD_ILLUMINATION_MODEL[] = "illum ";

const char KEYWORD_AMBIENT_TEXTURE_MAP[] = "map_Ka ";
const char KEYWORD_DIFFUSE_TEXTURE_MAP[] = "map_Kd ";
const char KEYWORD_SPECULAR_COLOR_TEXTURE_MAP[] = "map_Ks ";
const char KEYWORD_SPECULAR_HIGHLIGHT_COMPONENT[] = "map_Ns ";
const char KEYWORD_ALPHA_TEXTURE_MAP[] = "map_d ";

// map_bump and bump are one and the same.
const char KEYWORD_MAP_BUMP[] = "map_bump ";
const char KEYWORD_BUMP_MAP[] = "bump ";

const char KEYWORD_DISPLACEMENT_MAP[] = "disp ";

// defaults to 'matte' channel of the image)
const char KEYWORD_STENCIL_DECAL_TEXTURE[] = "decal ";

/*
Illumination model (illum tag)
illum	Description
0	Color on, ambient off
1	Color on, ambient on
2	Highlight on
3	Reflection on and ray trace on
4	Transparency: Glass on, Reflection: Ray trace on
5	Reflection: Fresnel on and ray trace on
6	Transparency: Refraction on, Reflection: Fresnel off and ray trace on
7	Transparency: Refraction on, Reflection: Fresnel on and ray trace on
8	Reflection on and ray trace off
9	Transparency: Glass on, Reflection: Ray trace off
10	Casts shadows onto invisible surfaces
*/

MaterialLibraryParser::MaterialLibraryParser() {
}

void MaterialLibraryParser::ParseLibrary(std::string materialFile) {
    std::vector<std::string> rawLines;

    try {
        rawLines = ReadFile(materialFile);
    }
    catch (std::runtime_error ex) {
        throw std::runtime_error(ex.what());
    }

    std::vector<Material> materials;
    int currentMaterial = -1;

    for (const auto& line : rawLines) {
        std::string_view view(line);

        // New material
        if (StartsWith(std::string(view), KEYWORD_NEW_MATERIAL)) {
            std::string materialName;

            if (!ProcessTagNewMaterial(view, materialName)) {
                return;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "New Material => {}", materialName));

            Material newMaterial(materialName);
            materials.push_back(newMaterial);
            currentMaterial = materials.size() -1;
            std::cout << "Current material index = " << currentMaterial << "\n";

        // Ambient colour
        } else if (StartsWith(std::string(view), KEYWORD_AMBIENT)) {
            if (currentMaterial == -1) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ka' keyword");
                return;
            }

            glm::vec3 ambientColour;
            if (!ProcessTagAmbientColour(view, &ambientColour)) {
                return;
            }

            materials[currentMaterial].SetAmbientColour(ambientColour);
            glm::vec3 colour;
            materials[currentMaterial].GetAmbientColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|AMBIENT COLOUR => R: {} G: {} B: {}",
                colour.x, colour.y, colour.z));

        // Diffuse colour
        } else if (StartsWith(std::string(view), KEYWORD_DIFFUSE)) {
            if (currentMaterial == -1) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Kd' keyword");
                return;
            }

            glm::vec3 diffuseColour;
            if (!ProcessTagDiffuseColour(view, &diffuseColour)) {
                return;
            }

            materials[currentMaterial].SetDiffuseColour(diffuseColour);
            glm::vec3 colour;
            materials[currentMaterial].GetDiffuseColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|DIFFUSE COLOUR => R: {} G: {} B: {}",
                colour.x, colour.y, colour.z));

        // Emissive colour
        } else if (StartsWith(std::string(view), KEYWORD_EMISSIVE)) {
            if (currentMaterial == -1) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ke' keyword");
                return;
            }

            glm::vec3 emissiveColour;
            if (!ProcessTagEmissiveColour(view, &emissiveColour)) {
                return;
            }

            materials[currentMaterial].SetEmissiveColour(emissiveColour);
            glm::vec3 colour;
            materials[currentMaterial].GetEmissiveColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|EMISSIVE COLOUR => R: {} G: {} B: {}",
                colour.x, colour.y, colour.z));

        // Specular colour
        } else if (StartsWith(std::string(view), KEYWORD_SPECULAR)) {
            if (currentMaterial == -1) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ks' keyword");
                return;
            }

            glm::vec3 specularColour;
            if (!ProcessTagSpecularColour(view, &specularColour)) {
                return;
            }

            materials[currentMaterial].SetSpecularColour(specularColour);
            glm::vec3 colour;
            materials[currentMaterial].GetSpecularColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|SPECULAR COLOUR => R: {} G: {} B: {}",
                colour.x, colour.y, colour.z));

        // Specular exponent
        } else if (StartsWith(std::string(view), KEYWORD_SPECULAR_EXPONENT)) {
            float specularExponent;
            if (!ProcessTagSpecularExponent(line, &specularExponent)) {
                return;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|SPECULAR EXPONENT => {}", specularExponent));

        // Transparent dissolve
        } else if (StartsWith(std::string(view), KEYWORD_TRANSPARENT_DISOLVE)) {
            std::cout << "Transparent dissolve: " << view << std::endl;

        // Optical density
        } else if (StartsWith(std::string(view), KEYWORD_OPTICAL_DENSITY)) {
            std::cout << "Optical density: " << view << std::endl;

            float opticalDensity;
            if (!ProcessTagOpticalDensity(line, &opticalDensity)) {
                return;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|OPTICAL DENSITY => {}", opticalDensity));

            materials[currentMaterial].SetOpticalDensity(opticalDensity);
            float density;
            materials[currentMaterial].GetOpticalDensity(&density);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|OPTICAL DENSITY => {}", density));

        // Illumination model
        } else if (StartsWith(std::string(view), KEYWORD_ILLUMINATION_MODEL)) {
            if (currentMaterial == -1) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'illum' keyword");
                return;
            }

            int illuminationModel;
            if (!ProcessTagIlluminationModel(line, &illuminationModel)) {
                return;
            }

            materials[currentMaterial].SetIlluminationModel(illuminationModel);
            int model;
            materials[currentMaterial].GetIlluminationModel(&model);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|ILLUMINATION MODEL => {}", model));

        // Ambient texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_AMBIENT_TEXTURE_MAP)) {
            std::cout << "Ambient texture map: " << view << std::endl;

        // Diffuse texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_DIFFUSE_TEXTURE_MAP)) {
            std::cout << "Diffuse texture map: " << view << std::endl;

        // Specular color texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_SPECULAR_COLOR_TEXTURE_MAP)) {
            std::cout << "Specular color texture map: " << view << std::endl;

        // Specular highlight component
        } else if (StartsWith(std::string(view),
                              KEYWORD_SPECULAR_HIGHLIGHT_COMPONENT)) {
            std::cout << "Specular highlight component: " << view << std::endl;

        // Alpha texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_ALPHA_TEXTURE_MAP)) {
            std::cout << "Alpha texture map: " << view << std::endl;

        // Bump map
        } else if ((StartsWith(std::string(view),
                               KEYWORD_MAP_BUMP)) ||
                   (StartsWith(std::string(view),
                               KEYWORD_BUMP_MAP))) {
            std::cout << "Bump map: " << view << std::endl;

        // Displacement map
        } else if (StartsWith(std::string(view),
                              KEYWORD_DISPLACEMENT_MAP)) {
        std::cout << "Displacement map: " << view << std::endl;

       // Stencil decal texture
        } else if (StartsWith(std::string(view),
                              KEYWORD_STENCIL_DECAL_TEXTURE)) {
            std::cout << "Stencil decal texture: " << view << std::endl;

        // Unknown tag : Doesn't mean it's invalid, it could be a tag that
        //               currently isn't parsed.
        } else {
            std::cout << "Unknown: " << view << std::endl;
        }
    }
}

/**
 * @brief Parses a new material tag line and extracts the material name.
 *
 * @param line The input line containing the new material definition.
 * @param material Reference to a string to store the parsed material name.
 * @return true if parsing was successful, false otherwise.
 */
bool MaterialLibraryParser::ProcessTagNewMaterial(std::string_view line,
                                                  std::string &material) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "New material line is invalid: '{}'", line));
        return false;
    }

    material = words[1];

    return true;
}

/**
 * @brief Parses an ambient colour tag line and extracts the RGB values.
 *
 * @param line The input line containing the ambient colour definition.
 * @param colour Pointer to glm::vec3 to store the parsed RGB values.
 * @return true if parsing was successful, false otherwise.
 */
bool MaterialLibraryParser::ProcessTagAmbientColour(std::string_view line,
                                                    glm::vec3 *colour) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 4) {
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) return false;
    if (!ParseFloat(words[2].c_str(), &green)) return false;
    if (!ParseFloat(words[3].c_str(), &blue)) return false;

    *colour = glm::vec3(red, green, blue);

    return true;
}

/**
 * @brief Parses a diffuse colour tag line and extracts the RGB values.
 *
 * @param line The input line containing the diffuse colour definition.
 * @param colour Pointer to glm::vec3 to store the parsed RGB values.
 * @return true if parsing was successful, false otherwise.
 */
bool MaterialLibraryParser::ProcessTagDiffuseColour(std::string_view line,
                                                    glm::vec3 *colour) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 4) {
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) return false;
    if (!ParseFloat(words[2].c_str(), &green)) return false;
    if (!ParseFloat(words[3].c_str(), &blue)) return false;

    *colour = glm::vec3(red, green, blue);

    return true;
}

/**
 * @brief Parses an emissive colour tag line and extracts the RGB values.
 *
 * @param line The input line containing the emissive colour definition.
 * @param colour Pointer to glm::vec3 to store the parsed RGB values.
 * @return true if parsing was successful, false otherwise.
 */
bool MaterialLibraryParser::ProcessTagEmissiveColour(std::string_view line,
                                                     glm::vec3 *colour) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 4) {
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) return false;
    if (!ParseFloat(words[2].c_str(), &green)) return false;
    if (!ParseFloat(words[3].c_str(), &blue)) return false;

    *colour = glm::vec3(red, green, blue);

    return true;
}

/**
 * @brief Parses a specular colour tag line and extracts the RGB values.
 *
 * @param line The input line containing the specular colour definition.
 * @param colour Pointer to glm::vec3 to store the parsed RGB values.
 * @return true if parsing was successful, false otherwise.
 */
bool MaterialLibraryParser::ProcessTagSpecularColour(std::string_view line,
                                                     glm::vec3 *colour) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 4) {
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) return false;
    if (!ParseFloat(words[2].c_str(), &green)) return false;
    if (!ParseFloat(words[3].c_str(), &blue)) return false;

    *colour = glm::vec3(red, green, blue);

    return true;
}

bool MaterialLibraryParser::ProcessTagSpecularExponent(std::string_view line,
                                                       float *shininess) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, "Material specular exponent invalid");
        return false;
    }

    if (!ParseFloat(words[1].c_str(), shininess)) return false;

    return true;
}

bool MaterialLibraryParser::ProcessTagTransparentDissolve(std::string_view line,
                                                          float *transparency) {
}

bool MaterialLibraryParser::ProcessTagOpticalDensity(std::string_view line,
                                                     float *density) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, "Material optical density invalid");
        return false;
    }

    if (!ParseFloat(words[1].c_str(), density)) return false;

    if ((*density < 0.001f) || (*density > 10.0f)) {
        LOG(Logger::LogLevel::Critical,
            "Material optical density invalid value");
        return false;
    }

    return true;
}

bool MaterialLibraryParser::ProcessTagIlluminationModel(std::string_view line,
                                                        int *density) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, "Material illumination model invalid");
        return false;
    }

    if (!ParseInt(words[1].c_str(), density)) return false;

    if ((*density < 0) || (*density > 10)) {
        LOG(Logger::LogLevel::Critical,
            "Material illumination model invalid value (range 0-10)");
        return false;
    }

    return true;
}

}   // namespace WaveFront
}   // namespace Meshborn
