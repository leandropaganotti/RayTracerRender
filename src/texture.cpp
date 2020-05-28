#include "texture.h"
#include "utils.h"
#include "consts.h"

namespace texture
{
    const std::shared_ptr<Texture> SolidWhite = Solid::Create("SolidWhite", Color::WHITE);
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

    if(image.read_ppm_bin(filepath.c_str()))
    {
        Texture2d *tex2d = new Texture2d(image);
        return Resource::Create(key, tex2d);
    }
    return nullptr;
}

const Vector3 Texture2d::get(const Vector2 &uv) const
{   
    float i = modulo(uv.v*gridSizeV) * (image.height()-1);
    float j = modulo(uv.u*gridSizeU) * (image.width()-1);
    return image.at(std::round(i), std::round(j));
}

Texture2d::Texture2d(Image &image)
{
    this->image = std::move(image);
    gridSizeU = gridSizeV = 1;
}

void Texture2d::setGridSizeV(unsigned int value)
{
    gridSizeV = value;
}

void Texture2d::setGridSizeU(unsigned int value)
{
    gridSizeU = value;
}
