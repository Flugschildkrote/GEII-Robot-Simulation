#include "Robot_1.h"
#include "gwScene.h"
#include "gwInputManager.h"
#include "gwGameObject.h"
#include "gwTexture.h"
#include "gwMaterial.h"
#include "gwMeshRenderer.h"
#include "gwStaticMesh.h"
#include "BulletCollision/GImpact/btGimpactShape.h"
#include "gwTransform.h"
#include "gwShader.h"
#include "gwResourceManager.h"

using namespace gwe;

glm::vec3 corner_left(-13.83, 7.43945, -13.83);


simu::Robot_1::Robot_1(Physics* physics, btVector3 pos, const std::string name, unsigned int comportement, const KeyBindings& bindings, float ui_offset) : mPhysics(physics), mStartPosition(pos), mName(name), mComportementID(comportement), mBindings(bindings), mUIOffset(ui_offset)
{
    mMode = MODE_MANUAL;
}

simu::Robot_1::~Robot_1(){
    delete mCollider.collisionShape;
    delete mCollider.motionState;
    delete mCollider.rigidBody;

      for(int i(0); i < 2; i++){
        for(short j(0); j < 4; j++){
            delete mLineSensor[i].cny_70[j];
        }
    }
}

void simu::Robot_1::Setup(gwe::gwScene* scene, gwe::gwShader* shader){
    SetupGameObject(scene);
    Setup_CNY_70(scene);
    Setup_Sharp_Sensors(scene);
    Setup_Collider();

    btTransform startPosition;
    startPosition.setIdentity();
    startPosition.setOrigin(mStartPosition);
    mCollider.rigidBody->setWorldTransform(startPosition);

    mPixy = new simu::Pixy(scene,shader,mName, mChassisObject);
    mPixy->getGameObject()->getTransform()->translate(glm::vec3(0,4, -10), GW_WORLD_SPACE);
    mPixy->getGameObject()->getTransform()->rotate( M_PI , glm::vec3(0,1, 0), GW_WORLD_SPACE);

    mUI = new simu::SimulationUI(mUIOffset, 0.0,0.3,0.3);
    mShader = new gwShader("GUI_Shader"+mName, "Data/Shaders/Default/GUI/Shader_GUI.vert", "Data/Shaders/Default/GUI/Shader_GUI.frag");
    mShader->load();
    mUI->setColor(glm::vec3(1.0,0,1.0));
    mUI->setTexture(dynamic_cast<gwTexture*>(gwe::singleton::gwResourceManager::getInstance()->getResource("Renderer1_1_Color0")));

   // gwe::gwStaticMesh *collider = new gwe::gwStaticMesh("Chassis_Collider"+mName, "Data/Meshs/Chassis_Collider_3.obj");
   // collider->load();
    /*float *vertices;
    int bufferSize = collider->getVertices(vertices);
    btTriangleMesh *tri = new btTriangleMesh;
    for(int i(0); i < bufferSize ; i+=9){
        tri->addTriangle(btVector3(vertices[i], vertices[i+1], vertices[i+2]),
                             btVector3(vertices[i+3], vertices[i+4], vertices[i+5]),
                             btVector3(vertices[i+6], vertices[i+7], vertices[i+8]));
        mCollider.collisionShape = new btConvexTriangleMeshShape(tri, true);

    }*/


    Create_Wheels(scene);

    for(int i(0); i < 2; i++){
        btVector3 vect = mWheel[i].rigidBody->getWorldTransform().getOrigin();//+pos[i];
        btVector3 vect2(0, 1 ,0);
        btVector3 vect3(1, 0, 0);

      // mWheelHinge[i] = new btHingeConstraint(*mCollider.rigidBody, *(mWheel[i].rigidBody), mWheel[i].transform.getOrigin(), btVector3(0,0,0), btVector3(1,0,0), btVector3(1,0,0));
        mWheelHinge[i] = new btHinge2Constraint(*mCollider.rigidBody, *(mWheel[i].rigidBody), vect, vect2, vect3);
        mWheelHinge[i]->setLowerLimit(-0.001);
        mWheelHinge[i]->setUpperLimit(0.001);
        mWheelHinge[i]->setLinearLowerLimit(btVector3(0,0,0));
        mWheelHinge[i]->setLinearUpperLimit(btVector3(0,0,0));
        mWheelHinge[i]->setDbgDrawSize(0.5f);

        mWheelHinge[i]->enableSpring(2, false);

        mWheelHinge[i]->enableMotor(3, true);
        mWheelHinge[i]->setMaxMotorForce(3, 10000.0f);
      /*  hinge_wheel_1->setStiffness(2, 0*SIMD_PI * SIMD_PI * 4.f);
        hinge_wheel_1->setDamping(2, 1000);*/
      //  mWheelHinge[i]->setEquilibriumPoint();

       // mHinge2 = hinge_wheel_1;*/
        mPhysics->getWorld()->addConstraint(mWheelHinge[i], true);

        {
            int motorAxis = 3;
           /** mWheelHinge[i]->enableMotor(true);
            mWheelHinge[i]->setMaxMotorImpulse(1000);**/
           // mWheelHinge[i]->setMotorTarget(0)
          /*  mWheelHinge[i]->enableMotor(motorAxis,true);
            mWheelHinge[i]->setMaxMotorForce(motorAxis,100);
            mWheelHinge[i]->setTargetVelocity(motorAxis,0);
            mWheelHinge[i]->setTargetVelocity(4, 0);
            mWheelHinge[i]->setTargetVelocity(4, 0);*/
        }

        {
            int motorAxis = 5;
           /// mWheelHinge[i]->enableMotor(motorAxis,false);
          //  mWheelHinge[i]->setMaxMotorForce(motorAxis,1000);
           // mWheelHinge[i]->setTargetVelocity(motorAxis,0);
           // hinge_wheel_1->
        }

    }


    {
        btCollisionShape* collisionShape =  new btSphereShape(1.4);
        btVector3 pos[2] = {btVector3(0,-6.2112,-9.99854), btVector3(0,-6.2112, 9.99854)};
        btTransform transform;

        for(int i(0); i < 2; i++){
            transform.setIdentity();
            transform.setOrigin(btVector3(0,0,0)+startPosition.getOrigin());
            btScalar mass = 1;

            btVector3 localInertia(0,0,0);
            collisionShape->calculateLocalInertia(mass,localInertia);
                //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
            btMotionState* motionState = new btDefaultMotionState(transform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionState,collisionShape,localInertia);
            btRigidBody *rigidBody = new btRigidBody(rbInfo);
            rigidBody->setFriction(10);
            rigidBody->setDamping(0,1);
            rigidBody->setCenterOfMassTransform(transform);
           // mCollider.rigidBody->setCollisionFlags(mCollider.rigidBody->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
            rigidBody->setActivationState(DISABLE_DEACTIVATION);
            //add the body to the dynamics world
            mPhysics->getWorld()->addRigidBody(rigidBody);
            btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*mCollider.rigidBody, *rigidBody, pos[i], btVector3(0,0,0));
            mPhysics->getWorld()->addConstraint(constraint, true);
        }
    }

    Create_Pelle();
    Sync_Pos_Whith_Collider();
}

