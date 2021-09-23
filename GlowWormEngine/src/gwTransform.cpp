#include "gwTransform.h"
#include "gwGameObject.h"

using namespace gwe;

gwe::gwTransform::gwTransform(gwGameObject* object) : mGameObject(object), mRelTransformations(1.0), mParentTransformations(1.0)
{
    calculateTransformations();
}

gwe::gwTransform::~gwTransform()
{
    //dtor
}


glm::vec3 gwe::gwTransform::getRelPos(void) const { return mRelPos; }
glm::vec3 gwe::gwTransform::getWorldPos(void) const { return mWorldPos; }

glm::vec3 gwe::gwTransform::getWorldForward(void) const { return mWorldForward; }
glm::vec3 gwe::gwTransform::getWorldUp(void) const { return mWorldUp; }
glm::vec3 gwe::gwTransform::getWoldSide(void) const { return mWorldSide; }

glm::mat4 gwe::gwTransform::getRelTransform(void) const { return mRelTransformations; }
glm::mat4 gwe::gwTransform::getWorldTransform(void) const { return mFinalTransformations; }

void gwe::gwTransform::setParentTransform(const glm::mat4 &parentTransform)
{
   mParentTransformations = parentTransform;
   calculateTransformations();
}

void gwe::gwTransform::rotate(const float &angle, const glm::vec3 &axis,  const GW_SPACE_REFERENCE &SPACE)
{
    switch(SPACE)
    {
    case GW_LOCAL_SPACE :
        mRelTransformations = glm::rotate(mRelTransformations, angle, axis);
        break;
    case GW_WORLD_SPACE :
        mRelTransformations = glm::rotate(mFinalTransformations,angle, glm::vec3(glm::vec4(glm::inverse(mFinalTransformations)*glm::vec4(axis, 0.0))));
        break;
    default:
        break;
    }
    calculateTransformations();
}

void gwe::gwTransform::translate(const glm::vec3 &translation,  const GW_SPACE_REFERENCE &SPACE)
{
    switch(SPACE)
    {
    case GW_LOCAL_SPACE :
        mRelTransformations = glm::translate(mRelTransformations, translation);
        break;
    case GW_WORLD_SPACE :
        mFinalTransformations = glm::translate(glm::mat4(1.0), translation)*mFinalTransformations;
        mRelTransformations = glm::inverse(mParentTransformations)*mFinalTransformations;
        break;
    case GW_PARENT_SPACE :
        break;
    }
    calculateTransformations();
}

void gwe::gwTransform::setTransform(const glm::mat4 &newTansform){
    mRelTransformations = newTansform;
    calculateTransformations();
}





void gwe::gwTransform::calculateTransformations(void)
{
    mFinalTransformations = mParentTransformations*mRelTransformations;

    mRelPos = glm::vec3(glm::vec4(mRelTransformations*glm::vec4(0,0,0,1.0)));
    mWorldPos = glm::vec3(glm::vec4(mFinalTransformations*glm::vec4(0,0,0,1.0)));

    mWorldForward = glm::vec3(glm::vec4(mFinalTransformations*glm::vec4(0,0,1,0)));
    mWorldUp = glm::vec3(glm::vec4(mFinalTransformations*glm::vec4(0,1,0,0)));
    mWorldSide = glm::vec3(glm::vec4(mFinalTransformations*glm::vec4(1,0,0,0)));

    mGameObject->updateChildrenTransform();
}
