#ifndef LIST_H
#define LIST_H

#include <map>
#include <string>
#include <memory>
#include "shape.h"
#include "texture.h"
#include "material.h"
#include "sphere.h"

template < class T>
class List {
public:
    List(): count(0) {}

    std::shared_ptr<T> add(std::shared_ptr<T> item, const std::string &name="")
    {
        ++count;
        return list[name=="" ? std::to_string(count) : name] = item;
    }
    std::shared_ptr<T> get(const std::string &name)
    {
        auto item = list.find(name);
        return item == list.end() ? nullptr : item->second;
    }
protected:
    size_t count;
    std::map<std::string, std::shared_ptr<T>> list;
};

class ShapeFactory {
public:
    struct{
        static std::shared_ptr<Sphere>      Create(const std::string &name, const Vector3 &center={0.0f}, const float &radius=1.0f)
        {
            std::shared_ptr<Sphere> sphere = Sphere::Create(center, radius);
            list.add(sphere);
            return sphere;
        }
        static std::shared_ptr<Instance>    CreateInstance(const Vector3 &center={0.0f}, const float &radius=1.0f);
        static std::shared_ptr<Instance>    CreateInstance(std::shared_ptr<Sphere> shape);
        static std::shared_ptr<Instance>    GetInstance(const std::string &name);
    } Spher;
private:
    static List<Shape> list;
};

class MaterialFactory{};

#endif // LIST_H
