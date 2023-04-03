#include "includes/color.h"
#include "includes/ray.h"
#include "includes/vec3.h"
#include <fstream>
#include <iostream>

/**
 * @brief If the ray hits this object it will paint a color whcih is given
 *
 * @param center
 * @param radius
 * @param r
 * @return
 */
double hit_sphere(const point3 &center, double radius, const ray &r) {
    vec3 originToCenter = r.origin() - center;
    auto directionDotProduct = dot(r.direction(), r.direction());
    auto originCenterDotDirection = 2.0 * dot(originToCenter, r.direction());
    auto originCenterLenghtSquared =
        dot(originToCenter, originToCenter) - radius * radius;
    auto discriminant = originCenterDotDirection * originCenterDotDirection -
                        4 * directionDotProduct * originCenterLenghtSquared;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-originCenterDotDirection - sqrt(discriminant)) /
               (2.0 * directionDotProduct);
    }
}

/**
 * @brief Colors the Background image and if there is something that is hitting
 * an object it will paint it $ t = 0.5 * (direction + 1.0) $ $ blenvalue =
 * (1-t) * color1 + t * color2 $
 *
 * @param r
 * @return
 */
color ray_color(const ray &r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char *argv[]) {

    // Image
    // Bessere auflosung von dem bild
    // Berechtnet die perfecte height des bildes aus
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    // How much the camera can see
    const double viewport_height = 2.0;
    const double viewport_width = aspect_ratio * viewport_height;
    // zoom length
    const double focal_length = 1.0;

    // where the camera is xyz
    const point3 origin(0, 0, 0);
    const vec3 horizontal(viewport_width, 0, 0);
    const vec3 vertical(0, viewport_height, 0);
    const point3 lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    std::cout << lower_left_corner << std::endl;

    std::ofstream file("image.ppm", std::ios::out | std::ios::binary);

    file << "P6" << std::endl;
    file << image_width << " " << image_height << std::endl;
    file << "255" << std::endl;

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray ray(origin,
                    lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(ray);
            write_color(file, pixel_color);
        }
    }

    file.close();

    return 0;
}
