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

            auto status = ProcessTagTransparentDissolve(line,
                                                        &transparentDissolve);
            if (status == ParseResult::Failure) {
                return ParseResult::Failure;

            } else if (status == ParseResult::Incomplete) {
                /* code */

            } else {
                currentMaterial->SetTransparentDissolve(
                    transparentDissolve);
                float transparency;
                currentMaterial->GetTransparentDissolve(&transparency);
                LOG(Logger::LogLevel::Debug, std::format(
                    "MATERIAL|TRANSPARENT DISSOLVE => {}", transparency));
            }

        // Optical density
        } else if (StartsWith(std::string(view), KEYWORD_OPTICAL_DENSITY)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'Ks' keyword");
                return ParseResult::Failure;
            }

            float opticalDensity;
            auto status = ProcessTagOpticalDensity(line, &opticalDensity);
            if (status == ParseResult::Failure) {
                return ParseResult::Failure;

            } else if (status == ParseResult::Incomplete) {
                /* code */

            } else {
                currentMaterial->SetOpticalDensity(opticalDensity);
                float density;
                currentMaterial->GetOpticalDensity(&density);
                LOG(Logger::LogLevel::Debug, std::format(
                    "MATERIAL|OPTICAL DENSITY => {}", density));
            }

        // Illumination model
        } else if (StartsWith(std::string(view), KEYWORD_ILLUMINATION_MODEL)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'illum' keyword");
                return ParseResult::Failure;
            }

            int illuminationModel;
            auto status = ProcessTagIlluminationModel(line, &illuminationModel);
            if (status == ParseResult::Failure) {
                return ParseResult::Failure;

            } else if (status == ParseResult::Incomplete) {
                /* code */

            } else {
                currentMaterial->SetIlluminationModel(illuminationModel);
                int model;
                currentMaterial->GetIlluminationModel(&model);
                LOG(Logger::LogLevel::Debug, std::format(
                    "MATERIAL|ILLUMINATION MODEL => {}", model));
            }

        // Ambient texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_AMBIENT_TEXTURE_MAP)) {
            if (!currentMaterial) {
                LOG(Logger::LogLevel::Critical, "Mis-ordered 'map_Ka' keyword");
                return ParseResult::Failure;
            }

            std::string ambientTextureMap;
            if (!ProcessTagAmbientTextureMap(line, &ambientTextureMap)) {
                return ParseResult::Failure;
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
                return ParseResult::Failure;
            }

            std::string diffuseTextureMap;
            if (!ProcessTagDiffuseTextureMap(line, &diffuseTextureMap)) {
                return ParseResult::Failure;
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
                return ParseResult::Failure;
            }

            std::string colourTextureMap;
            if (!ProcessTagSpecularColorTextureMap(line, &colourTextureMap)) {
                return ParseResult::Failure;
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
                return ParseResult::Failure;
            }

            std::string highlightComponent;
            if (!ProcessTagSpecularHighlightConponent(line,
                                                       &highlightComponent)) {
                return ParseResult::Failure;
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
                return ParseResult::Failure;
            }

            std::string alphaTextureMap;
            if (!ProcessTagAlphaTextureMap(line, &alphaTextureMap)) {
                return ParseResult::Failure;
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
                return ParseResult::Failure;
            }

            std::string bumpMap;
            if (!ProcessTagBumpMap(line, &bumpMap)) {
                return ParseResult::Failure;
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
                return ParseResult::Failure;
            }

            std::string displacementMap;
            if (!ProcessTagDisplacementMap(line, &displacementMap)) {
                return ParseResult::Failure;
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
                return ParseResult::Failure;
            }

            std::string decalTexture;
            if (!ProcessTagStencilDecalTexture(line, &decalTexture)) {
                return ParseResult::Failure;
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

    return ParseResult::Success;
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
 * @brief Parses and validates the ambient colour tag from a material line.
 *
 * This function extracts and validates RGB components from the input line,
 * ensuring the values are valid floats and fall within the accepted range
 * defined by AMBIENT_COLOUR_MIN and AMBIENT_COLOUR_MAX.
 *
 * The input line must contain exactly four elements:
 *   - The first word is the tag name (e.g. "Ka") and is ignored
 *   - The remaining three are the red, green, and blue float values
 *
 * If float parsing fails or the number of elements is incorrect, the
 * function returns failure. If all floats are parsed but at least one
 * value is out of range, the result is incomplete.
 *
 * @param line   A string_view representing the material line containing the
 *               ambient colour definition.
 * @param colour A pointer to an RGB object that will be populated with the
 *               parsed values if successful.
 *
 * @return ParseResult::Success if parsing succeeded and all values are
 *         within range.
 *         ParseResult::Incomplete if values were parsed but at least one is
 *         out of range.
 *         ParseResult::Failure if parsing failed due to format or conversion
 *         issues.
 */
bool MaterialLibraryParser::ProcessTagAmbientColour(
                std::string_view line, RGB *colour) {
    bool invalidValue = false;
    auto words = SplitElementString(std::string(line));

    if (words.size() != 4) {
        LOG(Logger::LogLevel::Critical,
            "Material ambient colour invalid elements");
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) {
        LOG(Logger::LogLevel::Critical,
            "Material ambient colour invalid red elements");
        return false;
    }

    if (!ParseFloat(words[2].c_str(), &green)) {
        LOG(Logger::LogLevel::Critical,
            "Material ambient colour invalid green elements");
        return false;
    }

    if (!ParseFloat(words[3].c_str(), &blue)) {
        LOG(Logger::LogLevel::Critical,
            "Material ambient colour invalid blue elements");
        return false;
    }

    *colour = RGB(red, green, blue);

    return true;
}

/**
 * @brief Parses and validates the diffuse colour tag from a material line.
 *
 * This function extracts and validates RGB components from the input line,
 * ensuring the values are valid floats and fall within the accepted range
 * defined by DIFFUSE_COLOUR_MIN and DIFFUSE_COLOUR_MAX.
 *
 * The input line must contain exactly four elements:
 *   - The first word is the tag name (e.g. "Kd") and is ignored
 *   - The remaining three are the red, green, and blue float values
 *
 * If float parsing fails or the number of elements is incorrect, the
 * function returns failure. If all floats are parsed but at least one
 * value is out of range, the result is incomplete.
 *
 * @param line   A string_view representing the material line containing the
 *               diffuse colour definition.
 * @param colour A pointer to an RGB object that will be populated with the
 *               parsed values if successful.
 *
 * @return ParseResult::Success if parsing succeeded and all values are
 *         within range.
 *         ParseResult::Incomplete if values were parsed but at least one is
 *         out of range.
 *         ParseResult::Failure if parsing failed due to format or conversion
 *         issues.
 */
bool MaterialLibraryParser::ProcessTagDiffuseColour(
                          std::string_view line, RGB *colour) {
    bool invalidValue = false;
    auto words = SplitElementString(std::string(line));

    if (words.size() != 4) {
        LOG(Logger::LogLevel::Critical,
            "Material diffuse colour invalid elements");
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) {
        LOG(Logger::LogLevel::Critical,
            "Material diffuse colour invalid red elements");
        return false;
    }

    if (!ParseFloat(words[2].c_str(), &green)) {
        LOG(Logger::LogLevel::Critical,
            "Material diffuse colour invalid green elements");
        return false;
    }

    if (!ParseFloat(words[3].c_str(), &blue)) {
        LOG(Logger::LogLevel::Critical,
            "Material diffuse colour invalid blue elements");
        return false;
    }

    *colour = RGB(red, green, blue);

    return true;
}

/**
 * @brief Parses and validates the emissive colour tag from a material
 *        definition line.
 *
 * This function extracts and validates RGB components from the given input
 * line, ensuring the values are parsable floats and within the valid emissive
 * colour range defined by `EMISSSIVE_COLOUR_MIN` and `EMISSSIVE_COLOUR_MAX`.
 *
 * The input line is expected to have exactly four elements:
 *   - The first word is ignored (typically the tag name, e.g. "Ke")
 *   - The remaining three are the red, green, and blue float values
 *
 * If any component fails to parse or the number of elements is incorrect, 
 * parsing fails.
 * If the RGB values parse correctly but one or more are out of range, the
 * result is marked as incomplete.
 *
 * @param line   A string_view representing the material line containing the
 *               emissive colour definition.
 * @param colour A pointer to an RGB object where the parsed colour will be
 *               stored if valid.
 *
 * @return ParseResult::Success if parsing succeeded and all values are within
 *                              range.
 *         ParseResult::Incomplete if values were parsed but one or more were
 *                                 out of range.
 *         ParseResult::Failure if parsing failed due to format or conversion
 *                              issues.
 */
bool MaterialLibraryParser::ProcessTagEmissiveColour(
                std::string_view line, RGB *colour) {
    auto words = SplitElementString(std::string(line));
    bool invalidValue = false;

    if (words.size() != 4) {
        LOG(Logger::LogLevel::Critical,
            "Material emissive colour invalid elements");
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) {
        LOG(Logger::LogLevel::Critical,
            "Material emissive colour invalid red elements");
        return false;
    }

    if (!ParseFloat(words[2].c_str(), &green)) {
        LOG(Logger::LogLevel::Warning,
            "Material emissive colour invalid green elements");
        return false;
    }

    if (!ParseFloat(words[3].c_str(), &blue)) {
        LOG(Logger::LogLevel::Critical,
            "Material emissive colour invalid blue elements");
        return false;
    }

    *colour = RGB(red, green, blue);

    return true;
}

/**
 * @brief Parses and validates the specular colour tag from a material line.
 *
 * This function extracts and validates RGB components from the input line,
 * ensuring the values are valid floats and fall within the accepted range
 * defined by SPECULAR_COLOUR_MIN and SPECULAR_COLOUR_MAX.
 *
 * The input line must contain exactly four elements:
 *   - The first word is the tag name (e.g. "Ks") and is ignored
 *   - The remaining three are the red, green, and blue float values
 *
 * If float parsing fails or the number of elements is incorrect, the
 * function returns failure. If all floats are parsed but at least one
 * value is out of range, the result is incomplete.
 *
 * @param line   A string_view representing the material line containing the
 *               specular colour definition.
 * @param colour A pointer to an RGB object that will be populated with the
 *               parsed values if successful.
 *
 * @return ParseResult::Success if parsing succeeded and all values are
 *         within range.
 *         ParseResult::Incomplete if values were parsed but at least one is
 *         out of range.
 *         ParseResult::Failure if parsing failed due to format or conversion
 *         issues.
 */
bool MaterialLibraryParser::ProcessTagSpecularColour(
                std::string_view line, RGB *colour) {
    bool invalidValue = false;
    auto words = SplitElementString(std::string(line));

    if (words.size() != 4) {
        LOG(Logger::LogLevel::Critical,
            "Material specular colour invalid elements");
        return false;
    }

    float red;
    float green;
    float blue;

    if (!ParseFloat(words[1].c_str(), &red)) {
        LOG(Logger::LogLevel::Critical,
            "Material specular colour invalid red elements");
        return false;
    }

    if (!ParseFloat(words[2].c_str(), &green)) {
        LOG(Logger::LogLevel::Critical,
            "Material specular colour invalid green elements");
        return false;
    }

    if (!ParseFloat(words[3].c_str(), &blue)) {
        LOG(Logger::LogLevel::Critical,
            "Material specular colour invalid blue elements");
        return false;
    }

    *colour = RGB(red, green, blue);

    return true;
}

/**
 * @brief Parses and validates the specular exponent (shininess) value from a
 *        material line.
 *
 * This function extracts a single floating-point value representing the
 * specular exponent (shininess) from the material definition line. The line
 * must contain exactly two elements:
 *   - The first word is the tag name (e.g. "Ns") and is ignored.
 *   - The second word is a float in the valid shininess range.
 *
 * If parsing fails or the number of elements is incorrect, the function
 * returns failure. If the value is parsed but outside the valid range
 * (SPECULAR_EXPONENT_MIN to SPECULAR_EXPONENT_MAX), the function returns
 * incomplete.
 *
 * @param line       The line from the material file containing the shininess
 *                   definition.
 * @param shininess  Pointer to a float that will be populated with the parsed
 *                   value on success.
 *
 * @return ParseResult::Success if parsed and within range.  
 *         ParseResult::Incomplete if parsed but out of range.  
 *         ParseResult::Failure on format or conversion errors.
 */
bool MaterialLibraryParser::ProcessTagSpecularExponent(
    std::string_view line, float *shininess) {
    bool invalidValue = false;
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, "Material specular exponent invalid");
        return false;
    }

    if (!ParseFloat(words[1].c_str(), shininess)) {
        LOG(Logger::LogLevel::Critical,
            "Material specular exponent invalid value");
        return false;
    }

    return true;
}

