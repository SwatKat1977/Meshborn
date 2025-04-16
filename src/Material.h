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
#ifndef MATERIAL_H_
#define MATERIAL_H_
#include <string>
#include "glm/glm.hpp"

namespace Meshborn {

class Material {
 public:
    explicit Material(std::string name);

    std::string GetName();

    void SetAmbientColour(glm::vec3 colour);
    bool GetAmbientColour(glm::vec3 *colour);

    void SetDiffuseColour(glm::vec3 colour);
    bool GetDiffuseColour(glm::vec3 *colour);

    void SetEmissiveColour(glm::vec3 colour);
    bool GetEmissiveColour(glm::vec3 *colour);

    void SetSpecularColour(glm::vec3 colour);
    bool GetSpecularColour(glm::vec3 *colour);

    void SetIlluminationModel(int model);
    bool GetIlluminationModel(int *model);

    void SetOpticalDensity(float density);
    bool GetOpticalDensity(float *density);

    void SetTransparentDissolve(float transparency);
    bool GetTransparentDissolve(float *transparency);

    void SetAmbientTextureMap(std::string map);
    bool GetAmbientTextureMap(std::string *map);

    void SetDiffuseTextureMap(std::string map);
    bool GetDiffuseTextureMap(std::string *map);

    void SetSpecularColourTextureMap(std::string map);
    bool GetSpecularColourTextureMap(std::string *map);

    void SetSpecularHighlightComponent(std::string map);
    bool GetSpecularHighlightComponent(std::string *map);

    void SetAlphaTextureMap(std::string map);
    bool GetAlphaTextureMap(std::string *map);

    void SetBumpMap(std::string map);
    bool GetBumpMap(std::string *map);

    void SetDisplacementMap(std::string map);
    bool GetDisplacementMap(std::string *map);

    void SetStencilDecalTexture(std::string map);
    bool GetStencilDecalTexture(std::string *map);

 private:
    std::string name_;

    // Ambient colour
    glm::vec3 ambientColour_;
    bool ambientColourSet_;

    // Diffuse colour
    glm::vec3 diffuseColour_;
    bool diffuseColourSet_;

    // Emissive colour
    glm::vec3 emissiveColour_;
    bool emissiveColourSet_;

    // Specular colour
    glm::vec3 specularColour_;
    bool specularColourSet_;

    // Illumination model
    int illuminationModel_;
    bool illuminationModelSet_;

    // Optical density
    float opticalDensity_;
    bool opticalDensitySet_;

    // Transparent dissolve
    float transparentDissolve_;
    bool transparentDissolveSet_;

    // Ambient texture map
    std::string ambientTextureMap_;
    bool ambientTextureMapSet_;

    // Diffuse texture map
    std::string diffuseTextureMap_;
    bool diffuseTextureMapSet_;

    // Specular colour texture map
    std::string specularColourTextureMap_;
    bool specularColourTextureMapSet_;

    // Specular highlight component
    std::string specularHighlightComponent_;
    bool specularHighlightComponentSet_;

    // Alpha texture map
    std::string alphaTextureMap_;
    bool alphaTextureMapSet_;

    // Bump map
    std::string bumpMap_;
    bool bumpMapSet_;

    // Displacement map
    std::string displacementMap_;
    bool displacementMapSet_;

    // Stencil decal texture
    std::string stencilDecalTexture_;
    bool stencilDecalTextureSet_;
};

}   // namespace Meshborn

#endif  // MATERIAL_H_
