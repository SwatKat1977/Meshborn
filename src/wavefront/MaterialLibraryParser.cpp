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
#include <iostream>     // TEMP
#include <vector>
#include "MaterialLibraryParser.h"

namespace Meshborn {
namespace WaveFront {

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

/*
Illumination model (illum tag)
illum	Description
0	Color on, ambient off
1	Color on, ambient on
2	Highlight on
3	Reflection on and ray trace on
4	Transparency: Glass on, Reflection: Ray trace on
5	Reflection: Fresnel on and ray trace on
6	Transparency: Refraction on, Reflection: Fresnel off and ray trace on
7	Transparency: Refraction on, Reflection: Fresnel on and ray trace on
8	Reflection on and ray trace off
9	Transparency: Glass on, Reflection: Ray trace off
10	Casts shadows onto invisible surfaces
*/

MaterialLibraryParser::MaterialLibraryParser() {
}

void MaterialLibraryParser::ParseLibrary(std::string materialFile) {
    std::vector<std::string> rawLines;

    try {
        rawLines = ReadFile(materialFile);
    }
    catch (std::runtime_error ex) {
        throw std::runtime_error(ex.what());
    }

    for (const auto& line : rawLines) {
        std::string_view view(line);

        // New material
        if (StartsWith(std::string(view), KEYWORD_NEW_MATERIAL)) {
            std::cout << "New Material: " << view << std::endl;

        // Ambient colour
        } else if (StartsWith(std::string(view), KEYWORD_AMBIENT)) {
            std::cout << "Ambient colour: " << view << std::endl;
            //  auto a = SplitElementString(std::string(view));
            //  printf("=> Count : %d\n", a.size());

        // Diffuse colour
        } else if (StartsWith(std::string(view), KEYWORD_DIFFUSE)) {
            std::cout << "Diffuse colour: " << view << std::endl;

        // Emissive colour
        } else if (StartsWith(std::string(view), KEYWORD_EMISSIVE)) {
            std::cout << "Emissive colour: " << view << std::endl;

        // Specular colour
        } else if (StartsWith(std::string(view), KEYWORD_SPECULAR)) {
            std::cout << "Specular colour: " << view << std::endl;

        // Specular exponent
        } else if (StartsWith(std::string(view), KEYWORD_SPECULAR_EXPONENT)) {
            std::cout << "Specular exponent: " << view << std::endl;

        // Transparent dissolve
        } else if (StartsWith(std::string(view), KEYWORD_TRANSPARENT_DISOLVE)) {
            std::cout << "Transparent dissolve: " << view << std::endl;

        // Optical density
        } else if (StartsWith(std::string(view), KEYWORD_OPTICAL_DENSITY)) {
            std::cout << "Optical density: " << view << std::endl;

        // Illumination model
        } else if (StartsWith(std::string(view), KEYWORD_ILLUMINATION_MODEL)) {
            std::cout << "Illumination model: " << view << std::endl;

        // Ambient texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_AMBIENT_TEXTURE_MAP)) {
            std::cout << "Ambient texture map: " << view << std::endl;

        // Diffuse texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_DIFFUSE_TEXTURE_MAP)) {
            std::cout << "Diffuse texture map: " << view << std::endl;

        // Specular color texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_SPECULAR_COLOR_TEXTURE_MAP)) {
            std::cout << "Specular color texture map: " << view << std::endl;

        // Specular highlight component
        } else if (StartsWith(std::string(view),
                              KEYWORD_SPECULAR_HIGHLIGHT_COMPONENT)) {
            std::cout << "Specular highlight component: " << view << std::endl;

        // Alpha texture map
        } else if (StartsWith(std::string(view),
                              KEYWORD_ALPHA_TEXTURE_MAP)) {
            std::cout << "Alpha texture map: " << view << std::endl;

        // Bump map
        } else if ((StartsWith(std::string(view),
                               KEYWORD_MAP_BUMP)) ||
                   (StartsWith(std::string(view),
                               KEYWORD_BUMP_MAP))) {
            std::cout << "Bump map: " << view << std::endl;

        // Displacement map
        } else if (StartsWith(std::string(view),
                              KEYWORD_DISPLACEMENT_MAP)) {
        std::cout << "Displacement map: " << view << std::endl;

       // Stencil decal texture
        } else if (StartsWith(std::string(view),
                              KEYWORD_STENCIL_DECAL_TEXTURE)) {
            std::cout << "Stencil decal texture: " << view << std::endl;

        // Unknown tag : Doesn't mean it's invalid, it could be a tag that
        //               currently isn't parsed.
        } else {
            std::cout << "Unknown: " << view << std::endl;
        }
    }
}

}   // namespace WaveFront
}   // namespace Meshborn
