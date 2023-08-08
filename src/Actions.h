#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "InputComparators.h"
#include "Hit.h"
#include "AnimationManager.h"

enum class CHAR1_STATE;
class Char1Data;
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
template <typename CharState_t, typename CharData, typename Char_t>
class Action
{
public:
    Action(CharState_t actionState_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_, bool isAttack_ = false, bool noLandTransition_ = false, bool isCrouchState_ = false, bool isFullCounter_ = false, bool isThrowStartup_ = false);
    virtual bool isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const;
    virtual const HurtboxVec getCurrentHurtboxes(int currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;
    virtual void outdated(Char_t &character_) const {};
    virtual void switchTo(Char_t &character_) const;
    virtual void update(Char_t &character_) const {};


    // 0 if not possible
    // 1 if possible
    // -1 if already active and is still possible
    virtual int isPossible(const InputQueue &inputQueue_, CharData charData_, int extendBuffer_) const = 0;
    virtual ~Action() {};

    const CharState_t actionState;
    const HurtboxFramesVec m_hurtboxes;
    const ANIMATIONS m_anim;
    const bool m_isAttack;
    const bool m_isThrowStartup;
    const bool m_isFullCounter;
    const bool m_noLandTransition;
    const bool m_isCrouchState;

protected:
    InputComparator_ptr incmp;
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
template <typename CharState_t, typename CharData, typename Char_t>
class Action_prolonged : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_prolonged(CharState_t actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_);
    virtual int isPossibleToProlong(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const;

protected:
    InputComparator_ptr incmp_prolonged;
};


/* ============================
 *
 *       ABSTRACT GROUND JUMP
 *
 *========================== */
template <typename CharState_t, typename CharData, typename Char_t>
class Action_jump : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_jump(CharState_t actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_);
    const Vector2<float> m_impulse;
    const float m_prejumpLen;
    const float m_maxHorInertia;
};


/* ============================
 *
 *       ABSTRACT ATTACK
 *
 *========================== */
template <typename CharState_t, typename CharData, typename Char_t>
class Action_attack : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_attack(CharState_t actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec hurtboxes_, std::vector<std::pair<std::pair<int, int>, Vector2<float>>> velocity_, ANIMATIONS anim_, bool noLandTransition_ = false, bool isCrouchState_ = false);
    virtual const HitsVec getCurrentHits(int currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;
    virtual const Vector2<float> *getCurrentVelocity(int currentFrame_) const;
    virtual bool isInCounterState(int currentFrame_) const;
    virtual void switchTo(Char_t &character_) const override;
    virtual void update(Char_t &character_) const override;
    const int m_fullDuration;
    const ActiveFramesVec m_hits;
    const std::vector<std::pair<std::pair<int, int>, Vector2<float>>> m_velocity;
    const FrameWindow m_counterWindow;

private:
    const Vector2<float> nullvec = {0.0f, 0.0f};
};

/* ============================
 *
 *       ABSTRACT THROW STARTUP
 *
 *========================== */
template <typename CharState_t, typename CharData, typename Char_t>
class Action_throw_startup : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_throw_startup(CharState_t actionState_, CharState_t whiffState_, CharState_t holdState_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_, float range_, FrameWindow activeWindow_, bool requiredAirborne_, THROW_LIST throw_);
    virtual void switchTo(Char_t &character_) const override;
    virtual void attemptThrow(Char_t &character_) const;

    void outdated(Char_t &character_) const override;
    const FrameWindow m_activeWindow;

protected:
    const float m_range;
    const CharState_t m_whiffState;
    const CharState_t m_holdState;
    const THROW_LIST m_throw;
    const bool m_requiredAirborne;
};

/* ============================
 *
 *       ABSTRACT THROW HOLD
 *
 *========================== */
template <typename CharState_t, typename CharData, typename Char_t>
class Action_throw_hold : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_throw_hold(CharState_t actionState_, CharState_t throwState_, float setRange_, float duration_, bool sideSwitch_);
    virtual void switchTo(Char_t &character_) const override;
    virtual void update(Char_t &character_) const override;
    void outdated(Char_t &character_) const override;
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;

protected:
    const float m_setRange;
    const float m_duration;
    bool m_sideSwitch;
    const CharState_t m_throwState;
};

/* ============================
 *
 *       ABSTRACT THROWN HOLD
 *
 *========================== */
