#include "texture.h"
#include "utils.h"

ChessBoard::ChessBoard(const Vector3f &color1, const Vector3f &color2, float rows, float cols, float angle):
    color1(color1), color2(color2), rows(rows), cols(cols), angle(angle)
{

}

Vector3f ChessBoard::get(float u, float v) const
{
    float s = u*cos(deg2rad(angle)) - v*sin(deg2rad(angle));
    float t = v*cos(deg2rad(angle)) + u*sin(deg2rad(angle));

    if ((modulo(s*cols) < 0.5f) ^ (modulo(t*rows) < 0.5f))
		return color1;
	else
		return color2;
}

Tiles::Tiles(const Vector3f &colorTile, const Vector3f &colorEdge, float rows, float cols, float angle, float uedge, float vedge):
    color1(colorTile), color2(colorEdge), rows(rows), cols(cols), angle(angle), uedge(uedge), vedge(vedge)
{

}

Vector3f Tiles::get(float u, float v) const
{
    float s = u*cos(deg2rad(angle)) - v*sin(deg2rad(angle));
    float t = v*cos(deg2rad(angle)) + u*sin(deg2rad(angle));
    if (modulo(s*cols) > uedge && modulo(t*rows) > vedge)
        return color1;
    else
        return color2;
}


