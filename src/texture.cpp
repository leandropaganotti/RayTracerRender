#include "texture.h"

ChessBoard::ChessBoard(float size, const Vector3f &color1, const Vector3f &color2)
{
	this->size = size;
	this->color1 = color1;
	this->color2 = color2;
}

Vector3f ChessBoard::get(float u, float v) const
{
	if ((int(u/size) + int(v/size)) % 2)
		return color1;
	else
		return color2;
}