void simu::Robot_1::Run(float deltaTime){

    gwe::singleton::gwInputManager *mInput = gwe::singleton::gwInputManager::getInstance();

    if(mInput->getKey(mBindings.autoMode1))
    {
        if (mMode != MODE_AUTO_1 || mComportementID != 1)
        {
            mMillis = 0;
            mMode = MODE_AUTO_1;
            mComportementID = 1;
            std::cout << mName << " : Mode auto (comportement = " << mComportementID << "!" << std::endl;
        }
    }
    else if (mInput->getKey(mBindings.autoMode2)) 
    {
        if (mMode != MODE_AUTO_1 || mComportementID != 2) {
            mMillis = 0;
            mMode = MODE_AUTO_1;
            mComportementID = 2;
            std::cout << mName << " : Mode auto (comportement = " << mComportementID << "!" << std::endl;
        }
    } 
    else if(mInput->getKey(mBindings.manualMode))
    {
        if (mMode != MODE_MANUAL) {
            std::cout <<  mName << " : Mode manuel !" << std::endl;
            mMode = MODE_MANUAL;
        }
    }

    Sync_Pos_Whith_Collider();
    Update_CNY_70();
    for(int i(0); i< 5; i++)
        mSharp[i]->Run();
    mPixy->Run();

    if(mMode == MODE_MANUAL){
        Mode_Manuel();
    }else if(mMode == MODE_AUTO_1){
        if(mComportementID == 1)
            Comportement_1();
        else
            Comportement_2();
    }

}

void simu::Robot_1::Sync_Pos_Whith_Collider(void){
    mCollider.transform = mCollider.rigidBody->getWorldTransform();
    btScalar localTransform[16];
    mCollider.transform.getOpenGLMatrix(localTransform);
    mChassisObject->getTransform()->setTransform(glm::make_mat4(localTransform));

    for(int i(0); i < 2; i++){
        mWheel[i].rigidBody->getMotionState()->getWorldTransform(mWheel[i].transform);
        btScalar wheelTransform[16];
        mWheel[i].transform.getOpenGLMatrix(wheelTransform);
        mWheelObject[i].gameObject->getTransform()->setTransform(glm::make_mat4(wheelTransform));
    }

}

