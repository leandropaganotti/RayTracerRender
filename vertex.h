#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "vector.h"

class Vertex: public Vector3f
{

public:

    Vertex(const Vector3f &v): Vector3f(v){}

    Vertex(float x): Vector3f(x){}

    Vertex(float x, float y, float z): Vector3f(x, y, z){}

    ~Vertex(){}
};

class VertexBuffer: public std::vector<Vertex>
{
public:
    VertexBuffer() {}

    friend std::ostream& operator << (std::ostream& os, const VertexBuffer& vb);
};

extern VertexBuffer vertexBuffer;

#endif // VERTEX_H
