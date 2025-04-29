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

/**
 * @class Model
 * @brief Represents a 3D model composed of multiple meshes and materials.
 *
 * The Model class stores a collection of Mesh objects and associated
 * materials.
 * It tracks the total number of meshes and materials explicitly for
 * convenience.
 */
class Model {
 public:
    /**
    * @brief Constructs an empty Model with zero meshes and materials.
    */
    Model() : totalMeshes(0), totalMaterials(0) {}

    /**
     * @brief A list of meshes that make up the model.
     */
    std::vector<Mesh> meshes;

    /**
     * @brief The total number of meshes in the model.
     *
     * This is typically equal to meshes.size(), but may be cached or used for
     * external tracking.
     */
    size_t totalMeshes;

    /**
     * @brief A mapping of material identifiers to material data.
     *
     * Used to look up materials associated with the meshes in the model.
     */
    MaterialMap materials;

    /**
     * @brief The total number of materials used in the model.
     *
     * This is typically equal to materials.size(), but may be cached or used
     * for external tracking.
     */
    size_t totalMaterials;
};

}   // namespace Meshborn

#endif  // MODEL_H_
