#include "SHARP_GP2Y.h"
#include "gwResourceManager.h"
#include "gwStaticMesh.h"
#include "gwTexture.h"
#include "gwScene.h"
#include "gwGameObject.h"
#include "gwMeshRenderer.h"
#include "gwMaterial.h"
#include "gwTransform.h"

using namespace gwe;

simu::SHARP_GP2Y::SHARP_GP2Y(Physics* physics, gwe::gwScene* scene, gwe::gwGameObject* parent) : mPhysics(physics), mParent(parent), mResultDistance(mMaxDist),mMinDist(2), mMaxDist(15)
{
    Setup(scene);
}

simu::SHARP_GP2Y::~SHARP_GP2Y()
{

}

void simu::SHARP_GP2Y::Run(void){
    gwe::gwTransform* transform = mGameObject->getTransform();
    glm::mat4 transformations = transform->getWorldTransform();
    glm::vec3 glm_start = transform->getWorldPos()+(transform->getWorldForward()*(-mMinDist));
    glm::vec3 glm_to = transform->getWorldPos()+transform->getWorldForward()*(-mMaxDist);

    btVector3 from = btVector3(glm_start.x,glm_start.y, glm_start.z );
    btVector3 to = btVector3(glm_to.x, glm_to.y, glm_to.z);

    btCollisionWorld::ClosestRayResultCallback result(from, to);
    mPhysics->getWorld()->rayTest(from, to, result);
    btVector3 color(0,1.0f,0);
    if(result.hasHit()){
        //std::cout << result.m_closestHitFraction << std::endl;
        //float centimeters = (mMaxDist-mMinDist)*result.m_closestHitFraction + mMinDist;
        //std::cout << centimeters<< std::endl;
        mResultDistance = (mMaxDist-mMinDist)*result.m_closestHitFraction + mMinDist;
        if(result.m_closestHitFraction >= 0.5){
            color = btVector3(1.0-(result.m_closestHitFraction-0.5)*2,1.0f,0);
        }else{
            color = btVector3(1.0f,result.m_closestHitFraction*2.0f,0);
        }

    }else{
        mResultDistance = mMaxDist;
    }


//    mOnWhite = false;
   /* for(int k(0); k <result.m_collisionObjects.size(); k++){
        if(result.m_collisionObjects.at(k)->getUserIndex() == USER_INDEX_LINES){
            mOnWhite = true;
        }
    }
    if(mOnWhite){
        color = btVector3(255,0,0);
    }*/

    mPhysics->getWorld()->getDebugDrawer()->drawLine(from, to, color);
}


gwe::gwGameObject* simu::SHARP_GP2Y::getGameObject(void) const { return mGameObject; }
float simu::SHARP_GP2Y::getResultDistance(void) const { return mResultDistance; }

void simu::SHARP_GP2Y::InitGameObject(gwe::gwScene* scene){
    std::string prefix = "SHARP_GP2Y_";
    int index = 0;
    while(scene->getGameObject(Physics::make_string(prefix, index))){
        index++;
    }
    mGameObject = new gwGameObject(Physics::make_string(prefix, index), mParent);
    scene->addGameObject(mGameObject);

    gwMeshRenderer* renderer = new gwMeshRenderer(dynamic_cast<gwStaticMesh*>(gwe::singleton::gwResourceManager::getInstance()->getResource(SHARP_GP2Y_MESH[RESOURCE_NAME])));
    mGameObject->addComponent(renderer);
    gwMaterial* material = new gwMaterial;
    material->setTexture(dynamic_cast<gwTexture*>(gwe::singleton::gwResourceManager::getInstance()->getResource(SHARP_GP2Y_TEXTURE[RESOURCE_NAME])));
    mGameObject->addComponent(material);
}

void simu::SHARP_GP2Y::InitResources(void){
    singleton::gwResourceManager* mgr = gwe::singleton::gwResourceManager::getInstance();

    if(mgr->getResource(SHARP_GP2Y_MESH[RESOURCE_NAME]) == nullptr){
        gwStaticMesh* mesh = new gwStaticMesh(SHARP_GP2Y_MESH[RESOURCE_NAME], SHARP_GP2Y_MESH[RESOURCE_FILE]);
        mesh->load();
    }

    if(mgr->getResource(SHARP_GP2Y_TEXTURE[RESOURCE_NAME]) == nullptr){
        gwTexture* texture = new gwTexture(SHARP_GP2Y_TEXTURE[RESOURCE_NAME], SHARP_GP2Y_TEXTURE[RESOURCE_FILE]);
        texture->load();
    }
}

void simu::SHARP_GP2Y::Setup(gwe::gwScene* scene){
    InitResources();
    InitGameObject(scene);
}
