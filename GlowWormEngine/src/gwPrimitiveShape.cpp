#include "gwPrimitiveShape.h"

using namespace gwe;

gwe::gwPrimitiveShape::gwPrimitiveShape(const std::string &name) : gwMesh(name)
{

}

gwe::gwPrimitiveShape::~gwPrimitiveShape()
{

}

bool gwe::gwPrimitiveShape::load(void)
{
    if(isLoaded())
        return false;
    generateShape();
    gwMesh::load();
    return true;
}
