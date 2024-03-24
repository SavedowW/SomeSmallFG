#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "InputComparators.h"
#include "Hit.h"
#include "AnimationManager.h"
#include "TimelineProperty.h"
#include "StateMarker.h"

class Character;
class Char1;

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
 *  this class. Internal transitions should
 *  be handled by the character.
 *
 *========================== */
class Action
{
public:
    Action(int actionState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isAttack_, bool isCrouchState_, bool isThrowStartup_,
    int consumeAirdash_, int consumeAirjump_, bool waitAirdashTimer_, bool waitAirjumpTimer_, bool isAirborne_);
    virtual bool isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const;
    virtual const HurtboxVec getCurrentHurtboxes(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;
    virtual void outdated(Character &character_) const {};
    virtual void switchTo(Character &character_) const;
    virtual void update(Character &character_) const {};
    virtual bool isInCounterState(uint32_t currentFrame_) const;
    virtual bool applyGravity(uint32_t currentFrame_) const;
    virtual bool canBlock(uint32_t currentFrame_) const;


    // 0 if not possible
    // 1 if possible
    // -1 if already active and is still possible
    virtual int isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const;
    virtual int responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const;
    virtual ~Action() {};

    const int actionState;
    const HurtboxFramesVec m_hurtboxes;
    const ANIMATIONS m_anim;
    const bool m_isAttack;
    const bool m_isThrowStartup;
    const bool m_isCrouchState;

protected:
    InputComparator_ptr incmp;
    TimelineProperty<bool> m_counterWindow;
    TimelineProperty<bool> m_gravityWindow;
    TimelineProperty<bool> m_blockWindow;
    StateMarker m_transitionableFrom;

    int m_consumeAirdash;
    int m_consumeAirjump;
    bool m_waitAirdashTimer;
    bool m_waitAirjumpTimer;
    bool m_isAirborne;
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
class Action_prolonged : public Action
{
public:
    Action_prolonged(int actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isCrouchState_,
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
class Action_jump : public Action
{
public:
    Action_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_);
    const Vector2<float> m_impulse;
    const float m_prejumpLen;
    const float m_maxHorInertia;
};

/* ============================
 *
 *       ABSTRACT AIR JUMP
 *
 *========================== */
class Action_airjump : public Action
{
public:
    Action_airjump(int actionState_, const Vector2<float> &impulse_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, StateMarker transitionableFrom_);
    virtual int responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const override;
    const Vector2<float> m_impulse;
};



/* ============================
 *
 *       ABSTRACT ATTACK
 *
 *========================== */
class Action_attack : public Action
{
public:
    Action_attack(int actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, TimelineProperty<Vector2<float>> &&velocity_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, StateMarker transitionableFrom_, bool isCrouchState_, bool isAirborne_);
    virtual const HitsVec getCurrentHits(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;
    virtual void switchTo(Character &character_) const override;
    virtual void update(Character &character_) const override;
    const int m_fullDuration;
    const TimelineProperty<Vector2<float>> m_velocity;

protected:
    const ActiveFramesVec m_hits;
};

/* ============================
 *
 *       ABSTRACT THROW STARTUP
 *
 *========================== */
class Action_throw_startup : public Action
{
public:
    Action_throw_startup(int actionState_, int whiffState_, int holdState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float range_, FrameWindow activeWindow_, bool requiredAirborne_, THROW_LIST throw_, StateMarker transitionableFrom_, bool isAirborne_);
    virtual void switchTo(Character &character_) const override;
    virtual void attemptThrow(Character &character_) const;

    void outdated(Character &character_) const override;
    const FrameWindow m_activeWindow;

protected:
    const float m_range;
    const int m_whiffState;
    const int m_holdState;
    const THROW_LIST m_throw;
    const bool m_requiredAirborne;
};

/* ============================
 *
 *       ABSTRACT THROW HOLD
 *
 *========================== */
class Action_throw_hold : public Action
{
public:
    Action_throw_hold(int actionState_, int throwState_, float setRange_, float duration_, bool sideSwitch_);
    virtual void switchTo(Character &character_) const override;
    virtual void update(Character &character_) const override;
    void outdated(Character &character_) const override;

protected:
    const float m_setRange;
    const float m_duration;
    bool m_sideSwitch;
    const int m_throwState;
};

/* ============================
 *
 *       ABSTRACT THROWN HOLD
 *
 *========================== */
class Action_thrown_hold : public Action
{
public:
    Action_thrown_hold(int actionState_, int thrownState_, ANIMATIONS anim_, float duration_);
    virtual void switchTo(Character &character_) const override;
    void outdated(Character &character_) const override;

protected:
    const float m_duration;
    const int m_thrownState;

};

/* ============================
 *
 *       ABSTRACT THROW WHIFF
 *
 *========================== */
class Action_throw_whiff : public Action
{
public:
    Action_throw_whiff(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float duration_, HurtboxFramesVec &&hurtboxes_);
    virtual void switchTo(Character &character_) const override;
    void outdated(Character &character_) const override;

protected:
    const float m_duration;
};

/* ============================
 *
 *       ABSTRACT THROW TECH
 *
 *========================== */
class Action_throw_tech : public Action
{
public:
    Action_throw_tech(int actionState_, InputComparator_ptr incmp_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, float duration_, HurtboxFramesVec &&hurtboxes_, THROW_TECHS_LIST throwTech_, StateMarker transitionableFrom_, bool isAirborne_);
    virtual void switchTo(Character &character_) const override;
    void outdated(Character &character_) const override;

protected:
    const float m_duration;
    const THROW_TECHS_LIST m_throwTech;
};

/* ============================
 *
 *       ABSTRACT LOCKED ANIMATION
 *      Holds full control of a character. A character in a locked animation
 *      is not affected by gravity, does not change velocity, inertia and animation
 *      from hits and does not change animation
 *
 *========================== */
class Action_locked_animation : public Action
{
public:
    Action_locked_animation(int actionState_, int quitState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, float duration_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_);
    virtual void switchTo(Character &character_) const override;
    //virtual void update(Character &character_) const override;
    void outdated(Character &character_) const override;

protected:
    const float m_duration;
    int m_quitState;
};


/* ============================
 *
 *       CHAR 1 ACTIONS
 *
 *========================== */

class Action_char1_idle : public Action
{
public:
    Action_char1_idle();
    virtual void switchTo(Character &character_) const override;
    virtual void update(Character &character_) const override;
};

class Action_char1_crouch : public Action_prolonged
{
public:
    Action_char1_crouch();
    virtual void switchTo(Character &character_) const override;
    virtual void update(Character &character_) const override;
};

class Action_char1_walk_fwd : public Action_prolonged
{
public:
    Action_char1_walk_fwd();
    virtual void switchTo(Character &character_) const override;
    virtual void update(Character &character_) const override;
};

class Action_char1_walk_bwd : public Action_prolonged
{
public:
    Action_char1_walk_bwd();
    virtual void switchTo(Character &character_) const override;
    virtual void update(Character &character_) const override;
};

class Action_char1_jump : public Action_jump
{
public:
    Action_char1_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_);
    virtual void switchTo(Character &character_) const override;
    const Vector2<float> m_impulse;
    const float m_prejumpLen;
    const float m_maxHorInertia;
};

class Action_char1_neutral_jump : public Action_char1_jump
{
public:
    Action_char1_neutral_jump();
};

class Action_char1_forward_jump : public Action_char1_jump
{
public:
    Action_char1_forward_jump();
};

class Action_char1_backward_jump : public Action_char1_jump
{
public:
    Action_char1_backward_jump();
};

class Action_char1_float : public Action
{
public:
    Action_char1_float();
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_air_dash_extention : public Action
{
public:
    Action_char1_air_dash_extention();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    virtual void setVelocity(Character &character_) const;
    virtual void update(Character &character_) const override;
    const int m_duration;
    const float m_baseSpd;
    const float m_spdMultiplier;
};

class Action_char1_airjump : public Action_airjump
{
public:
    Action_char1_airjump(const Vector2<float> &impulse_, InputComparator_ptr incmp_);
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_neutral_doublejump : public Action_char1_airjump
{
public:
    Action_char1_neutral_doublejump();
};

class Action_char1_forward_doublejump : public Action_char1_airjump
{
public:
    Action_char1_forward_doublejump();
};

class Action_char1_backward_doublejump : public Action_char1_airjump
{
public:
    Action_char1_backward_doublejump();
};

class Action_char1_ground_dash : public Action_prolonged
{
public:
    Action_char1_ground_dash();
    virtual void update(Character &character_) const override;
    const float m_accel;
    const float m_maxspd;
};

class Action_char1_step: public Action
{
public:
    Action_char1_step();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_duration;
};

class Action_char1_step_recovery : public Action
{
public:
    Action_char1_step_recovery();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    virtual void update(Character &character_) const override;
    const int m_recoveryLen;
};

class Action_char1_ground_backdash: public Action
{
public:
    Action_char1_ground_backdash();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_totalDuration;
};

class Action_char1_ground_dash_recovery : public Action
{
public:
    Action_char1_ground_dash_recovery();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_recoveryLen;
};

class Action_char1_air_dash : public Action
{
public:
    Action_char1_air_dash();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_duration;
};

class Action_char1_air_backdash : public Action
{
public:
    Action_char1_air_backdash();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_duration;
};

class Action_char1_soft_landing_recovery : public Action
{
public:
    Action_char1_soft_landing_recovery();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_recoveryLen;
};

class Action_char1_hard_landing_recovery : public Action
{
public:
    Action_char1_hard_landing_recovery();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_recoveryLen;
};

class Action_char1_vulnerable_landing_recovery : public Action
{
public:
    Action_char1_vulnerable_landing_recovery();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_recoveryLen;
};

class Action_char1_jc_landing_recovery : public Action
{
public:
    Action_char1_jc_landing_recovery();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
    const int m_recoveryLen;
};

class Action_char1_soft_knockdown : public Action
{
public:
    Action_char1_soft_knockdown();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_hard_knockdown : public Action
{
public:
    Action_char1_hard_knockdown();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_knockdown_recovery : public Action
{
public:
    Action_char1_knockdown_recovery();
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_ground_attack : public Action_attack
{
public:
    Action_char1_ground_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, TimelineProperty<Vector2<float>> &&velocity_, StateMarker transitionableFrom_, bool isCrouchState_);
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_air_attack : public Action_attack
{
public:
    Action_char1_air_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, StateMarker transitionableFrom_);
    virtual void outdated(Character &character_) const override;
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_move_JC : public Action_char1_air_attack
{
public:
    Action_char1_move_JC();
    virtual void update(Character &character_) const override;
};

class Action_char1_move_214C : public Action_char1_ground_attack
{
public:
    Action_char1_move_214C();
    virtual void update(Character &character_) const override;
};

class Action_char1_move_projectile : public Action_char1_ground_attack
{
public:
    Action_char1_move_projectile();
};

// THROW RELATED STUFF

// THROW ACTIONS

// NORMAL THROW
class Action_char1_normal_throw_startup : public Action_throw_startup
{
public:
    Action_char1_normal_throw_startup();
};

class Action_char1_normal_throw_hold : public Action_throw_hold
{
public:
    Action_char1_normal_throw_hold();
};

class Action_char1_back_throw_startup : public Action_throw_startup
{
public:
    Action_char1_back_throw_startup();
};

class Action_char1_back_throw_hold : public Action_throw_hold
{
public:
    Action_char1_back_throw_hold();
};

class Action_char1_normal_throw_whiff : public Action_throw_whiff
{
public:
    Action_char1_normal_throw_whiff();
};

class Action_char1_normal_throw : public Action_locked_animation
{
public:
    Action_char1_normal_throw();
    virtual void update(Character &character_) const override;
};

// AIR THROW

class Action_char1_normal_air_throw_startup : public Action_throw_startup
{
public:
    Action_char1_normal_air_throw_startup();
};

class Action_char1_normal_air_throw_hold : public Action_throw_hold
{
public:
    Action_char1_normal_air_throw_hold();
};

class Action_char1_back_air_throw_startup : public Action_throw_startup
{
public:
    Action_char1_back_air_throw_startup();
};

class Action_char1_back_air_throw_hold : public Action_throw_hold
{
public:
    Action_char1_back_air_throw_hold();
};

class Action_char1_normal_air_throw_whiff : public Action_throw_whiff
{
public:
    Action_char1_normal_air_throw_whiff();
};

class Action_char1_normal_air_throw : public Action_locked_animation
{
public:
    Action_char1_normal_air_throw();
    virtual void update(Character &character_) const override;
};


// THROW TECHS
class Action_char1_throw_tech : public Action_throw_tech
{
public:
    Action_char1_throw_tech();
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_throw_tech_char1 : public Action_throw_tech
{
public:
    Action_char1_throw_tech_char1();
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_air_throw_tech : public Action_throw_tech
{
public:
    Action_char1_air_throw_tech();
    virtual void switchTo(Character &character_) const override;
};

class Action_char1_air_throw_tech_char1 : public Action_throw_tech
{
public:
    Action_char1_air_throw_tech_char1();
    virtual void switchTo(Character &character_) const override;
};


// GETTING THROWN ACTIONS

class Action_char1_thrown_char1_normal_hold : public Action_thrown_hold
{
public:
    Action_char1_thrown_char1_normal_hold();
};

class Action_char1_thrown_char1_normal : public Action_locked_animation
{
public:
    Action_char1_thrown_char1_normal();
    virtual void update(Character &character_) const override;
    virtual void outdated(Character &character_) const override;
};

class Action_char1_thrown_char1_normal_air_hold : public Action_thrown_hold
{
public:
    Action_char1_thrown_char1_normal_air_hold();
};

class Action_char1_thrown_char1_normal_air : public Action_locked_animation
{
public:
    Action_char1_thrown_char1_normal_air();
    virtual void update(Character &character_) const override;
    virtual void outdated(Character &character_) const override;
};




#endif