#ifndef BALLE_TENNIS_H
#define BALLE_TENNIS_H


#include "gwGlobalHeader.h"
#include "Physics.h"

#ifndef RESOURCE_NAME
#define RESOURCE_NAME 0
#endif // RESOURCE_NAME

#ifndef RESOURCE_FILE
#define RESOURCE_FILE 1
#endif // RESOURCE_FILE

const std::string BALL_MESH[2] = {"BALL_MESH", "Data/Meshs/Tennis_Ball.obj"};
const std::string BALL_TEXTURE[2] = {"BALL_TEXTURE", "Data/Textures/Tennis_Ball.jpg"};

namespace simu{

class Balle_Tennis
{
public:
    Balle_Tennis(Physics* physics);
    virtual ~Balle_Tennis();

    void setup(gwe::gwScene* scene, gwe::gwShader* shader, const btVector3 &pos = btVector3(0,40,0));
    void run(void);

protected:
    gwe::gwGameObject* mBallGameObject;
    gwe::gwStaticMesh* mBallMesh;
    gwe::gwTexture* mBallTexture;
    gwe::gwMaterial* mBallMaterial;
    gwe::gwMeshRenderer* mBallRenderer;
private:

    void initResources(void);

    btCollisionShape* mCollisionShape;
    btTransform mColliderTransform;
    btScalar mMass;
    btDefaultMotionState* mMotionState;
    btRigidBody* mRigidBody;

    Physics* mPhysics;
    bool mouseClicked;

};

}

#endif // BALLE_TENNIS_H
