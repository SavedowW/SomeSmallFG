#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "InputComparators.h"
#include "Hit.h"
#include "AnimationManager.h"
#include "TimelineProperty.h"
#include "StateMarker.h"
#include "ProjectileRecipe.h"

class InteractableStateMachine;

enum class THROW_LIST {
    CHAR1_NORMAL_THROW,
    CHAR1_AIR_THROW
};

enum class THROW_TECHS_LIST {
    CHAR1_GROUND,
    CHAR1_AIR,
    NONE
};

/* ============================
 *
 *       ABSTRACT ACTION
 *  All actions should be inherited from
 *  this class.
 *
 *========================== */

class Action
{
public:
    Action(int actionState_, HurtboxFramesVec &&hurtboxes_, int anim_, StateMarker transitionableFrom_, bool isAttack_, bool isAirborne_);
    virtual ~Action() = default;

    virtual void switchTo(InteractableStateMachine &character_);
    virtual void outdated(InteractableStateMachine &character_);
    virtual void update(InteractableStateMachine &character_);
    virtual bool onLand(InteractableStateMachine &character_);
    virtual int isPossible(const InputQueue &inputQueue_, InteractableStateMachine *char_, int extendBuffer_) const;

    virtual bool isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const;
    virtual int responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const;

    virtual const HurtboxVec getCurrentHurtboxes(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;

    Action *setSwitchData(bool realign_, int timerValue_, bool velToInertia_, bool callForPriority_,
    Vector2<float> mulOwnVel_, Vector2<float> mulOwnInr_, Vector2<float> mulOwnDirVel_, Vector2<float> mulOwnDirInr_, Vector2<float> rawAddVel_, Vector2<float> rawAddInr_);
    Action *setAnimResetData(int animResetFrame_, int animResetDirection_);

    Action *setUpdateMovementData(TimelineProperty<Vector2<float>> &&mulOwnVelUpd_, TimelineProperty<Vector2<float>> &&mulOwnInrUpd_, TimelineProperty<Vector2<float>> &&mulOwnDirVelUpd_,
    TimelineProperty<Vector2<float>> &&mulOwnDirInrUpd_, TimelineProperty<Vector2<float>> &&rawAddVelUpd_, TimelineProperty<Vector2<float>> &&rawAddInrUpd_);
    Action *setUpdateSpeedLimitData(TimelineProperty<float> &&ownVelLimitUpd_, TimelineProperty<float> &&ownInrLimitUpd_);
    Action *setUpdateCamShakeData(TimelineProperty<Vector2<int>> &&camShakeUpd_);
    Action *setUpdateRealignData(TimelineProperty<bool> &&updRealign_);

    Action *setLandingMovementData(Vector2<float> mulOwnVel_, Vector2<float> mulOwnInr_, Vector2<float> mulOwnDirVel_, Vector2<float> mulOwnDirInr_, Vector2<float> rawAddVel_, Vector2<float> rawAddInr_);
    Action *setLandingRecoveryState(int m_recoveryState_);

    Action *setOutdatedFlags(bool setRealign_, bool setAirborne_, bool setAboveGroundOtd_);
    Action *setOutdatedTransition(int targetState_);
    Action *setOutdatedMovementData(Vector2<float> mulOwnVel_, Vector2<float> mulOwnInr_, Vector2<float> mulOwnDirVel_, Vector2<float> mulOwnDirInr_, Vector2<float> rawAddVel_, Vector2<float> rawAddInr_);

    Action *setIsPossibleProjectileCheck(int ownTypeToNotExist_);


    const int actionState;
    const HurtboxFramesVec m_hurtboxes;
    const int m_anim;
    const bool m_isAttack;

protected:
    StateMarker m_transitionableFrom;
    bool m_isAirborne;

    int m_checkOwnProjectileToNotExist = -1;

    bool m_realign = false;
    int m_timerValue = 0;
    bool m_velToInertia = false;
    bool m_resetDefenseState = false;
    bool m_setAirAttackFlag = false;
    bool m_resetPushback = false;
    bool m_callForPriority = false;
    Vector2<float> m_mulOwnVel{1.0f, 1.0f};
    Vector2<float> m_mulOwnInr{1.0f, 1.0f};
    Vector2<float> m_mulOwnDirVel;
    Vector2<float> m_mulOwnDirInr;
    Vector2<float> m_rawAddVel;
    Vector2<float> m_rawAddInr;

    int m_animResetFrame = -1;
    int m_animResetDirection = 1;

    bool m_usingUpdateMovement = false;
    TimelineProperty<Vector2<float>> m_mulOwnVelUpd;
    TimelineProperty<Vector2<float>> m_mulOwnInrUpd;
    TimelineProperty<Vector2<float>> m_mulOwnDirVelUpd;
    TimelineProperty<Vector2<float>> m_mulOwnDirInrUpd;
    TimelineProperty<Vector2<float>> m_rawAddVelUpd;
    TimelineProperty<Vector2<float>> m_rawAddInrUpd;

    TimelineProperty<float> m_ownVelLimitUpd;
    TimelineProperty<float> m_ownInrLimitUpd;

    TimelineProperty<Vector2<int>> m_camShakeUpd;

    TimelineProperty<bool> m_updRealign;

    bool m_setRealignOtd = false;
    bool m_setAirborneOtd = false;
    bool m_setAboveGroundOtd = false;

    Vector2<float> m_mulOwnVelOtd{1.0f, 1.0f};
    Vector2<float> m_mulOwnInrOtd{1.0f, 1.0f};
    Vector2<float> m_mulOwnDirVelOtd;
    Vector2<float> m_mulOwnDirInrOtd;
    Vector2<float> m_rawAddVelOtd;
    Vector2<float> m_rawAddInrOtd;

    int m_targetStateOutdated = -1;

    int m_recoveryOnLand = -1;
    Vector2<float> m_mulOwnVelLand{1.0f, 1.0f};
    Vector2<float> m_mulOwnInrLand{1.0f, 1.0f};
    Vector2<float> m_mulOwnDirVelLand;
    Vector2<float> m_mulOwnDirInrLand;
    Vector2<float> m_rawAddVelLand;
    Vector2<float> m_rawAddInrLand;


};

/* ============================
 *
 *       ABSTRACT CHARACTER ACTION
 *  All character actions should be inherited from
 *  this class.
 *
 *========================== */
class ActionCharacter : public Action
{
public:
    ActionCharacter(int actionState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, int anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isAttack_, bool isCrouchState_, bool isThrowStartup_,
    int consumeAirdash_, int consumeAirjump_, bool waitAirdashTimer_, bool waitAirjumpTimer_, bool isAirborne_);
    virtual bool isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const;
    virtual void outdated(InteractableStateMachine &character_);
    virtual void switchTo(InteractableStateMachine &character_);
    virtual void update(InteractableStateMachine &character_);
    virtual bool onLand(InteractableStateMachine &character_);
    virtual bool isInCounterState(uint32_t currentFrame_) const;
    virtual bool applyGravity(uint32_t currentFrame_) const;
    virtual bool canBlock(uint32_t currentFrame_) const;



