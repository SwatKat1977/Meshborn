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
#include <string>
#include <string_view>
#include <vector>
#include "Logger.h"

class Vertex {
 public:
    Vertex(float x, float y, float z) :
        x_(x), y_(y), z_(z), w_(DEFAULT_W_VALUE) {}

    Vertex(float x, float y, float z, float w) :
        x_(x), y_(y), z_(z), w_(w) {}

    float X() const { return x_; }
    float Y() const { return y_; }
    float Z() const { return z_; }
    float W() const { return w_; }

 private:
    Vertex() = delete;  // No default construction allowed

    const float DEFAULT_W_VALUE = 1.0f;

    float x_;
    float y_;
    float z_;
    float w_;
};

#include "wavefront/WaveFrontObjLoader.h"
#include "Meshborn.h"
#include "Logger.h"

class ConsoleLogger: public Meshborn::Logger::ILogger {
public:
    ConsoleLogger() = default;

    void Log(Meshborn::Logger::LogLevel level,
             const std::string& message){
        std::cout << message << std::endl;
    }
};


int main(int argc, char** argv) {
    ConsoleLogger logger;
    Meshborn::SetLogger(std::make_unique<ConsoleLogger>());

    try {
        Meshborn::WaveFront::LoadFromFile("crate.obj");
    }
    catch (std::runtime_error ex) {
        std::cout << "[EXCEPTION] " << ex.what() << "\n";
    }

    return 0;
}
