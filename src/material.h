#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"
#include "texture.h"
#include "resource.h"

#include <vector>

class Material: public Resource
{
public:
    enum class Type { DIFFUSE, SPECULAR, TRANSPARENT };
    Type type;
    Vector3 Kd;
    Vector3 E;
    float Ks;
    float Ns;
    float R0;
    float Ni;
    std::shared_ptr<Texture>  texture;

    static std::shared_ptr<Material> Create(const std::string &key, Type type);
    static std::shared_ptr<Material> Create(const std::string &key);

    static std::shared_ptr<Material> Get(const std::string &key);

    static const std::shared_ptr<Material> DiffuseWhite;
    static const std::shared_ptr<Material> Glass;
    static const std::shared_ptr<Material> Mirror;

    ~Material(){
        std::cout << "dtor "  << key << std::endl;
    }
protected:
    Material(Type type);
    Material();
};

#endif // MATERIAL_H