void simu::Robot_1::Update_CNY_70(void){

    mLineSensor[0].bigWhiteLine = true;
    mLineSensor[1].bigWhiteLine = true;

    for(int i(0); i < 2; i++){
        bool whiteLine_10 = false;
        for(short j(0); j < 4; j++){
            mLineSensor[i].cny_70[j]->Run();
            mLineSensor[i].bigWhiteLine &= mLineSensor[i].cny_70[j]->detectsWhite();
        }
    }
    mBallDetector->Run();
}

void simu::Robot_1::Create_Wheels(gwe::gwScene* scene){

    mWheelObject[0].gameObject = new gwGameObject("Roue_0"+mName);
    mWheelObject[0].mesh = new gwStaticMesh("Roue_0"+mName, "Data/Meshs/Roue_1.obj");
    mWheelObject[0].material = new gwMaterial;
    mWheelObject[0].renderer = new gwMeshRenderer(mWheelObject[0].mesh);
    mWheelObject[0].mesh->load();
    mWheelObject[0].material->setColor(glm::vec3(150,150,150));
    mWheelObject[0].gameObject->addComponent(mWheelObject[0].renderer);
    mWheelObject[0].gameObject->addComponent(mWheelObject[0].material);
    scene->addGameObject(mWheelObject[0].gameObject);

    mWheelObject[1].gameObject = new gwGameObject("Roue_1"+mName);
    mWheelObject[1].mesh = new gwStaticMesh("Roue_1"+mName, "Data/Meshs/Roue_1.obj");
    mWheelObject[1].material = new gwMaterial;
    mWheelObject[1].renderer = new gwMeshRenderer(mWheelObject[1].mesh);
    mWheelObject[1].mesh->load();
    mWheelObject[1].material->setColor(glm::vec3(150,150,150));
    mWheelObject[1].gameObject->addComponent(mWheelObject[1].renderer);
    mWheelObject[1].gameObject->addComponent(mWheelObject[1].material);
    scene->addGameObject(mWheelObject[1].gameObject);


    btVector3 startPos = mCollider.rigidBody->getWorldTransform().getOrigin();
    btVector3 pos[] = {btVector3(12.746, -4.7112, 0),
                       btVector3(-12.746, -4.7112, 0)};

    for(int i(0); i < 2; i++){
        mWheel[i].collisionShape =  new btCylinderShapeX(btVector3(0.2,3,3)); // 0.8cm sur 6*6 cm
        mWheel[i].transform.setIdentity();
        mWheel[i].transform.setOrigin(pos[i]+startPos); //
        mWheel[i].mass = 1;

        btVector3 localInertia(0,0,0);
        mWheel[i].collisionShape->calculateLocalInertia(mWheel[i].mass, localInertia);
            //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        mWheel[i].motionState = new btDefaultMotionState(mWheel[i].transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mWheel[i].mass,mWheel[i].motionState,mWheel[i].collisionShape,localInertia);
        mWheel[i].rigidBody = new btRigidBody(rbInfo);
        mWheel[i].rigidBody->setFriction(1000.0);
       // mWheel[i].rigidBody->setRollingFriction(10);
       // mCollider.rigidBody->setCollisionFlags(mCollider.rigidBody->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        mWheel[i].rigidBody->setActivationState(DISABLE_DEACTIVATION);
        //add the body to the dynamics world
        mPhysics->getWorld()->addRigidBody(mWheel[i].rigidBody);
        mWheel[i].transform.setOrigin(pos[i]); //

    }
}

