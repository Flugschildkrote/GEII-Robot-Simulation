#ifndef GWTRANSFORM_H
#define GWTRANSFORM_H

#include "gwGlobalHeader.h"

namespace gwe{


class gwTransform
{
public:
    gwTransform(gwGameObject* object);
    virtual ~gwTransform();

    glm::vec3 getRelPos(void) const;
    glm::vec3 getWorldPos(void) const;

    glm::vec3 getWorldForward(void) const;
    glm::vec3 getWorldUp(void) const;
    glm::vec3 getWoldSide(void) const;

    glm::mat4 getRelTransform(void) const;
    glm::mat4 getWorldTransform(void) const;

    void rotate(const float &angle, const glm::vec3 &axis, const GW_SPACE_REFERENCE &SPACE = GW_LOCAL_SPACE);
    void translate(const glm::vec3 &translation, const GW_SPACE_REFERENCE &SPACE = GW_LOCAL_SPACE);

    void setParentTransform(const glm::mat4 &parentTransform); // Need to call calculate transformations;
    void setTransform(const glm::mat4 &newTansform);

protected:
    gwGameObject* mGameObject;

    glm::vec3 mRelPos;
    glm::vec3 mWorldPos;

    glm::vec3 mWorldForward;
    glm::vec3 mWorldUp;
    glm::vec3 mWorldSide;

    glm::mat4 mRelTransformations;
    glm::mat4 mParentTransformations;
    glm::mat4 mFinalTransformations;


    void calculateTransformations(void); /* Function called at each transformaton, need to call object->updateChildTransform */
};


}
#endif // GWTRANSFORM_H
