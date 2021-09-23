#ifndef PHYSICS_H
#define PHYSICS_H

#include "gwGlobalHeader.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#define USER_INDEX_ROBOT 1
#define USER_INDEX_FIELD 2
#define USER_INDEX_LINES 3
#define USER_INDEX_BALL  4


namespace simu{

struct Collider{
    btCollisionShape* collisionShape;
    btTransform transform;
    btScalar mass;
    btDefaultMotionState* motionState;
    btRigidBody* rigidBody;
};}

class Physics
{
public:
    Physics();
    virtual ~Physics();
    static std::string make_string(const std::string &prefix, unsigned int suffix);

    void Setup();
    btDiscreteDynamicsWorld* getWorld(void);
private:
    btDiscreteDynamicsWorld* mPhysicsWorld;
};








class GLDebugDrawer : public btIDebugDraw
{
public:
   GLDebugDrawer(glm::mat4 view, glm::mat4 proj);
   GLDebugDrawer(void);
   virtual ~GLDebugDrawer();
   void setMatrices(glm::mat4 view, glm::mat4 proj);
   void setProjection(glm::mat4 proj);


 //  virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);

   virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

  // virtual void   drawSphere(const btVector3& p, btScalar radius, const btVector3& color);

   //virtual void   drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);

   virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) { }

   virtual void   reportErrorWarning(const char* warningString) {}

   virtual void   draw3dText(const btVector3& location, const char* textString) { }

   virtual void   setDebugMode(int debugMode);

   virtual int      getDebugMode() const { return m_debugMode; }

   virtual void flushLines();
private :
    void Load(void);
    int m_debugMode;
    GLuint mVboID;  GLuint mVaoID;
    gwe::gwShader *mShader;

    std::vector<float> mVertices;
    std::vector<float> mColors;

   // int mLinesCounter;
    int mVerticesBytes;
    int mColorsBytes;

    int mMaxVerticesBytes;
    glm::mat4 mProjection;
    glm::mat4 mView;
};

#endif // PHYSICS_H
