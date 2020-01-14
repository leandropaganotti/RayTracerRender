#include "texture.h"
#include "utils.h"

std::map<std::string, std::shared_ptr<Texture>> Texture::TextureList;

const std::shared_ptr<Texture> Texture::SolidWhite = Solid::Create("SolidWhite");

Texture::Texture(const std::string &name)
{
    this->name = name=="" ? std::to_string(TextureList.size()) : name;
}

std::shared_ptr<Texture> Texture::GetByName(const std::string &name)
{
    auto tex = TextureList.find(name);
    return tex == TextureList.end() ? nullptr : tex->second;
}

std::shared_ptr<ChessBoard> ChessBoard::Create(const std::string &name, const Vector3 &color1, const Vector3 &color2, float rows, float cols, float angle)
{
    std::shared_ptr<ChessBoard> chessboard(new ChessBoard(name));
    chessboard->color1 = color1;
    chessboard->color2 = color2;
    chessboard->rows = rows;
    chessboard->cols = cols;
    chessboard->angle = angle;    
    TextureList[chessboard->getName()] = chessboard;
    return chessboard;
}

const Vector3& ChessBoard::get(const Vector2 &uv) const
{
    float s = uv.u*cos(deg2rad(angle)) - uv.v*sin(deg2rad(angle));
    float t = uv.v*cos(deg2rad(angle)) + uv.u*sin(deg2rad(angle));

    if ((modulo(s*cols) < 0.5f) ^ (modulo(t*rows) < 0.5f))
		return color1;
	else
		return color2;
}

std::shared_ptr<Tiles> Tiles::Create(const std::string &name, const Vector3 &colorTile, const Vector3 &colorEdge, float rows, float cols, float angle, float uedge, float vedge)
{
    std::shared_ptr<Tiles> tiles(new Tiles(name));
    tiles->colorTile = colorTile;
    tiles->colorEdge = colorEdge;
    tiles->rows = rows;
    tiles->cols = cols;
    tiles->angle = angle;
    tiles->uedge = uedge;
    tiles->vedge = vedge;
    TextureList[tiles->getName()] = tiles;
    return tiles;
}

const Vector3& Tiles::get(const Vector2 &uv) const
{
    float s = uv.u*cos(deg2rad(angle)) - uv.v*sin(deg2rad(angle));
    float t = uv.v*cos(deg2rad(angle)) + uv.u*sin(deg2rad(angle));
    if (modulo(s*cols) > uedge && modulo(t*rows) > vedge)
        return colorTile;
    else
        return colorEdge;
}



