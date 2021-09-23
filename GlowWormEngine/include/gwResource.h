#ifndef GWRESOURCE_H
#define GWRESOURCE_H

#include "gwGlobalHeader.h"

namespace gwe {

class gwResource
{
public:
    gwResource(const std::string &name);
    virtual ~gwResource();

    virtual bool free(void) = 0;
    std::string getFilePath(void) const;
    std::string getName(void) const;
    bool isLoaded(void) const;
    virtual bool load(void) = 0;
private :
    bool mLoaded;
    std::string mName;
};

}
#endif // GWRESOURCE_H
