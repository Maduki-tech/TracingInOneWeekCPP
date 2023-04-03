#include "color.h"
#include <iostream>
void write_color(std::ostream &out, color pixel_color) {

  int ir = static_cast<int>(255.999 * pixel_color.x());
  int ig = static_cast<int>(255.999 * pixel_color.y());
  int ib = static_cast<int>(255.999 * pixel_color.z());

  /* std::cout << r << " " << g << " " << b << std::endl; */
  out << static_cast<unsigned char>(ir) << static_cast<unsigned char>(ig)
      << static_cast<unsigned char>(ib);
}

