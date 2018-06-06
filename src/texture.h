#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "vector.h"

class Texture
{
public:
	Texture() = default;
	virtual ~Texture() = default;
	virtual Vector3f get(float u, float v) const = 0;
};

class ChessBoard: public Texture
{
public:
	ChessBoard(float size=1.0f, const Vector3f &color1={0.0f}, const Vector3f &color2={1.0f});
	virtual Vector3f get(float u, float v) const;

private:
	float size;
	Vector3f color1;
	Vector3f color2;
};

#endif /* TEXTURE_H_ */
