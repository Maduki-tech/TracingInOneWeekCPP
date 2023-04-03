#include "includes/color.h"
#include "includes/vec3.h"
#include <fstream>
#include <iostream>
int main(int argc, char *argv[]) {
    const int image_width = 500;
    const int image_height = 500;
    std::ofstream file("image.ppm", std::ios::out | std::ios::binary);

    file << "P6" << std::endl;
    file << image_width << " " << image_height << std::endl;
    file << "255" << std::endl;

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(double(i) / (image_width - 1),
                              double(j) / (image_height - 1), 0.25);
            write_color(file, pixel_color);

        }
    }

    file.close();

    return 0;
}