    // 0 if not possible
    // 1 if possible
    // -1 if already active and is still possible
    virtual int isPossible(const InputQueue &inputQueue_, InteractableStateMachine *char_, int extendBuffer_) const;
    virtual ~ActionCharacter() {};

    ActionCharacter *setSwitchData(bool realign_, int timerValue_, bool velToInertia_, bool resetDefenseState_, bool setAirAttackFlag_, bool resetPushback_, bool callForPriority_,
    Vector2<float> mulOwnVel_, Vector2<float> mulOwnInr_, Vector2<float> mulOwnDirVel_, Vector2<float> mulOwnDirInr_, Vector2<float> rawAddVel_, Vector2<float> rawAddInr_);
    ActionCharacter *setHitstunAnimation(int hitstunAnim_);
    ActionCharacter *setOutdatedFlags(bool setRealign_, bool setThrowInvul_, bool setAirborne_, bool enterHitstun_, bool setAboveGroundOtd_);

    ActionCharacter *setUpdateCreateProjectile(TimelineProperty<bool> &&createPt_, PTRecipe recipe_);

    ActionCharacter *setDisadvantageFlags(bool isBlockstun_, bool isHitstun_, bool isKnockdown_);

    const bool m_isThrowStartup;
    const bool m_isCrouchState;

