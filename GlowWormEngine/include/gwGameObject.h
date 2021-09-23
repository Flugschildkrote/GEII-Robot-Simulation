#ifndef GWGAMEOBJECT_H
#define GWGAMEOBJECT_H

#include "gwGlobalHeader.h"

namespace gwe{


class gwGameObject
{
public:
    gwGameObject(const std::string &name, gwGameObject* parent = nullptr);
    virtual ~gwGameObject();


    bool addChild(gwGameObject* child);
    bool addComponent(gwComponent* component);
    void deleteChildren(void);
    bool findChild(const std::string &name);
    gwGameObject* getChild(const std::string &childName) const;
    gwComponent* getComponent(GW_COMPONENT_TYPE TYPE) const;
    GW_LINK_STATE getLinkState(void) const;
    const std::string getName(void) const;
    gwGameObject* getParent(void) const;
    gwScene* getScene(void) const;
    gwTransform* getTransform(void) const;

    GW_LINK_STATE linkScene(gwScene* scene);
    bool removeChild(const std::string &childName);
    bool removeComponent(GW_COMPONENT_TYPE TYPE);
    void updateChildrenTransform(void);
    virtual void update(void);
    virtual void render(void);

    //void tick(void);
   // void render(void);
    //void updateScene(void);




  //  void addComponent(void);

   // void setCamera(gwCamera *camera);
   // gwCamera* getCamera(void) const;
protected:
    const std::string mName;
    gwTransform *mTransform;
    gwScene* mScene;

    gwGameObject* mParent;
    std::map<std::string, gwGameObject*> mChildren;

private :
    void updateTransform(const glm::mat4 &parentWorldTransform);

    gwComponent* mComponents[GW_COMPONENT_COUNT];
    GW_LINK_STATE mLinkState;
    bool mOnTransition;


   // gwCamera* mCamera;
};


}
#endif // GWGAMEOBJECT_H
