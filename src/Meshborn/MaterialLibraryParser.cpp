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
#include <format>
#include <iostream>
#include <vector>
#include "MaterialLibraryParser.h"
#include "Material.h"
#include "LoggerManager.h"

namespace Meshborn {

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

MaterialLibraryParser::MaterialLibraryParser() {
}

/**
 * Parses a material library (.mtl) file and populates a map of materials.
 * Each material is created based on the tags encountered in the file, and
 * their properties (e.g., ambient, diffuse, specular colors, textures) are set
 * accordingly. The function handles various material-related tags such as
 * 'Ka', 'Kd', 'Ks', 'Ns', etc.
 *
 * The process includes handling texture maps, transparency, bump maps, and
 * optical density, as well as ensuring correct ordering of the tags. If any
 * tag is out of order or invalid, the function logs an error and returns
 * false.
 *
 * @param materialFile Path to the material file (.mtl) to be parsed.
 * @param materials A pointer to a map where the parsed materials will be
 *                  stored, keyed by their names.
 * @return true if the material file was successfully parsed and all materials
 *              were processed;
 *         false if an error occurred during parsing (e.g., invalid tags or
 *               misordered keywords).
 */
bool MaterialLibraryParser::ParseLibrary(std::string materialFile,
                                         MaterialMap *materials) {
    std::vector<std::string> rawLines;

    try {
        rawLines = ReadFile(materialFile);
    }
    catch (std::runtime_error ex) {
        throw std::runtime_error(ex.what());
    }

    std::shared_ptr<Material> currentMaterial = nullptr;

    for (const auto& line : rawLines) {
        std::string_view view(line);

        // New material
        if (StartsWith(std::string(view), KEYWORD_NEW_MATERIAL)) {
            std::string materialName;

            if (!ProcessTagNewMaterial(view, &materialName)) {
                return false;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "NEW MATERIAL => {}", materialName));

            auto newMaterial = std::make_shared<Material>(materialName);
            (*materials)[materialName] = newMaterial;

            auto it = materials->find(materialName);
            if (it != materials->end()) {
                currentMaterial = it->second;
            }

        // Ambient colour
        } else if (StartsWith(std::string(view), KEYWORD_AMBIENT)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ka' keyword");
                return false;
            }

            RGB ambientColour;
            if (!ProcessTagAmbientColour(view, &ambientColour)) {
                return false;
            }

            currentMaterial->SetAmbientColour(ambientColour);
            RGB colour;
            currentMaterial->GetAmbientColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|AMBIENT COLOUR => R: {} G: {} B: {}",
                colour.red, colour.green, colour.blue));

        // Diffuse colour
        } else if (StartsWith(std::string(view), KEYWORD_DIFFUSE)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Kd' keyword");
                return false;
            }

            RGB diffuseColour;
            if (!ProcessTagDiffuseColour(view, &diffuseColour)) {
                return false;
            }

            currentMaterial->SetDiffuseColour(diffuseColour);
            RGB colour;
            currentMaterial->GetDiffuseColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|DIFFUSE COLOUR => R: {} G: {} B: {}",
                colour.red, colour.green, colour.blue));

        // Emissive colour
        } else if (StartsWith(std::string(view), KEYWORD_EMISSIVE)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ke' keyword");
                return false;
            }

            RGB emissiveColour;
            if (!ProcessTagEmissiveColour(view, &emissiveColour)) {
                return false;
            }

            currentMaterial->SetEmissiveColour(emissiveColour);
            RGB colour;
            currentMaterial->GetEmissiveColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|EMISSIVE COLOUR => R: {} G: {} B: {}",
                colour.red, colour.green, colour.blue));

        // Specular colour
        } else if (StartsWith(std::string(view), KEYWORD_SPECULAR)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ks' keyword");
                return false;
            }

            RGB specularColour;
            if (!ProcessTagSpecularColour(view, &specularColour)) {
                return false;
            }

            currentMaterial->SetSpecularColour(specularColour);
            RGB colour;
            currentMaterial->GetSpecularColour(&colour);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|SPECULAR COLOUR => R: {} G: {} B: {}",
                colour.red, colour.green, colour.blue));

        // Specular exponent
        } else if (StartsWith(std::string(view), KEYWORD_SPECULAR_EXPONENT)) {
            float specularExponent;
            if (!ProcessTagSpecularExponent(line, &specularExponent)) {
                return false;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|SPECULAR EXPONENT => {}", specularExponent));

        // Transparent dissolve
        } else if (StartsWith(std::string(view), KEYWORD_TRANSPARENT_DISOLVE)) {
            float transparentDissolve;

            if (!ProcessTagTransparentDissolve(line, &transparentDissolve)) {
                return false;
            }

            currentMaterial->SetTransparentDissolve(
                transparentDissolve);
            float transparency;
            currentMaterial->GetTransparentDissolve(&transparency);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|TRANSPARENT DISSOLVE => {}", transparency));

        // Optical density
        } else if (StartsWith(std::string(view), KEYWORD_OPTICAL_DENSITY)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ks' keyword");
                return false;
            }

            float opticalDensity;
            if (!ProcessTagOpticalDensity(line, &opticalDensity)) {
                return false;
            }

            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|OPTICAL DENSITY => {}", opticalDensity));

            currentMaterial->SetOpticalDensity(opticalDensity);
            float density;
            currentMaterial->GetOpticalDensity(&density);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|OPTICAL DENSITY => {}", density));

        // Illumination model
        } else if (StartsWith(std::string(view), KEYWORD_ILLUMINATION_MODEL)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'illum' keyword");
                return false;
            }

            int illuminationModel;
            if (!ProcessTagIlluminationModel(line, &illuminationModel)) {
                return false;
            }

            currentMaterial->SetIlluminationModel(illuminationModel);
            int model;
            currentMaterial->GetIlluminationModel(&model);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|ILLUMINATION MODEL => {}", model));

        // Ambient texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_AMBIENT_TEXTURE_MAP)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'map_Ka' keyword");
                return false;
            }

            std::string ambientTextureMap;
            if (!ProcessTagAmbientTextureMap(line, &ambientTextureMap)) {
                return false;
            }

            currentMaterial->SetAmbientTextureMap(ambientTextureMap);
            std::string textureMap;
            currentMaterial->GetAmbientTextureMap(&textureMap);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|AMBIENT TEXTURE MAP => {}", textureMap));

        // Diffuse texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_DIFFUSE_TEXTURE_MAP)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'map_Kd' keyword");
                return false;
            }

            std::string diffuseTextureMap;
            if (!ProcessTagDiffuseTextureMap(line, &diffuseTextureMap)) {
                return false;
            }

            currentMaterial->SetDiffuseTextureMap(diffuseTextureMap);
            std::string textureMap;
            currentMaterial->GetDiffuseTextureMap(&textureMap);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|DIFFUSE TEXTURE MAP => {}", textureMap));

        // Specular color texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_SPECULAR_COLOR_TEXTURE_MAP)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'map_Ks' keyword");
                return false;
            }

            std::string colourTextureMap;
            if (!ProcessTagSpecularColorTextureMap(line, &colourTextureMap)) {
                return false;
            }

            currentMaterial->SetSpecularColourTextureMap(
                colourTextureMap);
            std::string textureMap;
            currentMaterial->GetSpecularColourTextureMap(&textureMap);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|SPECULAR COLOUR TEXTURE MAP => {}", textureMap));

        // Specular highlight component
        } else if (StartsWith(std::string(view),
                              KEYWORD_SPECULAR_HIGHLIGHT_COMPONENT)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'map_Ns' keyword");
                return false;
            }

            std::string highlightComponent;
            if (!ProcessTagSpecularHighlightConponent(line,
                                                       &highlightComponent)) {
                return false;
            }

            currentMaterial->SetSpecularHighlightComponent(
                highlightComponent);
            std::string component;
            currentMaterial->GetSpecularHighlightComponent(
                &component);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|SPECULAR HIGHLIGHT COMPONENT => {}", component));

        // Alpha texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_ALPHA_TEXTURE_MAP)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'map_d' keyword");
                return false;
            }

            std::string alphaTextureMap;
            if (!ProcessTagAlphaTextureMap(line, &alphaTextureMap)) {
                return false;
            }

            currentMaterial->SetAlphaTextureMap(alphaTextureMap);
            std::string textureMap;
            currentMaterial->GetAlphaTextureMap(&textureMap);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|ALPHA TEXTURE MAP => {}", textureMap));

        // Bump map
        } else if ((StartsWith(std::string(view),
                               KEYWORD_MAP_BUMP)) ||
                   (StartsWith(std::string(view),
                               KEYWORD_BUMP_MAP))) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical,
                    "Mis-ordered 'bump/map_bump' keyword");
                return false;
            }

            std::string bumpMap;
            if (!ProcessTagBumpMap(line, &bumpMap)) {
                return false;
            }

            currentMaterial->SetBumpMap(bumpMap);
            std::string textureMap;
            currentMaterial->GetBumpMap(&textureMap);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|BUMP MAP => {}", textureMap));

        // Displacement map
        } else if (StartsWith(std::string(view),
                              KEYWORD_DISPLACEMENT_MAP)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'disp' keyword");
                return false;
            }

            std::string displacementMap;
            if (!ProcessTagDisplacementMap(line, &displacementMap)) {
                return false;
            }

            currentMaterial->SetDisplacementMap(displacementMap);
            std::string map;
            currentMaterial->GetDisplacementMap(&map);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|DISPLACEMENT MAP => {}", map));

       // Stencil decal texture
        } else if (StartsWith(std::string(view),
                              KEYWORD_STENCIL_DECAL_TEXTURE)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'decal' keyword");
                return false;
            }

            std::string decalTexture;
            if (!ProcessTagStencilDecalTexture(line, &decalTexture)) {
                return false;
            }

            currentMaterial->SetStencilDecalTexture(decalTexture);
            std::string texture;
            currentMaterial->GetStencilDecalTexture(&texture);
            LOG(Logger::LogLevel::Debug, std::format(
                "MATERIAL|STENCIL DECAL TEXTURE => {}", texture));

        // Unknown tag : Doesn't mean it's invalid, it could be a tag that
        //               currently isn't parsed.
        } else {
            LOG(Logger::LogLevel::Debug, std::format(
                "Unknown material tag '{}'", view));
        }
    }

    return true;
}

