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
#ifndef WAVEFRONT_MATERIALLIBRARYPARSER_H_
#define WAVEFRONT_MATERIALLIBRARYPARSER_H_
#include <string>
#include "BaseWavefrontParser.h"

namespace Meshborn {
namespace WaveFront {

class MaterialLibraryParser {
 public:
    MaterialLibraryParser();

    void ParseLibrary(std::string materialFile);
};


}   // namespace WaveFront
}   // namespace Meshborn

#endif  // WAVEFRONT_MATERIALLIBRARYPARSER_H_
