## !! Wichtig !!
Es wird P6 benutzt und kein P3
unterschied 
| P3 | P6 |
|---|---|
| Benutzt werte von 0 bis 255 | Benutzt werte von 0.0 bis 1.0f|
| wenn man werte in das bild schreiben will, muss man leerzeichen zwischen der rbg werten lassen und eine neue zeile am ende| Werte werden ohne leerzeichen in das bild geladen und es wird eine neue zeile nach den rbg werten gemacht|
| Werte in dem Bild sind als `int` angegeben | Werte in dem Bild sind als `char` angegeben|


## Leeres bild erstellen
Es muss ein Bild erstellt werden in diesem Bild wird dann die width und `height` angegeben. Und metadaten wie P6 und den maxValue eingegeben.

```C++
#include <fstream>
#include <iostream>

int main() {
    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render
    std::ofstream outfile("image.ppm", std::ios::out | std::ios::binary);
    outfile << "P6\n" << image_width << ' ' << image_height << "\n255\n";

    outfile.close();
    return 0;
}
```

## Bild mit farben füllen (Farbverlauf erstellen)
Um einen Wert in den jeweiligen pixeln zu erstellen, muss eine Loop erstellt werden. Diese loop wir einmal auf der `height` laufen (erste loop). In dieser Loop wird dann eine weitere loop laufen, diese wird dann von links nach recht laufen also `width` (innere loop).

```c++
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            // Write the color values as binary data
            outfile << static_cast<unsigned char>(ir)
                    << static_cast<unsigned char>(ig)
                    << static_cast<unsigned char>(ib);
        }
    }

```


## Zusammen sieht das ganze so aus 
```c++
#include <fstream>
#include <iostream>

int main() {
    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render
    std::ofstream outfile("image.ppm", std::ios::out | std::ios::binary);
    outfile << "P6\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            // Write the color values as binary data
            outfile << static_cast<unsigned char>(ir) << static_cast<unsigned char>(ig) << static_cast<unsigned char>(ib);
        }
    }

    outfile.close();
    return 0;
}

```

Und der output sieht folgendermaßen aus:

![Output](https://raytracing.github.io/images/img-1.01-first-ppm-image.png)


## Erstellung der Vectorklasse 

### Was diese klasse macht
- In dieser klasse, werden die Pixel werte der jeweiligen Pixel zurück gegeben.
- Sie wird zum zeichnen benutzt.
- Hat operatoren für + - / * 
- man kann die länge herrausfinden

```c++
// Vec3.h

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:
        vec3() : e{0,0,0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

    public:
        double e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

#endif
```

#### Utitliy Function für den vec3
Hier werden einige Funktionen definiert, die später nützlich werden wie `dot`, `cross`, und verschiedene rechnungen
```c++
// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}
``` 

## Color Function
Nun wird die Funktion erstellt, die den jeweiligen Pixeln die farbe geben wird.
Im  großen und ganzen wird die Loop aus der `main` genommen und hier her vorschoben

```c++
// color.h

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif
```

## Main anpassen

``` c++
// main.cpp

#include "includes/color.h"
#include "includes/vec3.h"
#include <fstream>
#include <iostream>
int main(int argc, char *argv[]) {
    const int image_width = 256;
    const int image_height = 256;
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
```

