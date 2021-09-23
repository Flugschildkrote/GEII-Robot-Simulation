#include "gwSkyboxMesh.h"

using namespace gwe;

gwe::gwSkyboxMesh::gwSkyboxMesh(const std::string &name, float boxSize) : gwBoxMesh(name, boxSize,boxSize,boxSize), mTIDBytes (0)
{

}

gwe::gwSkyboxMesh::~gwSkyboxMesh()
{

}

void gwe::gwSkyboxMesh::genBuffers(void)
{
    if(glIsBuffer(mVboID) == GL_TRUE)
        glDeleteBuffers(1,&mVboID);
    glGenBuffers(1, &mVboID);

    glBindBuffer(GL_ARRAY_BUFFER, mVboID);
        glBufferData(GL_ARRAY_BUFFER, mVerticesBytes+mNormalsBytes+mTCoordsBytes+mTIDBytes, 0, mMOTION_STATE); // MotionState : Dynamic / Static
        glBufferSubData(GL_ARRAY_BUFFER, 0              , mVerticesBytes, mVertices);
        glBufferSubData(GL_ARRAY_BUFFER, mVerticesBytes , mNormalsBytes , mNormals);
        glBufferSubData(GL_ARRAY_BUFFER, mVerticesBytes + mNormalsBytes , mTCoordsBytes , mTCoords);
        glBufferSubData(GL_ARRAY_BUFFER, mVerticesBytes + mNormalsBytes + mTCoordsBytes , mTIDBytes , &mTID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(mVaoID) == GL_TRUE)
        glDeleteVertexArrays(1,&mVaoID);
    glGenVertexArrays(1,&mVaoID);

    glBindVertexArray(mVaoID);
        glBindBuffer(GL_ARRAY_BUFFER, mVboID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVerticesBytes));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVerticesBytes+mNormalsBytes));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE , 0, BUFFER_OFFSET(mVerticesBytes+mNormalsBytes+mTCoordsBytes));
            glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void gwe::gwSkyboxMesh::generateShape(void)
{
    gwBoxMesh::generateShape();
    mTIDBytes = 36*sizeof(float);
    mTID[0] = 0;    mTID[1] = 0;    mTID[2] = 0;
    mTID[3] = 0;    mTID[4] = 0;    mTID[5] = 0;

    mTID[6] = 1;    mTID[7] = 1;    mTID[8] = 1;
    mTID[9] = 1;    mTID[10] = 1;    mTID[11] = 1;

    mTID[12] = 2;    mTID[13] = 2;    mTID[14] = 2;
    mTID[15] = 2;    mTID[16] = 2;    mTID[17] = 2;

    mTID[18] = 3;    mTID[19] = 3;    mTID[20] = 3;
    mTID[21] = 3;    mTID[22] = 3;    mTID[23] = 3;

    mTID[24] = 4;    mTID[25] = 4;    mTID[26] = 4;
    mTID[27] = 4;    mTID[28] = 4;    mTID[29] = 4;

    mTID[30] = 5;    mTID[31] = 5;    mTID[32] = 5;
    mTID[33] = 5;    mTID[34] = 5;    mTID[35] = 5;
}


