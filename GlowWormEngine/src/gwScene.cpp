#include "gwScene.h"
#include "gwGameManager.h"
#include "gwGameObject.h"
#include "gwSkybox.h"
#include "gwCamera.h"
#include "gwTransform.h"
#include "gwLight.h"

using namespace gwe;

gwe::gwScene::gwScene(const std::string &name) : mName(name),/* mCamera(nullptr), mSkybox(nullptr),*/ mRootNode(nullptr),
 mAmbiantColor(1.0 ,1.0 ,1.0), mAmbiantIntensity(0.4), mLightsCount(0), mEnabledRenderer(nullptr)
{
    singleton::gwGameManager::getInstance()->addScene(this);
    mRootNode = new gwGameObject(mName+"_RootNode");
    addGameObject(mRootNode);
    std::cout << "[INFO] Scene \"" << mName << "\" created." << std::endl;
}

gwe::gwScene::~gwScene()
{
    while(mSceneRenderers.size() > 0)
    {
        delete *mSceneRenderers.begin();
    }
    delete mRootNode;
    std::cout << "[DELETED][SCENE] \"" << mName << "\" deleted" << std::endl;
}

bool gwe::gwScene::addGameObject(gwGameObject* object)
{
    if(mGameObjects.find(object->getName()) != mGameObjects.end())
    {
        std::cout << "[ERREUR] : Scene \"" << mName << "\" already have a GameObject named \"" << object->getName() << "\", instruction skipped." << std::endl;
        return false;
    }
    if(object->getScene() != nullptr)
        object->getScene()->removeGameObject(object->getName());
    mLinkStates[object->getName()] = GW_LINK_ADD;
    GW_LINK_STATE result = object->linkScene(this);
    mLinkStates.erase(object->getName());
    if(result == GW_LINK_ADD)
    {
        mGameObjects[object->getName()] = object;
        if(object != mRootNode && object->getParent() == nullptr)
            mRootNode->addChild(object);
        std::cout << "[INFO] GameObject \"" << object->getName() << "\" added to Scene \"" << mName << "\"." << std::endl;
        return true;
    }
    std::cout << "[ERROR] Failed to link Scene \"" << mName << "\" to GameObject \"" << object->getName() << "\"." << std::endl;
    return false;
}

bool gwe::gwScene::addSceneRenderer(gwSceneRenderer *renderer)
{
    if(renderer->getScene() != nullptr)
    {
        std::cout << "[ERROR][SCENE] Can't add renderer to scene \"" << mName << "\". Renderer is already linked to a scene." << std::endl;
        return false;
    }
    mSceneRenderers.insert(renderer);
    return true;
}

//gwCamera* gwe::gwScene::getCamera(void) const { return mCamera; }
gwGameObject* gwe::gwScene::getGameObject(const std::string &objectName) const
{
    if(mGameObjects.find(objectName) == mGameObjects.end())
    {
        std::cout << "[ERROR] Can't find GameObject \"" << objectName << "\" in Scene \"" << mName << "\"." << std::endl;
        return nullptr;
    }
    return mGameObjects.at(objectName);
}
GW_LINK_STATE gwe::gwScene::getLinkState(const std::string &objectName) const
{
    if(mLinkStates.find(objectName) == mLinkStates.end())
		return GW_LINK_DENIED;
	return mLinkStates.at(objectName);
}
std::string gwe::gwScene::getName(void) const { return mName; }
gwSceneRenderer* gwe::gwScene::getEnabledRenderer(void) const { return mEnabledRenderer; }

gwGameObject* gwe::gwScene::getRootNode(void) const { return mRootNode; }
//gwSkybox* gwe::gwScene::getSkybox(void) const { return mSkybox; }


bool gwe::gwScene::removeGameObject(const std::string &objectName)
{
    if(mGameObjects.find(objectName) == mGameObjects.end())
    {
        std::cout << "[INFO] Can't find GameObject \"" << objectName << "\" in Scene \"" << mName << "\" , instruction skipped." << std::endl;
        return false;
    }
    mLinkStates[objectName] = GW_LINK_REMOVE;
    GW_LINK_STATE result = mGameObjects[objectName]->linkScene(this);
    mLinkStates.erase(objectName);
    if(result == GW_LINK_REMOVE)
    {
        mGameObjects.erase(objectName);
        std::cout << "[INFO] GameObject \"" << objectName << "\" removed from Scene \"" << mName << "\"." << std::endl;
        return true;
    }
    std::cout << "[ERROR] Failed to remove GameObject \"" << objectName << "\" from Scene \"" << mName << "\"." << std::endl;
    return false;
}
bool gwe::gwScene::removeSceneRenderer(gwSceneRenderer* renderer)
{
    if(renderer->getScene() != this)
        return false;
    if(mEnabledRenderer == renderer)
        mEnabledRenderer = nullptr;

    if(mSceneRenderers.find(renderer) != mSceneRenderers.end())
    {
        mSceneRenderers.erase(renderer);
        return true;
    }

    return false;
}


