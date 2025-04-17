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
#ifndef WAVEFRONT_MATERIALLIBRARYPARSER_H_
#define WAVEFRONT_MATERIALLIBRARYPARSER_H_
#include <memory>
#include <string>
#include <unordered_map>
#include "BaseWavefrontParser.h"
#include "Material.h"

namespace Meshborn {
namespace WaveFront {

using MaterialMap = std::unordered_map<std::string, std::shared_ptr<Material>>;


class MaterialLibraryParser : public BaseWavefrontParser {
 public:
    MaterialLibraryParser();

    bool ParseLibrary(std::string materialFile, MaterialMap *materials);

 private:
    bool ProcessTagNewMaterial(std::string_view line, std::string *material);

    bool ProcessTagAmbientColour(std::string_view line, RGB *colour);
    bool ProcessTagDiffuseColour(std::string_view line, RGB *colour);
    bool ProcessTagEmissiveColour(std::string_view line, RGB *colour);
    bool ProcessTagSpecularColour(std::string_view line, RGB *colour);

    bool ProcessTagSpecularExponent(std::string_view line, float *shininess);

    bool ProcessTagTransparentDissolve(std::string_view line,
                                       float *transparency);

    bool ProcessTagOpticalDensity(std::string_view line, float *density);

    bool ProcessTagIlluminationModel(std::string_view line, int *density);

    bool ProcessTagAmbientTextureMap(std::string_view line, std::string *map);

    bool ProcessTagDiffuseTextureMap(std::string_view line, std::string *map);

    bool ProcessTagSpecularColorTextureMap(std::string_view line,
                                           std::string *map);

    bool ProcessTagSpecularHighlightConponent(std::string_view line,
                                              std::string *component);

    bool ProcessTagAlphaTextureMap(std::string_view line, std::string *map);

    bool ProcessTagBumpMap(std::string_view line, std::string *map);

    bool ProcessTagDisplacementMap(std::string_view line, std::string *map);

    bool ProcessTagStencilDecalTexture(std::string_view line,
                                       std::string *texture);
};

}   // namespace WaveFront
}   // namespace Meshborn

#endif  // WAVEFRONT_MATERIALLIBRARYPARSER_H_
