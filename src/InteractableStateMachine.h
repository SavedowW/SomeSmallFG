#ifndef INTERACTABLE_STATE_MACHINE_H_
#define INTERACTABLE_STATE_MACHINE_H_
#include "Vector2.h"
#include "Application.h"
#include "PriorityHandler.h"
#include "ActionResolver.h"

class Character;

class InteractableStateMachine
{
public:
    InteractableStateMachine(Application &application_, Vector2<float> pos_, int stateCnt_, Camera *cam_);
    ~InteractableStateMachine() = default;

    void setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_);

    Vector2<float> getPos() const;
    void setPos(Vector2<float> pos_);

    Vector2<float> getVelocity() const;
    Vector2<float> getInertia() const;
    Vector2<float> getFullVelocity() const;
    virtual void turnVelocityToInertia(float horMultiplier_ = 0.9f);

    virtual void updateOwnOrientation();
    virtual void updateDirToEnemy();
    virtual ORIENTATION getInputDir() const;
    Vector2<float> getHorDirToEnemy() const;
    Vector2<float> getOwnHorDir() const;
    ORIENTATION getOwnOrientation() const;
    ORIENTATION getOrientationToEnemy() const;

    virtual void initiate() = 0;
    virtual void loadAnimations(Application &application_) = 0;
    void callForPriority();

    virtual bool isAirborne() const;
    virtual bool isInHitstop() const;

    void applyCancelWindow(CancelWindow cw_);
    bool isCancelAllowed(int cancelTarget_);

    virtual HIT_RESULT applyHit(HitEvent &hitEvent_) = 0;

protected:
    void switchTo(int state_);

    Vector2<float> m_pos;
    Vector2<float> m_velocity;
    Vector2<float> m_inertia;
    float m_inertiaDrag = 1;
    bool m_airborne = false;

    Action *m_currentAction;
    ActionResolver m_actionResolver;
    int m_currentState;

    bool m_inHitstop = false;
    FrameTimer m_hitstopTimer;
    int m_extendedBuffer = 0;

    int m_playerId;

    Character *m_otherCharacter;
    PriorityHandler *m_priorityHandler = nullptr;

    std::map<ANIMATIONS, std::unique_ptr<Animation>> m_animations;
    Animation *m_currentAnimation;
    
    ORIENTATION m_dirToEnemy;
    ORIENTATION m_ownOrientation;

    FrameTimer m_timer;
    int framesInState = 1;
    Camera *m_cam = nullptr;

    CancelWindow m_currentCancelWindow;
    FrameTimer m_cancelTimer;
    bool m_cancelAvailable = false;

    friend Action_throw_startup;
    friend Action_throw_tech;
    friend Action_throw_hold;
    friend Action_thrown_hold;
    friend Action_throw_whiff;
    friend Action_locked_animation;
    friend ActionCharacter;
    friend Action_throw_startup;
    friend Action_throw_hold;
    friend Action_thrown_hold;
    friend Action_throw_whiff;
    friend Action_throw_tech;
    friend Action_locked_animation;
    friend Action_jump;
    friend Action_prolonged;
    friend Action_attack;
    friend Action_float;
    friend Action_airjump;
};

#endif