template <typename CharState_t, typename CharData, typename Char_t>
class Action_thrown_hold : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_thrown_hold(CharState_t actionState_, CharState_t thrownState_, ANIMATIONS anim_, float duration_);
    virtual void switchTo(Char_t &character_) const override;
    void outdated(Char_t &character_) const override;
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;

protected:
    const float m_duration;
    const CharState_t m_thrownState;

};

/* ============================
 *
 *       ABSTRACT THROWN WHIFF
 *
 *========================== */
template <typename CharState_t, typename CharData, typename Char_t>
class Action_throw_whiff : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_throw_whiff(CharState_t actionState_, ANIMATIONS anim_, float duration_, HurtboxFramesVec hurtboxes_);
    virtual void switchTo(Char_t &character_) const override;
    void outdated(Char_t &character_) const override;
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;

protected:
    const float m_duration;
};

/* ============================
 *
 *       ABSTRACT THROW TECH
 *
 *========================== */
template <typename CharState_t, typename CharData, typename Char_t>
class Action_throw_tech : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_throw_tech(CharState_t actionState_, InputComparator_ptr incmp_, ANIMATIONS anim_, float duration_, HurtboxFramesVec hurtboxes_, THROW_TECHS_LIST throwTech_);
    virtual void switchTo(Char_t &character_) const override;
    void outdated(Char_t &character_) const override;

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
template <typename CharState_t, typename CharData, typename Char_t>
class Action_locked_animation : public Action<CharState_t, CharData, Char_t>
{
public:
    Action_locked_animation(CharState_t actionState_, CharState_t quitState_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_, float duration_);
    virtual void switchTo(Char_t &character_) const override;
    //virtual void update(Char_t &character_) const override;
    void outdated(Char_t &character_) const override;
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;

protected:
    const float m_duration;
    CharState_t m_quitState;
};


/* ============================
 *
 *       CHAR 1 ACTIONS
 *
 *========================== */

class Action_char1_idle : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_idle();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const;
    virtual void update(Char1 &character_) const override;
};

class Action_char1_crouch : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_crouch();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const;
    virtual void update(Char1 &character_) const override;
};

class Action_char1_walk_fwd : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_walk_fwd();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const;
    virtual void update(Char1 &character_) const override;
};

class Action_char1_walk_bwd : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_walk_bwd();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const;
    virtual void update(Char1 &character_) const override;
};

class Action_char1_jump : public Action_jump<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_jump(CHAR1_STATE actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_);
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const override;
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

class Action_char1_float : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_float();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const;
};

class Action_char1_air_dash_extention : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_air_dash_extention();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    virtual void setVelocity(Char1 &character_) const;
    virtual void update(Char1 &character_) const override;
    const int m_duration;
    const float m_baseSpd;
    const float m_spdMultiplier;
};

class Action_char1_airjump : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_airjump(const Vector2<float> &impulse_, InputComparator_ptr incmp_);
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const;
    const Vector2<float> m_impulse;
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

class Action_char1_ground_dash : public Action_prolonged<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_ground_dash();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void update(Char1 &character_) const override;
    const float m_accel;
    const float m_maxspd;
};

class Action_char1_step: public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_step();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_duration;
};

class Action_char1_step_recovery : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_step_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    virtual void update(Char1 &character_) const override;
    const int m_recoveryLen;
};

class Action_char1_ground_backdash: public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_ground_backdash();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_totalDuration;
};

class Action_char1_ground_dash_recovery : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_ground_dash_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_recoveryLen;
};

class Action_char1_air_dash : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_air_dash();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_duration;
};

class Action_char1_air_backdash : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_air_backdash();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_duration;
};

class Action_char1_soft_landing_recovery : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_soft_landing_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_recoveryLen;
};

class Action_char1_hard_landing_recovery : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_hard_landing_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_recoveryLen;
};

class Action_char1_vulnerable_landing_recovery : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_vulnerable_landing_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_recoveryLen;
};

class Action_char1_jc_landing_recovery : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_jc_landing_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
    const int m_recoveryLen;
};

class Action_char1_soft_knockdown : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_soft_knockdown();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
};

class Action_char1_hard_knockdown : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_hard_knockdown();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
};

class Action_char1_knockdown_recovery : public Action<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_knockdown_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
};

