#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "vector.h"
#include "consts.h"
#include <map>
#include <memory>

class Texture
{
protected:
    Texture(const std::string &name="");
    std::string name;
    static std::map<std::string, std::shared_ptr<Texture>> TextureList;
public:    
	virtual ~Texture() = default;
    const std::string& getName(){ return name; }
    virtual const Vector3& get(const Vector2 &uv) const = 0;

    static std::shared_ptr<Texture> GetByName(const std::string &name);

    static const std::shared_ptr<Texture> SolidWhite;
};

class Solid: public Texture
{
public:
    static std::shared_ptr<Texture> Create(const std::string &name="")  { return std::shared_ptr<Texture>(new Solid(name));}
    const Vector3& get(const Vector2 &) const { return Color::WHITE; }
protected:
    Solid(const std::string &name):Texture(name){}
};

class ChessBoard: public Texture
{
public:    
    static std::shared_ptr<ChessBoard> Create(const std::string &name="", const Vector3 &color1={0.0f}, const Vector3 &color2={1.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f);
    const Vector3& get(const Vector2 &uv) const;
protected:
    ChessBoard(const std::string &name=""):Texture(name){}
    Vector3 color1;
    Vector3 color2;
    float rows;
    float cols;
    float angle;
};

class Tiles: public Texture
{
public:
    static std::shared_ptr<Tiles> Create(const std::string &name="", const Vector3 &colorTile={1.0f}, const Vector3 &colorEdge={0.0f}, float rows=1.0f, float cols=1.0f, float angle=0.0f, float uedge=0.01f, float vedge=0.0f);
    const Vector3& get(const Vector2 &uv) const;
protected:
    Tiles(const std::string &name=""):Texture(name){}
    Vector3 colorTile;
    Vector3 colorEdge;
    float rows;
    float cols;
    float angle;
    float uedge;
    float vedge;
};
#endif /* TEXTURE_H_ */
