#ifndef SHARP_GP2Y_H
#define SHARP_GP2Y_H

#include "gwGlobalHeader.h"
#include "Physics.h"

#ifndef RESOURCE_NAME
#define RESOURCE_NAME 0
#endif // RESOURCE_NAME

#ifndef RESOURCE_FILE
#define RESOURCE_FILE 1
#endif // RESOURCE_FILE

const std::string SHARP_GP2Y_MESH[2] = {"SHARP_GP2Y_MESH", "Data/Meshs/SHARP_GP2Y.obj"};
const std::string SHARP_GP2Y_TEXTURE[2] = {"SHARP_GP2Y_TEXTURE", "Data/Textures/CNY_70.jpg"};

namespace simu{

class SHARP_GP2Y
{
public:
    SHARP_GP2Y(Physics* physics, gwe::gwScene* scene, gwe::gwGameObject* parent = nullptr);
    virtual ~SHARP_GP2Y();

    void Run(void);
    gwe::gwGameObject* getGameObject(void) const;

    float getResultDistance(void) const;

protected:

    void InitGameObject(gwe::gwScene* scene);
    void InitResources(void);
    void Setup(gwe::gwScene* scene);

    Physics* mPhysics;
    gwe::gwGameObject* mGameObject;
    gwe::gwGameObject* mParent;

    float mResultDistance;
    float mMinDist;
    float mMaxDist;
};
}

#endif // SHARP_GP2Y_H
