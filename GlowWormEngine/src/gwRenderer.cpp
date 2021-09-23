#include "gwRenderer.h"
#include "gwCamera.h"
#include "gwGameObject.h"
#include "gwScene.h"
#include "gwGameManager.h"
#include "gwSkybox.h"
#include "gwTransform.h"
#include "gwTexture.h"
#include "gwShader.h"

using namespace gwe;

gwe::gwSceneRenderer::gwSceneRenderer(gwScene* scene, GW_RENDERER_TYPE renderer_type) : mType(renderer_type),
mProjection(1.0), mNear(5.0f),  mFar(1000.0f), mAngle(70.0f),  mSize(nullptr), mLastSize(0,0),  mCamera(nullptr), mScene(nullptr), mShader(nullptr), mSkybox(nullptr)
{
    setup(scene);
}


gwe::gwSceneRenderer::~gwSceneRenderer()
{
    if(mScene != nullptr)
    if(!mScene->removeSceneRenderer(this))
       std::cout << "[ERROR] While removing renderer from Scene \"" << mScene->getName() << "\"." << std::endl;
    std::cout << "[RENDERER] Deleted" << std::endl;
}

float     gwe::gwSceneRenderer::getAngle(void) const { return mAngle; }
gwCamera* gwe::gwSceneRenderer::getCamera(void) const { return mCamera; }
float     gwe::gwSceneRenderer::getFar(void) const { return mFar; }
float     gwe::gwSceneRenderer::getNear(void) const { return mNear; }
glm::mat4 gwe::gwSceneRenderer::getProjection(void) const {return mProjection; }
gwScene*  gwe::gwSceneRenderer::getScene(void)  const { return mScene;  }
gwShader* gwe::gwSceneRenderer::getShader(void) const { return mShader; }
gwSize    gwe::gwSceneRenderer::getSize(void) const { return *mSize; }
gwSkybox* gwe::gwSceneRenderer::getSkybox(void) const { return mSkybox; }

void gwe::gwSceneRenderer::update(void)
{
    if(mLastSize != *mSize)
        updateProjection();
    mLastSize = *mSize;
}

void gwe::gwSceneRenderer::render(void)
{
    if(mShader == nullptr){
        return;
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0,0, mSize->getW(), mSize->getH());
    if(mCamera == nullptr)
        std::cout << "[WARNING][SCENE] Scene \"" << mScene->getName() << "\" : Camera is set to nullptr." << std::endl;
    if(mSkybox != nullptr)
        mSkybox->render(mProjection*mCamera->getView()* glm::mat4(glm::translate(glm::mat4(1.0),mCamera->getLinkedObject()->getTransform()->getWorldPos())));
    if(mScene->getRootNode() != nullptr)
        mScene->getRootNode()->render();
}


void gwe::gwSceneRenderer::setAngle(float angle)
{
    if(angle == mAngle)
        return;
    mAngle = angle;
    updateProjection();
}

bool gwe::gwSceneRenderer::setCamera(gwCamera* camera)
{
    /*if(camera->getLinkedObject()->getScene() != mScene)
    {
        std::cout << "Can't set camera to renderer, scenes are not the same" << std::endl;
        return false;
    }*/
    mCamera = camera;
    return true;
}
void gwe::gwSceneRenderer::setFar(float far)
{
    if(mFar == far)
        return;
    mFar = far;
    updateProjection();
}
void gwe::gwSceneRenderer::setNear(float near)
{
    if(mNear == near)
        return;
    mNear = near;
    updateProjection();
}

void gwe::gwSceneRenderer::setShader(gwShader* shader) { mShader = shader; }

void gwe::gwSceneRenderer::setSkybox(gwSkybox* skybox) { mSkybox = skybox; }

void gwe::gwSceneRenderer::setup(gwScene* scene)
{
    if(scene->addSceneRenderer(this))
        mScene = scene;
    else
        std::cout << "[ERROR][SCENE_RENDERER] Initialization failed." << std::endl;
    mSize = gwe::singleton::gwGameManager::getInstance()->getRenderSize();
    updateProjection();
}

