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

    /**
 * @brief Constructs a new Material with the given name.
 * 
 * Initializes the ambient colour to black (0, 0, 0) and marks it as unset.
 * 
 * @param name The name of the material.
 */
Material::Material(std::string name) : name_(name) {
    ambientColour_ = glm::vec3(0.0f, 0.0f, 0.0f);
    ambientColourSet_ = false;

    diffuseColour_ = glm::vec3(0.0f, 0.0f, 0.0f);
    diffuseColourSet_ = false;

    specularColour_ = glm::vec3(0.0f, 0.0f, 0.0f);
    specularColourSet_ = false;
}

/**
 * @brief Sets the ambient colour of the material.
 * 
 * @param colour A glm::vec3 representing the ambient RGB colour values.
 */
void Material::SetAmbientColour(glm::vec3 colour) {
    ambientColour_ = colour;
    ambientColourSet_ = true;
}

/**
 * @brief Retrieves the ambient colour of the material.
 * 
 * @param[out] colour The ambient colour will be stored here if set.
 * @return true if the ambient colour was set and returned; false otherwise.
 */
bool Material::GetAmbientColour(glm::vec3 *colour) {
    if (!ambientColourSet_) {
        return false;
    }

    *colour = ambientColour_;
    return true;
}

/**
 * @brief Sets the diffuse colour of the material.
 * 
 * @param colour A glm::vec3 representing the diffuse RGB colour values.
 */
void Material::SetDiffuseColour(glm::vec3 colour) {
    diffuseColour_ = colour;
    diffuseColourSet_ = true;
}

/**
 * @brief Retrieves the diffuse colour of the material.
 * 
 * @param[out] colour The diffuse colour will be stored here if set.
 * @return true if the diffuse colour was set and returned; false otherwise.
 */
bool Material::GetDiffuseColour(glm::vec3 *colour) {
    if (!diffuseColourSet_) {
        return false;
    }

    *colour = diffuseColour_;
    return true;
}

void Material::SetEmissiveColour(glm::vec3 colour) {
    emissiveColour_ = colour;
    emissiveColourSet_ = true;
}

bool Material::GetEmissiveColour(glm::vec3 *colour) {
    if (!emissiveColourSet_) {
        return false;
    }

    *colour = emissiveColour_;
    return true;
}

/**
 * @brief Sets the specular colour of the material.
 *
 * This method assigns the given RGB colour vector to the material's specular
 * colour and marks it as explicitly set.
 *
 * @param colour The RGB colour value to use as the material's specular colour.
 */
void Material::SetSpecularColour(glm::vec3 colour) {
    specularColour_ = colour;
    specularColourSet_ = true;
}

/**
 * @brief Retrieves the specular colour of the material, if set.
 *
 * If the specular colour has been set previously, this method copies the stored
 * colour into the provided reference and returns true. If not, it returns false
 * without modifying the reference.
 *
 * @param[out] colour Reference to a glm::vec3 to receive the specular colour.
 * @return true if the specular colour has been set, false otherwise.
 */
bool Material::GetSpecularColour(glm::vec3 *colour) {
    if (!specularColourSet_) {
        return false;
    }

    *colour = specularColour_;
    return true;
}

void Material::SetIlluminationModel(int model) {
    illuminationModel_ = model;
    illuminationModelSet_ = true;
}

bool Material::GetIlluminationModel(int *model) {
    if (!illuminationModelSet_) {
        return false;
    }

    *model = illuminationModel_;
    return true;
}

void Material::SetOpticalDensity(float density) {
    opticalDensity_ = density;
    opticalDensitySet_ = true;
}

bool Material::GetOpticalDensity(float *density) {
    if (!opticalDensitySet_) {
        return false;
    }

    *density = opticalDensity_;
    return true;
}


}   // namespace Meshborn
