#ifndef CHAR_1_H_
#define CHAR_1_H_

#include "Character.h"
#include "ActionResolver.h"
#include "FrameTimer.h"

class Char1;

enum class CHAR1_STATE {
    IDLE = 0,
    CROUCH,
    HITSTUN,
    HITSTUN_AIR,
    WALK_FWD,
    WALK_BWD,
    PREJUMP,
    JUMP,
    FLOAT,
    SOFT_LANDING_RECOVERY,
    VULNERABLE_LANDING_RECOVERY,
    HARD_LANDING_RECOVERY,
    GROUND_DASH,
    GROUND_DASH_RECOVERY,
    GROUND_BACKDASH,
    AIR_DASH,
    AIR_DASH_EXTENTION,
    AIR_BACKDASH,
    STEP,
    STEP_RECOVERY,
    MOVE_A,
    MOVE_B,
    MOVE_C,
    MOVE_236C,
    MOVE_2B,
    MOVE_JA,
    MOVE_JC,
    MOVE_JC_LANDING_RECOVERY,
    MOVE_214C,
    BLOCKSTUN_STANDING,
    BLOCKSTUN_CROUCHING,
    BLOCKSTUN_AIR,
    HARD_KNOCKDOWN,
    SOFT_KNOCKDOWN,
    KNOCKDOWN_RECOVERY,
    THROW_NORMAL_STARTUP,
    THROW_NORMAL_HOLD,
    THROW_NORMAL_WHIFF,
    THROW_NORMAL_ANIM,
    THROW_BACK_STARTUP,
    THROW_BACK_HOLD,
    THROWN_CHAR1_NORMAL_HOLD,
    THROWN_CHAR1_NORMAL_ANIM,
    THROW_NORMAL_AIR_STARTUP,
    THROW_NORMAL_AIR_HOLD,
    THROW_NORMAL_AIR_WHIFF,
    THROW_NORMAL_AIR_ANIM,
    THROW_BACK_AIR_STARTUP,
    THROW_BACK_AIR_HOLD,
    THROWN_CHAR1_NORMAL_AIR_HOLD,
    THROWN_CHAR1_NORMAL_AIR_ANIM,
    THROW_TECH_OWN,
    THROW_TECH_CHAR1,
    AIR_THROW_TECH_OWN,
    AIR_THROW_TECH_CHAR1
    };

struct Char1Data : public CharData<CHAR1_STATE>
{
    bool usedDoubleJump;
    bool canDoubleJumpAfterPrejump;
    bool canAirdashAfterPrejump;
    bool usedAirDash;
    bool inHitstop;
    bool inBlockstun;
    BLOCK_FRAME blockFrame;
    std::set<int> *cancelOptions = nullptr;
};

class ActionResolver_Char1 : public ActionResolver<CHAR1_STATE, Char1Data, Char1>
{
public:
    ActionResolver_Char1(InputSystem *input_);

    void createActions() final;
};

class Char1 : public Character
{
public:
    Char1(Application &application_, Vector2<float> pos_, Camera *cam_, ParticleManager *particleManager_);
    virtual ~Char1() {};

    void loadAnimations(Application &application_) final;
    void initiate() final;
    void proceedCurrentState() final;
    void updateState() final;
    void land() final;
    bool canApplyDrag() const final;
    bool canBeDraggedByInertia() const final;
    HitsVec getHits(bool allHits_ = false) final;
    HurtboxVec getHurtboxes() final;
    HIT_RESULT applyHit(HitEvent &hitEvent) final;
    void updateBlockState() final;
    bool isInHitstun() const final;
    bool isInBlockstun() const final;
    bool isKnockedDown() const final;
    bool canApplyGravity() const final;
    void touchedWall(int sideDir_) final;

    void attemptThrow() final;

    std::string CharStateData() const final;

protected:
    void switchToIdle();
    void jumpUsingAction();
    void switchToSoftLandingRecovery();
    void enterKndRecovery();
    void switchToFloat();
    Char1Data generateCharData();
    bool isInActiveFrames() const;
    void enterHitstunAnimation(const PostHitProperties &props_) final;

    void throwTeched(THROW_TECHS_LIST tech_) final;

    Collider getUntiedPushbox() const final;
    void enterThrown(THROW_LIST throw_);

    std::set<int> m_appliedHits;

    const Action<CHAR1_STATE, Char1Data, Char1> *m_currentAction;
    const Action<CHAR1_STATE, Char1Data, Char1> *m_reservedAction = nullptr;
    ActionResolver_Char1 m_actionResolver;

    CHAR1_STATE m_currentState = CHAR1_STATE::IDLE;
    FrameTimer m_timer;
    bool m_usedDoubleJump = false;
    bool m_usedAirDash = false;
    bool m_usedAirAttack = false;

    Collider m_standingHurtbox;
    Collider m_crouchingHurtbox;
    Collider m_airHitstunHurtbox;

    int framesInState = 1;
    CHAR1_STATE lastState = CHAR1_STATE::IDLE;

    CancelWindow m_currentCancelWindow;
    FrameTimer m_cancelTimer;

    int m_jumpFramesCounter = 0;
    int m_airadashFramesCounter = 0;
    

    friend Action<CHAR1_STATE, Char1Data, Char1>;
    friend Action_throw_startup<CHAR1_STATE, Char1Data, Char1>;
    friend Action_throw_hold<CHAR1_STATE, Char1Data, Char1>;
    friend Action_thrown_hold<CHAR1_STATE, Char1Data, Char1>;
    friend Action_throw_whiff<CHAR1_STATE, Char1Data, Char1>;
    friend Action_throw_tech<CHAR1_STATE, Char1Data, Char1>;
    friend Action_locked_animation<CHAR1_STATE, Char1Data, Char1>;
    friend Action_char1_normal_throw;
    friend Action_char1_normal_air_throw;
    friend Action_char1_thrown_char1_normal;
    friend Action_char1_thrown_char1_normal_air;
    friend Action_char1_throw_tech;
    friend Action_char1_throw_tech_char1;
    friend Action_char1_air_throw_tech;
    friend Action_char1_air_throw_tech_char1;
    friend Action_jump<CHAR1_STATE, Char1Data, Char1>;
    friend Action_prolonged<CHAR1_STATE, Char1Data, Char1>;
    friend Action_attack<CHAR1_STATE, Char1Data, Char1>;
    friend Action_char1_ground_attack;
    friend Action_char1_crouch;
    friend Action_char1_idle;
    friend Action_char1_walk_fwd;
    friend Action_char1_walk_bwd;
    friend Action_char1_airjump;
    friend Action_char1_jump;
    friend Action_char1_ground_backdash;
    friend Action_char1_ground_dash;
    friend Action_char1_ground_dash_recovery;
    friend Action_char1_step;
    friend Action_char1_step_recovery;
    friend Action_char1_soft_landing_recovery;
    friend Action_char1_jc_landing_recovery;
    friend Action_char1_soft_knockdown;
    friend Action_char1_hard_knockdown;
    friend Action_char1_knockdown_recovery;
    friend Action_char1_air_attack;
    friend Action_char1_air_dash;
    friend Action_char1_air_dash_extention;
    friend Action_char1_air_backdash;
    friend Action_char1_float;
    friend Action_char1_move_JC;
    friend Action_char1_vulnerable_landing_recovery;
    friend Action_char1_hard_landing_recovery;
    friend Action_char1_move_214C;
};

#endif