/**
 * @brief Parses and validates the transparent dissolve value from a material
 *        line.
 *
 * This function extracts a single floating-point value representing
 * transparency (dissolve) from the material definition line. The line must
 * contain exactly two elements:
 *   - The first word is the tag name (e.g. "d") and is ignored.
 *   - The second word is a float in the allowed transparency range.
 *
 * If parsing fails or the number of elements is incorrect, the function
 * returns failure. If the value is parsed but outside the valid range
 * (TRANSPARENT_DISSOLVE_MIN to TRANSPARENT_DISSOLVE_MAX), the function
 * returns incomplete.
 *
 * @param line         The line from the material file containing the dissolve
 *                     definition.
 * @param transparency Pointer to a float that will be populated with the
 *                     parsed value on success.
 *
 * @return ParseResult::Success if parsed and within range.
 *         ParseResult::Incomplete if parsed but out of range.
 *         ParseResult::Failure on format or conversion errors.
 */
bool MaterialLibraryParser::ProcessTagTransparentDissolve(
    std::string_view line, float *transparency) {
    bool invalidValue = false;
    auto words = SplitElementString(std::string(line));

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical,
            "Material transparent dissolve invalid");
        return false;
    }

    if (!ParseFloat(words[1].c_str(), transparency)) {
        LOG(Logger::LogLevel::Critical,
            "Transparent dissolve value invalid");
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
ParseResult MaterialLibraryParser::ProcessTagOpticalDensity(
    std::string_view line, float *density) {
    auto words = SplitElementString(std::string(line));
    bool invalidValue = false;

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, "Material optical density invalid");
        return ParseResult::Failure;
    }

    if (!ParseFloat(words[1].c_str(), density)) {
        LOG(Logger::LogLevel::Critical,
            "Optical density value invalid");
        return ParseResult::Failure;
    }

    if (!FloatInRange(*density,
                      OPTICAL_DENSITY_MIN,
                      OPTICAL_DENSITY_MAX)) {
        LOG(Logger::LogLevel::Warning,
            "Material optical density value out of range");
        invalidValue = true;
    }

    return invalidValue ? ParseResult::Incomplete : ParseResult::Success;
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
ParseResult MaterialLibraryParser::ProcessTagIlluminationModel(
    std::string_view line, int *model) {
    auto words = SplitElementString(std::string(line));
    bool invalidValue = false;

    if (words.size() != 2) {
        LOG(Logger::LogLevel::Critical, "Material illumination model invalid");
        return ParseResult::Failure;
    }

    if (!ParseInt(words[1].c_str(), model)) {
        LOG(Logger::LogLevel::Critical,
            "Material illumination model value invalid");
        return ParseResult::Failure;
    }

    if (!IntInRange(*model,
                    ILLUMINATION_MODEL_MIN,
                    ILLUMINATION_MODEL_MAX)) {
        LOG(Logger::LogLevel::Warning,
            "Material illumination model invalid value (range 0-10)");
        invalidValue = true;
    }

    return invalidValue ? ParseResult::Incomplete : ParseResult::Success;
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
