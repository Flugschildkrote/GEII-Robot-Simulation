#ifndef GWSKYBOXMESH_H
#define GWSKYBOXMESH_H

#include "gwGlobalHeader.h"
#include "gwBoxMesh.h"

namespace gwe {

class gwSkyboxMesh : public gwBoxMesh
{
public:
    gwSkyboxMesh(const std::string &name, float boxSize = 4.0);
    virtual ~gwSkyboxMesh(void);

protected:
    virtual void genBuffers(void);
    virtual void generateShape(void);
    float mTID[36];
    int mTIDBytes;
};

}
#endif // GWSKYBOXMESH_H
