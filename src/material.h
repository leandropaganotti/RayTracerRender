#pragma once

#include <vector>
#include "vector.h"
#include "texture.h"


class ScatterData
{
public:
    Ray scattered;
    Vector3 f = 0.0f;
    float pdf = 0.0f;
};

enum MaterialType { DIFFUSE, SPECULAR, MIRROR, METAL, TRANSPARENT, DIFFUSE_LIGHT};

struct Material;
class IntersectionData;

namespace material
{
    extern std::shared_ptr<Material> DiffuseWhite;
    extern std::shared_ptr<Material> Glass;
    extern std::shared_ptr<Material> Mirror;
}

class Material
{
public:
    Material(const std::string &name="", MaterialType type=MaterialType::DIFFUSE);
    virtual ~Material() = default;

    static std::shared_ptr<Material> Create(MaterialType type);
    static std::shared_ptr<Material> Create(const std::string &name, MaterialType type);

    std::string name;
    MaterialType type = MaterialType::DIFFUSE;
    Vector3 Kd = 1.0;
    Vector3 Ka = 0.1;
    Vector3 emission = 0.0;
    float Ks = 0.0;
    float Ns = 30.0;
    float R0 = 0.98;
    float Ni = 1.55; // refractive index for glass
    float fuzz = 0.5;
    std::shared_ptr<Texture>  texture;

    virtual Vector3 emittance(){ return vector::ZERO; }
    virtual void bsdf(const IntersectionData &, ScatterData &) const {}
    virtual Vector3 bsdf(const IntersectionData &, const Vector3 &) const { return vector::ZERO; }

};

class Diffuse: public Material
{
public:
    Diffuse(const std::string& name=""): Material(name, MaterialType::DIFFUSE){}
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;
    virtual Vector3 bsdf(const IntersectionData &isec, const Vector3 &wi) const override;
};

class Specular: public Material
{
public:
    Specular(const std::string& name=""): Material(name, MaterialType::SPECULAR){}
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;
    virtual Vector3 bsdf(const IntersectionData &isec, const Vector3 &wi) const override;
};

class Mirror: public Material{
public:
    Mirror(const std::string& name=""): Material(name, MaterialType::MIRROR){}
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;
};

class Transparent: public Material{
public:
    Transparent(const std::string& name=""): Material(name, MaterialType::TRANSPARENT){}
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;
};

class Metal: public Material{
public:
    Metal(const std::string& name=""): Material(name, MaterialType::METAL){}
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;
};

class DiffuseLight: public Material{
public:
    DiffuseLight(const std::string& name=""): Material(name, MaterialType::DIFFUSE_LIGHT){}
    virtual void bsdf(const IntersectionData &isec, ScatterData &srec) const override;
};
