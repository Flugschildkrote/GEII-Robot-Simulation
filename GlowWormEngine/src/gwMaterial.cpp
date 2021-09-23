#include "gwMaterial.h"
#include "gwShader.h"
#include "gwTexture.h"

using namespace gwe;

gwe::gwMaterial::gwMaterial(void) : gwComponent(GW_COMPONENT_MATERIAL), mColor(0,1,0), mTexture(nullptr), mTextureIntensity(0.0),
mReflectFactor(0.5), mReflectPower(100), mLightSensitive(true)
{

}

gwe::gwMaterial::~gwMaterial(void)
{
    //dtor
}

bool gwe::gwMaterial::enableTexture(float intensity)
{
    if(mTexture == nullptr && intensity != 0)
    {
        std::cout << "[ERROR][MATERIAL] Can't enable nullptr texture." << std::endl;
        return false;
    }

    mTextureIntensity = intensity;
    return true;
}

glm::vec3 gwe::gwMaterial::getColor(void) const { return (mColor*255.0f); }
//gwShader* gwe::gwMaterial::getShader(void) const { return mShader; }
float gwe::gwMaterial::getReflectFactor(void) const { return mReflectFactor; }
float gwe::gwMaterial::getReflectPower(void) const { return mReflectPower; }
gwTexture* gwe::gwMaterial::getTexture(void) const { return mTexture; }

void gwe::gwMaterial::render(GLuint shaderID)
{
    glUniform3f(glGetUniformLocation(shaderID, "material.color"), mColor.x, mColor.y, mColor.z);
    glUniform1f(glGetUniformLocation(shaderID, "material.pictureIntensity"), mTextureIntensity);
    glUniform1i(glGetUniformLocation(shaderID, "material.lightSensitive"), (unsigned int) mLightSensitive);

    if(mLightSensitive)
    {
        glUniform1f(glGetUniformLocation(shaderID, "material.reflectFactor"), mReflectFactor );
        glUniform1f(glGetUniformLocation(shaderID, "material.reflectPower"), mReflectPower);
    }

    if(mTextureIntensity != 0)
    {
        glUniform1i(glGetUniformLocation(shaderID, "material.picture"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture->getID());
    }
}

void gwe::gwMaterial::setColor(const glm::vec3 &color) { mColor = (color/255.0f); }
//void gwe::gwMaterial::setShader(gwShader* shader) { mShader = shader; }
void gwe::gwMaterial::setReflectivity(float factor, float power)
{
    mReflectFactor = factor;
    mReflectPower = power;
}
void gwe::gwMaterial::setTexture(gwTexture* texture, float intensity)
{
     mTexture = texture;
     mTextureIntensity = intensity;
}

