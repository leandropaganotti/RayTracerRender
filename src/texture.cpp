#include "texture.h"
#include "utils.h"

ChessBoard::ChessBoard(const Vector3f &color1, const Vector3f &color2, size_t rows, size_t cols):
    color1(color1), color2(color2), rows(rows), cols(cols)
{

}

Vector3f ChessBoard::get(float u, float v) const
{
    if ((modulo(u*cols) < 0.5f) ^ (modulo(v*rows) < 0.5f))
		return color1;
	else
		return color2;
}

Tiles::Tiles(const Vector3f &colorTile, const Vector3f &colorEdge, float rows, float cols, float uedge, float vedge):
    colorTile(colorTile), colorEdge(colorEdge), rows(rows), cols(cols), uedge(uedge), vedge(vedge)
{

}

Vector3f Tiles::get(float u, float v) const
{
    if (modulo(u*cols) > uedge && modulo(v*rows) > vedge)
        return colorTile;
    else
        return colorEdge;
}


