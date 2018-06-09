#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "vector.h"

class Texture
{
public:
    Texture(float umax=1.0f, float vmax=1.0f);
	virtual ~Texture() = default;
	virtual Vector3f get(float u, float v) const = 0;

    float getUmax() const
    {
        return umax;
    }
    void setUmax(float value)
    {
        umax = value;
    }

    float getVmax() const
    {
        return vmax;
    }
    void setVmax(float value)
    {
        vmax = value;
    }
protected:
    float umax;
    float vmax;
};

class ChessBoard: public Texture
{
public:
    ChessBoard(const Vector3f &color1={0.0f}, const Vector3f &color2={1.0f}, size_t rows=1, size_t cols=1, float umax=1.0f, float vmax=1.0f);
	virtual Vector3f get(float u, float v) const;
private:
    size_t rows;
    size_t cols;

	Vector3f color1;
	Vector3f color2;
};


class Tiles: public Texture
{
public:
    Tiles(float umax=1.0f, float vmax=1.0f);
    Vector3f get(float u, float v) const;

private:
    Vector3f colorTile;
    Vector3f colorEdge;
    float uedge;
    float vedge;
};
#endif /* TEXTURE_H_ */
