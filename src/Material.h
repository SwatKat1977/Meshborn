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
};

}   // namespace Meshborn

#endif  // MATERIAL_H_
