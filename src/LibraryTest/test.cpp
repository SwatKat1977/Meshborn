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
#include <iostream>         /// TEMP
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include "Logger.h"
#include "Model.h"

const std::map<Meshborn::Logger::LogLevel, std::string> LogLevelToString {
    { Meshborn::Logger::LogLevel::Debug,    "DEBUG" },
    { Meshborn::Logger::LogLevel::Info,     "INFO" },
    { Meshborn::Logger::LogLevel::Warning,  "WARNING" },
    { Meshborn::Logger::LogLevel::Error,    "ERROR" },
    { Meshborn::Logger::LogLevel::Critical, "CRITICAL" }
};

#include "WaveFrontObjParser.h"
#include "Meshborn.h"
#include "Logger.h"

class ConsoleLogger: public Meshborn::Logger::ILogger {
 public:
    ConsoleLogger() = default;

    void Log(Meshborn::Logger::LogLevel level,
             const std::string& message){
        std::cout << "[" << LogLevelToString.at(level) << "] "
                  << message << std::endl;
    }
};

int main(int argc, char** argv) {
    std::string filename;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if ((arg == "-f" || arg == "--file") && i + 1 < argc) {
            filename = argv[++i];
        }
    }

    if (filename.empty()) {
        std::cerr << "Usage: " << argv[0] << " -f <filename>\n";
        return 1;
    }

    ConsoleLogger logger;
    Meshborn::SetLogger(std::make_unique<ConsoleLogger>());

    std::cout << "Loading '" << filename << "'\n";

    Meshborn::Model model;

    try {
        bool status;
        auto model = Meshborn::WaveFrontObjParser().ParseObj(filename);
        status = (!model) ? false : true;
        std::cout << "[DEBUG] Parse object return status of " << status << "\n";
    }
    catch (std::runtime_error ex) {
        std::cout << "[EXCEPTION] " << ex.what() << "\n";
    }

    return 0;
}
