#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "resource.h"
#include "vector.h"
#include <map>
#include <memory>

class Texture;

enum class TextureType { Solid=1, Tiles, ChessBoard };

namespace texture
{
    extern const std::shared_ptr<Texture> SolidWhite;
}

class Texture: public Resource
{
public:    
    virtual ~Texture();
    virtual const Vector3& get(const Vector2 &uv) const = 0;
protected:
    Texture();
};

class Solid: public Texture
{
public:
    static std::shared_ptr<Texture> Create(const std::string &key, const Vector3 &color);
    const Vector3& get(const Vector2 &) const;
protected:
    Solid(const Vector3 &color);
    Vector3 color;
};

class ChessBoard: public Texture
{
public:    
    static std::shared_ptr<ChessBoard> Create(const std::string &key, const Vector3 &color1={0.0f}, const Vector3 &color2={1.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f);
    const Vector3& get(const Vector2 &uv) const;
protected:
    ChessBoard();
    Vector3 color1;
    Vector3 color2;
    float rows;
    float cols;
    float angle;
};

class Tiles: public Texture
{
public:
    static std::shared_ptr<Tiles> Create(const std::string &key, const Vector3 &colorTile={1.0f}, const Vector3 &colorEdge={0.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f, float uedge=0.01f, float vedge=0.0f);
    const Vector3& get(const Vector2 &uv) const;
protected:
    Tiles();
    Vector3 colorTile;
    Vector3 colorEdge;
    float rows;
    float cols;
    float angle;
    float uedge;
    float vedge;
};
#endif /* TEXTURE_H_ */
