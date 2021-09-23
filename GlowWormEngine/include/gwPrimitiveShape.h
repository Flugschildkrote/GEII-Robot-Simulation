#ifndef GWPRIMITIVESHAPE_H
#define GWPRIMITIVESHAPE_H

#include "gwGlobalHeader.h"
#include "gwMesh.h"

namespace gwe {

class gwPrimitiveShape : public gwMesh
{
public:
    gwPrimitiveShape(const std::string &name);
    virtual ~gwPrimitiveShape(void);

    virtual bool load(void);

protected :
    virtual void generateShape(void) = 0;
};


}

#endif // GWPRIMITIVESHAPE_H
