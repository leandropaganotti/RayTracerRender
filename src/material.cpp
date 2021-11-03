#include "material.h"
#include "resource.h"
#include "intersectiondata.h"
#include "utils.h"

namespace material
{
    std::shared_ptr<Material> DiffuseWhite = Resource<Material>::Create("DiffuseWhite", MaterialType::DIFFUSE);
    std::shared_ptr<Material> Glass = Resource<Material>::Create("Glass", MaterialType::TRANSPARENT);
    //std::shared_ptr<Material> Mirror = Resource<Material>::Create("Mirror", MaterialType::SPECULAR);
    std::shared_ptr<Material> Mirror = Resource<Material>::Create("Mirror", MaterialType::MIRROR);

}

std::shared_ptr<Material> Material::Create(MaterialType type)
{
    return Create("", type);
}

std::shared_ptr<Material> Material::Create(const std::string &name, MaterialType type)
{
    std::shared_ptr<Material> material;
    switch (type) {
    case MaterialType::DIFFUSE:
        material.reset(new Diffuse(name));
        break;
    case MaterialType::MIRROR:
        material.reset(new Mirror(name));
        break;
    case MaterialType::TRANSPARENT:
        material.reset(new Transparent(name));
        break;
    default:
        material.reset(new Material(name, type));
    }
    return material;
}

Material::Material(const std::string &name, MaterialType type)
{
    this->name = name;
    this->type = type;
}

void Diffuse::bsdf(const IntersectionData &isec, ScatterData &srec) const
{
    Vector3 tex(1.0);

    if(texture) {
        tex = texture->get(isec.uv);
    }

    srec.pdf[0] = 1.0 / (2.0 * M_PI);
    Vector3 wi = randomUnitVectorInHemisphereOf(isec.normal);
    srec.scattered[0] = Ray(isec.phit + 0.0001*isec.normal, wi);
    srec.f[0] =  (Kd * tex) / M_PI;
}

void Mirror::bsdf(const IntersectionData &isec, ScatterData &srec) const
{
    srec.pdf[0] = 1.0;
    Vector3 wi = reflect(isec.wo, isec.normal).normalize();
    srec.scattered[0] = Ray(isec.phit + 0.0001*isec.normal, wi);
    srec.f[0] =  Kd / std::fabs(isec.wo ^ isec.normal);
}

void Transparent::bsdf(const IntersectionData &isec, ScatterData &srec) const
{
    Vector3 normal = isec.normal;
    float etai = 1.0, etat=isec.material->Ni;
    float cosThetaI = isec.wo ^ isec.normal;

    if (cosThetaI > 0.0) {
        normal = -normal;
        std::swap(etai, etat);
    }

    float Re = fresnel(isec.wo, normal, etai, etat);

    if (Re < 1.0){
        if (dis(gen) < 0.5){
            srec.pdf[0] = 0.5;
            srec.scattered[0] = Ray(isec.phit + 0.0001 * normal, reflect(isec.wo, normal).normalize());
            srec.f[0] = Kd * Re / std::fabs(cosThetaI);
        } else {
            srec.pdf[1] = 0.5;
            srec.scattered[1] = Ray(isec.phit - 0.0001 * normal, refract(isec.wo, normal, etai, etat).normalize());
            srec.f[1] = Kd * (1.0 - Re) / std::fabs(cosThetaI);
        }
    } else {
        srec.pdf[0] = 1.0;
        srec.scattered[0] = Ray(isec.phit + 0.0001 * normal, reflect(isec.wo, normal).normalize());
        srec.f[0] =  Kd / std::fabs(cosThetaI);
    }
}