void gwe::gwScene::render(bool clrScr)
{
    if(clrScr)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(std::set<gwSceneRenderer*>::iterator it(mSceneRenderers.begin()); it != mSceneRenderers.end(); it++)
    {
        mEnabledRenderer = *it;
        if(mEnabledRenderer->getType() == GW_OFF_SCREEN_RENDERER)
            mEnabledRenderer->render();
    }

    for(std::set<gwSceneRenderer*>::iterator it(mSceneRenderers.begin()); it != mSceneRenderers.end(); it++)
    {
        mEnabledRenderer = *it;
        if(mEnabledRenderer->getType() == GW_SCREEN_RENDERER)
            mEnabledRenderer->render();
    }
}

//void gwe::gwScene::setCamera(gwCamera* camera) { mCamera = camera; }
//void gwe::gwScene::setSkybox(gwSkybox *skybox) { mSkybox = skybox; }

void gwe::gwScene::update(void)
{
    if(mRootNode != nullptr)
        mRootNode->update();

    mLightsCount = 0;
    for(std::map<std::string, gwGameObject*>::iterator it(mGameObjects.begin()); it != mGameObjects.end(); it++)
    {
        if(mLightsCount >= 100)
        {
            std::cout << "[ERROR][SCENE] Scene \"" << mName << "\" max light amount reached !" << std::endl;
            break;
        }

        gwLight* light = dynamic_cast<gwLight*>(it->second->getComponent(GW_COMPONENT_LIGHT));
        if(light != nullptr)
        {
            if(mLights[mLightsCount] != light)
                mLights[mLightsCount] = light;
            mLightsCount++;
        }
    }

     for(std::set<gwSceneRenderer*>::iterator it(mSceneRenderers.begin()); it != mSceneRenderers.end(); it++)
        (*it)->update();
}

void gwe::gwScene::setAmbiantLight(const glm::vec3 &color, float intensity)
{
    mAmbiantColor = color/255.0f;
    mAmbiantIntensity = intensity;
}

void gwe::gwScene::sendLights(const GLuint &shaderID)
{
    glUniform1f(glGetUniformLocation(shaderID, "ambiant.intensity"), mAmbiantIntensity);
    glUniform3f(glGetUniformLocation(shaderID, "ambiant.color"), mAmbiantColor.x, mAmbiantColor.y, mAmbiantColor.z);
    for(unsigned int i(0); i < mLightsCount; i++)
    {
        std::stringstream ss;
        ss << i;
        std::string str = ss.str();
        glUniform1i(glGetUniformLocation(shaderID, ("light["+str+"].type").c_str()), mLights[i]->getLightType());
        glUniform1f(glGetUniformLocation(shaderID, ("light["+str+"].intensity").c_str()), mLights[i]->getIntensity());
        glUniform3f(glGetUniformLocation(shaderID, ("light["+str+"].color").c_str()), mLights[i]->getColor().x, mLights[i]->getColor().y, mLights[i]->getColor().z);
        if(mLights[i]->getLightType() == GW_LIGHT_DIRECTIONAL)
        {
            glm::vec3 dir = mLights[i]->getLinkedObject()->getTransform()->getWorldForward();
            glUniform3f(glGetUniformLocation(shaderID, ("light["+str+"].dir").c_str()), dir.x, dir.y, dir.z);
        }
        else if(mLights[i]->getLightType() == GW_LIGHT_POINT)
        {
            glm::vec3 pos = mLights[i]->getLinkedObject()->getTransform()->getWorldPos();
            glUniform3f(glGetUniformLocation(shaderID, ("light["+str+"].pos").c_str()), pos.x, pos.y, pos.z);
            glUniform3f(glGetUniformLocation(shaderID, ("light["+str+"].attenuation").c_str()),
                        mLights[i]->getAttenuation(GW_CONSTANT_ATTENUATION),
                        mLights[i]->getAttenuation(GW_LINEAR_ATTENUATION),
                        mLights[i]->getAttenuation(GW_EXP_ATTENUATION));

        }
    }
    glUniform1i(glGetUniformLocation(shaderID, "lightsCount"), mLightsCount);
}
