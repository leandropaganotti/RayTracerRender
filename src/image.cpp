#include "image.h"
#include <iostream>
#include <fstream>

Image::Image(size_t width, size_t height): _width(width), _height(height), buffer(nullptr)
{
    if ( width > 0 && height > 0)
        resize(width, height);
}

Image::~Image()
{
    destroy();
}

Image &Image::operator=(Image &&img)
{
    move(img);
    return *this;
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
    (void) fprintf(fp, "P6\n%zu %zu\n255\n", _width, _height);
    for (size_t i = 0; i < _height; ++i)
    {
        for (size_t j = 0; j < _width; ++j)
        {
            const float gamma=1.0/2.2;
            unsigned char color[3];
            color[0] = pow(clamp(buffer[i][j].r), gamma) * 255;  /* red */
            color[1] = pow(clamp(buffer[i][j].g), gamma) * 255;  /* green */
            color[2] = pow(clamp(buffer[i][j].b), gamma) * 255;  /* blue */
            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
}

bool Image::read_ppm_bin(const char *filename)
{
    std::ifstream ifs;
    auto getNext = [&ifs](std::string &next){
        ifs >> next;
        while (next == "#") {
            ifs.ignore(255, '\n');
            ifs >> next;
        }
    };

    ifs.open(filename, std::ios::binary);
    try {
        if (ifs.fail()) {
            throw("Can't open input file");
        }
        std::string header, next;
        size_t w, h, b;
        ifs >> header;
        if (header != "P6") throw("Can't read ppm file other than P6 format");
        getNext(next);
        w = std::atoi(next.c_str());
        getNext(next);
        h = std::atoi(next.c_str());
        getNext(next);
        b = std::atoi(next.c_str());
        if(w == 0 || h == 0)
        {
            throw ("Can't read ppm file, width and height must be > 0");
        }
        ifs.ignore(255, '\n');
        resize(w, h);
        unsigned char pix[3];
        for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < w; ++j) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            buffer[i][j].r = pix[0] / 255.f;
            buffer[i][j].g = pix[1] / 255.f;
            buffer[i][j].b = pix[2] / 255.f;
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }
    return true;
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
    if (this->_width != width || this->_height != height)
    {
        if (width > 0 && height > 0)
        {
            destroy();
            buffer = new_array_2d<Vector3>(width, height);
            this->_width = width;
            this->_height = height;
        }
        else
        {
            std::cerr << "error: image resize failed: new image size not valid" << std::endl;
        }
    }
}

void Image::destroy()
{
    if(buffer)
    {
        del_array_2d<Vector3>(buffer, _height);
        buffer = nullptr;
        _width = _height = 0;
    }
}


