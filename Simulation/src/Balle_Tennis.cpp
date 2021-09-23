#include "Balle_Tennis.h"

#include "gwGameObject.h"
#include "gwTexture.h"
#include "gwStaticMesh.h"
#include "gwScene.h"
#include "gwMaterial.h"
#include "gwMeshRenderer.h"
#include "gwTransform.h"
#include "gwInputManager.h"
#include "gwResourceManager.h"
#include "gwStaticMesh.h"
#include "gwTexture.h"

using namespace gwe;

simu::Balle_Tennis::Balle_Tennis(Physics* physics) : mPhysics(physics), mouseClicked(false)
{
    //ctor
}

simu::Balle_Tennis::~Balle_Tennis()
{
    //dtor
}

void simu::Balle_Tennis::setup(gwe::gwScene* scene, gwe::gwShader *shader, const btVector3 &pos){

    initResources();

    std::string prefix = "Ball_";
    int index = 0;
    while(scene->getGameObject(Physics::make_string(prefix, index))){
        index++;
    }

    mBallGameObject = new gwGameObject(Physics::make_string(prefix, index));
    scene->addGameObject(mBallGameObject);

    mBallMaterial = new gwMaterial;
    mBallRenderer = new gwMeshRenderer(mBallMesh);

    mBallMaterial->setTexture(mBallTexture, 1.0);
    mBallGameObject->addComponent(mBallRenderer);
    mBallGameObject->addComponent(mBallMaterial);

    scene->addGameObject(mBallGameObject);
    mBallGameObject->getTransform()->translate(glm::vec3(0,4,0));

    {
    /** Collider */
        mCollisionShape= new btSphereShape(btScalar(3.25));
        /// Create Dynamic Objects
        mColliderTransform.setIdentity();
        mMass = .58f;
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mMass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            mCollisionShape->calculateLocalInertia(mMass,localInertia);
        mColliderTransform.setOrigin(pos);
                //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        mMotionState = new btDefaultMotionState(mColliderTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mMass,mMotionState,mCollisionShape,localInertia);
        mRigidBody = new btRigidBody(rbInfo);
        mPhysics->getWorld()->addRigidBody(mRigidBody);
        mRigidBody->setRestitution(0.6);
        mRigidBody->setDamping(0.3, 0.5);
        mRigidBody->setFriction(0.5);
        mRigidBody->setUserIndex(USER_INDEX_BALL);
        mRigidBody->setCollisionFlags(mRigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
    }
}

void simu::Balle_Tennis::run(void){
    mRigidBody->getMotionState()->getWorldTransform(mColliderTransform);
    btScalar localTransform[16];
    mColliderTransform.getOpenGLMatrix(localTransform);
    mBallGameObject->getTransform()->setTransform(glm::make_mat4(localTransform));

    gwe::singleton::gwInputManager *input = gwe::singleton::gwInputManager::getInstance();
    if(input->getMouseButton(1)){
        if(!mouseClicked){
            //mRigidBody->applyCentralImpulse(btVector3(0.1,0.1,0.1));
        }
        mouseClicked = true;
    }else
    {
        mouseClicked = false;
    }

}

void simu::Balle_Tennis::initResources(void){
    singleton::gwResourceManager* mgr = gwe::singleton::gwResourceManager::getInstance();

    mBallMesh = static_cast<gwStaticMesh*>(mgr->getResource(BALL_MESH[RESOURCE_NAME]));
    if(mBallMesh == nullptr){
        mBallMesh = new gwStaticMesh(BALL_MESH[RESOURCE_NAME], BALL_MESH[RESOURCE_FILE]);
        mBallMesh->load();
    }

    mBallTexture = static_cast<gwTexture*>(mgr->getResource(BALL_TEXTURE[RESOURCE_NAME]));
    if(mBallTexture == nullptr){
        mBallTexture = new gwTexture(BALL_TEXTURE[RESOURCE_NAME], BALL_TEXTURE[RESOURCE_FILE]);
        mBallTexture->load();
    }

}