void simu::Robot_1::Create_Pelle(void){
    btCompoundShape* pelleShape = new btCompoundShape;
    /* Objet 1 */
    btCylinderShapeX* pelle_box1 = new btCylinderShapeX(btVector3(0.5, 3.5, 3.5));
    btBoxShape* pelle_box3 = new btBoxShape(btVector3(5, 3.5, 0.5));
    btBoxShape* shootingShape = new btBoxShape(btVector3(3.9,3,0.5));

    btVector3 startPos = mCollider.rigidBody->getWorldTransform().getOrigin();
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(-4.5, 0,0));
    pelleShape->addChildShape(transform, pelle_box1);
    transform.setOrigin(btVector3(4.5, 0,0));
    pelleShape->addChildShape(transform, pelle_box1);
    transform.setOrigin(btVector3(0, 0 , 4));
    pelleShape->addChildShape(transform, pelle_box3);


    btRigidBody* mPelleBody;
    /** PELLE **/
    {
        transform.setIdentity();
        btScalar mass = 0.5;
        btVector3 localInertia(0,0,0);
        transform.setOrigin(btVector3(0,-5,-20)+startPos);
        pelleShape->calculateLocalInertia(mass,localInertia);
        btMotionState* motionState = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionState,pelleShape,localInertia);
        mPelleBody = new btRigidBody(rbInfo);
        mPelleBody->setFriction(10);
        mPelleBody->setRestitution(0.1);
        mPhysics->getWorld()->addRigidBody(mPelleBody);
    }

    btRigidBody* mShooterBody;
    /** Shooter **/
    {
        transform.setIdentity();
        btScalar mass = 0.5;
        btVector3 localInertia(0,0,0);
        transform.setOrigin(btVector3(0,-5,-20)+startPos);
        shootingShape->calculateLocalInertia(mass,localInertia);
        btMotionState* motionState = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionState,shootingShape,localInertia);
        mShooterBody = new btRigidBody(rbInfo);
        mShooterBody->setFriction(10);
        mShooterBody->setRestitution(0.1);
        mPhysics->getWorld()->addRigidBody(mShooterBody);
    }

    transform.setOrigin(btVector3(0,-3,0));
    transform.setRotation( btQuaternion(btVector3(0,1,0), 1.0/2.0*M_PI ));

    btTransform transform2;
    transform2.setOrigin(btVector3(0,0,0));
    transform2.setRotation(btQuaternion(btVector3(0,1,0), 1.0/2.0*M_PI ));

    mShooterConstraint = new btSliderConstraint(*mCollider.rigidBody, *mShooterBody, transform, transform2, true);
    mPhysics->getWorld()->addConstraint(mShooterConstraint);

    mPelleRotationonstraint = new btHingeConstraint(*mCollider.rigidBody, *mPelleBody, btVector3(0,-3,-16.5), btVector3(0,0,0),  btVector3(1,0,0),  btVector3(1,0,0));
    mPhysics->getWorld()->addConstraint(mPelleRotationonstraint);


    mShooterConstraint->setDbgDrawSize(0.5f);
    mPelleRotationonstraint->setDbgDrawSize(0.5f);

    mShooterConstraint->setLowerLinLimit(12.5);
    mShooterConstraint->setUpperLinLimit(26);
    mShooterConstraint->setMaxLinMotorForce(100000.0f);
    mShooterConstraint->setPoweredLinMotor(true);

    mPelleRotationonstraint->setLimit(M_PI/2.0f, M_PI-0.01);
    mPelleRotationonstraint->enableAngularMotor(true, 0,100000.0f );

    mShooterBody->setActivationState(DISABLE_DEACTIVATION);
    mPelleBody->setActivationState(DISABLE_DEACTIVATION);

}

void simu::Robot_1::SetupGameObject(gwe::gwScene* scene){
    mChassisObject = new gwGameObject("Chassis"+mName);
    mChassisMesh = new gwStaticMesh("Chassis_Rond"+mName, "Data/Meshs/Chassis_Rond.obj");

    mChassisMaterial = new gwMaterial;
    mChassisRenderer = new gwMeshRenderer(mChassisMesh);
    mChassisMesh->load();
    mChassisMaterial->setColor(glm::vec3(200,200,200));
    mChassisObject->addComponent(mChassisRenderer);
    mChassisObject->addComponent(mChassisMaterial);
    scene->addGameObject(mChassisObject);
}

void simu::Robot_1::Setup_CNY_70(gwScene* scene){
    float xOffset = -6;
    float yOffset = -10;
    float espace = 2.6871;
    glm::vec3 points[4] = { glm::vec3(xOffset, -0.15 , yOffset),       glm::vec3(xOffset+espace, -0.15, yOffset),
                            glm::vec3(xOffset, -0.15, yOffset+espace), glm::vec3(xOffset+espace, -0.15, yOffset+espace)};

    for(unsigned i(0); i < 2; i++){
        for(short j(0); j < 4; j++){
            glm::vec3 point = points[j];
            if(i == 1){ point.x *= -1; }
            mLineSensor[i].startPoint[j] = point;
            mLineSensor[i].cny_70[j] = new simu::CNY_70(mPhysics, scene, mChassisObject);
            mLineSensor[i].cny_70[j]->getGameObject()->getTransform()->translate(point);
        }
    }

    mBallDetector = new simu::CNY_70(mPhysics, scene, mChassisObject, 5);
    mBallDetector->getGameObject()->getTransform()->translate(glm::vec3(4.8,-2,-15));
    mBallDetector->getGameObject()->getTransform()->rotate(-90, glm::vec3(0,0,1));
}

