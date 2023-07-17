#ifndef CHAR_1_H_
#define CHAR_1_H_

#include "Character.h"
#include "ActionResolver.h"
#include "FrameTimer.h"

enum class CHAR1_STATE {
    NONE = 0,
    IDLE,
    WALK_FWD,
    WALK_BWD,
    PREJUMP,
    JUMP,
    SOFT_LANDING_RECOVERY,
    GROUND_DASH,
    GROUND_DASH_RECOVERY,
    MOVE_A,
    HITSTUN
    };

struct Char1Data : public CharData<CHAR1_STATE>
{
    bool usedDoubleJump;
    bool usedAirDash;
    bool inHitstop;
};

class ActionResolver_Char1 : public ActionResolver<CHAR1_STATE, Char1Data>
{
public:
    ActionResolver_Char1(InputSystem *input_);

    void createActions() final;
};

class Char1 : public Character
{
public:
    Char1(Application &application_, Vector2<float> pos_);
    virtual ~Char1() {};

    void loadAnimations(Application &application_) final;
    void initiate() final;
    void updateState() final;
    void land() final;
    bool canApplyDrag() const final;
    bool canBeDraggedByInertia() const final;
    HitsVec getHits() final;
    HurtboxVec getHurtboxes() final;
    void applyHit(const HitEvent &hitEvent) final;

protected:
    void switchToIdle();
    void jumpUsingAction();
    void jumpUsingAirjumpAction();
    void switchToSoftLandingRecovery();
    Char1Data generateCharData();

    std::set<int> m_appliedHits;

    const Action<CHAR1_STATE, Char1Data> *m_currentAction;
    ActionResolver_Char1 m_actionResolver;

    CHAR1_STATE m_currentState = CHAR1_STATE::IDLE;
    FrameTimer m_timer;
    bool m_usedDoubleJump = false;
    bool m_usedAirDash = false;

    Hitbox m_standingHurtbox;

    int framesInState = 1;
    CHAR1_STATE lastState = CHAR1_STATE::IDLE;
};

#endif