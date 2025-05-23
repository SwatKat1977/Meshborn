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
#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>
#include <functional>

namespace Meshborn {
namespace Logger {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class ILogger {
 public:
    virtual ~ILogger() = default;
    virtual void Log(LogLevel level, const std::string& message) = 0;
};

}   // namespace Logger
}   // namespace Meshborn

#endif  // LOGGER_H_
