#ifndef ACTION_PROJECTILE_H_
#define ACTION_PROJECTILE_H_
#include "Actions.h"
#include "Projectile.h"

class ActionProjectile: public Action
{
public:
    ActionProjectile(int actionState_, HurtboxFramesVec &&hurtboxes_, int anim_, StateMarker transitionableFrom_, bool isAttack_, bool isAirborne_);
    virtual ~ActionProjectile() = default;

    virtual void outdated(InteractableStateMachine &character_) override;

    virtual void ownHitApplied(const HitEvent &ev_, InteractableStateMachine &owner_) {};
    virtual void ownHitClashed(const Hit &ev_, InteractableStateMachine &owner_) {};

    ActionProjectile *setOutdatedExpire(bool expireWhenOutdated_);

protected:
    bool m_expireWhenOutdated = false;

};

class ActionProjectileHitProvider: public ActionProjectile
{
public:
    ActionProjectileHitProvider(int actionState_, HurtboxFramesVec &&hurtboxes_, int anim_, StateMarker transitionableFrom_, bool isAirborne_, Hit &&hitProto_, int hitCount_);

    virtual void switchTo(InteractableStateMachine &character_) override;

    virtual void ownHitApplied(const HitEvent &ev_, InteractableStateMachine &owner_) override;
    virtual void ownHitClashed(const Hit &ev_, InteractableStateMachine &owner_) override;
    virtual Hit getCurrentHit(const Vector2<float>& offset_, ORIENTATION ownOrientation_, InteractableStateMachine &owner_);

protected:
    Hit m_hitProto;
    int m_maxHitCount = 0;
    int m_hitsLeft = 0;
    bool m_hitUsed = false;

};

#endif