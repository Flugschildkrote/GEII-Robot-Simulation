#include "gwGameManager.h"
#include "gwScene.h"
#include "gwResourceManager.h"
//#include "Scene.h"
//#include "GUIManager.h"
//#include "Input.h"

using namespace gwe::singleton;

gwe::singleton::gwGameManager::gwGameManager(void) : mEnabledScene(nullptr), mRenderSize(0,0)  //: mCurrentScene(0), mInput(Input::getInstance()), mFrameTime(0), mLastTime(0)
{
    std::cout << "[SINGLETON] \"GameManager\" created." << std::endl;
    //mInputManager = gwInputManager::getInstance();
    //LOG("", LOG_CLEAR);
   // LOG("", LOG_NEW_SESSION);
   // LOG("SceneManager created", LOG_INFO);
   // RessourceManager::getInstance();
}

gwe::singleton::gwGameManager::~gwGameManager(void)
{

    for(std::map<std::string, gwScene*>::iterator it(mScenes.begin()); it != mScenes.end(); it++)
        delete it->second;
    gwResourceManager::deleteInstance();
    std::cout << "[SINGLETON] \"GameManager\" deleted." << std::endl;
}


bool gwe::singleton::gwGameManager::addScene(gwScene *scene)
{
    if(mScenes.find(scene->getName()) != mScenes.end())
    {
        std::cout << "[ERROR] Scene \"" << scene->getName() << "\" already exists" << std::endl;
        return false;
    }
    mScenes[scene->getName()] = scene;
    return true;
}

void gwe::singleton::gwGameManager::deleteScenes(void)
{
    for(std::map<std::string, gwScene*>::iterator it(mScenes.begin()); it != mScenes.end(); it++)
        delete it->second;
    mScenes.clear();
    std::cout << "[INFO] SceneManager's Scenes deleted" << std::endl;
}

bool gwe::singleton::gwGameManager::enableScene(const std::string &sceneName)
{
    if(mScenes.find(sceneName) == mScenes.end())
    {
        std::cout << "Can't enable scene \"" << sceneName << "\"" << std::endl;
        return false;
    }
    mEnabledScene = mScenes[sceneName];
    return true;
}

gwe::gwScene* gwe::singleton::gwGameManager::getEnabledScene(void) const { return mEnabledScene; }
const gwe::gwSize* gwe::singleton::gwGameManager::getRenderSize(void) const { return &mRenderSize; }

gwe::gwScene* gwe::singleton::gwGameManager::getScene(const std::string &sceneName) const
{
    if(mScenes.find(sceneName) == mScenes.end())
    {
        std::cout << "Can't find Scene \"" << sceneName << "\""<< std::endl;
        return nullptr;
    }
    return mScenes.at(sceneName);
}

bool gwe::singleton::gwGameManager::removeScene(const std::string &sceneName)
{
    if(mScenes.find(sceneName) == mScenes.end())
    {
        std::cout << "Can't remove Scene \"" << sceneName << "\""<< std::endl;
        return false;
    }
    mScenes.erase(sceneName);
    return true;
}

void gwe::singleton::gwGameManager::render(bool clrScr)
{
    if(mEnabledScene == nullptr)
    {
        std::cout << "[WARNING][SINGLETON][GAME_MANAGER] Can't render nullptr Scene." << std::endl;
        return;
    }

    mEnabledScene->render(clrScr);
}

void gwe::singleton::gwGameManager::update(void)
{
    if(mEnabledScene == nullptr)
    {
        std::cout << "[WARNING][SINGLETON][GAME_MANAGER] Can't update nullptr Scene." << std::endl;
        return;
    }

    mEnabledScene->update();
}

void gwe::singleton::gwGameManager::setRenderSize(const gwSize &newSize) { mRenderSize = newSize; }

/*bool gwGameManager::run()
{
    uint32_t currentTime = SDL_GetTicks();
    mFrameTime = currentTime-mLastTime;
    mLastTime = currentTime;

    if(mEnabledScene != nullptr)
        mEnabledScene->run();

    mInputManager->updateEvent();
    if(mInputManager->quit())
        return false;

    std::cout << "FPS : " << (float) 1.0f/(mFrameTime/1000.0f) << std::endl;
    if(mEnabledScene != nullptr)
        mEnabledScene->run();
    return true;

}

uint32_t gwGameManager::getFrameTime(void) { return mFrameTime; }
float gwGameManager::getDeltaTime(void) { return (float) mFrameTime/1000.0f; }*/
