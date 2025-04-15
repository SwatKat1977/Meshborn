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
#ifndef WAVEFRONT_BASEWAVEFRONTPARSER_H_
#define WAVEFRONT_BASEWAVEFRONTPARSER_H_
#include <string>
#include <vector>

namespace Meshborn {
namespace WaveFront {

class BaseWavefrontParser {
 protected:
    std::vector<std::string> ReadFile(const std::string& filename);

    std::vector<std::string> SplitElementString(const std::string& str);

    bool StartsWith(const std::string& line, const std::string& prefix);

    bool ParseFloat(const char* str, float& out);
};

}   // namespace WaveFront
}   // namespace Meshborn

#endif  //  WAVEFRONT_BASEWAVEFRONTPARSER_H_
