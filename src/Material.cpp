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
#include "Material.h"

namespace Meshborn {

Material::Material(std::string name) : name_(name) {
    glm::vec3 ambientColour_ = glm::vec3(0.0f, 0.0f, 0.0f);
    ambientColourSet_ = false;
}

void Material::SetAmbientColour(glm::vec3 colour) {
    ambientColour_ = colour;
    ambientColourSet_ = true;
}

bool Material::GetAmbientColour(glm::vec3 &colour) {
    if (!ambientColourSet_) {
        return false;
    }

    colour = ambientColour_;
    return true;
}

}   // namespace Meshborn
    