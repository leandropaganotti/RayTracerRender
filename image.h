#ifndef IMAGE_H
#define IMAGE_H

#include <vector.h>

class Image
{
    Vector3f** new_buffer(size_t w, size_t h);
    void del_buffer(Vector3f **img, size_t h);

public:
     Image();
    ~Image();

    void resize(uint16_t w, uint16_t h);

    void destroy();

    Vector3f& at(size_t i, size_t j);

    void save_ppm(const char* filename) const;

    void save_ppm_bin(const char * filename) const;

    friend std::ostream& operator << (std::ostream& os, Image& img);


    size_t width;
    size_t height;
    Vector3f **buffer;

    void move(Image &image);
};

#endif // IMAGE_H
