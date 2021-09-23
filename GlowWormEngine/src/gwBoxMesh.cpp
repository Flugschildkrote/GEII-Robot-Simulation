#include "gwBoxMesh.h"
using namespace gwe;


gwe::gwBoxMesh::gwBoxMesh(const std::string &name, float width, float height, float depth) : gwPrimitiveShape(name), mWidth(width), mHeight(height), mDepth(depth)
{

}

gwe::gwBoxMesh::~gwBoxMesh()
{

}

float gwe::gwBoxMesh::getWidth(void) const { return mWidth; }
float gwe::gwBoxMesh::getHeight(void) const { return mHeight; }
float gwe::gwBoxMesh::getDepth(void) const { return mDepth; }


void gwe::gwBoxMesh::generateShape(void)
{
    mVertices = new float[108];
    mVerticesSize = 108;
    mVerticesCount = 36;
    mVerticesBytes = 108*sizeof(float);

    float X = mWidth/2.0f;       float Y = mHeight/2.0f;      float Z = mDepth/2.0f;

    mVertices[0] = -X;      mVertices[1] =  -Y;     mVertices[2] =  Z;
    mVertices[3] =  X;      mVertices[4] =  -Y;     mVertices[5] =  Z;
    mVertices[6] =  X;      mVertices[7] =   Y;     mVertices[8] =  Z;
    mVertices[9] = -X;      mVertices[10] = -Y;     mVertices[11] = Z;
    mVertices[12]= -X;      mVertices[13] =  Y;     mVertices[14] = Z;
    mVertices[15]=  X;      mVertices[16] =  Y;     mVertices[17] = Z;

    mVertices[18] =  X;     mVertices[19] = -Y;     mVertices[20] =  Z;
    mVertices[21] =  X;     mVertices[22] = -Y;     mVertices[23] = -Z;
    mVertices[24] =  X;     mVertices[25] =  Y;     mVertices[26] = -Z;
    mVertices[27] =  X;     mVertices[28] = -Y;     mVertices[29] =  Z;
    mVertices[30] =  X;     mVertices[31] =  Y;     mVertices[32] =  Z;
    mVertices[33] =  X;     mVertices[34] =  Y;     mVertices[35] = -Z;

    mVertices[36] =  X;     mVertices[37] = -Y;     mVertices[38] = -Z;
    mVertices[39] = -X;     mVertices[40] = -Y;     mVertices[41] = -Z;
    mVertices[42] = -X;     mVertices[43] =  Y;     mVertices[44] = -Z;
    mVertices[45] =  X;     mVertices[46] = -Y;     mVertices[47] = -Z;
    mVertices[48] =  X;     mVertices[49] =  Y;     mVertices[50] = -Z;
    mVertices[51] = -X;     mVertices[52] =  Y;     mVertices[53] = -Z;

    mVertices[54] = -X;     mVertices[55] = -Y;     mVertices[56] = -Z;
    mVertices[57] = -X;     mVertices[58] = -Y;     mVertices[59] =  Z;
    mVertices[60] = -X;     mVertices[61] =  Y;     mVertices[62] =  Z;
    mVertices[63] = -X;     mVertices[64] = -Y;     mVertices[65] = -Z;
    mVertices[66] = -X;     mVertices[67] =  Y;     mVertices[68] = -Z;
    mVertices[69] = -X;     mVertices[70] =  Y;     mVertices[71] =  Z;

    mVertices[72] = -X;     mVertices[73] =  Y;     mVertices[74] =  Z;
    mVertices[75] =  X;     mVertices[76] =  Y;     mVertices[77] =  Z;
    mVertices[78] =  X;     mVertices[79] =  Y;     mVertices[80] = -Z;
    mVertices[81] = -X;     mVertices[82] =  Y;     mVertices[83] =  Z;
    mVertices[84] = -X;     mVertices[85] =  Y;     mVertices[86] = -Z;
    mVertices[87] =  X;     mVertices[88] =  Y;     mVertices[89] = -Z;

    mVertices[90] =  X;     mVertices[91] = -Y;     mVertices[92] =  Z;
    mVertices[93] = -X;     mVertices[94] = -Y;     mVertices[95] =  Z;
    mVertices[96] = -X;     mVertices[97] = -Y;     mVertices[98] = -Z;
    mVertices[99] =  X;     mVertices[100] = -Y;    mVertices[101] =  Z;
    mVertices[102] =  X;    mVertices[103] = -Y;    mVertices[104] = -Z;
    mVertices[105] = -X;    mVertices[106] = -Y;    mVertices[107] = -Z;

    mNormals = new float[108];       mNormalsSize = 108;      mNormalsBytes = 108*sizeof(float);
    mNormals[0] = 0;        mNormals[1] = 0;        mNormals[2] = 1;
    mNormals[3] = 0;        mNormals[4] = 0;        mNormals[5] = 1;
    mNormals[6] = 0;        mNormals[7] = 0;        mNormals[8] = 1;
    mNormals[9] = 0;        mNormals[10] = 0;        mNormals[11] = 1;
    mNormals[12] = 0;        mNormals[13] = 0;       mNormals[14] = 1;
    mNormals[15] = 0;        mNormals[16] = 0;        mNormals[17] = 1;

    mNormals[18] = 1;        mNormals[19] = 0;        mNormals[20] = 0;
    mNormals[21] = 1;        mNormals[22] = 0;        mNormals[23] = 0;
    mNormals[24] = 1;        mNormals[25] = 0;        mNormals[26] = 0;
    mNormals[27] = 1;        mNormals[28] = 0;        mNormals[29] = 0;
    mNormals[30] = 1;        mNormals[31] = 0;        mNormals[32] = 0;
    mNormals[33] = 1;        mNormals[34] = 0;        mNormals[35] = 0;

    mNormals[36] = 0;        mNormals[37] = 0;        mNormals[38] = -1;
    mNormals[39] = 0;        mNormals[40] = 0;        mNormals[41] = -1;
    mNormals[42] = 0;        mNormals[43] = 0;        mNormals[44] = -1;
    mNormals[45] = 0;        mNormals[46] = 0;        mNormals[47] = -1;
    mNormals[48] = 0;        mNormals[49] = 0;        mNormals[50] = -1;
    mNormals[51] = 0;        mNormals[52] = 0;        mNormals[53] = -1;

    mNormals[54] = -1;     mNormals[55] = 0;     mNormals[56] = 0;
    mNormals[57] = -1;     mNormals[58] = 0;     mNormals[59] = 0;
    mNormals[60] = -1;     mNormals[61] = 0;     mNormals[62] = 0;
    mNormals[63] = -1;     mNormals[64] = 0;     mNormals[65] = 0;
    mNormals[66] = -1;     mNormals[67] = 0;     mNormals[68] = 0;
    mNormals[69] = -1;     mNormals[70] = 0;     mNormals[71] = 0;

    mNormals[72] =  0;     mNormals[73] =  1;     mNormals[74] =  0;
    mNormals[75] =  0;     mNormals[76] =  1;     mNormals[77] =  0;
    mNormals[78] =  0;     mNormals[79] =  1;     mNormals[80] =  0;
    mNormals[81] =  0;     mNormals[82] =  1;     mNormals[83] =  0;
    mNormals[84] =  0;     mNormals[85] =  1;     mNormals[86] =  0;
    mNormals[87] =  0;     mNormals[88] =  1;     mNormals[89] =  0;

    mNormals[90] =  0;     mNormals[91] = -1;     mNormals[92] =  0;
    mNormals[93] =  0;     mNormals[94] = -1;     mNormals[95] =  0;
    mNormals[96] =  0;     mNormals[97] = -1;     mNormals[98] =  0;
    mNormals[99] =  0;     mNormals[100] = -1;    mNormals[101] =  0;
    mNormals[102] =  0;    mNormals[103] = -1;    mNormals[104] =  0;
    mNormals[105] =  0;    mNormals[106] = -1;    mNormals[107] =  0;

    mTCoords = new float[72];       mTCoordsSize = 72;      mTCoordsBytes = 72*sizeof(float);
    mTCoords[0] = 0;        mTCoords[1] = 0;
    mTCoords[2] = 1;        mTCoords[3] = 0;
    mTCoords[4] = 1;        mTCoords[5] = 1;
    mTCoords[6] = 0;        mTCoords[7] = 0;
    mTCoords[8] = 0;        mTCoords[9] = 1;
    mTCoords[10] = 1;       mTCoords[11] = 1;

    mTCoords[12] = 0;       mTCoords[13] = 0;
    mTCoords[14] = 1;       mTCoords[15] = 0;
    mTCoords[16] = 1;       mTCoords[17] = 1;
    mTCoords[18] = 0;       mTCoords[19] = 0;
    mTCoords[20] = 0;       mTCoords[21] = 1;
    mTCoords[22] = 1;       mTCoords[23] = 1;

    mTCoords[24] = 0;       mTCoords[25] = 0;
    mTCoords[26] = 1;       mTCoords[27] = 0;
    mTCoords[28] = 1;       mTCoords[29] = 1;
    mTCoords[30] = 0;       mTCoords[31] = 0;
    mTCoords[32] = 0;       mTCoords[33] = 1;
    mTCoords[34] = 1;       mTCoords[35] = 1;

    mTCoords[36] = 0;       mTCoords[37] = 0;
    mTCoords[38] = 1;       mTCoords[39] = 0;
    mTCoords[40] = 1;       mTCoords[41] = 1;
    mTCoords[42] = 0;       mTCoords[43] = 0;
    mTCoords[44] = 0;       mTCoords[45] = 1;
    mTCoords[46] = 1;       mTCoords[47] = 1;

    mTCoords[48] = 0;       mTCoords[49] = 0;
    mTCoords[50] = 1;       mTCoords[51] = 0;
    mTCoords[52] = 1;       mTCoords[53] = 1;
    mTCoords[54] = 0;       mTCoords[55] = 0;
    mTCoords[56] = 0;       mTCoords[57] = 1;
    mTCoords[58] = 1;       mTCoords[59] = 1;

    mTCoords[60] = 0;       mTCoords[61] = 0;
    mTCoords[62] = 1;       mTCoords[63] = 0;
    mTCoords[64] = 1;       mTCoords[65] = 1;
    mTCoords[66] = 0;       mTCoords[67] = 0;
    mTCoords[68] = 0;       mTCoords[69] = 1;
    mTCoords[70] = 1;       mTCoords[71] = 1;
}

