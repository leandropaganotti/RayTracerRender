#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "vector.h"

class Texture
{
public:
	virtual ~Texture() = default;
	virtual Vector3f get(float u, float v) const = 0;
};

class ChessBoard: public Texture
{
public:
    ChessBoard(const Vector3f &color1={0.0f}, const Vector3f &color2={1.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f);
	virtual Vector3f get(float u, float v) const;
private:
    Vector3f color1;
    Vector3f color2;
    float rows;
    float cols;
    float angle;
};


class Tiles: public Texture
{
public:
    Tiles(const Vector3f &color1={1.0f}, const Vector3f &color2={0.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f, float uedge=0.01f, float vedge=0.0f);
    Vector3f get(float u, float v) const;

private:
    Vector3f color1;
    Vector3f color2;
    float rows;
    float cols;
    float angle;
    float uedge;
    float vedge;
};
#endif /* TEXTURE_H_ */