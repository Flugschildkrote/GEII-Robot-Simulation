#ifndef TERRAIN_TENNIS_H
#define TERRAIN_TENNIS_H

#include "gwGlobalHeader.h"
#include "Physics.h"

#define _GROUND 0
#define _LEFT 1
#define _RIGHT 2
#define _BACK 3
#define _FRONT 4

namespace simu{

class Terrain_Tennis
{
public:
    Terrain_Tennis(Physics* physics);
    virtual ~Terrain_Tennis();

    void setup(gwe::gwScene* scene, gwe::gwShader* shader);
    void run(void);

protected:
    /** Main Field **/
    gwe::gwGameObject* mFieldGameObject;
    gwe::gwStaticMesh* mFieldMesh;
    gwe::gwTexture* mFieldTexture;
    gwe::gwMaterial* mFieldMaterial;
    gwe::gwMeshRenderer* mFieldRenderer;

    /** COLLIDER **/
    Collider mColliders[5];
    btGhostObject* mWhiteLines;

    Physics* mPhysics;
private:

};

}

#endif // TERRAIN_TENNIS_H