void gwe::gwSceneRenderer::updateProjection(void) { mProjection = glm::perspective(mAngle, mSize->getW()/mSize->getH(), mNear, mFar); }








/*gwe::gwSceneRenderer::gwSceneRenderer(float angle, gwCamera* camera, float far, float near, gwScene* scene, gwShader *shader, gwSkybox* skybox) : mType(renderer_type)
mProjection(1.0), mNear(near), mFar(far), mAngle(angle), mSize(nullptr), mLastSize(0,0), mCamera(camera), mScene(scene), mShader(shader), mSkybox(skybox)
{
    mType = GW_SCREEN_RENDERER;
    setup(scene);
}*/


gwe::gwScreenRenderer::gwScreenRenderer(gwScene* scene) : gwSceneRenderer(scene, GW_SCREEN_RENDERER) { }
gwe::gwScreenRenderer::~gwScreenRenderer(void) { }

void gwe::gwScreenRenderer::render(void)
{
    gwe::gwSceneRenderer::render();
}













gwe::gwOffScreenRenderer::gwOffScreenRenderer(const std::string &prefix, float w, float h, gwScene* scene) : gwSceneRenderer(scene, GW_OFF_SCREEN_RENDERER),
mFboID(0), mColorTexture(nullptr), mDepthTexture(nullptr), mDepthBufferID(0)
{
    mSize = new gwSize(w, h);
    mColorTexture = new gwTexture(prefix+"_Color0", w, h, GL_RGBA, GL_RGBA);
    mDepthTexture = new gwTexture(prefix+"_Depth", w, h, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24);
    mColorTexture->load();
    mDepthTexture->load();
    setupFbo();
}

gwe::gwOffScreenRenderer::~gwOffScreenRenderer(void)
{
    delete mSize;
}

void gwe::gwOffScreenRenderer::render(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFboID);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gwSceneRenderer::render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gwe::gwOffScreenRenderer::setupFbo(void)
{
    if(glIsFramebuffer(mFboID) == GL_TRUE)
        glDeleteFramebuffers(1, &mFboID);
    glGenFramebuffers(1, &mFboID);

    glBindFramebuffer(GL_FRAMEBUFFER, mFboID);
        // Crée la zone de mémoire pour stocker le depthBuffer
        {
            if(glIsRenderbuffer(mDepthBufferID) == GL_TRUE)
                    glDeleteRenderbuffers(1, &mDepthBufferID);
            glGenRenderbuffers(1, &mDepthBufferID);

            glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferID);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mSize->getW(), mSize->getH());
            
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBufferID); // Atache le depth buffer à celui du fbo
            glBindRenderbuffer(GL_RENDERBUFFER,0);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture->getID() , 0); // Affiche le rendu dans la texture de couleur
      //  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->getID(), 0); // Affiche le depth buffer dans une texture

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            glDeleteFramebuffers(1, &mFboID);
            glDeleteRenderbuffers(1, &mDepthBufferID);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            std::cout << "[ERROR] FBO loading failed" << std::endl;
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*if(glIsFramebuffer(mFboID) == GL_TRUE)
        glDeleteFramebuffers(1, &mFboID);
    glGenFramebuffers(1, &mFboID);

    glBindFramebuffer(GL_FRAMEBUFFER, mFboID);
        // Crée la zone de mémoire pour stocker le depthBuffer
        {
            if(glIsRenderbuffer(mDepthBufferID) == GL_TRUE)
                    glDeleteRenderbuffers(1, &mDepthBufferID);
            glGenRenderbuffers(1, &mDepthBufferID);

            glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferID);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mWidth, mHeight);
            glBindRenderbuffer(GL_RENDERBUFFER,0);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBufferID); // Atache le depth buffer à celui du fbo
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTextureID , 0); // Affiche le rendu dans la texture de couleur
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTextureID, 0); // Affiche le depth buffer dans une texture

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            glDeleteFramebuffers(1, &mFboID);
            glDeleteRenderbuffers(1, &mDepthBufferID);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            std::cout << "[ERROR] FBO loading failed" << std::endl;
            return false;
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
