#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"
#include "texture.h"
#include "resource.h"

#include <vector>

class Material;

enum class MaterialType { DIFFUSE, SPECULAR, TRANSPARENT };

namespace material
{
    extern const std::shared_ptr<Material> DiffuseWhite;
    extern const std::shared_ptr<Material> Glass;
    extern const std::shared_ptr<Material> Mirror;
}

class Material: public Resource
{
public:
    MaterialType type;
    Vector3 Kd;
    Vector3 Ka;
    Vector3 E;
    float Ks;
    float Ns;
    float R0;
    float Ni;
    std::shared_ptr<Texture>  texture;

    static std::shared_ptr<Material> Create(const std::string &key, MaterialType type);
    static std::shared_ptr<Material> Create(const std::string &key);

    static std::shared_ptr<Material> Get(const std::string &key);



    ~Material();
protected:
    Material(MaterialType type);
    Material();
};

#endif // MATERIAL_H
