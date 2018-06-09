#include "texture.h"
#include "utils.h"

Texture::Texture(float umax, float vmax): umax(umax), vmax(vmax)
{

}

ChessBoard::ChessBoard(const Vector3f &color1, const Vector3f &color2, size_t rows, size_t cols, float umax, float vmax):
    Texture(umax, vmax), color1(color1), color2(color2), rows(rows), cols(cols)
{

}

Vector3f ChessBoard::get(float u, float v) const
{
    if ((modulo(u*cols) < 0.5f) ^ (modulo(v*rows) < 0.5f))
		return color1;
	else
		return color2;
}

Tiles::Tiles(float umax, float vmax): Texture(umax, vmax)
{
    colorTile = 1;
    colorEdge = 0;
    uedge = 0.01;
    vedge = 0.1;
}

Vector3f Tiles::get(float u, float v) const
{
    if (modulo(u) > uedge && modulo(v) > vedge)
        return colorTile;
    else
        return colorEdge;
}


