#ifndef GWMATERIAL_H
#define GWMATERIAL_H

#include "gwGlobalHeader.h"
#include "gwComponent.h"

namespace gwe {

class gwMaterial : public gwComponent
{
public:
    gwMaterial(void);
    virtual ~gwMaterial(void);

    bool enableTexture(float intensity = 1.0f);

    glm::vec3 getColor(void) const;
    bool isLightSensitive(void) const { return mLightSensitive; }
    float getReflectFactor(void) const;
    float getReflectPower(void) const;
    gwTexture* getTexture(void) const;

    void render(GLuint shaderID);

    void setColor(const glm::vec3 &color);
    void setLightSensitive(bool sensitive) { mLightSensitive = sensitive; }
    void setReflectivity(float factor, float power);
    void setTexture(gwTexture *texture, float intensity = 1.0f);

protected:
    glm::vec3 mColor;
    //gwShader* mShader;
    gwTexture* mTexture;
    float mTextureIntensity;

    float mReflectFactor;
    float mReflectPower;
    bool mLightSensitive;
private:
};

}

#endif // GWMATERIAL_H