    bool m_isBlockstun = false;
    bool m_isHitstun = false;
    bool m_isKnockdown = false;

protected:
    InputComparator_ptr incmp;
    TimelineProperty<bool> m_counterWindow;
    TimelineProperty<bool> m_gravityWindow;
    TimelineProperty<bool> m_blockWindow;

    int m_consumeAirdash;
    int m_consumeAirjump;
    bool m_waitAirdashTimer;
    bool m_waitAirjumpTimer;

    bool m_resetDefenseState = false;
    bool m_setAirAttackFlag = false;
    bool m_resetPushback = false;

    int m_animResetFrame = -1;
    int m_animResetDirection = 1;

    bool m_setThrowInvulOtd = false;
    bool m_enterHitstunOtd = false;

    int m_hitstunAnimation = (int)HITSTUN_ANIMATION::NONE;

    PTRecipe m_ptRecipe;
    TimelineProperty<bool> m_createPt;
};


/* ============================
 *
 *       ABSTRACT PROLONGED ACTION
 *   This includes all actions that can be
 *   prolonged with input that differs from
 *   the usual one
 * 
 *   Examples: forward ground dash (66 to
 *   begin, hold 6 to continue), some stances
 *   ([1-9]X / 236X / Whatever + X to initiate,
 *   hold X to continue), etc
 *
 *========================== */
class Action_prolonged : public ActionCharacter
{
public:
    Action_prolonged(int actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec &&hurtboxes_, int anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isCrouchState_,
    int consumeAirdash_, int consumeAirjump_, bool waitAirdashTimer_, bool waitAirjumpTimer_, bool isAirborne_);
    virtual int isPossibleToProlong(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const;
    virtual int responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const override;

protected:
    InputComparator_ptr incmp_prolonged;
};


/* ============================
 *
 *       ABSTRACT GROUND JUMP
 *
 *========================== */
class Action_jump : public ActionCharacter
{
public:
    Action_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, int anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_);
    virtual void switchTo(InteractableStateMachine &character_) override;
    Action_jump *setAirActionTimers(int airjumpTimerValue_, int airdashTimerValue_);
    const Vector2<float> m_impulse;
    const float m_prejumpLen;
    const float m_maxHorInertia;

private:
    int m_airjumpTimerValue;
    int m_airdashTimerValue;
};

/* ============================
 *
 *       ABSTRACT AIR JUMP
 *
 *========================== */
class Action_airjump : public ActionCharacter
{
public:
    Action_airjump(int actionState_, const Vector2<float> &impulse_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, int anim_, StateMarker transitionableFrom_, int floatState_);
    virtual int responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const override;
    virtual int isPossible(const InputQueue &inputQueue_, InteractableStateMachine *char_, int extendBuffer_) const;
    virtual void update(InteractableStateMachine &character_) override;
    const Vector2<float> m_impulse;

private:
    int m_floatState;
};



/* ============================
 *
 *       ABSTRACT ATTACK
 *
 *========================== */
class Action_attack : public ActionCharacter
{
public:
    Action_attack(int actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, int anim_, TimelineProperty<bool> &&gravityWindow_, StateMarker transitionableFrom_, bool isCrouchState_, bool isAirborne_);
    virtual const HitsVec getCurrentHits(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;
    virtual const bool isActive(uint32_t currentFrame_) const;
    virtual void switchTo(InteractableStateMachine &character_) override;
    void resetOpponentsHits(InteractableStateMachine &character_);
    const int m_fullDuration;

protected:
    ActiveFramesVec m_hits;
};

/* ============================
 *
 *       ABSTRACT THROW STARTUP
 *
 *========================== */
class Action_throw_startup : public ActionCharacter
{
public:
    Action_throw_startup(int actionState_, int whiffState_, int holdState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, int anim_, TimelineProperty<bool> &&gravityWindow_, float range_, FrameWindow activeWindow_, bool requiredAirborne_, THROW_LIST throw_, StateMarker transitionableFrom_, bool isAirborne_);
    virtual void attemptThrow(InteractableStateMachine &character_) const;