void simu::Robot_1::Setup_Sharp_Sensors(gwScene* scene){
    for(int i(0); i< 5; i++)
        mSharp[i] = new simu::SHARP_GP2Y(mPhysics, scene, mChassisObject);

    mSharp[SHARP_LEFT]->getGameObject()->getTransform()->translate(glm::vec3(-10.5, 1, 0), GW_WORLD_SPACE);
    mSharp[SHARP_LEFT]->getGameObject()->getTransform()->rotate(90, glm::vec3(0 ,1, 0), GW_LOCAL_SPACE);

    mSharp[SHARP_FRONT_LEFT]->getGameObject()->getTransform()->translate(glm::vec3(-7.42 , 1, -7.42), GW_WORLD_SPACE);
    mSharp[SHARP_FRONT_LEFT]->getGameObject()->getTransform()->rotate(45, glm::vec3(0 ,1, 0), GW_LOCAL_SPACE);

    mSharp[SHARP_FRONT]->getGameObject()->getTransform()->translate(glm::vec3(0, 1, -10.5), GW_WORLD_SPACE);

    mSharp[SHARP_FRONT_RIGHT]->getGameObject()->getTransform()->translate(glm::vec3(7.42, 1, -7.42), GW_WORLD_SPACE);
    mSharp[SHARP_FRONT_RIGHT]->getGameObject()->getTransform()->rotate(-45, glm::vec3(0 ,1, 0), GW_LOCAL_SPACE);

    mSharp[SHARP_RIGHT]->getGameObject()->getTransform()->translate(glm::vec3(10.5, 1, 0), GW_WORLD_SPACE);
    mSharp[SHARP_RIGHT]->getGameObject()->getTransform()->rotate(-90, glm::vec3(0 ,1, 0), GW_LOCAL_SPACE);
}

void simu::Robot_1::RenderUI(){
    mPixy->updateTexture();
    mUI->Render(mShader);
}

void simu::Robot_1::Mode_Manuel(void){
    float ang_speed = 2.0f;
    float mot_speed = 10.0f;
    gwe::singleton::gwInputManager *mInput = gwe::singleton::gwInputManager::getInstance();

    if(mInput->getMouseButton(1)){ mot_speed *= 4; }
    if(mInput->getMouseButton(3)){ mot_speed /= 4; }

    float x = 0;
    float y = 0;
    float pelleAngVelocity = 10;

    if(mInput->getKey(mBindings.manual_MoveForwardKey)){ y++; }
    if(mInput->getKey(mBindings.manual_MoveBackwardKey)){ y--; }
    if(mInput->getKey(mBindings.manual_MoveLeftKey)){ x++; }
    if(mInput->getKey(mBindings.manual_MoveRightKey)){ x--; }
    if(mInput->getKey(mBindings.manual_OpenShovelKey)){ pelleAngVelocity = -10.0f; }

    float left = 0;
    float right = 0;

    if(x == 0 && y == 1){ /** AVANCE*/ left = 1; right = 1; }
    else if(x == 1 && y == 1){ /** AV_DROITE*/ left = 1; right = 0; }
    else if(x == 1 && y == 0){ /** DROITE*/ left = 1; right = -1;}
    else if(x == 1 && y == -1){ /** ARRIERE_DROITE*/ left = -1; right = 0;}
    else if(x == 0 && y == -1){ /** ARRIERE*/ left = -1; right = -1;}
    else if(x == -1 && y == -1){ /** ARRIERE_GAUCHE left*/ left = 0; right = -1;}
    else if(x == -1 && y == 0){ /** GAUCHE*/ left = -1; right = 1; }
    else if(x == -1 && y == 1){ /** AV_GAUCHE*/ left = 0; right = 1;}
    else if(x == 0 && y == 0){ /** ARRET*/ left = 0; right = 0; }

      
    mWheelHinge[0]->setTargetVelocity(3,left*mot_speed);
    mWheelHinge[1]->setTargetVelocity(3,right*mot_speed);



    mPelleRotationonstraint->setMotorTargetVelocity(pelleAngVelocity);
        if(mInput->getKey(mBindings.manual_ShootKey)){
            mShooterConstraint->setTargetLinMotorVelocity(20000);
        }else{
            mShooterConstraint->setTargetLinMotorVelocity(-20000);
        }

     if(mLineSensor[0].bigWhiteLine | mLineSensor[1].bigWhiteLine){
        mChassisMaterial->setColor(glm::vec3(200,100,100));
    }else{
        mChassisMaterial->setColor(glm::vec3(200,200,200));
    }

}

