#ifndef GWSQUAREMESH_H
#define GWSQUAREMESH_H

#include "gwGlobalHeader.h"
#include "gwPrimitiveShape.h"

namespace gwe {

class gwSquareMesh : public gwPrimitiveShape
{
public:
    gwSquareMesh(const std::string &name, float width, float height);
    virtual ~gwSquareMesh(void);


    float getWidth(void) const;
    float getHeight(void) const;

protected:
    const float mWidth, mHeight;
private:
    void generateShape(void);
};


}
#endif // GWSQUAREMESH_H
