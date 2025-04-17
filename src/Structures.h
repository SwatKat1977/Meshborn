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
#ifndef STRUCTURES_H_
#define STRUCTURES_H_

namespace Meshborn {

struct RGB {
    float red;
    float green;
    float blue;
};

struct Point3D {
    float x, y, z;

    Point3D() : x(0), y(0), z(0) {}
    Point3D(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Point4D {
    float x, y, z, w;

    Point4D() : x(0), y(0), z(0), w(1) {}
    Point4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

struct TextureCoordinates {
    float u, v, w;

    TextureCoordinates() : u(0), v(0), w(0) {}
    TextureCoordinates(float u, float v, float w) : u(u), v(v), w(w) {}
};

}   // namespace Meshborn

#endif  // STRUCTURES_H_
