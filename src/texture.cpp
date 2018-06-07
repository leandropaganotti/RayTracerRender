#include "texture.h"

ChessBoard::ChessBoard(float width, float height, const Vector3f &color1, const Vector3f &color2)
{
	this->width = width;
	this->height = height;
	this->color1 = color1;
	this->color2 = color2;
}

Vector3f ChessBoard::get(float u, float v) const
{
	u = u / width;
	v = v / height;
	if ((int(u * 2.0f) + int(v * 2.0f)) % 2)
		return color1;
	else
		return color2;
}
