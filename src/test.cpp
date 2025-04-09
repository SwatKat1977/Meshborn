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

const std::string POLYGONAL_FACE_ELEMENT = "f ";
const std::string VECTOR_ELEMENT = "v ";
const std::string VECTOR_NORMAL_ELEMENT = "vn ";
const std::string TEXTURE_COORDINATE_ELEMENT = "vt ";

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


void parse(std::vector<std::string> lines)
{
    for (const auto& line : lines) {
        
        std::string_view view(line);

        if (view.starts_with(POLYGONAL_FACE_ELEMENT)) {
            // Vertex position
            std::cout << "Found polygonal face: " << view << '\n';
        }
        else if (view.starts_with(VECTOR_ELEMENT)) {
            // Vertex position
            std::cout << "Found vertex: " << view << '\n';
        }
        else if (view.starts_with(VECTOR_NORMAL_ELEMENT)) {
            // Vertex normal
            std::cout << "Found vertex normal: " << view << '\n';
        }
        else if (view.starts_with(TEXTURE_COORDINATE_ELEMENT)) {
            // Vertex normal
            std::cout << "Found texture coordinate: " << view << '\n';
        }
        else
        {
            std::cout << line << '\n';
        }
    }
}


#include "wavefront/WaveFrontObjLoader.h"

int main(int argc, char** argv)
{
    Meshborn::WaveFront::LoadFromFile("crate.obj");

    return 0;

    try
    {

        /*
        auto lines = readLines("crate.obj");
        parse(lines);
        */

        /*
        for (const auto& line : lines) {
            std::cout << line << '\n';
        }
        */
    }
    catch (std::runtime_error ex)
    {
        std::cout << "[EXCEPTION] " << ex.what() << "\n";
    }

    return 0;
}
