#ifndef GWBOXMESH_H
#define GWBOXMESH_H

#include "gwGlobalHeader.h"
#include "gwPrimitiveShape.h"

namespace gwe {


class gwBoxMesh : public gwPrimitiveShape
{
    public:
        gwBoxMesh(const std::string &name, float width, float height, float depht);
        virtual ~gwBoxMesh();

        float getWidth(void) const;
        float getHeight(void) const;
        float getDepth(void) const;

    protected:
        const float mWidth, mHeight, mDepth;
        void generateShape(void);
};


}
#endif // GWBOXMESH_H
