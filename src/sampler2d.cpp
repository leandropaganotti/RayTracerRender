#include "sampler2d.h"
#include "image.h"
#include "utils.h"

void CreateSomplesImage(const std::string &filename, std::vector<Vector2> &samples, size_t width, size_t height)
{
    Image image(width, height, 0.0f);

    for (auto sample: samples)
    {
        int j = roundf(sample.x * (width-1));
        int i = roundf(sample.y * (height-1));
        image.at(i, j) = 1.0f;
    }
    image.write_jpg(filename.c_str());
}
