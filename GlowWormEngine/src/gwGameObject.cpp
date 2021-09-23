#include "gwGameObject.h"
#include "gwTransform.h"

#include "gwScene.h"
#include "gwGameManager.h"
#include "gwComponent.h"
#include "gwCamera.h"
#include "gwMaterial.h"
#include "gwShader.h"
#include "gwMeshRenderer.h"

using namespace gwe;

gwe::gwGameObject::gwGameObject(const std::string &name, gwGameObject* parent) : mName(name), mScene(nullptr), mParent(nullptr), mLinkState(GW_LINK_DENIED), mOnTransition(false)//, mCamera(nullptr)
{
    mTransform = new gwTransform(this);
    mChildren.clear();
    if(parent != nullptr)
    {
        if(parent->getScene() != nullptr)
            parent->getScene()->addGameObject(this);
        parent->addChild(this);
    }

    for(unsigned int i(0); i < GW_COMPONENT_COUNT; i++)
        mComponents[i] = nullptr;
}

gwe::gwGameObject::~gwGameObject()
{
    if(mScene != nullptr)
        mScene->removeGameObject(mName);
    else if(mParent != nullptr)
        mParent->removeChild(mName);

    deleteChildren();

    for(unsigned int i(0); i < GW_COMPONENT_COUNT; i++)
        if(mComponents[i] != nullptr)
            delete mComponents[i];
    delete mTransform;
    std::cout << "[DELETED][GAME_OBJECT] \"" << mName << "\" deleted" << std::endl;
}

void gwe::gwGameObject::deleteChildren(void)
{

    std::cout << "[DELETING] " << mName << std::endl;
    if(mChildren.size() == 0)
        return;

    while(mChildren.size() != 0)
    {
        std::map<std::string, gwGameObject*>::iterator it(mChildren.begin());
        delete it->second;
    }

   /* for(std::map<std::string, gwGameObject*>::iterator it(mChildren.begin()); it != mChildren.end(); it++)
    {
        std::cout << it->second->getName() << std::endl;
        //std::cout << "OK" << std::endl;
        delete it->second;
       // it->second = nullptr;
    }*/
    mChildren.clear();
}

bool gwe::gwGameObject::findChild(const std::string &name)
{
    if(mName == name)
        return true;
    for(std::map<std::string, gwGameObject*>::iterator it(mChildren.begin()); it != mChildren.end(); it++)
    {
        if(it->second->findChild(name) == true)
            return true;
    }
    return false;
}

gwGameObject* gwe::gwGameObject::getChild(const std::string &childName) const
{
    if(mChildren.find(childName) == mChildren.end())
    {
        std::cout << "[ERROR] Object \"" << childName << "\" is not a child of \"" << mName << "\"" << std::endl;
        return nullptr;
    }
    return mChildren.at(childName);
}
gwComponent* gwe::gwGameObject::getComponent(GW_COMPONENT_TYPE TYPE) const { return mComponents[TYPE]; }
GW_LINK_STATE gwe::gwGameObject::getLinkState(void) const { return mLinkState; }
const std::string gwe::gwGameObject::getName(void) const {return mName; }
gwGameObject* gwe::gwGameObject::getParent(void) const { return mParent; }
gwScene* gwe::gwGameObject::getScene(void) const { return mScene; }
gwTransform* gwe::gwGameObject::getTransform(void) const { return mTransform; }


