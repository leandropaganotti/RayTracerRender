#pragma once

#include "vector.h"
#include "utils.h"

class Image
{     
public:
    Image(int width=0, int height=0);
    Image(int width, int height, const Vector3 &value);

    ~Image();

    Image(Image &&image);
    Image & operator=(Image &&image);

    inline Vector3 & at(int i, int j);
    inline const Vector3 & at(int i, int j) const;

    bool read_ppm_bin(const char * filename);
    bool read(const char * filename);
    void write_jpg(const char* filename, float gamma=1/2.2f) const;
    void write_hdr(const char* filename) const;
    void write_png(const char* filename, float gamma=1/2.2f) const;
    void write_ppm(const char* filename) const;
    void write_ppm_bin(const char * filename) const;

    int   width() const;
    void  width(const int value);
    int   height() const;
    void  height(const int value);

    void resize(int width, int height);

    friend std::ostream& operator << (std::ostream& os, Image& img);

protected:
    int w;
    int h;

    Vector3 *buffer;
    void destroy();
};


inline
Vector3 &Image::at(int i, int j)
{
    return buffer[i*w+j];
}
inline
const Vector3 &Image::at(int i, int j) const
{
    return buffer[i*w+j];
}
inline
int Image::width() const
{
    return w;
}
inline
void Image::width(const int value)
{
    w = value;
}
inline
int Image::height() const
{
    return h;
}
inline
void Image::height(const int value)
{
    h = value;
}
