#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "gwGlobalHeader.h"
#include "gwResource.h"

namespace gwe {

class gwMesh : public gwResource
{
public:
    gwMesh(const std::string &name);
    virtual ~gwMesh(void);

    unsigned int getVertices(float *&buffer) const;         unsigned int getVerticesBytes(void) const;
    unsigned int getNormals(float *&buffer) const;          unsigned int getNormalsBytes(void) const;
    unsigned int getTextureCoords(float *&buffer ) const;   unsigned int getTextureCoordsBytes(void) const;
    unsigned int getVerticesCount(void) const;
    GLuint getDrawMode(void) const;
    GLuint getMotionState(void) const;
    GLuint getVao(void) const;

    virtual bool load(void) = 0;
    virtual bool free(void);
protected:
    //Mesh(GameObject *object);
   // Mesh(const std::string &name, const std::string &filePath);
  //  virtual ~Mesh(void);

  //  virtual bool load(void);
    virtual void genBuffers(void);

//    MeshReader *mReader;
    float *mVertices;   unsigned int mVerticesSize;     unsigned int mVerticesBytes;
    float *mNormals;    unsigned int mNormalsSize;      unsigned int mNormalsBytes;
    float *mTCoords;    unsigned int mTCoordsSize;      unsigned int mTCoordsBytes;

    unsigned int mVerticesCount;

    GLuint mDRAW_MODE;
    GLuint mMOTION_STATE;

    GLuint mVboID;
    GLuint mVaoID;
};


}
/*class Plane : public Mesh
{
public :
    Plane(const float &width, const float &height, GameObject* object);
    ~Plane(void);
private:
};


class Rect : public Mesh
{
public:
    Rect(const float &width,const float &height, const float &depth, GameObject* object);
    ~Rect(void);
private:
};
*/

#endif // MESH_H_INCLUDED
