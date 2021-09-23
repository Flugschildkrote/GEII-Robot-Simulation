#include "SimulationUI.h"
#include "gwShader.h"
#include "gwTexture.h"

simu::SimulationUI::SimulationUI(float x, float y, float w, float h, float depth) : mTexture(nullptr), mModelMatrix(1.0), mColor(0,0,0,1), mVaoID(0), mVboID(0)
{
    mProjection = glm::ortho(0.0f,1.0f,0.0f,1.0f);

    mVertices[0] = x;     mVertices[1] = 1-y-h;   //mVertices[2] = z;
    mVertices[3] = x+w;   mVertices[4] = 1-y-h;
    mVertices[6] = x+w;   mVertices[7] = 1-y;

    mVertices[9] = x;   mVertices[10] = 1-y-h;
    mVertices[12] = x;   mVertices[13] = 1-y;
    mVertices[15] = x+h;   mVertices[16] = 1-y;

    for(int i(2); i <= 18; i+=3){
        mVertices[i] = depth;
    }

    mTextureCoord[0] = 0;    mTextureCoord[1] = 0;
    mTextureCoord[2] = 1;    mTextureCoord[3] = 0;
    mTextureCoord[4] = 1;    mTextureCoord[5] = 1;

    mTextureCoord[6] = 0;    mTextureCoord[7] = 0;
    mTextureCoord[8] = 0;    mTextureCoord[9] = 1;
    mTextureCoord[10] = 1;   mTextureCoord[11] = 1;

    if(glIsBuffer(mVboID) == GL_TRUE)
        glDeleteBuffers(1,&mVboID);
    glGenBuffers(1, &mVboID);

    glBindBuffer(GL_ARRAY_BUFFER, mVboID);
        glBufferData(GL_ARRAY_BUFFER, 18*sizeof(float)+12*sizeof(float), 0, GL_STATIC_DRAW); // MotionState : Dynamic / Static
        glBufferSubData(GL_ARRAY_BUFFER, 0              , 18*sizeof(float), mVertices);
        glBufferSubData(GL_ARRAY_BUFFER, 18*sizeof(float) , 12*sizeof(float) , mTextureCoord);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(mVaoID) == GL_TRUE)
        glDeleteVertexArrays(1,&mVaoID);
    glGenVertexArrays(1,&mVaoID);

    glBindVertexArray(mVaoID);
        glBindBuffer(GL_ARRAY_BUFFER, mVboID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(18*sizeof(float)));
            glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

simu::SimulationUI::~SimulationUI()
{
    if(glIsBuffer(mVboID) == GL_TRUE)
        glDeleteBuffers(1,&mVboID);

    if(glIsVertexArray(mVaoID) == GL_TRUE){
        glDeleteVertexArrays(1, &mVaoID);
    }
}

glm::mat4 simu::SimulationUI::getModelMatrix(void) const { return mModelMatrix; }
void simu::SimulationUI::setModelMatrix(const glm::mat4 &matrix){ mModelMatrix = matrix; }
void simu::SimulationUI::setTexture(gwe::gwTexture* texture){ mTexture = texture; mColor.w = 0; }
void simu::SimulationUI::setColor(const glm::vec3 &color){ mColor = glm::vec4(color, mColor.w); }

void simu::SimulationUI::Render(gwe::gwShader* shader){
    glm::mat4 matrix = mProjection*mModelMatrix;
    glUseProgram(shader->getProgramID());
        glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
        glUniform4f(glGetUniformLocation(shader->getProgramID(), "color"), mColor.x, mColor.y, mColor.z, mColor.w);
        if(mTexture != nullptr){
            glUniform1i(glGetUniformLocation(shader->getProgramID(), "tex"), 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mTexture->getID());
        }

        glBindVertexArray(mVaoID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    glUseProgram(0);
}

