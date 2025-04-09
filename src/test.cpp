#include <fstream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> readLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            // Strip Windows carriage return
            if (line.back() == '\r')
            {
                line.pop_back();
            }

            lines.push_back(std::move(line));
        }
    }

    return lines;
}


#include <iostream>

class Vertex
{
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
    Vertex() = delete; // No default construction allowed

    const float DEFAULT_W_VALUE = 1.0f;

    float x_;
    float y_;
    float z_;
    float w_;
};

#include "wavefront/WaveFrontObjLoader.h"

int main(int argc, char** argv)
{
    try
    {
        Meshborn::WaveFront::LoadFromFile("crate.obj");
    }
    catch (std::runtime_error ex)
    {
        std::cout << "[EXCEPTION] " << ex.what() << "\n";
    }

    return 0;
}
