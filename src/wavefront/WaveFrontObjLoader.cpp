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
#include <fstream>
#include <iostream>         /// TEMPORARY - TO BE DELETED!!!
#include <utility>
#include "WaveFrontObjLoader.h"

namespace Meshborn {
namespace WaveFront {

std::vector<std::string> ReadObjFile(const std::string& filename);

void LoadFromFile(std::string filename) {
    auto lines = ReadObjFile(filename);
    for (const auto& line : lines) {
        std::cout << line << '\n';
    }
}

std::vector<std::string> ReadObjFile(const std::string& filename) {
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

}   // namespace WaveFront
}   // namespace Meshborn
