#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "gwGlobalHeader.h"
#include "gwSingleton.h"


namespace gwe
{
namespace singleton
{


class gwGameManager : public gwSingleton<gwGameManager>
{
    friend class gwSingleton<gwGameManager>;
public:
    bool addScene(gwScene* scene);
    void deleteScenes(void);
    bool enableScene(const std::string &sceneName);
    gwScene* getEnabledScene(void) const;
    gwScene* getScene(const std::string &sceneName) const;
    const gwSize* getRenderSize(void) const;

    bool removeScene(const std::string &sceneName);

    void render(bool clrScr);
    void update(void);

    void setRenderSize(const gwSize &newSize);
   /// bool run(void);
   /// Return the last frame time in milliseconds
 //   uint32_t getFrameTime(void);
   // float getDeltaTime(void); /// Return elapsed time since last frame in milliseconds
protected:
    gwGameManager(void);
    ~gwGameManager(void);

private :
    std::map<std::string, gwe::gwScene*> mScenes;
    gwScene* mEnabledScene;
    gwSize mRenderSize;
};


}
}

#endif // GAMEMANAGER_H_INCLUDED
