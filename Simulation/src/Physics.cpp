#include "Physics.h"
#include "gwShader.h"
#include "gwScene.h"
#include "gwResourceManager.h"


Physics::Physics()
{
    //ctor
}

Physics::~Physics()
{
    //dtor
}

void Physics::Setup(void){
    btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
    mPhysicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
    mPhysicsWorld->setGravity(btVector3(0,-981,0));

    GLDebugDrawer *drawer = new GLDebugDrawer;
    drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits | btIDebugDraw::DBG_DrawContactPoints);
    mPhysicsWorld->setDebugDrawer(drawer);

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    /*{
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(0.5),btScalar(0.5),btScalar(0.5)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,1.0,0));

		btScalar mass(0.0);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.0f);
		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		mPhysicsWorld->addRigidBody(body);
    }*/

   /* {
        btCollisionShape* groundShape2 = new btBoxShape(btVector3(btScalar(5),btScalar(0.1),btScalar(5)));

		collisionShapes.push_back(groundShape2);

		btTransform groundTransform2;
		groundTransform2.setIdentity();
		groundTransform2.setOrigin(btVector3(0,-0.1,0));

		btScalar mass(0.0);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.0f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape2->calculateLocalInertia(mass,localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform2);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape2,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		body->setUserIndex(0);
//		body->getO

		//add the body to the dynamics world

		mPhysicsWorld->addRigidBody(body);
    }*/

    /*{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(10.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

			startTransform.setOrigin(btVector3(0,0,0));

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);
            body->setUserIndex(1);

			mPhysicsWorld->addRigidBody(body);
	}*/

}

btDiscreteDynamicsWorld* Physics::getWorld(void){
    return mPhysicsWorld;
}

GLDebugDrawer::GLDebugDrawer(glm::mat4 view, glm::mat4 proj) : btIDebugDraw(), mVboID(0), mVaoID(0),
mVerticesBytes(0), mProjection(proj),mView(view)
{
    mShader = new gwe::gwShader("Wireframe_Bullet", "DATA/Shaders/Default/Wireframe/GLDebugDraw.vert",
                         "DATA/Shaders/Default/Wireframe/GLDebugDraw.frag",
                        "DATA/Shaders/Default/Wireframe/GLDebugDraw.geom" );
    mShader->load();

    //for(unsigned int i(0); i < 6; i++)
    //mVertices[0] = 0;    mVertices[1] = 0;  mVertices[2] = 0;
   // mVertices[3] = 0;    mVertices[4] = 0;  mVertices[5] = 0;
  //  mMaxVerticesBytes = 6000*sizeof(float);
    //mVertices[6] = 0.5;    mVertices[7] = 1;  mVertices[8] = 0;

    //Load();
}

GLDebugDrawer::GLDebugDrawer(void) : btIDebugDraw(), mVboID(0), mVaoID(0),
mVerticesBytes(0), mView(1.0), mProjection(1.0)
{
    mShader = new gwe::gwShader("Wireframe_Bullet", "DATA/Shaders/Default/Wireframe/GLDebugDraw.vert",
                         "DATA/Shaders/Default/Wireframe/GLDebugDraw.frag",
                        "DATA/Shaders/Default/Wireframe/GLDebugDraw.geom" );
    mShader->load();

    //for(unsigned int i(0); i < 6; i++)
   // mVertices[0] = 0;    mVertices[1] = 0;  mVertices[2] = 0;
    //mVertices[3] = 0;    mVertices[4] = 0;  mVertices[5] = 0;
    //mMaxVerticesBytes = 6000*sizeof(float);
    //mVertices[6] = 0.5;    mVertices[7] = 1;  mVertices[8] = 0;

    //Load();
}

GLDebugDrawer::~GLDebugDrawer()
{

}

void GLDebugDrawer::setMatrices(glm::mat4 view, glm::mat4 proj)
{
    mProjection = proj;
    mView = view;
}

void GLDebugDrawer::setProjection(glm::mat4 proj){
    mProjection = proj;
}


void GLDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
   // std::cout << "DRAW" << std::endl;
    //int index = mLinesCounter*6;
    mVertices.push_back(from.x());    mVertices.push_back(from.y());   mVertices.push_back(from.z());
    mVertices.push_back(to.x());    mVertices.push_back(to.y());    mVertices.push_back(to.z());

    mColors.push_back(color.x());    mColors.push_back(color.y());    mColors.push_back(color.z());
    mColors.push_back(color.x());    mColors.push_back(color.y());    mColors.push_back(color.z());

    //mLinesCounter ++;
}

void GLDebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

void GLDebugDrawer::Load(void)
{

    if(glIsBuffer(mVboID) != GL_TRUE)
        glGenBuffers(1, &mVboID);

    glBindBuffer(GL_ARRAY_BUFFER, mVboID);
        glBufferData(GL_ARRAY_BUFFER, mVerticesBytes + mColorsBytes, 0, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0 ,  mVerticesBytes , mVertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, mVerticesBytes, mColorsBytes, mColors.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(mVaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &mVaoID);

    glGenVertexArrays(1, &mVaoID);

    glBindVertexArray(mVaoID);
        glBindBuffer(GL_ARRAY_BUFFER, mVboID);
            glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0 , BUFFER_OFFSET(0));
            glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVerticesBytes));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLDebugDrawer::flushLines()
{
    mVerticesBytes = mVertices.size()*sizeof(float);
    mColorsBytes = mColors.size()*sizeof(float);

    Load();

    glUseProgram(mShader->getProgramID());
        glBindVertexArray(mVaoID);
            glm::mat4 modelViewProj = mProjection*mView;
            glUniformMatrix4fv(glGetUniformLocation(mShader->getProgramID(),"modelViewProjection"), 1, GL_FALSE, glm::value_ptr(modelViewProj));
            glDrawArrays(GL_LINES,0, mVertices.size() / 3);
        glBindVertexArray(0);
    glUseProgram(0);

    mVertices.clear();
    mColors.clear();
    //mLinesCounter = 0;
}

std::string Physics::make_string(const std::string &prefix, unsigned int suffix){
    std::string str = prefix;
    int index = 1;
    while(index < suffix){
        index *=10;
    }

    do{
        if(index > 1)
            index /= 10;
        int v = suffix/index;
        suffix = suffix%index;
        std::cout << suffix << std::endl;
        str+= v+'0';
    }while(index != 1);
    return str;
}
