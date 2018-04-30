#include "vertex.h"

VertexBuffer vertexBuffer;

std::ostream &operator <<(std::ostream &os, const VertexBuffer &vb)
{
    os << "VertexBuffer: " << std::endl;
    for(size_t i= 0 ; i < vb.size(); ++i)
        os << "|" << i << "->" << vb[i] << std::endl;
    return os;
}
