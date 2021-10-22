#pragma once
#include "shape.h"
#include <memory>
#include <vector>

class Primitive;
class Object;

template <class T>
class Aggregate: public Shape
{
public:
    Aggregate() = default;
    virtual ~Aggregate() = default;

    virtual void build() = 0;
    virtual void clear() { shapes.clear(); };

    void add(std::shared_ptr<T> shape) { shapes.push_back(shape); }
    const std::shared_ptr<T>& operator[](size_t i) const { return shapes[i]; }
    std::shared_ptr<T>& operator[](size_t i) { return shapes[i]; }
    size_t size() { return shapes.size(); }

protected:
    std::vector<std::shared_ptr<T>> shapes;
};
