#ifndef IMAGE_H
#define IMAGE_H

#include <vector.h>
#include "utils.h"

class Image
{   
    size_t _width;
    size_t _height;
    float _aspectRatio;
    Vector3f **buffer;

public:
    Image(size_t width=0, size_t height=0);
    ~Image();

    void resize(size_t width, size_t height);
    void destroy();

    Vector3f& at(size_t i, size_t j);

    void save_ppm(const char* filename) const;
    void save_ppm_bin(const char * filename) const;

    void move(Image &image);

    size_t  width() const;
    void    width(const size_t value);
    size_t  height() const;
    void    height(const size_t value);
    float   aspectRatio() const;

    friend std::ostream& operator << (std::ostream& os, Image& img);
};


inline
Vector3f &Image::at(size_t i, size_t j)
{
    return buffer[i][j];
}
inline
size_t Image::width() const
{
    return _width;
}
inline
void Image::width(const size_t value)
{
    _width = value;
}
inline
size_t Image::height() const
{
    return _height;
}
inline
void Image::height(const size_t value)
{
    _height = value;
}
inline
float Image::aspectRatio() const
{
    return _aspectRatio;
}



#endif // IMAGE_H
