#include "gwStaticMesh.h"

using namespace gwe;
gwe::gwStaticMesh::gwStaticMesh(const std::string &name, const std::string &sourceFile) : gwMesh(name), mSourceFile(sourceFile)
{

}

gwe::gwStaticMesh::~gwStaticMesh()
{
    //dtor
}

bool gwStaticMesh::loadObj(void)
{
    std::ifstream objFile(mSourceFile.c_str());
    if(!objFile)
    {
        std::cout << "[ERROR] Can't open file \""+mSourceFile+"\"" << std::endl;
        return false;
    }

    const char *line2;
    std::string line;
    int v_Count(0);       int vt_Count(0);      int vn_Count(0);
    while(std::getline(objFile, line))
    {
        line2 = line.c_str();
        int i[9];
        float f[3];
        if(sscanf(line2, "v %f %f %f", &f[0], &f[1], &f[2]) == 3)
            v_Count ++;
        else if(sscanf(line2, "vt %f %f", &f[0], &f[1] )== 2)
            vt_Count ++;
        else if(sscanf(line2, "vn %f %f %f", &f[0], &f[1], &f[2]) == 3)
            vn_Count ++;
        else if(sscanf(line2,"f %i/%i/%i %i/%i/%i %i/%i/%i", &i[0], &i[1],&i[2], &i[3],&i[4], &i[5],&i[6], &i[7],&i[8]) == 9)
            mVerticesCount += 3;

    }

    objFile.close();
    objFile.open(mSourceFile.c_str());

    glm::vec3 *tmpVertices = new glm::vec3[v_Count];    int tmpVCompteur = 0;
    glm::vec3 *tmpNormals = new glm::vec3[vn_Count];    int tmpVnCompteur = 0;
    glm::vec2 *tmpCoordText = new glm::vec2[vt_Count];  int tmpVtCompteur = 0;

    mVertices = new float[mVerticesCount*3];  mVerticesSize = mVerticesCount*3;
    mNormals = new float[mVerticesCount*3];   mNormalsSize = mVerticesCount*3;
    mTCoords = new float[mVerticesCount*2];   mTCoordsSize = mVerticesCount*2;

    mVerticesBytes = mVerticesSize*sizeof(float);
    mTCoordsBytes = mTCoordsSize*sizeof(float);
    mNormalsBytes = mNormalsSize*sizeof(float);

    int compteur = 0;
    while(std::getline(objFile, line))
    {
        line2 = line.c_str();
        int i[9];
        float f[3];
        if(sscanf(line2, "v %f %f %f", &f[0], &f[1], &f[2]) == 3)
        {
            tmpVertices[tmpVCompteur].x = f[0];  tmpVertices[tmpVCompteur].y = f[1];   tmpVertices[tmpVCompteur].z = f[2];
            tmpVCompteur++;
        }
        else if(sscanf(line2, "vt %f %f", &f[0], &f[1]) == 2)
        {
            tmpCoordText[tmpVtCompteur].x = f[0];   tmpCoordText[tmpVtCompteur].y = f[1];
            tmpVtCompteur++;
        }
        else if(sscanf(line2, "vn %f %f %f", &f[0], &f[1], &f[2]) == 3)
        {
            tmpNormals[tmpVnCompteur].x = f[0]; tmpNormals[tmpVnCompteur].y = f[1];    tmpNormals[tmpVnCompteur].z = f[2];
            tmpVnCompteur++;
        }
        else if(sscanf(line2,"f %i/%i/%i %i/%i/%i %i/%i/%i",  &i[0],&i[1],&i[2],  &i[3],&i[4],&i[5],  &i[6],&i[7],&i[8]) == 9)
        {
            int index = compteur*9;         int index2 = compteur*6;
            mVertices[index] = tmpVertices[i[0]-1].x;     mVertices[index+1] = tmpVertices[i[0]-1].y;   mVertices[index+2] = tmpVertices[i[0]-1].z;
            mVertices[index+3] = tmpVertices[i[3]-1].x;     mVertices[index+4] = tmpVertices[i[3]-1].y;   mVertices[index+5] = tmpVertices[i[3]-1].z;
            mVertices[index+6] = tmpVertices[i[6]-1].x;     mVertices[index+7] = tmpVertices[i[6]-1].y;   mVertices[index+8] = tmpVertices[i[6]-1].z;

            mNormals[index] = tmpNormals[i[2]-1].x;     mNormals[index+1] = tmpNormals[i[2]-1].y;   mNormals[index+2] = tmpNormals[i[2]-1].z;
            mNormals[index+3] = tmpNormals[i[5]-1].x;     mNormals[index+4] = tmpNormals[i[5]-1].y;   mNormals[index+5] = tmpNormals[i[5]-1].z;
            mNormals[index+6] = tmpNormals[i[8]-1].x;     mNormals[index+7] = tmpNormals[i[8]-1].y;   mNormals[index+8] = tmpNormals[i[8]-1].z;

            mTCoords[index2] = tmpCoordText[i[1]-1].x;     mTCoords[index2+1] = tmpCoordText[i[1]-1].y;
            mTCoords[index2+2] = tmpCoordText[i[4]-1].x;     mTCoords[index2+3] = tmpCoordText[i[4]-1].y;
            mTCoords[index2+4] = tmpCoordText[i[7]-1].x;     mTCoords[index2+5] = tmpCoordText[i[7]-1].y;
            compteur ++;
        }

    }

    delete[] tmpVertices;
    delete[] tmpNormals;
    delete[] tmpCoordText;

    objFile.close();
    return true;
}

bool gwStaticMesh::load(void)
{
     if(isLoaded())
        return false;

    std::string extension(mSourceFile.c_str()+mSourceFile.size()-3);
    int result = -1;
    // 1 -> loaded ; -1 -> wrong extension; -2 -> failed load file

    if(extension == "obj")
    {
        if(loadObj())
            result = 1;
        else
            result = -2;
    }
    else
        result = -1;


    std::cout << "[STATIC_MESH] in \"" << getName() << "\" : ";
    switch(result)
    {
    case 1 :
        std::cout << "\"" << mSourceFile << "\" loaded successfully" << std::endl;
        break;
    case -1 :
        std::cout << "Format \"" << extension << "\" is not supported" << std::endl;
        break;
    case -2 :
        std::cout << "Failed to load \"" << mSourceFile << "\"" << std::endl;
        break;
    default :
        break;
    }

    if(result != 1)
        return false;
    return gwMesh::load();
}