/**
 * @brief Parses a new material tag line and extracts the material name.
 *
 * @param line The input line containing the new material definition.
 * @param material Reference to a string to store the parsed material name.
 * @return true if parsing was successful, false otherwise.
 */
bool MaterialLibraryParser::ProcessTagNewMaterial(std::string_view line,
                                                  std::string *material) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "New material line is invalid: '{}'", line));
        return false;
    }

    *material = words[1];

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
                                                    RGB *colour) {
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

    *colour = RGB(red, green, blue);

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
                                                    RGB *colour) {
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

    *colour = RGB(red, green, blue);

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
                                                     RGB *colour) {
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

    *colour = RGB(red, green, blue);

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
                                                     RGB *colour) {
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

    *colour = RGB(red, green, blue);

    return true;
}

/**
 * Parses an 'Ns' (specular exponent) line from a material library (.mtl) file
 * and extracts the shininess value.
 *
 * The specular exponent controls the focus of specular highlights on the
 * material. Higher values result in smaller, sharper highlights, simulating
 * glossier surfaces. The line must contain exactly two tokens: the keyword
 * 'Ns' and a float value.
 *
 * @param line The input line from the .mtl file (e.g., "Ns 96.078431").
 * @param shininess Pointer to a float where the extracted shininess value will
 *                   be stored.
 * @return true if the line is well-formed and the value was successfully parsed;
 *         false otherwise.
 */
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