bool gwe::gwGameObject::addChild(gwGameObject* child)
{
    std::cout << "Trying to add " << child->mName << " to " << mName << std::endl;
    if(child == nullptr)
    {
        std::cout << "[ERROR] Can't add nullptr child to \"" << mName << "\"." << std::endl;
        return false;
    }
    else if(mChildren.find(child->mName) != mChildren.end())
    {
        std::cout << "[ERROR] Object \"" << child->mName << "\" is already child of \"" << mName << "\", instruction skipped." << std::endl;
        return false;
    }
    else if(child->mScene != mScene)
    {
        std::string str1, str2;
        if(child->mScene == nullptr)
            str1 = "(linked to nullptr scene)";
        else
            str1 = "(linked to scene \"" + child->mScene->getName() + "\")";
        if(mScene == nullptr)
            str2 = "(linked to nullptr scene)";
        else
            str2 = "(linked to scene \"" + mScene->getName() + "\")";

        std::cout << "[ERROR] Can't add child \"" << child->mName << str1 << " to \"" <<
        mName << "\"" << str2 << ". Linked scenes are not the same." << std::endl;
        return false;
    }
    else if(child->findChild(mName))
    {
        std::cout << "[ERROR] Circular paternity is not allowed, GameObject \"" << child->mName << "\" is a parent of \"" << mName << "\", instruction skipped." << std::endl;
        return false;
    }
    if(child->mParent != nullptr){
        gwGameObject* parent = child->mParent;
        parent->mOnTransition = true;
        parent->removeChild(child->getName());
        parent->mOnTransition = false;
    }

    child->mParent = this;
    mChildren[child->getName()] = child;
    child->updateTransform(mTransform->getWorldTransform());
    std::cout << "[INFO] GameObject \"" << child->mName << "\" added as child to \"" << mName << "\"" << std::endl;
    return true;
}

bool gwe::gwGameObject::addComponent(gwComponent* component)
{
    if(component == nullptr)
    {
        std::cout << "[ERROR] Can't add nullptr component to GameObject \"" << mName << "\"." << std::endl;
        return false;
    }

    if(mComponents[component->getComponentType()] != nullptr)
    {
        if(!removeComponent(component->getComponentType()))
        {
            std::cout << "[ERROR] An error occurred when tried to add component to GameObject \"" << mName << "\"." << std::endl;
			return false;
        }
    }

	mLinkState = GW_LINK_ADD;
	GW_LINK_STATE result = component->linkObject(this);
	mLinkState = GW_LINK_DENIED;
	if(result != GW_LINK_ADD)
    {
        std::cout << "[ERROR] An error occurred when tried to add component to GameObject \"" << mName << "\"." << std::endl;
		return false;
    }
    std::cout << "[INFO] Component \"" << component->getComponentType() << "\" added to GameObject \"" << mName << "\"" << std::endl;
    mComponents[component->getComponentType()] = component;
	return true;
}

GW_LINK_STATE gwe::gwGameObject::linkScene(gwScene* scene)
{
    if(scene == nullptr)
    {
        std::cout << "[ERROR] Can't link GameObject \"" << mName << "\" to nullptr scene." << std::endl;
        return GW_LINK_DENIED;
    }
    if(scene->getLinkState(mName) == GW_LINK_ADD)
    {
        mScene = scene;
        for(std::map<std::string, gwGameObject*>::iterator it(mChildren.begin()); it != mChildren.end(); it++)
        {
            scene->addGameObject(it->second);
        }
        if(mParent != nullptr)
            if(mParent->mScene != mScene)
                mParent->removeChild(mName);
        return GW_LINK_ADD;
    }
    else if(scene->getLinkState(mName) == GW_LINK_REMOVE)
    {
        mScene = nullptr;
        for(std::map<std::string, gwGameObject*>::iterator it(mChildren.begin()); it != mChildren.end(); it++)
            scene->removeGameObject(it->second->mName);
        if(mParent != nullptr)
            if(mParent->mScene != nullptr)
                mParent->removeChild(mName);
        return GW_LINK_REMOVE;
    }
    return GW_LINK_DENIED;
}

bool gwe::gwGameObject::removeChild(const std::string &childName)
{
    std::cout << "Trying to remove " << childName << " from " << mName << std::endl;
    if(mChildren.find(childName) == mChildren.end())
    {
        std::cout << "[ERROR] GameObject \"" << childName << "\" is not a child of \"" << mName << "\"" << std::endl;
        return false;
    }
    gwGameObject* child = mChildren[childName];
    mChildren.erase(mChildren.find(childName));
    child->mParent = nullptr;
    if(mScene != nullptr && child->mScene == mScene && (!mOnTransition))
        mScene->getRootNode()->addChild(child);
    std::cout << "[INFO] GameObject \"" << childName << "\" removed from GameObject \"" << mName << "\"" << std::endl;
    return true;
}