    const FrameWindow m_activeWindow;

protected:
    const float m_range;
    const int m_holdState;
    const THROW_LIST m_throw;
    const bool m_requiredAirborne;
};

/* ============================
 *
 *       ABSTRACT THROW HOLD
 *
 *========================== */
class Action_throw_hold : public ActionCharacter
{
public:
    Action_throw_hold(int actionState_, int throwState_, float setRange_, float duration_, bool sideSwitch_, int anim_);
    virtual void switchTo(InteractableStateMachine &character_) override;
    void outdated(InteractableStateMachine &character_) override;

protected:
    const float m_setRange;
    const float m_duration;
    bool m_sideSwitch;
};

/* ============================
 *
 *       ABSTRACT THROWN HOLD
 *
 *========================== */
class Action_thrown_hold : public ActionCharacter
{
public:
    Action_thrown_hold(int actionState_, int thrownState_, int anim_, float duration_);
    virtual void switchTo(InteractableStateMachine &character_) override;

protected:
    const float m_duration;

};

/* ============================
 *
 *       ABSTRACT THROW WHIFF
 *
 *========================== */
class Action_throw_whiff : public ActionCharacter
{
public:
    Action_throw_whiff(int actionState_, int anim_, TimelineProperty<bool> &&gravityWindow_, float duration_, HurtboxFramesVec &&hurtboxes_, int idleState_, int floatState_);
    void outdated(InteractableStateMachine &character_) override;

protected:
    const float m_duration;
    int m_idleState;
    int m_floatState;
};

/* ============================
 *
 *       ABSTRACT THROW TECH
 *
 *========================== */
class Action_throw_tech : public ActionCharacter
{
public:
    Action_throw_tech(int actionState_, InputComparator_ptr incmp_, int anim_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, float duration_, HurtboxFramesVec &&hurtboxes_, THROW_TECHS_LIST throwTech_, StateMarker transitionableFrom_, bool isAirborne_, int idleState_, int floatState_);
    virtual void switchTo(InteractableStateMachine &character_) override;
    void outdated(InteractableStateMachine &character_) override;

protected:
    const float m_duration;
    const THROW_TECHS_LIST m_throwTech;
    int m_idleState;
    int m_floatState;
};

/* ============================
 *
 *       ABSTRACT LOCKED ANIMATION
 *      Holds full control of a character. A character in a locked animation
 *      is not affected by gravity, does not change velocity, inertia and animation
 *      from hits and does not change animation
 *
 *========================== */
class Action_locked_animation : public ActionCharacter
{
public:
    Action_locked_animation(int actionState_, int quitState_, HurtboxFramesVec &&hurtboxes_, int anim_, float duration_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_);
    virtual void switchTo(InteractableStateMachine &character_) override;
    virtual void update(InteractableStateMachine &character_) override;
    void outdated(InteractableStateMachine &character_) override;

    Action_locked_animation *setUpdateHitsToOpponent(TimelineProperty<HitData*> &&hitsToOpponent_);

protected:
    TimelineProperty<HitData*> m_hitsToOpponent;

