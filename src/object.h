#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include "material.h"
#include "texture.h"
#include "intersection.h"
#include "shape.h"
#include "vector"

class Object: public virtual Shape
{           
public:

    Object(): tex(std::shared_ptr<Texture>(new SolidWhite)){}

    virtual ~Object() = default;

    const std::shared_ptr<Texture> & getTexture() const { return tex; }
    void setTexture(const std::shared_ptr<Texture> &tex) { if (tex != nullptr) this->tex = tex; }
    const Material & getMaterial() const { return material; }
    void setMaterial(const Material &material) { this->material = material; }

    virtual const Vector3 color(const IntersectionData&) const
    {
//        const std::pair<float, float> _uv = uv(isec.phit, isec.idx);
//        return tex->get(_uv.first, _uv.second) * material.kd;
        return material.kd;
    }
    virtual const std::pair<float, float> uv(const Vector3 &, size_t) const
    {
        return std::pair<float, float>(0.0f,0.0f);
    }
protected:
    std::shared_ptr<Texture> tex;
    Material material;
};

typedef std::vector<std::unique_ptr<Object>> ObjectVector;

#endif // OBJECT_H
