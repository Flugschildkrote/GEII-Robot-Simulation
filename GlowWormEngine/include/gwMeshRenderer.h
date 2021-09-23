#ifndef GWMESHRENDERER_H
#define GWMESHRENDERER_H

#include "gwGlobalHeader.h"
#include "gwComponent.h"

namespace gwe {

class gwMeshRenderer : public gwComponent
{
public:
    gwMeshRenderer(gwMesh* mesh = nullptr);
    virtual ~gwMeshRenderer(void);

    gwMesh* getMesh(void) const;
    void render(void);
    void setMesh(gwMesh* mesh);

protected:
    gwMesh* mMesh;

};
}
#endif // GWMESHRENDERER_H
