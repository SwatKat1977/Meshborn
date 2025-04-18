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
    ambientColour_ = RGB(0.0f, 0.0f, 0.0f);
    ambientColourSet_ = false;

    diffuseColour_ = RGB(0.0f, 0.0f, 0.0f);
    diffuseColourSet_ = false;

    specularColour_ = RGB(0.0f, 0.0f, 0.0f);
    specularColourSet_ = false;
}

std::string Material::GetName() {
    return name_;
}

/**
 * @brief Sets the ambient colour of the material.
 * 
 * @param colour A RGB representing the ambient RGB colour values.
 */
void Material::SetAmbientColour(RGB colour) {
    ambientColour_ = colour;
    ambientColourSet_ = true;
}

/**
 * @brief Retrieves the ambient colour of the material.
 * 
 * @param[out] colour The ambient colour will be stored here if set.
 * @return true if the ambient colour was set and returned; false otherwise.
 */
bool Material::GetAmbientColour(RGB *colour) {
    if (!ambientColourSet_) {
        return false;
    }

    *colour = ambientColour_;
    return true;
}

/**
 * @brief Sets the diffuse colour of the material.
 * 
 * @param colour A RGB representing the diffuse RGB colour values.
 */
void Material::SetDiffuseColour(RGB colour) {
    diffuseColour_ = colour;
    diffuseColourSet_ = true;
}

/**
 * @brief Retrieves the diffuse colour of the material.
 * 
 * @param[out] colour The diffuse colour will be stored here if set.
 * @return true if the diffuse colour was set and returned; false otherwise.
 */
bool Material::GetDiffuseColour(RGB *colour) {
    if (!diffuseColourSet_) {
        return false;
    }

    *colour = diffuseColour_;
    return true;
}

/**
 * @brief Sets the emissive color for the material.
 *
 * The emissive color defines how much light the material emits, independent
 * of lighting. This makes the material appear self-illuminating.
 *
 * @param colour The emissive color to set (represented as a glm::vec3).
 */
void Material::SetEmissiveColour(RGB colour) {
    emissiveColour_ = colour;
    emissiveColourSet_ = true;
}

/**
 * @brief Retrieves the emissive color for the material.
 *
 * Returns the stored emissive color if it has been set. Otherwise, returns
 * false.
 *
 * @param colour Pointer to a glm::vec3 to store the emissive color.
 * @return true if the color was set and retrieved, false otherwise.
 */
