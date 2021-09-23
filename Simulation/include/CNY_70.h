#ifndef CNY_70_H
#define CNY_70_H

#include "Physics.h"
#include "gwGlobalHeader.h"

#ifndef RESOURCE_NAME
#define RESOURCE_NAME 0
#endif // RESOURCE_NAME

#ifndef RESOURCE_FILE
#define RESOURCE_FILE 1
#endif // RESOURCE_FILE

const std::string CNY_70_MESH[2] = {"CNY_70_MESH", "Data/Meshs/CNY_70.obj"};
const std::string CNY_70_TEXTURE[2] = {"CNY_70_TEXTURE", "Data/Textures/CNY_70.jpg"};


namespace simu{

class CNY_70
{
public :
    CNY_70(Physics* physics, gwe::gwScene* scene, gwe::gwGameObject* object = nullptr, float length = 10.0f);
    virtual ~CNY_70();

    void Run(void);
    gwe::gwGameObject* getGameObject(void) const;

    bool detectsWhite(void) const;
private :
    void InitGameObject(gwe::gwScene* scene);
    void InitResources(void);
    void Setup(gwe::gwScene* scene);

    Physics* mPhysics;
    gwe::gwGameObject* mGameObject;
    gwe::gwGameObject* mParent;

    bool mOnWhite;
    float mLength;
};
}

#endif // CNY_70_H
