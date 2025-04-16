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
#ifndef LOGGERMANAGER_H_
#define LOGGERMANAGER_H_
#include <memory>
#include <mutex>    // NOLINT
#include <utility>
#include "Logger.h"

namespace Meshborn {
namespace Logger {

class LoggerManager {
 public:
    static LoggerManager& Instance() {
        static LoggerManager instance;
        return instance;
    }

    void SetLogger(std::shared_ptr<ILogger> newLogger) {
        std::lock_guard<std::mutex> lock(mutex_);
        logger_ = std::move(newLogger);
    }

    bool HasLogger() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return logger_ != nullptr;
    }

    ILogger& GetLogger() {
        std::lock_guard<std::mutex> lock(mutex_);
        return *logger_;
    }

 private:
    LoggerManager() = default;
    std::shared_ptr<ILogger> logger_;
    mutable std::mutex mutex_;
};

#ifndef DISABLE_LOGGING
  #ifdef MESHBORN_LOG_DEBUG
    #define SHOULD_LOG_DEBUG true
  #else
    #define SHOULD_LOG_DEBUG false
  #endif

  #define LOG(level, message)                                           \
    do {                                                                \
      if (Logger::LoggerManager::Instance().HasLogger()) {              \
        if ((level) != Logger::LogLevel::Debug || SHOULD_LOG_DEBUG) {   \
          Logger::LoggerManager::Instance().GetLogger().Log((level),    \
                                                            (message)); \
        }                                                               \
      }                                                                 \
    } while (0)
#else
  #define LOG(level, message) do {} while (0)
#endif

}   // namespace Logger
}   // namespace Meshborn

#endif  //  LOGGERMANAGER_H_
