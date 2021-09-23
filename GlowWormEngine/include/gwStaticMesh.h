#ifndef GWSTATICMESH_H
#define GWSTATICMESH_H

#include "gwGlobalHeader.h"
#include "gwMesh.h"

namespace gwe{
class gwStaticMesh : public gwMesh
{
public:
    gwStaticMesh(const std::string &name, const std::string &sourceFile);
    virtual ~gwStaticMesh(void);

    virtual bool load(void);
protected:
    bool loadObj(void);
    std::string mSourceFile;
};
}

#endif // GWSTATICMESH_H
