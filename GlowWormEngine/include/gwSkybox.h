#ifndef GWSKYBOX_H
#define GWSKYBOX_H

#include "gwGlobalHeader.h"
#include "gwResource.h"

namespace gwe {

class gwSkybox : public gwResource
{
public:
    gwSkybox(const std::string &name, const std::string &shader, const std::string &up, const std::string &down,
             const std::string &left, const std::string &right, const std::string &forward, const std::string &back, gwSkyboxMesh* mesh);
    gwSkybox(const std::string &name, gwShader*shader, const std::string &sourceFile);
    virtual ~gwSkybox();

    virtual bool load(void);
    virtual bool free(void);

    void render(const glm::mat4 &MVP);

protected:

    bool validElement(tinyxml2::XMLElement* element, const std::string &value);

    gwShader* mShader;
    GLuint mTexturesID[6];
    gwSkyboxMesh* mMesh;

    std::string mSourceFile;
};

}
#endif // GWSKYBOX_H
