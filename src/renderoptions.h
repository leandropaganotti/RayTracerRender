#pragma once
#include "vector.h"

enum class Illumination
{
    Minimum,
    Phong,
    GlobalIlumination,
    GlobalIluminationWithDirectSampling
};

struct RenderOptions
{
    RenderOptions()
    {
        illum = Illumination::Minimum;
        ambientIndex = 1.0;
        spp = 1;
        gridSizeXY = 1;
        maxDepth = 1;
        bgColor = Vector3(0.0f);
    }
    Illumination    illum;
    float           ambientIndex;
    size_t          spp;
    size_t          gridSizeXY;
    size_t          maxDepth;
    Vector3         bgColor;

    friend std::ostream &operator <<(std::ostream &os, const RenderOptions &opt);
};

inline
std::ostream & operator <<(std::ostream &os, const RenderOptions &opt)
{
    return os << "RenderOptions: Illumination=" << (int)opt.illum << ", Ni=" << opt.ambientIndex << ", SPP=" << opt.spp << ", gridSizeXY=" << opt.gridSizeXY << ", maxDepth=" << opt.maxDepth << ", bgColor=" << opt.bgColor ;
}
