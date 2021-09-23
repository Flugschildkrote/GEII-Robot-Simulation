#ifndef gwResourceManager_H
#define gwResourceManager_H

#include "gwGlobalHeader.h"
#include "gwSingleton.h"


namespace gwe{ namespace singleton{

class gwResourceManager : public gwSingleton<gwResourceManager>
{
    friend class gwSingleton<gwResourceManager>;

public:
    bool addResource(gwResource* resource);
    gwResource* getResource(const std::string &resourceName) const;
    bool removeResource(const std::string &resourceName);
protected:
    gwResourceManager(void);
    virtual ~gwResourceManager(void);

private :
    std::map<std::string, gwResource*> mResources;
};

}}
#endif // gwResourceManager_H
