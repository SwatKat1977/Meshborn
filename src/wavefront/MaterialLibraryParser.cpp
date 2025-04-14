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
}

}   // namespace WaveFront
}   // namespace Meshborn
