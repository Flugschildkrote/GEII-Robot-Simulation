#ifndef GWRENDERER_H
#define GWRENDERER_H

#include "gwGlobalHeader.h"

namespace gwe {



class gwSceneRenderer
{
public :
    gwSceneRenderer(gwScene* scene, GW_RENDERER_TYPE renderer_type);
    virtual ~gwSceneRenderer(void);

    bool operator<(const gwSceneRenderer &other) { return (mType < other.mType); }
    float getAngle(void) const;
    gwCamera* getCamera(void) const;
    float getFar(void) const;
    float getNear(void) const;
    glm::mat4 getProjection(void) const;
    gwScene*  getScene(void)  const;
    gwShader* getShader(void) const;
    gwSize    getSize(void) const;
    gwSkybox* getSkybox(void) const;
    GW_RENDERER_TYPE getType(void) const { return mType; }

    void update(void);
    virtual void render(void) = 0;

    void setAngle(float angle);
    bool setCamera(gwCamera* camera);
    void setFar(float far);
    void setNear(float near);
    void setShader(gwShader* shader);
    void setSkybox(gwSkybox* skybox);
protected :
    void setup(gwScene* scene);
    void updateProjection(void);

    const GW_RENDERER_TYPE mType;
    glm::mat4 mProjection;
    float mNear, mFar, mAngle;
    const gwSize* mSize;
    gwSize mLastSize;

    gwCamera* mCamera;
    gwScene*  mScene;
    gwShader* mShader;
    gwSkybox* mSkybox;
};

class gwScreenRenderer : public gwSceneRenderer
{
public :
    gwScreenRenderer(gwScene* scene);
    ~gwScreenRenderer(void);

    virtual void render(void);

};
/*class gwScreenRenderer
{

};*/

class gwOffScreenRenderer : public gwSceneRenderer
{
public:
    gwOffScreenRenderer(const std::string &prefix, float w, float h, gwScene *scene);
    ~gwOffScreenRenderer(void);
    virtual void render(void);

    gwTexture* getColorTexture(void) const { return mColorTexture; }
    gwTexture* getDepthTexture(void) const { return mDepthTexture; }
protected :
    void setupFbo(void);
    GLuint mFboID;
    gwTexture* mColorTexture;
    gwTexture* mDepthTexture;
    GLuint mDepthBufferID;
};


/*class gwSceneRenderer
{
public:
    gwSceneRenderer(gwScene* scene, const std::string &name, bool isFBO = false);
    virtual ~gwRenderer(void);

    void setCamera(gwCamera* camera);
    void setShader(gwShader* shader);
    void setSkybox(gwSkybox* skybox);

    gwCamera* getCamera(void) const;
    gwShader* getShader(void) const;
    gwSkybox* getSkybox(void) const;

protected:
    const std::string mName;
    gwScene* mScene;

    gwCamera* mCamera;
    gwShader* mShader;
    gwSkybox* mSkybox;

};*/

}
#endif // GWRENDERER_H
