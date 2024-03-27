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
    MOVE_STEP_C,
    MOVE_236C,
    MOVE_2B,
    MOVE_4A,
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
    AIR_THROW_TECH_CHAR1,
    PROJECTILE_CHAR
    };

class Char1 : public Character
{
public:
    Char1(Application &application_, Vector2<float> pos_, Camera *cam_, ParticleManager *particleManager_);
    virtual ~Char1() {};

    void provideActions();

    void loadAnimations(Application &application_) final;
    void initiate() final;
    void proceedCurrentState() final;
    void land() final;
    bool canApplyDrag() const final;
    bool canBeDraggedByInertia() const final;
    HurtboxVec getHurtboxes() final;
    HIT_RESULT applyHit(HitEvent &hitEvent) final;
    void applyClash(const Hit &clashedHit_) final;
    void updateBlockState() final;
    bool isInHitstun() const final;
    bool isInBlockstun() const final;
    bool isInInstantBlockstun() const final;
    bool isInAttackState() const final;
    bool isKnockedDown() const final;
    bool canApplyGravity() const final;
    float touchedWall(int sideDir_) final;
    void turnVelocityToInertia(float horMultiplier_ = 0.9f) final;

    virtual ORIENTATION getInputDir() const final;

    void attemptThrow() final;

    std::string CharStateData() const final;

protected:
    virtual void jumpUsingAction() override;
    void switchToSoftLandingRecovery();
    virtual void enterKndRecovery() override;
    bool isInActiveFrames() const;
    void enterHitstunAnimation(const PostHitProperties &props_) final;

    void throwTeched(THROW_TECHS_LIST tech_) final;

    Collider getUntiedPushbox() const final;
    void enterThrown(THROW_LIST throw_);

    CHAR1_STATE lastState = CHAR1_STATE::IDLE;
    
};

#endif