class Action_char1_ground_attack : public Action_attack<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_ground_attack(CHAR1_STATE actionState_, ANIMATIONS anim_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec hurtboxes_, std::vector<std::pair<std::pair<int, int>, Vector2<float>>> velocity_, bool noLandTransition_ = false, bool isCrouchState_ = false);
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
};

class Action_char1_air_attack : public Action_attack<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_air_attack(CHAR1_STATE actionState_, ANIMATIONS anim_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec hurtboxes_);
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void outdated(Char1 &character_) const override;
    virtual void switchTo(Char1 &character_) const;
};

class Action_char1_jab : public Action_char1_ground_attack
{
public:
    Action_char1_jab();
};

class Action_char1_move_B : public Action_char1_ground_attack
{
public:
    Action_char1_move_B();
};

class Action_char1_move_C : public Action_char1_ground_attack
{
public:
    Action_char1_move_C();
};


class Action_char1_move_236C : public Action_char1_ground_attack
{
public:
    Action_char1_move_236C();
};

class Action_char1_move_2B : public Action_char1_ground_attack
{
public:
    Action_char1_move_2B();
};

class Action_char1_move_JA : public Action_char1_air_attack
{
public:
    Action_char1_move_JA();
};

class Action_char1_move_JC : public Action_char1_air_attack
{
public:
    Action_char1_move_JC();
    virtual void update(Char1 &character_) const override;
};

class Action_char1_move_214C : public Action_char1_ground_attack
{
public:
    Action_char1_move_214C();
    virtual void update(Char1 &character_) const override;
};

// THROW RELATED STUFF

// THROW ACTIONS

// NORMAL THROW
class Action_char1_normal_throw_startup : public Action_throw_startup<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_throw_startup();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
};

class Action_char1_normal_throw_hold : public Action_throw_hold<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_throw_hold();
};

class Action_char1_back_throw_startup : public Action_throw_startup<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_back_throw_startup();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
};

class Action_char1_back_throw_hold : public Action_throw_hold<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_back_throw_hold();
};

class Action_char1_normal_throw_whiff : public Action_throw_whiff<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_throw_whiff();
};

class Action_char1_normal_throw : public Action_locked_animation<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_throw();
    virtual void update(Char1 &character_) const override;
};

// AIR THROW

class Action_char1_normal_air_throw_startup : public Action_throw_startup<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_air_throw_startup();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
};

class Action_char1_normal_air_throw_hold : public Action_throw_hold<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_air_throw_hold();
};

class Action_char1_back_air_throw_startup : public Action_throw_startup<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_back_air_throw_startup();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
};

class Action_char1_back_air_throw_hold : public Action_throw_hold<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_back_air_throw_hold();
};

class Action_char1_normal_air_throw_whiff : public Action_throw_whiff<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_air_throw_whiff();
};

class Action_char1_normal_air_throw : public Action_locked_animation<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_normal_air_throw();
    virtual void update(Char1 &character_) const override;
};


// THROW TECHS
class Action_char1_throw_tech : public Action_throw_tech<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_throw_tech();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const override;
};

class Action_char1_throw_tech_char1 : public Action_throw_tech<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_throw_tech_char1();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const override;
};

class Action_char1_air_throw_tech : public Action_throw_tech<CHAR1_STATE, Char1Data, Char1> // TODO:
{
public:
    Action_char1_air_throw_tech();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const override;
};

class Action_char1_air_throw_tech_char1 : public Action_throw_tech<CHAR1_STATE, Char1Data, Char1> // TODO:
{
public:
    Action_char1_air_throw_tech_char1();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_, int extendBuffer_) const override;
    virtual void switchTo(Char1 &character_) const override;
};


// GETTING THROWN ACTIONS

class Action_char1_thrown_char1_normal_hold : public Action_thrown_hold<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_thrown_char1_normal_hold();
};

class Action_char1_thrown_char1_normal : public Action_locked_animation<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_thrown_char1_normal();
    virtual void update(Char1 &character_) const override;
    virtual void outdated(Char1 &character_) const override;
};

class Action_char1_thrown_char1_normal_air_hold : public Action_thrown_hold<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_thrown_char1_normal_air_hold();
};

class Action_char1_thrown_char1_normal_air : public Action_locked_animation<CHAR1_STATE, Char1Data, Char1>
{
public:
    Action_char1_thrown_char1_normal_air();
    virtual void update(Char1 &character_) const override;
    virtual void outdated(Char1 &character_) const override;
};




#endif