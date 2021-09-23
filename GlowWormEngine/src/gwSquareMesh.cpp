#include "gwSquareMesh.h"

using namespace gwe;

gwe::gwSquareMesh::gwSquareMesh(const std::string &name, float width, float height) : gwPrimitiveShape(name), mWidth(width), mHeight(height)
{

}

gwe::gwSquareMesh::~gwSquareMesh()
{

}

float gwe::gwSquareMesh::getWidth(void) const { return mWidth; }
float gwe::gwSquareMesh::getHeight(void) const { return mHeight; }

void gwe::gwSquareMesh::generateShape(void)
{
    mVertices = new float[18];
    mVerticesSize = 18;
    mVerticesCount = 6;
    mVerticesBytes = 18*sizeof(float);

    float X = mWidth/2.0f;
    float Y = mHeight/2.0f;

    mVertices[0] =  -X;  mVertices[1] =  -Y;  mVertices[2] = 0;
    mVertices[3] =   X;  mVertices[4] =  -Y;  mVertices[5] = 0;
    mVertices[6] =   X;  mVertices[7] =   Y;  mVertices[8] = 0;

    mVertices[9] =  -X;  mVertices[10] = -Y;  mVertices[11] = 0;
    mVertices[12] = -X;  mVertices[13] =  Y;  mVertices[14] = 0;
    mVertices[15] =  X;  mVertices[16] =  Y;  mVertices[17] = 0;


    mNormals = new float[18];   mNormalsSize = 18;      mNormalsBytes = 18*sizeof(float);
    mNormals[0] =  0;  mNormals[1]  = 0;  mNormals[2] = 1;
    mNormals[3] =  0;  mNormals[4]  = 0;  mNormals[5] = 1;
    mNormals[6] =  0;  mNormals[7]  = 0;  mNormals[8] = 1;

    mNormals[9] =  0;  mNormals[10] =  0;  mNormals[11] = 1;
    mNormals[12] = 0;  mNormals[13] =  0;  mNormals[14] = 1;
    mNormals[15] = 0;  mNormals[16] =  0;  mNormals[17] = 1;

    mTCoords = new float[12];   mTCoordsSize = 12;      mTCoordsBytes = 12*sizeof(float);
    mTCoords[0] = 0;    mTCoords[1] = 0;
    mTCoords[2] = 1;    mTCoords[3] = 0;
    mTCoords[4] = 1;    mTCoords[5] = 1;

    mTCoords[6] = 0;    mTCoords[7] = 0;
    mTCoords[8] = 0;    mTCoords[9] = 1;
    mTCoords[10] = 1;    mTCoords[11] = 1;
}