void simu::Robot_1::Comportement_2(void){
    /** Avancer sur les balles et tirer**/
    /** Si contact avec CNY, S'orienter vers le fond du cours et y aller, une fois arrivé recommencer à scruter**/
    float ang_speed = 2.0f;     float mot_speed = 10.0f;    float left_speed = 0;       float right_speed = 0;
    bool ouvrir_pelle = true;   bool mShooting = false;

    /** [Définition de l'axe sur lequel tirer] **/
    glm::vec3 shootingAxis;
    {
        if(mChassisObject->getTransform()->getWorldPos().x < 0){ shootingAxis = glm::vec3(-1,0,0); }
        else{ shootingAxis = glm::vec3(1,0,0); }
    }

    Uint32 time = SDL_GetTicks();
    elapsedTime += time-lastTime;
    mMillis += time-lastTime;
    lastTime = time;

    if(mMillis > 90000){
        mChassisMaterial->setColor(glm::vec3(50,200,50));
        mWheelHinge[ROUE_GAUCHE]->setTargetVelocity(3,0);
        mWheelHinge[ROUE_DROITE]->setTargetVelocity(3,0);
        return;
    }

    mChassisMaterial->setColor(glm::vec3(100,100,200));

    if((mLineSensor[0].bigWhiteLine | mLineSensor[1].bigWhiteLine) && ETAT_ROBOT != ROBOT_CNY_ERROR){
        SAVE_ETAT_ROBOT = ETAT_ROBOT;
        ETAT_ROBOT = ROBOT_CNY_ERROR;
        elapsedTime = 0;
    }else if(ETAT_ROBOT != ROBOT_CNY_ERROR && mBallDetector->detectsWhite()){
        ETAT_ROBOT = ROBOT_BALL_CATCHED;
    }else if(ETAT_ROBOT != ROBOT_CNY_ERROR){
        ETAT_ROBOT = ROBOT_NORMAL;
    }

    if(ETAT_ROBOT == ROBOT_NORMAL){
        if(elapsedTime > 250){
            elapsedTime = 0;
             /** Si il n'y a pas de balle dans la pelle :
            On tente de suivre une balle : **/
            std::vector<simu::Pixy_Block> blocks = mPixy->getBlocks();

            /** Si on détecte un block **/
            if(blocks.size() > 0){
                biggerBlock = blocks[0]; /** On essaie de suivre le block le plus proche **/

                for(int i(1); i < blocks.size(); i++){
                    int biggerY = biggerBlock.yCenter-(biggerBlock.height/2);
                    int blockY = blocks[i].yCenter-(blocks[i].height/2);
                   // std::cout << blockY << std::endl;

                   /** Si le bloc est plus bas **/
                    if(blockY < biggerY){
                        biggerBlock = blocks[i];
                    }else if(blockY == biggerY){
                        if(blocks[i].getArea() > biggerBlock.getArea())
                            biggerBlock = blocks[i];
                        else if(blocks[i].getArea() == biggerBlock.getArea()){
                            if(blocks[i].xCenter < biggerBlock.xCenter){
                                biggerBlock = blocks[i];
                            }
                        }
                    }
                }
            }
        }
         std::cout << "Block : " << biggerBlock.xCenter << " " << biggerBlock.yCenter << std::endl;
        /** si le block n'est pas centré, on le centre*/
        int xCenter = biggerBlock.xCenter;
        int yCenter = biggerBlock.yCenter;


        if(xCenter < PIXY_CENTER_X-2){ // on tourne a gauche
            left_speed = 3;
            right_speed = 8;
        }else if(xCenter > PIXY_CENTER_X+2) { // on tourne à droite
            left_speed = 8;
            right_speed = 3;
        }else{
            left_speed = 20;
            right_speed = 20;
        }
    }else if(ETAT_ROBOT == ROBOT_BALL_CATCHED){
        ouvrir_pelle = false; /** Si il y a une balle dans la pelle, on ferme la pelle*/
        float angularPos = glm::orientedAngle(shootingAxis, mChassisObject->getTransform()->getWorldForward(), glm::vec3(0,1,0)); /** Si on est pas aligné vers le camp adverse, on s'aligne*/
        std::cout << angularPos << std::endl;

        if(angularPos > 5){
            left_speed = 8;
            right_speed = -8;
        }else if(angularPos < -5){
            left_speed = -8;
            right_speed = 8;
        }else{
            ouvrir_pelle = true;  /** Phase de tir */
            if(mSharp[SHARP_FRONT]->getResultDistance() <= 3){ /** Si le capteur avant détecte alors la pelle est ouverte*/
                mShooting = true;  /** On tire **/
            }
        }
    }else if(ETAT_ROBOT == ROBOT_CNY_ERROR){
        mChassisMaterial->setColor(glm::vec3(200,100,100));
        if(elapsedTime < 10000){
            float angle = glm::orientedAngle(mChassisObject->getTransform()->getWorldForward(),-shootingAxis,glm::vec3(0,1,0));
            std::cout <<angle << std::endl;
            if(angle > 5 || angle < -5){
                left_speed = -8;
                right_speed = 8;
            }else
                elapsedTime = 10000;

        }/*else if(elapsedTime < 6000){
            left_speed = -4;
            right_speed = 4;
        }*/else{
            ETAT_ROBOT = SAVE_ETAT_ROBOT;
        }
    }

    mWheelHinge[ROUE_GAUCHE]->setTargetVelocity(3,left_speed);
    mWheelHinge[ROUE_DROITE]->setTargetVelocity(3,right_speed);
    /** On teste les CNY **/

    if(mShooting){
        mShooterConstraint->setTargetLinMotorVelocity(20000);
        ETAT_ROBOT = ROBOT_CNY_ERROR;
    }else
        mShooterConstraint->setTargetLinMotorVelocity(-20000);

    if(ouvrir_pelle)
        mPelleRotationonstraint->setMotorTargetVelocity(-10);
    else
        mPelleRotationonstraint->setMotorTargetVelocity(10);

}