bool Material::GetEmissiveColour(RGB *colour) {
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
void Material::SetSpecularColour(RGB colour) {
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
bool Material::GetSpecularColour(RGB *colour) {
    if (!specularColourSet_) {
        return false;
    }

    *colour = specularColour_;
    return true;
}

/**
 * @brief Sets the illumination model for the material.
 *
 * This method sets the illumination model, which defines how the material
 * interacts with light (e.g., diffuse, specular, reflection).
 *
 * @param model The illumination model index to set.
 */
void Material::SetIlluminationModel(int model) {
    illuminationModel_ = model;
    illuminationModelSet_ = true;
}

/**
 * @brief Retrieves the current illumination model.
 *
 * Returns the stored illumination model if it has been set. Otherwise,
 * returns false.
 *
 * @param model Pointer to an integer where the model will be stored.
 * @return true if the model was set and retrieved, false otherwise.
 */
bool Material::GetIlluminationModel(int *model) {
    if (!illuminationModelSet_) {
        return false;
    }

    *model = illuminationModel_;
    return true;
}

/**
 * @brief Sets the optical density (index of refraction) of the material.
 *
 * Optical density affects how much light bends as it enters the material.
 * Common values range from 1.0 to 2.0, depending on the material type.
 *
 * @param density The optical density value to set.
 */
void Material::SetOpticalDensity(float density) {
    opticalDensity_ = density;
    opticalDensitySet_ = true;
}

/**
 * @brief Retrieves the current optical density value.
 *
 * Returns the stored optical density if it has been set. Otherwise,
 * returns false.
 *
 * @param density Pointer to a float where the value will be stored.
 * @return true if the value was set and retrieved, false otherwise.
 */
bool Material::GetOpticalDensity(float *density) {
    if (!opticalDensitySet_) {
        return false;
    }

    *density = opticalDensity_;
    return true;
}

/**
 * @brief Sets the transparent dissolve value for the material.
 *
 * This value defines how transparent the material is:
 *   - 1.0 = fully opaque
 *   - 0.0 = fully transparent
 *
 * @param transparency The transparency value to set (range: 0.0 to 1.0).
 */
void Material::SetTransparentDissolve(float transparency) {
    transparentDissolve_ = transparency;
    transparentDissolveSet_ = true;
}

/**
 * @brief Retrieves the transparent dissolve value for the material.
 *
 * Returns the stored transparency value if it has been set. Otherwise,
 * returns false.
 *
 * @param transparency Pointer to a float to store the transparency value.
 * @return true if the value was set and retrieved, false otherwise.
 */
bool Material::GetTransparentDissolve(float *transparency) {
    if (!transparentDissolveSet_) {
        return false;
    }

    *transparency = transparentDissolve_;
    return true;
}

/**
 * @brief Sets the ambient texture map path for the material.
 *
 * This defines the texture used for ambient lighting on the material.
 *
 * @param map The file path to the ambient texture map.
 */
void Material::SetAmbientTextureMap(std::string map) {
    ambientTextureMap_ = map;
    ambientTextureMapSet_ = true;
}

/**
 * @brief Retrieves the ambient texture map path for the material.
 *
 * Returns the stored texture map path if it has been set. Otherwise,
 * returns false.
 *
 * @param map Pointer to a string where the map path will be stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetAmbientTextureMap(std::string *map) {
    if (!ambientTextureMapSet_) {
        return false;
    }

    *map = ambientTextureMap_;
    return true;
}

/**
 * @brief Sets the diffuse texture map path for the material.
 *
 * The diffuse texture defines the base color and surface detail of the
 * material under direct lighting.
 *
 * @param map The file path to the diffuse texture map.
 */
void Material::SetDiffuseTextureMap(std::string map) {
    diffuseTextureMap_ = map;
    diffuseTextureMapSet_ = true;
}

/**
 * @brief Retrieves the diffuse texture map path for the material.
 *
 * Returns the stored diffuse texture map path if it has been set. Otherwise,
 * returns false.
 *
 * @param map Pointer to a string where the map path will be stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetDiffuseTextureMap(std::string *map) {
    if (!diffuseTextureMapSet_) {
        return false;
    }

    *map = diffuseTextureMap_;
    return true;
}

/**
 * @brief Sets the specular colour texture map path for the material.
 *
 * This texture map defines the color and intensity of specular highlights,
 * affecting how shiny or reflective the surface appears.
 *
 * @param map The file path to the specular colour texture map.
 */
void Material::SetSpecularColourTextureMap(std::string map) {
    specularColourTextureMap_ = map;
    specularColourTextureMapSet_ = true;
}

/**
 * @brief Retrieves the specular colour texture map path for the material.
 *
 * Returns the stored specular colour texture map path if it has been set.
 * Otherwise, returns false.
 *
 * @param map Pointer to a string where the map path will be stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetSpecularColourTextureMap(std::string *map) {
    if (!specularColourTextureMapSet_) {
        return false;
    }

    *map = specularColourTextureMap_;
    return true;
}

/**
 * @brief Sets the specular highlight component texture map path.
 *
 * This texture map controls the specular exponent or sharpness of specular
 * highlights on the material surface.
 *
 * @param map The file path to the specular highlight component texture map.
 */
void Material::SetSpecularHighlightComponent(std::string map) {
    specularHighlightComponent_ = map;
    specularHighlightComponentSet_ = true;
}

/**
 * @brief Retrieves the specular highlight component texture map path.
 *
 * Returns the stored specular highlight component map path if it has been
 * set. Otherwise, returns false.
 *
 * @param map Pointer to a string where the map path will be stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetSpecularHighlightComponent(std::string *map) {
    if (!specularHighlightComponentSet_) {
        return false;
    }

    *map = specularHighlightComponent_;
    return true;
}

/**
 * @brief Sets the alpha texture map path for the material.
 *
 * The alpha texture map defines transparency information for the material,
 * typically used for masking or fading parts of the surface.
 *
 * @param map The file path to the alpha texture map.
 */
void Material::SetAlphaTextureMap(std::string map) {
    alphaTextureMap_ = map;
    alphaTextureMapSet_ = true;
}

/**
 * @brief Retrieves the alpha texture map path for the material.
 *
 * Returns the stored alpha texture map path if it has been set. Otherwise,
 * returns false.
 *
 * @param map Pointer to a string where the map path will be stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetAlphaTextureMap(std::string *map) {
    if (!alphaTextureMapSet_) {
        return false;
    }

    *map = alphaTextureMap_;
    return true;
}

/**
 * @brief Sets the bump map texture path for the material.
 *
 * The bump map simulates small surface variations to create the illusion of
 * surface detail without adding actual geometry.
 *
 * @param map The file path to the bump map texture.
 */
void Material::SetBumpMap(std::string map) {
    bumpMap_ = map;
    bumpMapSet_ = true;
}

/**
 * @brief Retrieves the bump map texture path for the material.
 *
 * Returns the stored bump map texture path if it has been set. Otherwise,
 * returns false.
 *
 * @param map Pointer to a string where the bump map path will be stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetBumpMap(std::string *map) {
    if (!bumpMapSet_) {
        return false;
    }

    *map = bumpMap_;
    return true;
}

/**
 * @brief Sets the displacement map texture path for the material.
 *
 * The displacement map alters the actual surface geometry of the material
 * by modifying its vertices based on the map's data.
 *
 * @param map The file path to the displacement map texture.
 */
void Material::SetDisplacementMap(std::string map) {
    displacementMap_ = map;
    displacementMapSet_ = true;
}

/**
 * @brief Retrieves the displacement map texture path for the material.
 *
 * Returns the stored displacement map texture path if it has been set.
 * Otherwise, returns false.
 *
 * @param map Pointer to a string where the displacement map path will be
 * stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetDisplacementMap(std::string *map) {
    if (!displacementMapSet_) {
        return false;
    }

    *map = displacementMap_;
    return true;
}

/**
 * @brief Sets the stencil decal texture path for the material.
 *
 * The stencil decal texture is used for applying patterns or effects on
 * specific areas of the material's surface, typically for detail work.
 *
 * @param map The file path to the stencil decal texture.
 */
void Material::SetStencilDecalTexture(std::string map) {
    stencilDecalTexture_ = map;
    stencilDecalTextureSet_ = true;
}

/**
 * @brief Retrieves the stencil decal texture path for the material.
 *
 * Returns the stored stencil decal texture path if it has been set.
 * Otherwise, returns false.
 *
 * @param map Pointer to a string where the stencil decal texture path will
 * be stored.
 * @return true if the map was set and retrieved, false otherwise.
 */
bool Material::GetStencilDecalTexture(std::string *map) {
    if (!stencilDecalTextureSet_) {
        return false;
    }

    *map = stencilDecalTexture_;
    return true;
}

}   // namespace Meshborn
