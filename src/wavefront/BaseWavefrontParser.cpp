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
#include <cctype>   // for std::isspace
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include "BaseWavefrontParser.h"

namespace Meshborn {
namespace WaveFront {

std::vector<std::string> BaseWavefrontParser::SplitElementString(
    const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::string> BaseWavefrontParser::ReadFile(
    const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Only keep the line if it's not empty or not a comment.
        if (!line.empty() && !line.starts_with("#")) {
            // Strip Windows carriage return
            if (line.back() == '\r') {
                line.pop_back();
            }

            lines.push_back(std::move(line));
        }
    }

    return lines;
}

bool BaseWavefrontParser::StartsWith(const std::string& line,
                                     const std::string& prefix) {
    size_t i = 0;
    while (i < line.size() && std::isspace(static_cast<unsigned char>(line[i]))) {
        ++i;
    }
    return line.compare(i, prefix.length(), prefix) == 0;
}

}   // namespace WaveFront
}   // namespace Meshborn
