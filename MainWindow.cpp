#include "MainWindow.h"
#include "gwGameManager.h"
#include "gwResourceManager.h"
#include "gwScene.h"
#include "gwGameObject.h"
#include "gwTexture.h"
#include "gwShader.h"
#include "gwBoxMesh.h"
#include "gwSquareMesh.h"
#include "gwCamera.h"
#include "gwMaterial.h"
#include "gwMeshRenderer.h"
#include "gwTransform.h"
#include "gwUtils.h"
#include "gwSkyboxMesh.h"
#include "gwSkybox.h"
#include "gwLight.h"
#include "gwInputManager.h"
#include "gwStaticMesh.h"

#include "Terrain_Tennis.h"
#include "Balle_Tennis.h"
#include "Robot_1.h"
#include "Physics.h"

#define NB_BALLES_TENNIS 20

using namespace gwe;

btDiscreteDynamicsWorld* mWorld;
gwCamera *Cam;
gwSceneRenderer* Renderer;
Physics *physics;

simu::Balle_Tennis *balles[NB_BALLES_TENNIS];
btVector3 ballesStartPos[NB_BALLES_TENNIS] = {
    btVector3(-10, 40, -10),
    btVector3(-20, 40, -20),
    btVector3(-30, 40, -30),
    btVector3(-40, 40, -40),
    btVector3(-50, 40, -50),
    btVector3(-60, 40, -60),
    btVector3(-70, 40, -70),
    btVector3(-80, 40, -80),
    btVector3(-90, 40, -90),
    btVector3(-100, 40, -100),

    btVector3(10, 40, -10),
    btVector3(20, 40, -20),
    btVector3(30, 40, -30),
    btVector3(40, 40, -40),
    btVector3(50, 40, -50),
    btVector3(60, 40, -60),
    btVector3(70, 40, -70),
    btVector3(80, 40, -80),
    btVector3(90, 40, -90),
    btVector3(100, 40, -100)
};

simu::Robot_1 *robot;
simu::Robot_1 *robot2;
bool playPhysics = false;


MainWindow::MainWindow(const std::string &title, const int &width, const int &height) : Window(title, width, height)
{

}
MainWindow::~MainWindow()
{
   // delete mDebugDrawer;
    //SceneManager::deleteInstance();
}

