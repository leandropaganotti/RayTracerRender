#include "shape.h"

const std::shared_ptr<Shape> Shape::Invisible = InvisibleShape::Create();

bool InvisibleShape::intersection(const Ray &, IntersectionData &) const { return false; }
bool InvisibleShape::intersection(const Ray &, float &) const { return false; }
const Vector3 InvisibleShape::normal(const Vector3 &, size_t) const { return {0,0,0}; }
const std::pair<float, float> InvisibleShape::uv(const Vector3 &, size_t) const { return std::pair<float, float> (0,0); }





