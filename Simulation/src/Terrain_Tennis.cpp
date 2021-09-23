#include "Terrain_Tennis.h"

#include "gwGameObject.h"
#include "gwTexture.h"
#include "gwStaticMesh.h"
#include "gwScene.h"
#include "gwMaterial.h"
#include "gwMeshRenderer.h"

using namespace gwe;

simu::Terrain_Tennis::Terrain_Tennis(Physics* physics) : mPhysics(physics)
{
    //ctor
}

simu::Terrain_Tennis::~Terrain_Tennis()
{
    //dtor
}

void simu::Terrain_Tennis::setup(gwe::gwScene* scene, gwe::gwShader *shader){
    mFieldGameObject = new gwGameObject("Main_Field");
    mFieldMesh = new gwStaticMesh("Main_Field_Mesh", "Data/Meshs/Tennis_Field.obj");
    mFieldTexture = new gwTexture("Main_Field_Texture", "Data/Textures/Tennis_Field.jpg");
    mFieldMaterial = new gwMaterial;
    mFieldRenderer = new gwMeshRenderer(mFieldMesh);

    mFieldMesh->load();
    mFieldTexture->load();

    mFieldMaterial->setTexture(mFieldTexture, 1.0);
    mFieldGameObject->addComponent(mFieldRenderer);
    mFieldGameObject->addComponent(mFieldMaterial);

    scene->addGameObject(mFieldGameObject);


    /** [_GROUND] **/
    {
        mColliders[_GROUND].collisionShape =  new btStaticPlaneShape(btVector3(0,1,0), 0);
        mColliders[_GROUND].transform.setIdentity();
        mColliders[_GROUND].transform.setOrigin(btVector3(0,0,0));
        mColliders[_GROUND].mass = 0.0;

        btVector3 localInertia(0,0,0);
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        mColliders[_GROUND].motionState = new btDefaultMotionState(mColliders[_GROUND].transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mColliders[_GROUND].mass,mColliders[_GROUND].motionState,mColliders[_GROUND].collisionShape,localInertia);
        mColliders[_GROUND].rigidBody = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        mPhysics->getWorld()->addRigidBody(mColliders[_GROUND].rigidBody);
    }

    /** [_LEFT] **/
    {
        mColliders[_LEFT].collisionShape =  new btBoxShape(btVector3(400,7.5, 1));
        mColliders[_LEFT].transform.setIdentity();
        mColliders[_LEFT].transform.setOrigin(btVector3(0,7.5,201));
        mColliders[_LEFT].mass = 0.0;

        btVector3 localInertia(0,0,0);
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        mColliders[_LEFT].motionState = new btDefaultMotionState(mColliders[_LEFT].transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mColliders[_LEFT].mass,mColliders[_LEFT].motionState,mColliders[_LEFT].collisionShape,localInertia);
        mColliders[_LEFT].rigidBody = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        mPhysics->getWorld()->addRigidBody(mColliders[_LEFT].rigidBody);
    }

    /** [_RIGHT] **/
    {
        mColliders[_RIGHT].collisionShape =  new btBoxShape(btVector3(400, 7.5, 1));
        mColliders[_RIGHT].transform.setIdentity();
        mColliders[_RIGHT].transform.setOrigin(btVector3(0,7.5, -201));
        mColliders[_RIGHT].mass = 0.0;

        btVector3 localInertia(0,0,0);
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        mColliders[_RIGHT].motionState = new btDefaultMotionState(mColliders[_RIGHT].transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mColliders[_RIGHT].mass,mColliders[_RIGHT].motionState,mColliders[_RIGHT].collisionShape,localInertia);
        mColliders[_RIGHT].rigidBody = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        mPhysics->getWorld()->addRigidBody(mColliders[_RIGHT].rigidBody);
    }

    /** [_BACK] **/
    {
        mColliders[_BACK].collisionShape =  new btBoxShape(btVector3(1, 7.5, 200));
        mColliders[_BACK].transform.setIdentity();
        mColliders[_BACK].transform.setOrigin(btVector3(401 ,7.5, 0));
        mColliders[_BACK].mass = 0.0;

        btVector3 localInertia(0,0,0);
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        mColliders[_BACK].motionState = new btDefaultMotionState(mColliders[_BACK].transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mColliders[_BACK].mass,mColliders[_BACK].motionState,mColliders[_BACK].collisionShape,localInertia);
        mColliders[_BACK].rigidBody = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        mPhysics->getWorld()->addRigidBody(mColliders[_BACK].rigidBody);
    }

    /** [_FRONT] **/
    {
        mColliders[_FRONT].collisionShape =  new btBoxShape(btVector3(1, 7.5, 200));
        mColliders[_FRONT].transform.setIdentity();
        mColliders[_FRONT].transform.setOrigin(btVector3(-401 ,7.5, 0));
        mColliders[_FRONT].mass = 0.0;

        btVector3 localInertia(0,0,0);
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        mColliders[_FRONT].motionState = new btDefaultMotionState(mColliders[_FRONT].transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mColliders[_FRONT].mass,mColliders[_FRONT].motionState,mColliders[_FRONT].collisionShape,localInertia);
        mColliders[_FRONT].rigidBody = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        mPhysics->getWorld()->addRigidBody(mColliders[_FRONT].rigidBody);
    }

    for(short i(0); i < 5; i++){
        mColliders[i].rigidBody->setRestitution(0.5);
        mColliders[i].rigidBody->setFriction(0.8);
        mColliders[i].rigidBody->setUserIndex(USER_INDEX_FIELD);
    }


    {
        /// SHAPE
        gwStaticMesh *mesh = new gwStaticMesh("Field_Collider", "Data/Meshs/Field_LineCollider.obj");
        mesh->load();

        btTriangleMesh *tri = new btTriangleMesh;
        float* vertices;
        int verticesCount = mesh->getVertices(vertices);


        for(unsigned int i(0); i < verticesCount; i+=9)
        {
            tri->addTriangle(btVector3(vertices[i], vertices[i+1], vertices[i+2]),
                             btVector3(vertices[i+3], vertices[i+4], vertices[i+5]),
                             btVector3(vertices[i+6], vertices[i+7], vertices[i+8]));
        }
        btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(tri, true);

        btGhostObject* mWhiteLines = new btPairCachingGhostObject();
        mWhiteLines->setUserIndex(USER_INDEX_LINES);
        mWhiteLines->setCollisionShape(shape);
        btTransform transform;
        transform.setIdentity();
        //transform.setOrigin(btVector3(0,0.001,0))
        mWhiteLines->setWorldTransform(transform);
        mPhysics->getWorld()->addCollisionObject(mWhiteLines);
        mWhiteLines->setCollisionFlags(mWhiteLines->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
       // object->setCustomDebugColor(btVector3(1,0,0));
       // btBvhTriangleMeshShape* new btBvhTriangleMeshShape()


    }

}

void simu::Terrain_Tennis::run(void){

}
