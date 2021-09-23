#ifndef GWLIGHT_H
#define GWLIGHT_H

#include "gwGlobalHeader.h"
#include "gwComponent.h"

namespace gwe {

class gwLight : public gwComponent
{
public:
    gwLight(glm::vec3 color, float intensity, GW_LIGHT_TYPE TYPE);
    virtual ~gwLight(void);

    float getAttenuation(GW_LIGHT_ATTENUATION TYPE) const;
    glm::vec3 getColor(void) const;
    float getIntensity(void) const;
    GW_LIGHT_TYPE getLightType(void) const;

    void setAttenuation(GW_LIGHT_ATTENUATION TYPE, float value);
    void setColor(glm::vec3 color);
    void setIntensity(float intensity);

protected:
    void clampColor(void);

    glm::vec3 mColor;
    float mIntensity;

    float mAttenuation[3];

    GW_LIGHT_TYPE mTYPE;
};


}
#endif // GWLIGHT_H
