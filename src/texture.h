#pragma once

#include "image.h"
#include "resource.h"
#include "vector.h"
#include <map>
#include <memory>

class Texture;

namespace texture
{
    extern const std::shared_ptr<Texture> SolidWhite;
}

enum class TextureType { SolidTexture=1, Tiles, CheckerBoard, Texture2d };

class Texture
{
public:    
    static std::shared_ptr<Texture> Create(const ParamSet &params);
    virtual ~Texture();
    virtual const Vector3 get(const Vector2 &uv) const = 0;
protected:
    Texture();
};

class SolidTexture: public Texture
{
public:
    static std::shared_ptr<SolidTexture> Create(const Vector3 &color);
    const Vector3 get(const Vector2 &) const;
protected:
    SolidTexture(const Vector3 &color);
    Vector3 color;
};

class Checkerboard: public Texture
{
public:    
    static std::shared_ptr<Checkerboard> Create(const Vector3 &color1={0.0f}, const Vector3 &color2={1.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f);
    const Vector3 get(const Vector2 &uv) const;
protected:
    Checkerboard(const Vector3 &color1={0.0f}, const Vector3 &color2={1.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f);
    Vector3 color1;
    Vector3 color2;
    float rows;
    float cols;
    float angle;
};

class Tiles: public Texture
{
public:
    static std::shared_ptr<Tiles> Create(const Vector3 &colorTile={1.0f}, const Vector3 &colorEdge={0.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f, float uedge=0.01f, float vedge=0.0f);
    const Vector3 get(const Vector2 &uv) const;
protected:
    Tiles(const Vector3 &colorTile={1.0f}, const Vector3 &colorEdge={0.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f, float uedge=0.01f, float vedge=0.0f);
    Vector3 colorTile;
    Vector3 colorEdge;
    float rows;
    float cols;
    float angle;
    float uedge;
    float vedge;
};

class Texture2d: public Texture
{
public:
    static std::shared_ptr<Texture2d> Create(const std::string &filepath, float w=1.0f, float h=1.0f);
    const Vector3 get(const Vector2 &uv) const override;
protected:
    Texture2d(Image &&img, float w, float h);
    Image image;
    float width;  // in meters
    float height; // in meters
};
