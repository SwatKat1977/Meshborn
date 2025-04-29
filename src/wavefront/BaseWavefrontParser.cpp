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
#include <cctype>   // for std::isspace
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include "BaseWavefrontParser.h"

namespace Meshborn {
namespace WaveFront {

/**
 * Splits a space-delimited string into individual tokens.
 *
 * This function takes a string and splits it into substrings using whitespace
 * as the delimiter. Consecutive spaces are treated as a single delimiter.
 *
 * @param str The input string to split.
 * @return A vector of strings containing each token in the original string.
 */
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

/**
 * Reads a text file line by line, filtering out empty lines and comments.
 *
 * This function opens the specified file and reads its contents line by line.
 * It ignores lines that are empty or begin with the comment character '#'.
 * On Windows systems, it also removes any carriage return characters ('\r')
 * from the end of each line.
 *
 * @param filename The path to the file to read.
 * @return A vector of strings, each representing a relevant line from the file.
 * @throws std::runtime_error if the file cannot be opened.
 */
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

/**
 * Checks whether a given line starts with a specific prefix, ignoring any
 * leading whitespace characters.
 *
 * This function skips over any leading whitespace (spaces, tabs, etc.) in the
 * input line before comparing the start of the line with the given prefix.
 * The comparison is case-sensitive and does not trim whitespace from the
 * prefix.
 *
 * @param line The string to check.
 * @param prefix The prefix to look for at the start of the line.
 * @return true if the line starts with the prefix after skipping leading whitespace,
 *         false otherwise.
 */
bool BaseWavefrontParser::StartsWith(const std::string& line,
                                     const std::string& prefix) {
    size_t i = 0;
    while (i < line.size() &&
           std::isspace(static_cast<unsigned char>(line[i]))) {
        ++i;
    }
    return line.compare(i, prefix.length(), prefix) == 0;
}

/**
 * Parses a string into a floating-point (float) value. This function checks
 * for errors during the conversion, such as out-of-range values and the presence
 * of invalid characters in the input string.
 *
 * The function uses `strtof` to convert the string to a `float`. If the input
 * is not a valid float or contains extra non-numeric characters, or if the
 * resulting value is out of the representable range for floats, the function
 * returns false.
 *
 * @param str The string to be parsed into a float.
 * @param out A pointer to a float where the parsed result will be stored if successful.
 * @return true if the string was successfully parsed into a valid float, false otherwise.
 */
bool BaseWavefrontParser::ParseFloat(const char* str, float *out) {
    errno = 0;
    char* end;
    float value = strtof(str, &end);
    if (errno == ERANGE || *end != '\0') return false;
    *out = value;
    return true;
}

/**
 * Parses a string into an integer (int) value. The function handles potential
 * errors that may arise during the conversion process, including out-of-range
 * values and invalid input formats.
 *
 * This function uses `std::strtoll` to perform the conversion, ensuring that
 * the value is within the valid range of an integer (`INT_MIN` to `INT_MAX`).
 * If the conversion fails due to invalid characters or out-of-range values,
 * the function will return false.
 *
 * @param str The string to be parsed into an integer.
 * @param out A pointer to an integer where the result will be stored if the
 *            parsing is successful.
 * @return true if the string was successfully parsed into a valid integer,
 *         false otherwise.
 */
bool BaseWavefrontParser::ParseInt(const char* str, int* out) {
    errno = 0;
    char* end;

    // Base 10 for decimal
    int64_t value = std::strtoll(str, &end, 10);

    if (errno == ERANGE || *end != '\0' || value < INT_MIN || value > INT_MAX) {
        return false;
    }

    *out = static_cast<int>(value);
    return true;
}

}   // namespace WaveFront
}   // namespace Meshborn
