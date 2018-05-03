#include "image.h"
#include <iostream>
#include <fstream>

Image::Image(): width(0), height(0), buffer(nullptr)
{

}

Image::~Image()
{
    destroy();
}

void Image::resize(uint16_t w, uint16_t h)
{
    destroy();
    buffer = new_buffer(w, h);
    width = w;
    height = h;
}

void Image::destroy()
{
    if(buffer)
    {
        del_buffer(buffer, height);
        buffer = nullptr;
        width = height = 0;
    }
}

inline
Vector3f &Image::at(size_t i, size_t j)
{
    return buffer[i][j];
}

void Image::save_ppm(const char *filename) const
{
    std::ofstream outfile (filename, std::ofstream::out);

    outfile << "P3\n";
    outfile << width << " " << height << " " << "255\n";
    for (uint16_t i=0; i < height; ++i)
    {
        for (uint16_t j=0; j < width; ++j)
            outfile << (int)round(255 * buffer[i][j][0]) << " " << (int)round( 255 * buffer[i][j][1]) << " " << (int)round(255 * buffer[i][j][2]) << " ";
        outfile << "\n";
    }
    outfile.close();
}

void Image::save_ppm_bin(const char *filename) const
{
    FILE *fp = fopen(filename, "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%lu %lu\n255\n", width, height);
    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
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
    width  = image.width;
    height = image.height;
    buffer = image.buffer;

    image.buffer = nullptr;
    image.width = 0;
    image.height = 0;

}

std::ostream& operator <<(std::ostream &os, Image &img)
{
    for( size_t i=0 ; i < img.height ; ++i )
    {
        for( size_t j=0 ; j < img.width ; ++j )
            os << img.at(i, j);
        os << std::endl;
    }
    return os;
}

Vector3f **Image::new_buffer(size_t w, size_t h)
{
    Vector3f** img = new Vector3f *[w];
    for(size_t i = 0; i < h; ++i)
        img[i] = new Vector3f[w]{};

    return img;
}

void Image::del_buffer(Vector3f **img, size_t h)
{
    for(size_t i = 0; i <h; i++)
        delete [] img[i];
    delete [] img;
    img=nullptr;
}
