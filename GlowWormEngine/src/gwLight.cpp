#include "gwLight.h"

using namespace gwe;

gwe::gwLight::gwLight(glm::vec3 color, float intensity, GW_LIGHT_TYPE TYPE) : gwComponent(GW_COMPONENT_LIGHT), mColor(color), mIntensity(intensity), mTYPE(TYPE)
{
    clampColor();
    mAttenuation[GW_CONSTANT_ATTENUATION] = 1;
    mAttenuation[GW_LINEAR_ATTENUATION] = 0;
    mAttenuation[GW_EXP_ATTENUATION] = 0;
}

gwe::gwLight::~gwLight(void)
{

}

float gwe::gwLight::getAttenuation(GW_LIGHT_ATTENUATION TYPE) const { return mAttenuation[TYPE]; }
glm::vec3 gwe::gwLight::getColor(void) const { return mColor; }
float gwe::gwLight::getIntensity(void) const { return mIntensity; }
GW_LIGHT_TYPE gwe::gwLight::getLightType(void) const { return mTYPE; }

void gwe::gwLight::setAttenuation(GW_LIGHT_ATTENUATION TYPE, float value) { mAttenuation[TYPE] = value; }
void gwe::gwLight::setColor(glm::vec3 color)
{
    mColor = color;
    clampColor();
}

void gwe::gwLight::setIntensity(float intensity) { mIntensity = intensity; }

void gwe::gwLight::clampColor(void) { mColor /= 255.0f; }
