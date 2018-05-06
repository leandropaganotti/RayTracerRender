#include "image.h"
#include <iostream>
#include <fstream>

Image::Image(size_t width, size_t height): _width(width), _height(height), buffer(nullptr)
{
    if ( width != 0 && height != 0)
        resize(width, height);
}

Image::~Image()
{
    destroy();
}

void Image::save_ppm(const char *filename) const
{
    std::ofstream outfile (filename, std::ofstream::out);

    outfile << "P3\n";
    outfile << _width << " " << _height << " " << "255\n";
    for (uint16_t i=0; i < _height; ++i)
    {
        for (uint16_t j=0; j < _width; ++j)
            outfile << (int)round(255 * buffer[i][j][0]) << " " << (int)round( 255 * buffer[i][j][1]) << " " << (int)round(255 * buffer[i][j][2]) << " ";
        outfile << "\n";
    }
    outfile.close();
}

void Image::save_ppm_bin(const char *filename) const
{
    FILE *fp = fopen(filename, "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%lu %lu\n255\n", _width, _height);
    for (size_t i = 0; i < _height; ++i)
    {
        for (size_t j = 0; j < _width; ++j)
        {
            static unsigned char color[3];
            color[0] = std::min((int)round(buffer[i][j][0] * 255), 255);  /* red */
            color[1] = std::min((int)round(buffer[i][j][1] * 255), 255);  /* green */
            color[2] = std::min((int)round(buffer[i][j][2] * 255), 255);  /* blue */
            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
}

void Image::move(Image &image)
{
    destroy();
    _width  = image._width;
    _height = image._height;
    buffer = image.buffer;

    image.buffer = nullptr;
    image._width = 0;
    image._height = 0;

}

std::ostream& operator <<(std::ostream &os, Image &img)
{
    for( size_t i=0 ; i < img._height ; ++i )
    {
        for( size_t j=0 ; j < img._width ; ++j )
            os << img.at(i, j);
        os << std::endl;
    }
    return os;
}

void Image::resize(size_t width, size_t height)
{
    destroy();
    if (width > 0 && height > 0)
    {
        buffer = new_array_2d<Vector3f>(width, height);
        this->_width = width;
        this->_height = height;
    }
}

void Image::destroy()
{
    if(buffer)
    {
        del_array_2d<Vector3f>(buffer, _height);
        buffer = nullptr;
    }
}
