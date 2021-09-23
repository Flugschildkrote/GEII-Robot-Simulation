#ifndef GWSCENE_H
#define GWSCENE_H

#include "gwGlobalHeader.h"
#include "gwRenderer.h"

#ifndef GW_MAX_LIGHTS
#define GW_MAX_LIGHTS 100
#endif // MAX_LIGHTS

namespace gwe
{


class gwSceneRenderer_Fonct
{
public:
    bool operator()(gwSceneRenderer* renderer1, gwSceneRenderer* renderer2) { return (*renderer2) < (*renderer1); }
};

class gwScene
{
public:
    gwScene(const std::string &name);
    virtual ~gwScene(void);

    bool addGameObject(gwGameObject* object);
    bool addSceneRenderer(gwSceneRenderer *renderer);

    gwGameObject* getGameObject(const std::string &objectName) const;
    GW_LINK_STATE getLinkState(const std::string &objectName) const;
    std::string getName(void) const;
    gwSceneRenderer* getEnabledRenderer(void) const;
    gwGameObject* getRootNode(void) const;

    bool removeGameObject(const std::string &objectName);
    bool removeSceneRenderer(gwSceneRenderer* renderer);
    void render(bool clrScr);
    void update(void);

    void setAmbiantLight(const glm::vec3 &color, float intensity);
    void sendLights(const GLuint &shaderID);
protected:

    const std::string mName;
private :
    std::map<std::string, gwGameObject*> mGameObjects;
    std::map<std::string, GW_LINK_STATE> mLinkStates;
    gwGameObject* mRootNode;

    /** LIGHTS */

    glm::vec3 mAmbiantColor;
    float mAmbiantIntensity;

    gwLight* mLights[GW_MAX_LIGHTS];
    unsigned int mLightsCount;

    std::set<gwSceneRenderer*> mSceneRenderers;
    gwSceneRenderer *mEnabledRenderer;
};


}
#endif // GWSCENE_H
