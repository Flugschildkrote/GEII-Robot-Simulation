#ifndef SIMULATIONUI_H
#define SIMULATIONUI_H

#include "gwGlobalHeader.h"

namespace simu{

class SimulationUI
{
public:
    SimulationUI(float x, float y, float w, float h, float depth = 0);
    virtual ~SimulationUI();

    glm::mat4 getModelMatrix(void) const;
    void setModelMatrix(const glm::mat4 &matrix);
    void setTexture(gwe::gwTexture* texture);
    void setColor(const glm::vec3 &color);

    void Render(gwe::gwShader* shader);
private:


    gwe::gwTexture* mTexture;
    glm::mat4 mModelMatrix;
    glm::mat4 mProjection;
    glm::vec4 mColor;
    GLuint mVaoID;
    GLuint mVboID;
    float mVertices[18];
    float mTextureCoord[12];
};

}

#endif // SIMULATIONUI_H
