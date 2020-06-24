#include "texture.h"
#include "utils.h"
#include "color.h"

namespace texture
{
    const std::shared_ptr<Texture> SolidWhite = Solid::Create("SolidWhite", color::WHITE);
}

Texture::Texture()
{
}

Texture::~Texture() {}

std::shared_ptr<ChessBoard> ChessBoard::Create(const std::string &key, const Vector3 &color1, const Vector3 &color2, float rows, float cols, float angle)
{
    ChessBoard *chessboard = new ChessBoard;
    chessboard->color1 = color1;
    chessboard->color2 = color2;
    chessboard->rows = rows;
    chessboard->cols = cols;
    chessboard->angle = angle;    
    return Resource::Create<ChessBoard>(key, chessboard);
}

const Vector3 ChessBoard::get(const Vector2 &uv) const
{
    float s = uv.u*cos(deg2rad(angle)) - uv.v*sin(deg2rad(angle));
    float t = uv.v*cos(deg2rad(angle)) + uv.u*sin(deg2rad(angle));

    if ((modulo(s*cols) < 0.5f) ^ (modulo(t*rows) < 0.5f))
		return color1;
	else
		return color2;
}

ChessBoard::ChessBoard(){}

std::shared_ptr<Tiles> Tiles::Create(const std::string &key, const Vector3 &colorTile, const Vector3 &colorEdge, float rows, float cols, float angle, float uedge, float vedge)
{
    Tiles *tiles = new Tiles;
    tiles->colorTile = colorTile;
    tiles->colorEdge = colorEdge;
    tiles->rows = rows;
    tiles->cols = cols;
    tiles->angle = angle;
    tiles->uedge = uedge;
    tiles->vedge = vedge;
    return Resource::Create<Tiles>(key, tiles);
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

Tiles::Tiles(){}

std::shared_ptr<Texture> Solid::Create(const std::string &key, const Vector3 &color)
{
    return Resource::Create<Texture>(key, new Solid(color));
}

const Vector3 Solid::get(const Vector2 &) const
{
    return color;
}

Solid::Solid(const Vector3 &color): color(color)
{

}

std::shared_ptr<Texture2d> Texture2d::Create(const std::string &key, const std::string &filepath)
{
    Image image;

    if(image.read(filepath.c_str()))
    {
        Texture2d *tex2d = new Texture2d(image);
        return Resource::Create(key, tex2d);
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

Texture2d::Texture2d(Image &image)
{
    this->image = std::move(image);
    width = height = 1.0; // meters
}

void Texture2d::setHeight(float value)
{
    height = value;
}

void Texture2d::setWidth(float value)
{
    width = value;
}