    const float m_duration;
};


/* ============================
 *
 *       ABSTRACT FLOAT STATE
 *      Just a temporary state to get jumping state
 *      after aerial action without double jumping
 *
 *========================== */
class Action_float : public ActionCharacter
{
public:
    Action_float(int actionState_, int realState_, HurtboxFramesVec &&hurtboxes_, int anim_, float horizontalTiltDelta_, float upTiltDelta_, float downTiltDelta_, float horizontalTiltInertiaLimit_);
    virtual void switchTo(InteractableStateMachine &character_) override;
    virtual void update(InteractableStateMachine &character_) override;

protected:
    int m_realState;
    float m_horizontalTiltDelta;
    float m_upTiltDelta;
    float m_downTiltDelta;

    float m_horizontalTiltInertiaLimit;
};


/* ============================
 *
 *       CHAR 1 ACTIONS
 *
 *========================== */

class Action_char1_idle : public ActionCharacter
{
public:
    Action_char1_idle(int animId_);
};

class Action_char1_crouch : public Action_prolonged
{
public:
    Action_char1_crouch(int animId_);
};

class Action_char1_walk_fwd : public Action_prolonged
{
public:
    Action_char1_walk_fwd(int animId_);
};

class Action_char1_walk_bwd : public Action_prolonged
{
public:
    Action_char1_walk_bwd(int animId_);
};

class Action_char1_neutral_jump : public Action_jump
{
public:
    Action_char1_neutral_jump(int animId_);
};

class Action_char1_forward_jump : public Action_jump
{
public:
    Action_char1_forward_jump(int animId_);
};

class Action_char1_backward_jump : public Action_jump
{
public:
    Action_char1_backward_jump(int animId_);
};

class Action_char1_air_dash_extention : public ActionCharacter
{
public:
    Action_char1_air_dash_extention(int animId_);
    const int m_duration;
    const float m_baseSpd;
    const float m_spdMultiplier;
};

class Action_char1_neutral_doublejump : public Action_airjump
{
public:
    Action_char1_neutral_doublejump(int animId_);
};

class Action_char1_forward_doublejump : public Action_airjump
{
public:
    Action_char1_forward_doublejump(int animId_);
};

class Action_char1_backward_doublejump : public Action_airjump
{
public:
    Action_char1_backward_doublejump(int animId_);
};

class Action_char1_ground_dash : public Action_prolonged
{
public:
    Action_char1_ground_dash(int animId_);
    const float m_accel;
    const float m_maxspd;
};

class Action_char1_step: public ActionCharacter
{
public:
    Action_char1_step(int animId_);
    const int m_duration;
};

class Action_char1_step_recovery : public ActionCharacter
{
public:
    Action_char1_step_recovery(int animId_);
    const int m_recoveryLen;
};

class Action_char1_ground_backdash: public ActionCharacter
{
public:
    Action_char1_ground_backdash(int animId_);
    const int m_totalDuration;
};

class Action_char1_ground_dash_recovery : public ActionCharacter
{
public:
    Action_char1_ground_dash_recovery(int animId_);
    const int m_recoveryLen;
};

class Action_char1_air_dash : public ActionCharacter
{
public:
    Action_char1_air_dash(int animId_);
    const int m_duration;
};

class Action_char1_air_backdash : public ActionCharacter
{
public:
    Action_char1_air_backdash(int animId_);
    const int m_duration;
};

class Action_char1_soft_landing_recovery : public ActionCharacter
{
public:
    Action_char1_soft_landing_recovery(int animId_);
    const int m_recoveryLen;
};

class Action_char1_hard_landing_recovery : public ActionCharacter
{
public:
    Action_char1_hard_landing_recovery(int animId_);
    const int m_recoveryLen;
};

class Action_char1_vulnerable_landing_recovery : public ActionCharacter
{
public:
    Action_char1_vulnerable_landing_recovery(int animId_);
    const int m_recoveryLen;
};

class Action_char1_jc_landing_recovery : public ActionCharacter
{
public:
    Action_char1_jc_landing_recovery(int animId_);
    const int m_recoveryLen;
};

class Action_char1_soft_knockdown : public ActionCharacter
{
public:
    Action_char1_soft_knockdown(int animId_);
};

class Action_char1_hard_knockdown : public ActionCharacter
{
public:
    Action_char1_hard_knockdown(int animId_);
};

class Action_char1_knockdown_recovery : public ActionCharacter
{
public:
    Action_char1_knockdown_recovery(int animId_);
};

class Action_char1_move_JC : public Action_attack
{
public:
    Action_char1_move_JC(int animId_, int animIdHit_, int animIdBlock_);
};

class Action_char1_move_214C : public Action_attack
{
public:
    Action_char1_move_214C(int animId_, int animIdHit_, int animIdBlock_);
};

class Action_char1_move_projectile : public Action_attack
{
public:
    Action_char1_move_projectile(int animId_);
};

// THROW RELATED STUFF

// THROW ACTIONS

// NORMAL THROW
class Action_char1_normal_throw_startup : public Action_throw_startup
{
public:
    Action_char1_normal_throw_startup(int animId_);
};

class Action_char1_normal_throw_hold : public Action_throw_hold
{
public:
    Action_char1_normal_throw_hold(int animId_);
};

class Action_char1_back_throw_startup : public Action_throw_startup
{
public:
    Action_char1_back_throw_startup(int animId_);
};

class Action_char1_back_throw_hold : public Action_throw_hold
{
public:
    Action_char1_back_throw_hold(int animId_);
};

class Action_char1_normal_throw_whiff : public Action_throw_whiff
{
public:
    Action_char1_normal_throw_whiff(int animId_);
};

class Action_char1_normal_throw : public Action_locked_animation
{
public:
    Action_char1_normal_throw(int animId_, int animIdHit_);
};

// AIR THROW

class Action_char1_normal_air_throw_startup : public Action_throw_startup
{
public:
    Action_char1_normal_air_throw_startup(int animId_);
};

class Action_char1_normal_air_throw_hold : public Action_throw_hold
{
public:
    Action_char1_normal_air_throw_hold(int animId_);
};

class Action_char1_back_air_throw_startup : public Action_throw_startup
{
public:
    Action_char1_back_air_throw_startup(int animId_);
};

class Action_char1_back_air_throw_hold : public Action_throw_hold
{
public:
    Action_char1_back_air_throw_hold(int animId_);
};

class Action_char1_normal_air_throw_whiff : public Action_throw_whiff
{
public:
    Action_char1_normal_air_throw_whiff(int animId_);
};

class Action_char1_normal_air_throw : public Action_locked_animation
{
public:
    Action_char1_normal_air_throw(int animId_, int animIdHit_);
};


// THROW TECHS
class Action_char1_throw_tech : public Action_throw_tech
{
public:
    Action_char1_throw_tech(int animId_);
};

class Action_char1_throw_tech_char1 : public Action_throw_tech
{
public:
    Action_char1_throw_tech_char1(int animId_);
};

class Action_char1_air_throw_tech : public Action_throw_tech
{
public:
    Action_char1_air_throw_tech(int animId_);
};

class Action_char1_air_throw_tech_char1 : public Action_throw_tech
{
public:
    Action_char1_air_throw_tech_char1(int animId_);
};


// GETTING THROWN ACTIONS

class Action_char1_thrown_char1_normal_hold : public Action_thrown_hold
{
public:
    Action_char1_thrown_char1_normal_hold(int animId_);
};

class Action_char1_thrown_char1_normal : public Action_locked_animation
{
public:
    Action_char1_thrown_char1_normal(int animId_);
};

class Action_char1_thrown_char1_normal_air_hold : public Action_thrown_hold
{
public:
    Action_char1_thrown_char1_normal_air_hold(int animId_);
};

class Action_char1_thrown_char1_normal_air : public Action_locked_animation
{
public:
    Action_char1_thrown_char1_normal_air(int animId_);
};




#endif