void simu::Robot_1::Comportement_1(void){
    float ang_speed = 2.0f;
    float mot_speed = 10.0f;
    float left_speed = 0;
    float right_speed = 0;
    bool ouvrir_pelle = true;
    bool mShooting = false;
    float counterSide = 1;

    /** [Définition de l'axe sur lequel tirer] **/
    glm::vec3 shootingAxis;
    {
        if(mChassisObject->getTransform()->getWorldPos().x < 0){
            shootingAxis = glm::vec3(-1,0,0);
            counterSide = -1;
        }
        else{
            shootingAxis = glm::vec3(1,0,0);
            counterSide = 1;
        }
    }

    Uint32 time = SDL_GetTicks();
    elapsedTime += time-lastTime;
    mMillis += time-lastTime;
    lastTime = time;
    if(mMillis > 90000){
        mChassisMaterial->setColor(glm::vec3(50,200,50));
        mWheelHinge[ROUE_GAUCHE]->setTargetVelocity(3,0);
        mWheelHinge[ROUE_DROITE]->setTargetVelocity(3,0);
        return;
    }

    mChassisMaterial->setColor(glm::vec3(200,200,200));

    if((mLineSensor[0].bigWhiteLine | mLineSensor[1].bigWhiteLine) && ETAT_ROBOT != ROBOT_CNY_ERROR){
        SAVE_ETAT_ROBOT = ETAT_ROBOT;
        ETAT_ROBOT = ROBOT_CNY_ERROR;
        elapsedTime = 0;
    }else if(ETAT_ROBOT != ROBOT_CNY_ERROR && mBallDetector->detectsWhite()){
        ETAT_ROBOT = ROBOT_BALL_CATCHED;
    }else if(ETAT_ROBOT != ROBOT_CNY_ERROR){
        ETAT_ROBOT = ROBOT_NORMAL;
    }

    if(ETAT_ROBOT == ROBOT_NORMAL){
        if(elapsedTime > 100){
            elapsedTime = 0;
             /** Si il n'y a pas de balle dans la pelle :
            On tente de suivre une balle : **/
            std::vector<simu::Pixy_Block> blocks = mPixy->getBlocks();

            /** Si on détecte un block **/
            if(blocks.size() > 0){
                biggerBlock = blocks[0]; /** On essaie de suivre le block le plus proche **/

                for(int i(1); i < blocks.size(); i++){
                    int biggerY = biggerBlock.yCenter-(biggerBlock.height/2);
                    int blockY = blocks[i].yCenter-(blocks[i].height/2);
                   // std::cout << blockY << std::endl;

                   /** Si le bloc est plus bas **/
                    if(blockY < biggerY){
                        biggerBlock = blocks[i];
                    }else if(blockY == biggerY){
                        if(blocks[i].getArea() > biggerBlock.getArea())
                            biggerBlock = blocks[i];
                        else if(blocks[i].getArea() == biggerBlock.getArea()){
                            if(blocks[i].xCenter < biggerBlock.xCenter){
                                biggerBlock = blocks[i];
                            }
                        }
                    }
                }
            }
        }
         std::cout << "Block : " << biggerBlock.xCenter << " " << biggerBlock.yCenter << std::endl;
        /** si le block n'est pas centré, on le centre*/
        int xCenter = biggerBlock.xCenter;
        int yCenter = biggerBlock.yCenter;


        if(xCenter < PIXY_CENTER_X-2){ // on tourne a gauche
            left_speed = 3;
            right_speed = 8;
        }else if(xCenter > PIXY_CENTER_X+2) { // on tourne à droite
            left_speed = 8;
            right_speed = 3;
        }else{
            left_speed = 20;
            right_speed = 20;
        }
    }else if(ETAT_ROBOT == ROBOT_BALL_CATCHED){
        ouvrir_pelle = false; /** Si il y a une balle dans la pelle, on ferme la pelle*/
        float angularPos = glm::orientedAngle(shootingAxis, mChassisObject->getTransform()->getWorldForward(), glm::vec3(0,1,0)); /** Si on est pas aligné vers le camp adverse, on s'aligne*/
        if(angularPos > 5){
            left_speed = 6;
            right_speed = -6;
        }else if(angularPos < -5){
            left_speed = -6;
            right_speed = 6;
        }else{
            ouvrir_pelle = true;  /** Phase de tir */
            if(mSharp[SHARP_FRONT]->getResultDistance() <= 3){ /** Si le capteur avant détecte alors la pelle est ouverte*/
                mShooting = true;  /** On tire **/
            }
        }
    }else if(ETAT_ROBOT == ROBOT_CNY_ERROR){
        mChassisMaterial->setColor(glm::vec3(200,100,100));
        if(elapsedTime < 3000){
            left_speed = -mot_speed;
            right_speed = -mot_speed;
        }/*else if(elapsedTime < 6000){
            left_speed = -4;
            right_speed = 4;
        }*/else{
            ETAT_ROBOT = SAVE_ETAT_ROBOT;
        }
    }

    mWheelHinge[ROUE_GAUCHE]->setTargetVelocity(3,left_speed);
    mWheelHinge[ROUE_DROITE]->setTargetVelocity(3,right_speed);
    /** On teste les CNY **/

    if(mShooting){
        mShooterConstraint->setTargetLinMotorVelocity(20000);
        ETAT_ROBOT = ROBOT_CNY_ERROR;
    }else
        mShooterConstraint->setTargetLinMotorVelocity(-20000);

    if(ouvrir_pelle)
        mPelleRotationonstraint->setMotorTargetVelocity(-10);
    else
        mPelleRotationonstraint->setMotorTargetVelocity(10);

}

