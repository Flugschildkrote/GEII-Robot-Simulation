#ifndef ROBOT_1_H
#define ROBOT_1_H

#include "Physics.h"
#include "gwGlobalHeader.h"
#include "CNY_70.h"
#include "SHARP_GP2Y.h"
#include "SimulationUI.h"
#include "Pixy.h"
#include "glm/gtx/vector_angle.hpp"

#define ROUE_DROITE 0
#define ROUE_GAUCHE 1

/*struct simu::Line_Sensor{

};*/
enum ENUM_ETAT_ROBOT{
    ROBOT_NORMAL = 0,
    ROBOT_CNY_ERROR = 1,
    ROBOT_BALL_CATCHED = 2
};

namespace simu{

enum ROBOT_MODE{
    MODE_MANUAL = 0,
    MODE_AUTO_1 = 1
};

enum SHARP_POS{
    SHARP_LEFT = 0,
    SHARP_FRONT_LEFT = 1,
    SHARP_FRONT = 2,
    SHARP_FRONT_RIGHT = 3,
    SHARP_RIGHT = 4
};

struct Sensor_4{
    CNY_70 *cny_70[4];
    glm::vec3 startPoint[4];
    float length = 10.0f;
    bool bigWhiteLine = false;
};

struct Sharps{

};

struct Wheel{
    gwe::gwGameObject* gameObject;
    gwe::gwStaticMesh* mesh;
    gwe::gwTexture* texture;
    gwe::gwMaterial* material;
    gwe::gwMeshRenderer* renderer;
};

class Robot_1
{
public:

    struct KeyBindings
    {
        SDL_Scancode manualMode;
        SDL_Scancode autoMode1;
        SDL_Scancode autoMode2;
        SDL_Scancode manual_OpenShovelKey;
        SDL_Scancode manual_ShootKey;
        SDL_Scancode manual_MoveForwardKey;
        SDL_Scancode manual_MoveBackwardKey;
        SDL_Scancode manual_MoveLeftKey;
        SDL_Scancode manual_MoveRightKey;
    };

    Robot_1(Physics* physics, btVector3 pos, const std::string name, unsigned int comportement, const KeyBindings& bindings, float ui_offset = 0.0f);
    virtual ~Robot_1();
    void Setup(gwe::gwScene* scene, gwe::gwShader* shader);
    void Run(float deltaTime);
    void RenderUI(void);
    gwe::gwGameObject* getGameObject(void) const { return mChassisObject; }
    void SetPos_Rot(const glm::vec3 &pos, float rot);

private:
    void Mode_Manuel(void);
    void Comportement_1(void);
    void Comportement_2(void);

    void SetupGameObject(gwe::gwScene* scene);
    void Setup_CNY_70(gwe::gwScene* scene);
    void Setup_Sharp_Sensors(gwe::gwScene* scene);
    void Setup_Collider(void);

    void Sync_Pos_Whith_Collider(void);
    void Update_CNY_70(void);
    void Update_Prox_Sencors(void);
    void Create_Pelle(void);
    /** GameObject_Chassis **/
    gwe::gwGameObject* mChassisObject;
    gwe::gwStaticMesh* mChassisMesh;
    gwe::gwTexture* mChassisTexture;
    gwe::gwMaterial* mChassisMaterial;
    gwe::gwMeshRenderer* mChassisRenderer;


    void Create_Wheels(gwe::gwScene* scene);
    Physics* mPhysics;
    Collider mCollider;
    Collider mWheel[2];
    Wheel mWheelObject[2];
    btHinge2Constraint* mWheelHinge[2];
    btSliderConstraint* mShooterConstraint;
    btHingeConstraint* mPelleRotationonstraint;

    Sensor_4 mLineSensor[2];
    SHARP_GP2Y *mSharp[5];
    SimulationUI* mUI;
    gwe::gwShader* mShader;
    Pixy* mPixy;
    simu::CNY_70* mBallDetector;

    /** SENSORS **/
    ROBOT_MODE mMode;


    Uint32 elapsedTime = 0;
    Uint32 lastTime = 0;
    Uint32 interval = 1000;
    Uint32 mMillis = 0;
    simu::Pixy_Block biggerBlock;
    const KeyBindings mBindings;

    ENUM_ETAT_ROBOT ETAT_ROBOT = ROBOT_NORMAL;
    ENUM_ETAT_ROBOT SAVE_ETAT_ROBOT = ROBOT_NORMAL;

    btVector3 mStartPosition;
    std::string mName;
    unsigned int mComportementID;
    float mUIOffset;
};

}

#endif // ROBOT_1_H