bool gwe::gwGameObject::removeComponent(GW_COMPONENT_TYPE TYPE)
{
    if(mComponents[TYPE] == nullptr)
    {
        std::cout << "[ERROR] Can't remove nullptr component from GameObject \"" << mName << "\", instruction skipped." << std::endl;
		return false;
    }

	mLinkState = GW_LINK_REMOVE;
	GW_LINK_STATE result = mComponents[TYPE]->linkObject(this);
	mLinkState = GW_LINK_DENIED;
	if(result != GW_LINK_REMOVE)
    {
        std::cout << "[ERROR] An error occurred when tried to remove component from GameObject \"" << mName << "\"." << std::endl;
		return false;
    }
    mComponents[TYPE] = nullptr;
    std::cout << "[INFO] Component \"" << TYPE << "\" removed from GameObject \"" << mName << "\"" << std::endl;
	return true;
}

void gwe::gwGameObject::updateChildrenTransform(void)
{
    for(std::map<std::string, gwGameObject*>::iterator it(mChildren.begin()); it != mChildren.end(); it++)
        it->second->updateTransform(mTransform->getWorldTransform());
}

void gwe::gwGameObject::updateTransform(const glm::mat4 &parentWorldTransform) { mTransform->setParentTransform(parentWorldTransform); }

void gwe::gwGameObject::update(void)
{
    if(mComponents[GW_COMPONENT_CAMERA] != nullptr)
        dynamic_cast<gwCamera*>(mComponents[GW_COMPONENT_CAMERA])->update();

    for(std::map<std::string, gwGameObject*>::iterator it(mChildren.begin()); it != mChildren.end(); it++)
        it->second->update();
}

void gwe::gwGameObject::render(void)
{
    if(mScene == nullptr)
        return;

    for(std::map<std::string, gwGameObject*>::iterator it(mChildren.begin()); it != mChildren.end(); it++)
        it->second->render();

    //if(mComponents[COMPONENT_MATERIAL] == nullptr)
       // std::cout << "[WARNING] GameObject \"" << mName << "\" : Material is set to nullptr. Display aborted." << std::endl;
    if(mComponents[GW_COMPONENT_MATERIAL] == nullptr ||
       mComponents[GW_COMPONENT_MESH_RENDERER] == nullptr ||
       mScene->getEnabledRenderer()->getCamera() == nullptr)
        return;


    GLuint programID = mScene->getEnabledRenderer()->getShader()->getProgramID();

    glUseProgram(programID);

        mScene->sendLights(programID);
       // mScene->getLightManager()->render(programID);

        gwMaterial* material = dynamic_cast<gwMaterial*>(mComponents[GW_COMPONENT_MATERIAL]);
        gwMeshRenderer* meshRenderer = dynamic_cast<gwMeshRenderer*>(mComponents[GW_COMPONENT_MESH_RENDERER]);
        gwCamera* camera = mScene->getEnabledRenderer()->getCamera();
       // glUniform3f(glGetUniformLocation(programID, "Color"), material->getColor().x, material->getColor().y, material->getColor().z);
        material->render(programID);
        glm::mat4 mvp = mScene->getEnabledRenderer()->getProjection()*camera->getView()*mTransform->getWorldTransform();
        glm::mat4 normalsMatix = glm::inverse(glm::transpose(mTransform->getWorldTransform()));
        glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(glGetUniformLocation(programID, "normalsMatrix"), 1, GL_FALSE, glm::value_ptr(normalsMatix));
        glUniformMatrix4fv(glGetUniformLocation(programID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(mTransform->getWorldTransform()));
        glm::vec3 pos = camera->getLinkedObject()->getTransform()->getWorldPos();
        glm::vec3 lookingDir = camera->getLinkedObject()->getTransform()->getWorldForward();
        glUniform3f(glGetUniformLocation(programID, "camPos"), pos.x, pos.y, pos.z);
        glUniform3f(glGetUniformLocation(programID, "camDir"),lookingDir.x, lookingDir.y, lookingDir.z);

               /// camera->render();
        meshRenderer->render();
        glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
