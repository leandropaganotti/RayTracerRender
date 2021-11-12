#include "material.h"
#include "resource.h"
#include "intersectiondata.h"
#include "utils.h"

const float bias = 0.001f;
namespace material
{
    std::shared_ptr<Material> DiffuseWhite = Resource<Material>::Create("DiffuseWhite", MaterialType::DIFFUSE);
    std::shared_ptr<Material> Glass = Resource<Material>::Create("Glass", MaterialType::TRANSPARENT);
    std::shared_ptr<Material> Glossy = Resource<Material>::Create("Glossy", MaterialType::SPECULAR);
    std::shared_ptr<Material> Mirror = Resource<Material>::Create("Mirror", MaterialType::MIRROR);
    std::shared_ptr<Material> Metal = Resource<Material>::Create("Mirror", MaterialType::METAL);
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
    case MaterialType::SPECULAR:
        material.reset(new Specular(name));
        break;
    case MaterialType::METAL:
        material.reset(new Metal(name));
        break;
    case MaterialType::DIFFUSE_LIGHT:
        material.reset(new DiffuseLight(name));
        break;
    default:
        material = nullptr;
        std::cerr << "Material type not recognized" << std::endl;
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

    Vector3 wi = uniformSampleHemisphere(isec.normal, srec.pdf);
    srec.scattered = Ray(isec.phit + bias*isec.normal, wi);
    srec.f =  (Kd * tex) / M_PI;
}

Vector3 Diffuse::bsdf(const IntersectionData &isec, const Vector3 &) const
{
    Vector3 tex(1.0);

    if(texture) {
        tex = texture->get(isec.uv);
    }
    return (Kd * tex) / M_PI;
}

void Mirror::bsdf(const IntersectionData &isec, ScatterData &srec) const
{
    srec.pdf = 1.0;
    Vector3 wi = reflect(isec.wo, isec.normal).normalize();
    srec.scattered = Ray(isec.phit + bias*isec.normal, wi);
    srec.f =  Kd / std::fabs(wi ^ isec.normal);
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
    float P = (Re + 0.5f) / 2.0f;

    if (Re == 1.0f){
        srec.pdf = 1.0f;
        srec.scattered = Ray(isec.phit + bias * normal, reflect(isec.wo, normal).normalize());
        srec.f =  Kd / std::fabs(srec.scattered.direction ^ isec.normal);
    }else if (dis(gen) < P) {
        srec.pdf = P;
        srec.scattered = Ray(isec.phit + bias * normal, reflect(isec.wo, normal).normalize());
        srec.f =  Re * Kd / std::fabs(srec.scattered.direction ^ isec.normal);
    } else {
        srec.pdf = (1.0f - P);
        srec.scattered = Ray(isec.phit - bias * normal, refract(isec.wo, normal, etai, etat).normalize());
        srec.f = (1.0 - Re) * Kd / std::fabs(srec.scattered.direction ^ isec.normal);
    }
}

void Specular::bsdf(const IntersectionData &isec, ScatterData &srec) const
{

    float Re = schlick(isec.wo, isec.normal, isec.material->R0);
    float P = (Re + 0.5f) / 2.0f;

    if(Re == 1.0f){
        // reflected
        srec.pdf = 1.0f;
        srec.scattered = Ray(isec.phit + bias * isec.normal, reflect(isec.wo, isec.normal).normalize());
        float cosThetaI = srec.scattered.direction ^ isec.normal;
        srec.f =  Kd / std::fabs(cosThetaI);
    } else if(dis(gen) < P) {
        // reflected
        srec.pdf = P;
        srec.scattered = Ray(isec.phit + bias * isec.normal, reflect(isec.wo, isec.normal).normalize());
        float cosThetaI = srec.scattered.direction ^ isec.normal;
        srec.f =  Re * Kd / std::fabs(cosThetaI);
    } else {
        // diffused
        Vector3 tex(1.0);

        if(texture) {
            tex = texture->get(isec.uv);
        }

        Vector3 wi = uniformSampleHemisphere(isec.normal, srec.pdf);
        srec.pdf *= 1.0f-P;
        srec.scattered = Ray(isec.phit + bias*isec.normal, wi);
        srec.f = (1.0 - Re) * (Kd * tex) / M_PI;
    }
}

Vector3 Specular::bsdf(const IntersectionData &isec, const Vector3 &) const
{
    Vector3 tex(1.0);

    if(texture) {
        tex = texture->get(isec.uv);
    }

    float Re = schlick(isec.wo, isec.normal, isec.material->R0);
    return (1.0 - Re) * (Kd * tex) / M_PI;
}

void Metal::bsdf(const IntersectionData &isec, ScatterData &srec) const
{
    float _1_pdf;
    srec.pdf = 1.0;
    Vector3 wi = (reflect(isec.wo, isec.normal).normalize() + fuzz * uniformSampleHemisphere(isec.normal, _1_pdf)).normalize();
    srec.scattered = Ray(isec.phit + bias*isec.normal, wi);
    srec.f =  Kd / std::fabs(wi ^ isec.normal);
}

void DiffuseLight::bsdf(const IntersectionData &isec, ScatterData &srec) const
{
    srec.pdf = 0.0;
}
