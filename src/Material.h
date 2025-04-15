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
    Material(std::string name);

    void SetAmbientColour(glm::vec3 colour);
    bool GetAmbientColour(glm::vec3 &colour);

private:
    std::string name_;
    glm::vec3 ambientColour_;
    bool ambientColourSet_;
};

}   // namespace Meshborn

#endif  // MATERIAL_H_
