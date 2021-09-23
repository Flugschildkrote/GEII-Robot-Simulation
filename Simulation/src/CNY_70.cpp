#include "CNY_70.h"
#include "gwResourceManager.h"
#include "gwStaticMesh.h"
#include "gwTexture.h"
#include "gwScene.h"
#include "gwGameObject.h"
#include "gwMeshRenderer.h"
#include "gwMaterial.h"
#include "gwTransform.h"

using namespace gwe;

simu::CNY_70::CNY_70(Physics* physics, gwScene* scene, gwGameObject* object, float length) : mPhysics(physics),mGameObject(nullptr), mParent(object),mOnWhite(false), mLength(length)
{
    Setup(scene);

}

simu::CNY_70::~CNY_70()
{
    delete mGameObject;
}

gwGameObject* simu::CNY_70::getGameObject(void) const { return mGameObject; }

void simu::CNY_70::Run(void){
    gwe::gwTransform* transform = mGameObject->getTransform();
    glm::mat4 transformations = transform->getWorldTransform();
    glm::vec3 myForward = glm::vec3(transformations*glm::vec4(0,-1,0,0));
    glm::vec3 glm_start = transform->getWorldPos();
    glm::vec3 glm_to = transform->getWorldPos()+myForward*mLength;

    btVector3 from = btVector3(glm_start.x,glm_start.y, glm_start.z );
    btVector3 to = btVector3(glm_to.x, glm_to.y, glm_to.z);

    btCollisionWorld::AllHitsRayResultCallback result(from, to);
    mPhysics->getWorld()->rayTest(from, to, result);

    mOnWhite = false;
    btVector3 color(0,206,209);
    for(int k(0); k <result.m_collisionObjects.size(); k++){
        if(result.m_collisionObjects.at(k)->getUserIndex() == USER_INDEX_LINES || result.m_collisionObjects.at(k)->getUserIndex() == USER_INDEX_BALL){
            mOnWhite = true;
        }
    }
    if(mOnWhite){
        color = btVector3(255,0,0);
    }

    mPhysics->getWorld()->getDebugDrawer()->drawLine(from, to, color);
}


void simu::CNY_70::InitGameObject(gwScene* scene){
    std::string prefix = "CNY_70_";
    int index = 0;
    while(scene->getGameObject(Physics::make_string(prefix, index))){
        index++;
    }

    mGameObject = new gwGameObject(Physics::make_string(prefix, index), mParent);
    scene->addGameObject(mGameObject);

    gwMeshRenderer* renderer = new gwMeshRenderer(dynamic_cast<gwStaticMesh*>(gwe::singleton::gwResourceManager::getInstance()->getResource(CNY_70_MESH[RESOURCE_NAME])));
    mGameObject->addComponent(renderer);
    gwMaterial* material = new gwMaterial;
    material->setTexture(dynamic_cast<gwTexture*>(gwe::singleton::gwResourceManager::getInstance()->getResource(CNY_70_TEXTURE[RESOURCE_NAME])));
    mGameObject->addComponent(material);
}

void simu::CNY_70::InitResources(void){
    singleton::gwResourceManager* mgr = gwe::singleton::gwResourceManager::getInstance();

    if(mgr->getResource(CNY_70_MESH[RESOURCE_NAME]) == nullptr){
        gwStaticMesh* mesh = new gwStaticMesh(CNY_70_MESH[RESOURCE_NAME], CNY_70_MESH[RESOURCE_FILE]);
        mesh->load();
    }

    if(mgr->getResource(CNY_70_TEXTURE[RESOURCE_NAME]) == nullptr){
        gwTexture* texture = new gwTexture(CNY_70_TEXTURE[RESOURCE_NAME], CNY_70_TEXTURE[RESOURCE_FILE]);
        texture->load();
    }

}

void simu::CNY_70::Setup(gwScene* scene){
    InitResources();
    InitGameObject(scene);
}

bool simu::CNY_70::detectsWhite(void) const { return mOnWhite; }

