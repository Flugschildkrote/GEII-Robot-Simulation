#include "gwComponent.h"
#include "gwGameObject.h"

using namespace gwe;

gwe::gwComponent::gwComponent(GW_COMPONENT_TYPE TYPE) : mTYPE(TYPE), mLinkedObject(nullptr)
{
    //ctor
}

gwe::gwComponent::~gwComponent()
{
    if(mLinkedObject != nullptr)
        mLinkedObject->removeComponent(mTYPE);
}

GW_COMPONENT_TYPE gwe::gwComponent::getComponentType(void) const { return mTYPE; }
gwGameObject* gwe::gwComponent::getLinkedObject(void) const { return mLinkedObject; }

GW_LINK_STATE gwe::gwComponent::linkObject(gwGameObject* object)
{
    if(object == nullptr){
        return GW_LINK_DENIED;
    }

	if(object->getLinkState() == GW_LINK_ADD){
		mLinkedObject = object;
		return GW_LINK_ADD;
	}
	else if(object->getLinkState() == GW_LINK_REMOVE){
		mLinkedObject = nullptr;
		return GW_LINK_REMOVE;
	}
	std::cout << "[COMPONENT] Can't add Component of Type " << mTYPE << " to GameObject \"" << object->getName() << "\" : Access denied." << std::endl;
	return GW_LINK_DENIED;
}