bool MainWindow::init(void)
{
    if(!Window::init())
        return false;

    glViewport(0,0,mWidth,mHeight);

    gwe::singleton::gwGameManager::getInstance();
    gwe::gwScene* scene1 = new gwe::gwScene("Scene_Test_1");
    gwe::singleton::gwGameManager::getInstance()->enableScene("Scene_Test_1");
    gwe::singleton::gwGameManager::getInstance()->setRenderSize(gwSize(mWidth, mHeight));


    gwe::gwGameObject* object2 = new gwe::gwGameObject("Object_2");
    gwe::gwGameObject* cameraObject = new gwe::gwGameObject("Object_3");

    scene1->addGameObject(object2);
    scene1->addGameObject(cameraObject);

    physics = new Physics;
    physics->Setup();
    //gwe::singleton::gwResourceManager::getInstance();
    gwe::gwBoxMesh* boxMesh = new gwe::gwBoxMesh("Box1",20.0,1.0,20.0 );    boxMesh->load();
    gwe::gwSquareMesh* squareMesh = new gwe::gwSquareMesh("Square1",1.0,1.0);    squareMesh->load();
    gwe::gwShader* shader = new gwe::gwShader("Shader1","Data/Shaders/Default/3D/Basic_3D.vert", "Data/Shaders/Default/3D/Basic_3D.frag");  shader->load();

    gwCamera* camera = new gwCamera();
    gwMaterial* material = new gwMaterial();

    cameraObject->addComponent(camera);

    cameraObject->getTransform()->translate(glm::vec3(2,2,-2));
    cameraObject->getTransform()->rotate(-45, glm::vec3(0,1,0));
    cameraObject->getTransform()->rotate(45, glm::vec3(1,0,0));

    gwShader* skyShader = new gwShader("skyShader","Data/Shaders/Default/Sky/Shader_Skybox.vert","Data/Shaders/Default/Sky/Shader_Skybox.frag");
    skyShader->load();

    gwSkyboxMesh* skyMesh = new gwSkyboxMesh("skyMesh");
    skyMesh->load();
    gwSkybox* skybox2 = new gwSkybox("Skybox_2",skyShader, "Data/Skyboxes/emerald/Emerald.gws");
    skybox2->load();

    simu::Terrain_Tennis *terrain = new simu::Terrain_Tennis(physics);
    terrain->setup(scene1, shader);

    for(int i(0); i < NB_BALLES_TENNIS; i++){
        balles[i] = new simu::Balle_Tennis(physics);
        balles[i]->setup(scene1, shader, ballesStartPos[i]);
    }

    gwScreenRenderer *renderer1 = new gwScreenRenderer(scene1);
    renderer1->setCamera(camera);
    renderer1->setShader(shader);


   /* gwCamera* camera2 = new gwCamera();
    gwOffScreenRenderer *renderer2 = new gwOffScreenRenderer("Renderer1", 500, 500, scene1);
    renderer2->setCamera(camera2);
    renderer2->setShader(shader);*/

    simu::Robot_1::KeyBindings bindingsRobot1;
    bindingsRobot1.autoMode1 = SDL_SCANCODE_KP_1;
    bindingsRobot1.autoMode2 = SDL_SCANCODE_KP_2;
    bindingsRobot1.manualMode = SDL_SCANCODE_KP_0;
    bindingsRobot1.manual_MoveForwardKey = SDL_SCANCODE_I;
    bindingsRobot1.manual_MoveBackwardKey = SDL_SCANCODE_K;
    bindingsRobot1.manual_MoveLeftKey = SDL_SCANCODE_J;
    bindingsRobot1.manual_MoveRightKey = SDL_SCANCODE_L;
    bindingsRobot1.manual_OpenShovelKey = SDL_SCANCODE_H;
    bindingsRobot1.manual_ShootKey = SDL_SCANCODE_U;

    robot = new simu::Robot_1(physics, btVector3(-120,7,0), "_1",1, bindingsRobot1, 0.0f);
    robot->Setup(scene1, shader);

    simu::Robot_1::KeyBindings bindingsRobot2;
    bindingsRobot2.autoMode1 = SDL_SCANCODE_KP_7;
    bindingsRobot2.autoMode2 = SDL_SCANCODE_KP_8;
    bindingsRobot2.manualMode = SDL_SCANCODE_KP_9;
    bindingsRobot2.manual_MoveForwardKey = SDL_SCANCODE_W;
    bindingsRobot2.manual_MoveBackwardKey = SDL_SCANCODE_S;
    bindingsRobot2.manual_MoveLeftKey = SDL_SCANCODE_A;
    bindingsRobot2.manual_MoveRightKey = SDL_SCANCODE_D;
    bindingsRobot2.manual_OpenShovelKey = SDL_SCANCODE_Q;
    bindingsRobot2.manual_ShootKey = SDL_SCANCODE_E;

    robot2 = new simu::Robot_1(physics, btVector3(120,0,0), "_2",2, bindingsRobot2, 0.7f);
    robot2->Setup(scene1, shader);
   // robot2->SetPos_Rot(glm::vec3(-100,0,0), 180);
   // scene1->getGameObject("PIXY_0")->addComponent(camera2);

   /* object2->addComponent(camera2);

    gwLight* light = new gwLight(glm::vec3(255,0,0), 1.01, GW_LIGHT_POINT);
    object2->addComponent(light);
    light->setAttenuation(GW_LINEAR_ATTENUATION, 0.30);
    light->setAttenuation(GW_EXP_ATTENUATION, 0.05);*/

    scene1->setAmbiantLight(glm::vec3(255,255,255),0.5);
    /*object2->getTransform()->translate(glm::vec3(0,10,0), GW_WORLD_SPACE);
    object2->getTransform()->rotate(90, glm::vec3(1,0,0), GW_WORLD_SPACE)*/;


    gwBoxMesh* boxLight = new gwBoxMesh("Box_Light", 1,1,1);
    boxLight->load();
    gwMeshRenderer* boxLightRenderer = new gwMeshRenderer(boxLight);
    //object2->addComponent(boxLightRenderer);
    gwMaterial* material2 = new gwMaterial();
    material2->setColor(glm::vec3(255,255,255));
    object2->addComponent(material2);
    object2->addComponent(new gwLight(glm::vec3(255, 255, 255), 0.4, GW_LIGHT_DIRECTIONAL));
    material->setReflectivity(0.7, 100);

   // renderer1->setSkybox(skybox2);

   // renderer2->setCamera(camera2);
    //renderer2->setShader(shader);

   // material2->setTexture(renderer2->getColorTexture());
   // material2->setLightSensitive(false);

    SDL_SetWindowSize(mSDL_Window, mWidth, mHeight);

    gwShader* normalsShader = new gwShader("NormalsShader", "Data/Shaders/Default/Wireframe/Shader_Normals.vert", "Data/Shaders/Default/Wireframe/Shader_Normals.frag", "Data/Shaders/Default/Wireframe/Shader_Normals.geom");
    normalsShader->load();

    mWorld = physics->getWorld();
    Cam = camera;
    Renderer = renderer1;
    /*mDebugDrawer = new GLDebugDrawer(camera->getView_p(), renderer1->getProjection());
    mDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    mWorld->setDebugDrawer(mDebugDrawer);*/


   /* gwScreenRenderer *normalsRenderer = new gwScreenRenderer(scene1);
    normalsRenderer->setCamera(camera);
    normalsRenderer->setShader(normalsShader);*/




   // delete shader;
    //scene1->addGameObject(object1);

  //  delete object1;
   /* delete object3;
    delete object2;
    delete object1;*/
    return true;
}


