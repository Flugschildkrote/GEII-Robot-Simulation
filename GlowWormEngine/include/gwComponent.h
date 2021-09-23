#ifndef GWCOMPONENT_H
#define GWCOMPONENT_H

#include "gwGlobalHeader.h"
namespace gwe {

class gwComponent
{

public:
    gwComponent(GW_COMPONENT_TYPE TYPE);
    virtual ~gwComponent(void) = 0;

    GW_COMPONENT_TYPE getComponentType(void) const;
    gwGameObject* getLinkedObject(void) const;
    GW_LINK_STATE linkObject(gwGameObject* object);
protected:
    const GW_COMPONENT_TYPE mTYPE;
    gwGameObject *mLinkedObject;
};


}
#endif // GWCOMPONENT_H
