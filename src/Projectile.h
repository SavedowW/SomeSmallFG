#ifndef PROJECTILE_H_
#define PROJECTILE_H_
#define DEBUG_

#include "InteractableStateMachine.h"

class Projectile : public InteractableStateMachine
{
public:
    Projectile(Application &application_, Vector2<float> pos_, int stateCnt_, Camera *cam_, ParticleManager *particleManager_, int initialState_, ORIENTATION initialOrientation_);
    void addAction(std::unique_ptr<Action> &&action_);

    void setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_) override;

    void expireSelf();
    bool isExpired() const;

protected:
    virtual void initiate() override;
    virtual void loadAnimations(Application &application_) override;
    virtual HIT_RESULT applyHit(HitEvent &hitEvent_) override;

    int m_initialState;
    bool m_expired = false;

};

#endif