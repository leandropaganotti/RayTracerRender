#include "texture.h"
#include "utils.h"
#include "color.h"

namespace texture
{
    const std::shared_ptr<Texture> SolidWhite = SolidTexture::Create(color::WHITE);
}

Texture::Texture(){}

Texture::~Texture() {}

std::shared_ptr<Checkerboard> Checkerboard::Create(const Vector3 &color1, const Vector3 &color2, float rows, float cols, float angle)
{
    return std::shared_ptr<Checkerboard>(new Checkerboard(color1, color2, rows, cols, angle));
}

const Vector3 Checkerboard::get(const Vector2 &uv) const
{
    float s = uv.u*cos(deg2rad(angle)) - uv.v*sin(deg2rad(angle));
    float t = uv.v*cos(deg2rad(angle)) + uv.u*sin(deg2rad(angle));

    if ((modulo(s*cols) < 0.5f) ^ (modulo(t*rows) < 0.5f))
		return color1;
	else
		return color2;
}

Checkerboard::Checkerboard(const Vector3 &color1, const Vector3 &color2, float rows, float cols, float angle):
    color1(color1), color2(color2), rows(rows), cols(cols), angle(angle)
{

}

std::shared_ptr<Tiles> Tiles::Create(const Vector3 &colorTile, const Vector3 &colorEdge, float rows, float cols, float angle, float uedge, float vedge)
{
    return std::shared_ptr<Tiles>(new Tiles(colorTile, colorEdge, rows, cols, angle, uedge, vedge));
}

const Vector3 Tiles::get(const Vector2 &uv) const
{
    float s = uv.u*cos(deg2rad(angle)) - uv.v*sin(deg2rad(angle));
    float t = uv.v*cos(deg2rad(angle)) + uv.u*sin(deg2rad(angle));
    if (modulo(s*cols) > uedge && modulo(t*rows) > vedge)
        return colorTile;
    else
        return colorEdge;
}

Tiles::Tiles(const Vector3 &colorTile, const Vector3 &colorEdge, float rows, float cols, float angle, float uedge, float vedge):
    colorTile(colorTile), colorEdge(colorEdge), rows(rows), cols(cols), angle(angle), uedge(uedge), vedge(vedge)
{

}

std::shared_ptr<SolidTexture> SolidTexture::Create(const Vector3 &color)
{
    return std::shared_ptr<SolidTexture>(new SolidTexture(color));
}

const Vector3 SolidTexture::get(const Vector2 &) const
{
    return color;
}

SolidTexture::SolidTexture(const Vector3 &color): color(color)
{

}

std::shared_ptr<Texture2d> Texture2d::Create(const std::string &filepath, float w, float h)
{
    Image image;

    if(image.read(filepath.c_str()))
    {
        return std::shared_ptr<Texture2d>(new Texture2d(std::move(image), w, h));
    }
    return nullptr;
}

const Vector3 Texture2d::get(const Vector2 &uv) const
{
    float u = uv.u / width;
    float v = uv.v / height;

    u -= int(u);
    v -= int(v);

    if(u<0.0f) u +=1.0f;
    if(v<0.0f) v +=1.0f;

    u *= image.width()-3;
    v *= image.height()-3;

    int iu = int(u);
    int iv = int(v);

    float tu = u-iu;
    float tv = v-iv;

    Vector3 c = image.at(iv, iu)*(1-tu)*(1-tv)+
            image.at(iv, iu+1)*tu*(1-tv)+
            image.at(iv+1, iu)*(1-tu)*tv+
            image.at(iv+1, iu+1)*tu*tv;
    return c;
}

Texture2d::Texture2d(Image &&img, float w, float h): image(std::move(img)), width(w), height(h)
{

}

std::shared_ptr<Texture> Texture::Create(const ParamSet &params)
{
    TextureType type;
    if(params.get<TextureType>("type", type))
    {
        switch (type)
        {
        case TextureType::SolidTexture:
        {
            Vector3 color(0);
            params.get<Vector3>("color", color);
            return SolidTexture::Create(color);
        }
        break;
        case TextureType::Tiles:
        {
            Vector3 color1(1), color2(0);
            float rows=1.0f, cols=1.0f, angle=0.0f, uedge=0.01f, vedge=0.01f;
            params.get<Vector3>("color1", color1);
            params.get<Vector3>("color2", color2);
            params.get<float>("rows", rows);
            params.get<float>("cols", cols);
            params.get<float>("angle", angle);
            params.get<float>("uedge", uedge);
            params.get<float>("vedge", vedge);
            return Tiles::Create(color1, color2, rows, cols, angle, uedge, vedge);
        }
        break;
        case TextureType::CheckerBoard:
        {
            Vector3 color1(1), color2(0);
            float rows=1.0f, cols=1.0f, angle=0.0f;
            params.get<Vector3>("color1", color1);
            params.get<Vector3>("color2", color2);
            params.get<float>("rows", rows);
            params.get<float>("cols", cols);
            params.get<float>("angle", angle);
            return Checkerboard::Create(color1, color2, rows, cols, angle);
        }
        break;
        case TextureType::Texture2d:
        {
            std::string filepath{};
            float w=1.0, h=1.0f;
            params.get<std::string>("src", filepath);
            params.get<float>("width", w);
            params.get<float>("height", h);
            return Texture2d::Create(filepath, w, h);
        }
        break;
        }
    }
    return nullptr;
}
