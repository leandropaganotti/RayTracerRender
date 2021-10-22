#pragma once
#include "object.h"
#include <memory>
#include <string>

class Mesh;

class ObjectFactory
{
public:
    ObjectFactory() = delete;

    static std::shared_ptr<Object> CreateSphereStatic(const Vector3 &position, const float radius, std::shared_ptr<Material> &material);
    static std::shared_ptr<Object> CreateSphere(std::shared_ptr<Material> &material, const Matrix4 &transform);

    static std::shared_ptr<Object> CreatePlaneStatic(const Vector3 &origin, const Vector3 &normal, std::shared_ptr<Material> &material);
    static std::shared_ptr<Object> CreatePlane(std::shared_ptr<Material> &material, const Matrix4 &transform);

    static std::shared_ptr<Object> CreateBox(std::shared_ptr<Material> &material, const Matrix4 &transform);

    static std::shared_ptr<Object> CreateCylinder(std::shared_ptr<Material> &material, const Matrix4 &transform);

    static std::shared_ptr<Object> CreateMesh(const std::string &src, std::shared_ptr<Material> &material, const Matrix4 &transform);
};