bool MainWindow::render(void)
{
    //SDL_Event event;
    uint32_t begTime = SDL_GetTicks();
    float rotSpeed = 0.1f;
    float linearSpeed = 100;

    gwTransform* transformm = gwe::singleton::gwGameManager::getInstance()->getScene("Scene_Test_1")->getGameObject("Object_3")->getTransform();
    gwTransform* transform2 = gwe::singleton::gwGameManager::getInstance()->getScene("Scene_Test_1")->getGameObject("Object_2")->getTransform();


    gwe::singleton::gwInputManager* input = gwe::singleton::gwInputManager::getInstance();
    input->updateEvent();
    input->lockCursor(true);
    input->showCursor(false);


    int X = 0, Y = 0;
    if(input->getKey(SDL_SCANCODE_LEFT))
        X--;
    if(input->getKey(SDL_SCANCODE_RIGHT))
        X++;
    if(input->getKey(SDL_SCANCODE_UP))
        Y--;
    if(input->getKey(SDL_SCANCODE_DOWN))
        Y++;

    transformm->rotate(-static_cast<float>(input->getXRel())*deltaTime*rotSpeed, glm::vec3(0,1,0), GW_WORLD_SPACE);
    transformm->rotate(static_cast<float>(input->getYRel())*deltaTime*rotSpeed, glm::vec3(1,0,0), GW_LOCAL_SPACE);

    if(input->quit())
    {
        //delete robot;
        gwe::singleton::gwGameManager::deleteInstance();
        gwe::singleton::gwInputManager::deleteInstance();
        return false;
    }

    if(input->getKey(SDL_SCANCODE_F5) && (!(SDL_GetWindowFlags(mSDL_Window) & SDL_WINDOW_FULLSCREEN)))
    {
        SDL_Rect rect;
        SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(mSDL_Window), &rect);
        SDL_SetWindowSize(mSDL_Window, rect.w, rect.h);
        SDL_SetWindowFullscreen(mSDL_Window, SDL_TRUE);
    }
    if(input->getKey(SDL_SCANCODE_ESCAPE) && (SDL_GetWindowFlags(mSDL_Window) & SDL_WINDOW_FULLSCREEN))
    {
        SDL_SetWindowFullscreen(mSDL_Window, SDL_FALSE);
        gwSize lastSize(gwe::singleton::gwInputManager::getInstance()->getPreviousWindowSize());
        SDL_SetWindowSize(mSDL_Window, lastSize.getW(), lastSize.getH());
    }

    if(input->sizeChanged())
    {
        gwe::singleton::gwGameManager::getInstance()->setRenderSize(input->getWindowSize());
    }

    if(input->getKey(SDL_SCANCODE_SPACE)){
        playPhysics = true;
    }



           /* if(event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                gwe::singleton::gwGameManager::getInstance()->setRenderSize(gwSize(event.window.data1, event.window.data1));

            }*/


       transformm->translate(glm::vec3(-X*linearSpeed*deltaTime, 0 , -Y*linearSpeed*deltaTime), GW_LOCAL_SPACE);
       transform2->translate(glm::vec3(0, -X*0.05 , 0 ), GW_WORLD_SPACE);


    /*else if(event.type == SDL_KEYDOWN)
    {
        if(event.)
    }*/

  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(playPhysics)
        mWorld->stepSimulation(btScalar(deltaTime), 10);

    gwe::singleton::gwGameManager::getInstance()->update();

    GLDebugDrawer* drawer = dynamic_cast<GLDebugDrawer*>(mWorld->getDebugDrawer());
    drawer->setMatrices(Cam->getView(), Renderer->getProjection());

     for(int i(0); i < NB_BALLES_TENNIS; i++){
        balles[i]->run();
    }

    robot->Run(deltaTime);
    robot2->Run(deltaTime);

    mWorld->debugDrawWorld();    //o/bject->

    gwe::singleton::gwGameManager::getInstance()->render(false);

    glClear(GL_DEPTH_BUFFER_BIT);
    robot->RenderUI();
    //robot2->RenderUI();


    SDL_GL_SwapWindow(mSDL_Window);


    bool useFramerate = false;
   // SDL_GL_SetSwapInterval(1);

        uint32_t finalTime = SDL_GetTicks();
        uint32_t timeElapsed = finalTime-begTime;
        uint32_t framerate = 30;
        uint32_t targetTime = (1000/framerate);

        if((timeElapsed < targetTime) && useFramerate)
        {
            SDL_Delay(targetTime-timeElapsed);
        }
        finalTime = SDL_GetTicks();
        timeElapsed = finalTime-begTime;
        deltaTime = (float) timeElapsed/1000.0f;
        referenceTime += timeElapsed;
        frameCount ++;
        if(referenceTime >= 1000)
        {
            std::cout << "FPS : " << frameCount << std::endl;
            referenceTime = 0;
            frameCount = 0;


    }
    return true;
}
