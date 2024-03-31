#ifndef ACTION_PROJECTILE_H_
#define ACTION_PROJECTILE_H_
#include "Actions.h"
#include "Projectile.h"

class ActionProjectile: public Action
{
public:
    ActionProjectile(int actionState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, StateMarker transitionableFrom_, bool isAttack_, bool isAirborne_);
    virtual ~ActionProjectile() = default;

    virtual void outdated(InteractableStateMachine &character_) override;

    ActionProjectile *setOutdatedExpire(bool expireWhenOutdated_);

protected:
    bool m_expireWhenOutdated = false;

};

#endif