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
#ifndef MODEL_H_
#define MODEL_H_
#include <map>
#include <vector>
#include "Material.h"
#include "Mesh.h"

namespace Meshborn {
namespace WaveFront {

class Model {
 public:
    Model() : totalMeshes(0), totalMaterials(0) {}

    std::vector<Mesh> meshes;
    size_t totalMeshes;

    MaterialMap materials;
    size_t totalMaterials;
};

}   // namespace WaveFront
}   // namespace Meshborn

#endif  // MODEL_H_
