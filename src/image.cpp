#include "image.h"

#include <iostream>
#include <fstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb-master/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb-master/stb_image.h"
#pragma GCC diagnostic pop

Image::Image(int width, int height): w(width), h(height), buffer(nullptr)
{
    if ( w>0 && h>0)
        buffer = new Vector3[w*h];
}

Image::Image(int width, int height, const Vector3 &value): w(width), h(height), buffer(nullptr)
{
    if ( w>0 && h>0) {
        buffer = new Vector3[w*h];

        for(int i=0; i < w*h; ++i)
            buffer[i] = value;
    }
}

Image::~Image()
{
    destroy();
}

Image::Image(Image &&image)
{
    buffer = nullptr;
    *this = std::move(image);
}

Image &Image::operator=(Image &&image)
{
    destroy();
    w  = image.w;
    h = image.h;
    buffer = image.buffer;

    image.buffer = nullptr;
    image.w = 0;
    image.h = 0;

    return *this;
}

void Image::write_jpg(const char *filename, float gamma) const
{
    std::string filenameext(filename);
    filenameext.append(".jpg");
    std::vector<uint8_t> data(w*h*3);
    for (int i=0; i < w*h; ++i)
    {
        data[i*3]   = pow(clamp(buffer[i].r), gamma) * 255.f;
        data[i*3+1] = pow(clamp(buffer[i].g), gamma) * 255.f;
        data[i*3+2] = pow(clamp(buffer[i].b), gamma) * 255.f;
    }
    stbi_write_jpg(filenameext.c_str(), w, h, 3, data.data(), 95);
}

void Image::write_hdr(const char *filename) const
{
    std::string filenameext(filename);
    filenameext.append(".hdr");
    stbi_write_hdr(filenameext.c_str(), w, h, 3, (float*)buffer);
}

void Image::write_png(const char *filename, float gamma) const
{
    std::string filenameext(filename);
    filenameext.append(".png");
    std::vector<uint8_t> data(w*h*3);
    for (int i=0; i < w*h; ++i)
    {
        data[i*3]   = pow(clamp(buffer[i].r), gamma) * 255.f;
        data[i*3+1] = pow(clamp(buffer[i].g), gamma) * 255.f;
        data[i*3+2] = pow(clamp(buffer[i].b), gamma) * 255.f;
    }
    stbi_write_png(filenameext.c_str(), w, h, 3, data.data(), w*3);
}

void Image::write_ppm(const char *filename) const
{
    std::ofstream outfile (std::string(filename)+".ppm", std::ofstream::out);

    outfile << "P3\n";
    outfile << w << " " << h << " " << "255\n";
    for (uint16_t i=0; i < h; ++i)
    {
        for (uint16_t j=0; j < w; ++j)
            outfile << (int)round(255 * buffer[i*w+j][0]) << " " << (int)round( 255 * buffer[i*w+j][1]) << " " << (int)round(255 * buffer[i*w+j][2]) << " ";
        outfile << "\n";
    }
    outfile.close();
}

void Image::write_ppm_bin(const char *filename) const
{
    std::string filenameext(filename);
    filenameext.append(".ppm");
    FILE *fp = fopen(filenameext.c_str(), "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", w, h);
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            const float gamma=1.0/2.2;
            unsigned char color[3];
            color[0] = pow(clamp(buffer[i*w+j].r), gamma) * 255;  /* red */
            color[1] = pow(clamp(buffer[i*w+j].g), gamma) * 255;  /* green */
            color[2] = pow(clamp(buffer[i*w+j].b), gamma) * 255;  /* blue */
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
        int w, h, b;
        ifs >> header;
        if (header != "P6") throw("Can't read ppm file other than P6 format");
        getNext(next);
        w = std::atoi(next.c_str());
        getNext(next);
        h = std::atoi(next.c_str());
        getNext(next);
        b = std::atoi(next.c_str());
        UNUSED(b);
        if(w == 0 || h == 0)
        {
            throw ("Can't read ppm file, width and height must be > 0");
        }
        ifs.ignore(255, '\n');
        resize(w, h);
        unsigned char pix[3];
        for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            buffer[i*w+j].r = pix[0] / 255.f;
            buffer[i*w+j].g = pix[1] / 255.f;
            buffer[i*w+j].b = pix[2] / 255.f;
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

bool Image::read(const char *filename)
{
    try {
        destroy();
        int n, width, height;
        buffer = (Vector3*)stbi_loadf(filename, &width, &height, &n, 3);
        if(!buffer)
        {
            std::string msg("Can't open input image file: ");
            msg.append(filename);
            throw(msg);
        }
        w=width;
        h=height;
        return true;
    }
    catch (const std::string &err) {
        std::cerr << err << std::endl;
        return false;
    }
}

std::ostream& operator <<(std::ostream &os, Image &img)
{
    for( int i=0 ; i < img.h ; ++i )
    {
        for( int j=0 ; j < img.w ; ++j )
            os << img.at(i, j);
        os << std::endl;
    }
    return os;
}

void Image::resize(int width, int height)
{
    if (this->w != width || this->h != height)
    {
        if (width > 0 && height > 0)
        {
            destroy();
            w = width;
            h = height;
            buffer = new Vector3[w*h];
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
        delete buffer;
        buffer = nullptr;
        w = h = 0;
    }
}


