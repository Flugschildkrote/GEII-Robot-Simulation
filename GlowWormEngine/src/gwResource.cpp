#include "gwResource.h"
#include "gwResourceManager.h"

using namespace gwe;

gwe::gwResource::gwResource(const std::string &name) : mLoaded(false), mName(name)
{
    while(!singleton::gwResourceManager::getInstance()->addResource(this))
    {
        std::cout << "[WARNING][RESOURCE] Can't add resource \"" << mName << "\" to resource manager, resource renamed \"" << mName+"_bis" << "\"" << std::endl;
        mName += "_bis";
    }
}

gwe::gwResource::~gwResource()
{
    singleton::gwResourceManager::getInstance()->removeResource(mName);
    std::cout << "[INFO][RESOURCE] \"" << mName << "\" deleted." << std::endl;
}

bool gwe::gwResource::free(void)
{
    mLoaded = false;
    std::cout << "[INFO][RESOURCE] \"" << mName << "\" freed." << std::endl;
    return true;
}

std::string gwe::gwResource::getName(void) const { return mName; }
bool gwe::gwResource::isLoaded(void) const { return mLoaded; }

bool gwe::gwResource::load(void)
{
    mLoaded = true;
    std::cout << "[INFO][RESOURCE] \"" << mName << "\" loaded." << std::endl;
    return true;
}


