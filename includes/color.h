#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
void write_color(std::ostream &out, color pixel_color) {

    int char_r = static_cast<int>(255.999 * pixel_color.x());
    int char_g = static_cast<int>(255.999 * pixel_color.y());
    int char_b = static_cast<int>(255.999 * pixel_color.z());

    out << static_cast<unsigned char>(char_r)
        << static_cast<unsigned char>(char_g)
        << static_cast<unsigned char>(char_b);
}

#endif