void simu::Robot_1::SetPos_Rot(const glm::vec3 &pos, float rot){
    mChassisObject->getTransform()->setTransform(glm::mat4(1.0));
    mChassisObject->getTransform()->translate(pos, GW_WORLD_SPACE);
    mChassisObject->getTransform()->rotate(rot, glm::vec3(0,1,0));
    btTransform transform;
    transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
    mCollider.transform = transform;
}

void simu::Robot_1::Setup_Collider(void){

    btTransform tmpTransform;
    tmpTransform.setIdentity();

    btCylinderShape* cylinder = new btCylinderShape(btVector3(12,0.15, 12));
    btBoxShape* boxShape = new btBoxShape(btVector3(12,2.633995,2.5));
    btCylinderShape* wheelCylinder = new btCylinderShape(btVector3(0.8,-6.58945/2, 0.8));

    btCompoundShape* finalShape = new btCompoundShape;
    finalShape->addChildShape(tmpTransform, cylinder);

    tmpTransform.setOrigin(btVector3(0,-2.9405,0));
    finalShape->addChildShape(tmpTransform, boxShape);

    tmpTransform.setOrigin(btVector3(0, -6.58945/2, -9.99854));
    finalShape->addChildShape(tmpTransform, wheelCylinder);

    tmpTransform.setOrigin(btVector3(0, -6.58945/2, 9.99854));
    finalShape->addChildShape(tmpTransform, wheelCylinder);

    mCollider.collisionShape = finalShape;

    mCollider.transform.setIdentity();
    mCollider.transform.setOrigin(btVector3(0,0,0));

    mCollider.mass = 10.0;

    btVector3 localInertia(0,0,0);
    mCollider.collisionShape->calculateLocalInertia(mCollider.mass,localInertia);
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    mCollider.motionState = new btDefaultMotionState(mCollider.transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mCollider.mass,mCollider.motionState,mCollider.collisionShape,localInertia);
    mCollider.rigidBody = new btRigidBody(rbInfo);
    mCollider.rigidBody->setUserIndex(USER_INDEX_ROBOT);
    mCollider.rigidBody->setFriction(1000);
   // mCollider.rigidBody->setCollisionFlags(mCollider.rigidBody->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    mCollider.rigidBody->setActivationState(DISABLE_DEACTIVATION);
    //add the body to the dynamics world
    mPhysics->getWorld()->addRigidBody(mCollider.rigidBody);
}
