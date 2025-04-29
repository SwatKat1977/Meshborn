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
#include "Meshborn.h"
#include "LoggerManager.h"

namespace Meshborn {

/**
 * Sets the global logger instance.
 *
 * Transfers ownership of the provided logger to the LoggerManager singleton,
 * replacing any existing logger. This allows centralized control over
 * logging behavior throughout the application.
 *
 * @param logger A unique pointer to a new ILogger implementation.
 */
void SetLogger(std::unique_ptr<Logger::ILogger> logger) {
    Logger::LoggerManager::Instance().SetLogger(std::move(logger));
}

}   // namespace Meshborn
