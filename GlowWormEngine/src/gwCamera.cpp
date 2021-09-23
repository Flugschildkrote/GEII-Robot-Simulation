#include "gwCamera.h"
#include "gwGameObject.h"
#include "gwTransform.h"
#include "gwUtils.h"

using namespace gwe;

gwe::gwCamera::gwCamera(void) : gwComponent(GW_COMPONENT_CAMERA),  mView(1.0)
{
}

gwe::gwCamera::~gwCamera(void) { }


glm::mat4 gwe::gwCamera::getView(void) const { return mView; }
glm::mat4* gwe::gwCamera::getView_p(void) { return &mView; }

void gwe::gwCamera::update(void)
{
    gwTransform* objTransform = mLinkedObject->getTransform();
    mView = glm::lookAt(objTransform->getWorldPos(), objTransform->getWorldPos()+objTransform->getWorldForward(), objTransform->getWorldUp());
}

void gwe::gwCamera::render(void)
{

}