/**
 * @brief Parses the transparency (dissolve) value from a material tag.
 *
 * This function reads a dissolve value from the input line and stores it in
 * the provided `transparency` pointer. The dissolve value defines the
 * material's transparency:
 *
 *   - 1.0 means fully opaque
 *   - 0.0 means fully transparent
 *
 * Valid values range from 0.0 to 1.0.
 *
 * @param line The input line containing the transparency value.
 * @param transparency Pointer to a float where the parsed value will be
 *                     stored.
 * @return true if parsing and validation succeed, false otherwise.
 */
bool MaterialLibraryParser::ProcessTagTransparentDissolve(std::string_view line,
                                                          float *transparency) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical,
            "Material transparent dissolve invalid");
        return false;
    }

    if (!ParseFloat(words[1].c_str(), transparency)) return false;

    if ((*transparency < 0.0f) || (*transparency > 1.0f)) {
        LOG(Logger::LogLevel::Critical,
            "Material transparent dissolve invalid value");
        return false;
    }

    return true;
}

/**
 * @brief Parses the optical density (index of refraction) from a line.
 *
 * This function extracts the optical density value (Ni) from a given line
 * and stores it in the provided `density` pointer. Optical density affects
 * how much light bends as it enters a material, commonly used for transparent
 * materials like glass or water.
 *
 * Valid values range from 0.001 to 10.0. Typical real-world values fall
 * between 1.0 and 2.0. Examples include:
 *   - Air: 1.0
 *   - Water: 1.33
 *   - Glass: 1.5
 *   - Diamond: 2.42
 *
 * @param line The input line containing the optical density value.
 * @param density Pointer to a float where the parsed value will be stored.
 * @return true if parsing and validation succeed, false otherwise.
 */
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

