#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "InputComparators.h"
#include "Hit.h"

enum class CHAR1_STATE;
class Char1Data;

/* ============================
 *
 *       ABSTRACT ACTION
 *  All actions should be inherited from
 *  this class. Internal transitions should
 *  be handled by the character.
 *
 *========================== */
template <typename CharState_t, typename CharData>
class Action
{
public:
    Action(CharState_t actionState_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_);
    virtual bool isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const;
    virtual const HurtboxVec getCurrentHurtboxes(int currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;

    // 0 if not possible
    // 1 if possible
    // -1 if already active and is still possible
    virtual int isPossible(const InputQueue &inputQueue_, CharData charData_) const = 0;
    virtual ~Action() {};

    const CharState_t actionState;
    const HurtboxFramesVec m_hurtboxes;

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
template <typename CharState_t, typename CharData>
class Action_prolonged : public Action<CharState_t, CharData>
{
public:
    Action_prolonged(CharState_t actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec hurtboxes_);
    virtual int isPossibleToProlong(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const;

protected:
    InputComparator_ptr incmp_prolonged;
};


/* ============================
 *
 *       ABSTRACT GROUND JUMP
 *
 *========================== */
template <typename CharState_t, typename CharData>
class Action_jump : public Action<CharState_t, CharData>
{
public:
    Action_jump(CharState_t actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_);
    const Vector2<float> m_impulse;
    const float m_prejumpLen;
    const float m_maxHorInertia;
};


/* ============================
 *
 *       ABSTRACT ATTACK
 *
 *========================== */
template <typename CharState_t, typename CharData>
class Action_attack : public Action<CharState_t, CharData>
{
public:
    Action_attack(CharState_t actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec hurtboxes_, std::vector<std::pair<std::pair<int, int>, Vector2<float>>> velocity_);
    virtual const HitsVec getCurrentHits(int currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const;
    virtual const Vector2<float> &getCurrentVelocity(int currentFrame_) const;
    const int m_fullDuration;
    const ActiveFramesVec m_hits;
    const std::vector<std::pair<std::pair<int, int>, Vector2<float>>> m_velocity;
};


/* ============================
 *
 *       CHAR 1 ACTIONS
 *
 *========================== */

class Action_char1_idle : public Action<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_idle();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
};

class Action_char1_walk_fwd : public Action<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_walk_fwd();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
};

class Action_char1_walk_bwd : public Action<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_walk_bwd();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
};

class Action_char1_jump : public Action_jump<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_jump(CHAR1_STATE actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_);
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
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
    //virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
};

class Action_char1_backward_jump : public Action_char1_jump
{
public:
    Action_char1_backward_jump();
    //virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
};

class Action_char1_airjump : public Action<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_airjump(const Vector2<float> &impulse_, InputComparator_ptr incmp_);
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
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

class Action_char1_ground_dash : public Action_prolonged<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_ground_dash();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
    const float m_accel;
    const float m_maxspd;
};

class Action_char1_ground_dash_recovery : public Action_prolonged<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_ground_dash_recovery();
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
    const int m_recoveryLen;
};

class Action_char1_attack : public Action_attack<CHAR1_STATE, Char1Data>
{
public:
    Action_char1_attack(CHAR1_STATE actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec hurtboxes_, std::vector<std::pair<std::pair<int, int>, Vector2<float>>> velocity_);
    virtual int isPossible(const InputQueue &inputQueue_, Char1Data charData_) const override;
};

class Action_char1_jab : public Action_char1_attack
{
public:
    Action_char1_jab();
};

class Action_char1_move_C : public Action_char1_attack
{
public:
    Action_char1_move_C();
};

#endif