/*
 * Parses the illumination model from a line in a material library file.
 *
 * The "illum" tag specifies how the surface is illuminated. The model is
 * defined by an integer value ranging from 0 to 10, each representing a
 * specific shading/lighting mode:
 *
 *   0  - Color on, ambient off
 *   1  - Color on, ambient on
 *   2  - Highlight on
 *   3  - Reflection on and ray trace on
 *   4  - Transparency: Glass on, Reflection: Ray trace on
 *   5  - Reflection: Fresnel on and ray trace on
 *   6  - Transparency: Refraction on, Reflection: Fresnel off and ray trace on
 *   7  - Transparency: Refraction on, Reflection: Fresnel on and ray trace on
 *   8  - Reflection on and ray trace off
 *   9  - Transparency: Glass on, Reflection: Ray trace off
 *  10  - Casts shadows onto invisible surfaces
 *
 * Parameters:
 *   line    - The input line containing the "illum" tag and its value.
 *   density - Pointer to store the parsed illumination model index.
 *
 * Returns:
 *   true if the illumination model is parsed and valid; false otherwise.
 */
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

/**
 * Parses a 'map_ka' (ambient texture map) line from a material
 * library (.mtl) file and extracts the associated texture filename.
 *
 * The ambient texture map defines the material’s response to ambient light,
 * typically used to simulate indirect or environmental lighting.
 * The line must have exactly two tokens: the keyword and the texture filename.
 * If valid, the texture filename is assigned to the output string.
 *
 * @param line The input line from the .mtl file (e.g., "map_ka ambient.jpg").
 * @param map Pointer to a string where the extracted texture filename will be
 *            stored.
 * @return true if the line is well-formed and the filename was successfully
 *              parsed;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagAmbientTextureMap(std::string_view line,
                                                        std::string *map) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Ambient texture map is invalid: '{}'", line));
        return false;
    }

    *map = words[1];

    return true;
}

/**
 * Parses a 'map_kd' (diffuse texture map) line from a material
 * library (.mtl) file and extracts the associated texture filename.
 *
 * The diffuse texture map defines the base color of the material’s surface
 * under diffuse lighting. This line must contain exactly two components:
 * the keyword and the texture filename. If the format is valid, the filename
 * is assigned to the provided string pointer.
 *
 * @param line The line from the .mtl file (e.g., "map_kd brick_diffuse.jpg").
 * @param map Pointer to a string where the extracted filename will be stored.
 * @return true if the line is correctly formatted and the filename was extracted;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagDiffuseTextureMap(std::string_view line,
                                                        std::string *map) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Diffuse texture map is invalid: '{}'", line));
        return false;
    }

    *map = words[1];

    return true;
}

/**
 * Parses a 'map_ks' (specular color texture map) line from a material
 * library (.mtl) file and extracts the associated texture filename.
 *
 * The specular color map controls the intensity and color of specular
 * reflections on the surface of the material. This line should have exactly
 * two elements: the keyword and the texture filename.
 * If valid, the filename is stored in the provided string pointer.
 *
 * @param line The line from the .mtl file (e.g., "map_ks specular_color.png").
 * @param map Pointer to a string where the extracted filename will be stored.
 * @return true if the line is correctly formatted and the texture filename was
 *              extracted;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagSpecularColorTextureMap(
    std::string_view line, std::string *map) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Specular texture map is invalid: '{}'", line));
        return false;
    }

    *map = words[1];

    return true;
}

/**
 * Parses a 'map_ns' (specular highlight component) line from a material
 * library (.mtl) file and extracts the corresponding texture map filename.
 *
 * This map typically defines the shininess level or specular exponent map
 * used in the material shading process.
 * The line should contain exactly two parts: the keyword and the texture
 * filename. If valid, the filename is stored in the provided string pointer.
 *
 * @param line The line from the .mtl file (e.g., "map_ns specular_map.png").
 * @param component Pointer to a string where the extracted filename will be 
 *                  stored.
 * @return true if the specular highlight component line is valid and
 *              successfully parsed;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagSpecularHighlightConponent(
    std::string_view line, std::string *component) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Specular highlight conponent is invalid: '{}'", line));
        return false;
    }

    *component = words[1];

    return true;
}

/**
 * Parses a 'map_d' (alpha texture map) line from a material library (.mtl)
 * file and extracts the associated texture map filename.
 *
 * This line typically specifies the transparency texture used in a material.
 * It should consist of exactly two parts: the keyword and the filename.
 * If valid, the filename is stored in the provided string pointer.
 *
 * @param line The line from the .mtl file (e.g., "map_d alpha_map.png").
 * @param map Pointer to a string where the extracted texture map filename
 *            will be stored.
 * @return true if the alpha texture map line is valid and successfully parsed;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagAlphaTextureMap(std::string_view line,
                                                       std::string *map) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Alpha texture map is invalid: '{}'", line));
        return false;
    }

    *map = words[1];

    return true;
}

/**
 * Parses a 'bump' (bump map) line from a material library (.mtl) file
 * and extracts the bump map filename.
 *
 * The line should contain exactly two elements: the 'bump' keyword and the
 * associated bump map filename. If the format is valid, the filename is
 * stored in the provided string pointer.
 *
 * @param line The line from the .mtl file (e.g., "bump normal_map.png").
 * @param map Pointer to a string where the extracted bump map filename
 *            will be saved.
 * @return true if the bump map line is valid and successfully parsed;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagBumpMap(std::string_view line,
                                              std::string *map) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Bump map is invalid: '{}'", line));
        return false;
    }

    *map = words[1];

    return true;
}

/**
 * Parses a 'disp' (displacement map) line from a material library (.mtl) file
 * and extracts the displacement map filename.
 *
 * The input line must consist of exactly two words: the 'disp' keyword
 * followed by the displacement map filename. If valid, the filename is
 * stored in the output string.
 *
 * @param line The line from the .mtl file (e.g., "disp height.png").
 * @param map Pointer to a string where the parsed displacement map filename
 *            will be stored.
 * @return true if the line is correctly formatted and parsing is successful;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagDisplacementMap(std::string_view line,
                                                      std::string *map) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Displacement map is invalid: '{}'", line));
        return false;
    }

    *map = words[1];

    return true;
}

/**
 * Parses a 'decal' line from a material library (.mtl) file and extracts
 * the stencil decal texture filename.
 *
 * The input line must consist of exactly two words: the 'decal' keyword
 * followed by the texture filename. If valid, the filename is written to
 * the provided output pointer.
 *
 * @param line The line from the .mtl file (e.g., "decal stencil.png").
 * @param texture Pointer to a string where the parsed texture filename will
 *                be stored.
 * @return true if the line is correctly formatted and parsing succeeds;
 *         false otherwise.
 */
bool MaterialLibraryParser::ProcessTagStencilDecalTexture(
    std::string_view line, std::string *texture) {
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, std::format(
            "Stencil decal texture is invalid: '{}'", line));
        return false;
    }

    *texture = words[1];

    return true;
}

}   // namespace Meshborn
