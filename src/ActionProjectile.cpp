#include "ActionProjectile.h"

ActionProjectile::ActionProjectile(int actionState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, StateMarker transitionableFrom_, bool isAttack_, bool isAirborne_) :
    Action(actionState_, std::move(hurtboxes_), anim_, std::move(transitionableFrom_), isAttack_, isAirborne_)
{
}

void ActionProjectile::outdated(InteractableStateMachine &character_)
{
    Action::outdated(character_);

    auto *pt = dynamic_cast<Projectile*>(&character_);

    if (m_expireWhenOutdated)
        pt->expireSelf();
}

ActionProjectile *ActionProjectile::setOutdatedExpire(bool expireWhenOutdated_)
{
    m_expireWhenOutdated = expireWhenOutdated_;
    return this;
}

ActionProjectileHitProvider::ActionProjectileHitProvider(int actionState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, StateMarker transitionableFrom_, bool isAirborne_, Hit &&hitProto_, int hitCount_) :
    ActionProjectile(actionState_, std::move(hurtboxes_), anim_, std::move(transitionableFrom_), true, isAirborne_),
    m_hitProto(std::move(hitProto_)),
    m_maxHitCount(hitCount_)
{
}

void ActionProjectileHitProvider::switchTo(InteractableStateMachine &character_)
{
    ActionProjectile::switchTo(character_);

    m_hitsLeft = m_maxHitCount;
    m_hitProto.initializeHitID();
}

void ActionProjectileHitProvider::ownHitApplied(const HitEvent &ev_, InteractableStateMachine &owner_)
{
    m_hitsLeft--;
    m_hitUsed = true;

    if (m_hitsLeft <= 0)
        owner_.m_timer.begin(1);
}

void ActionProjectileHitProvider::ownHitClashed(const Hit &ev_, InteractableStateMachine &owner_)
{
    m_hitsLeft--;
    m_hitUsed = true;

    if (m_hitsLeft <= 0)
        owner_.m_timer.begin(1);
}

Hit ActionProjectileHitProvider::getCurrentHit(const Vector2<float>& offset_, ORIENTATION ownOrientation_, InteractableStateMachine &owner_)
{

    if (!owner_.isInHitstop() && m_hitUsed && m_hitsLeft > 0)
    {
        m_hitUsed = false;
        m_hitProto.initializeHitID();
    }

    auto nhit = m_hitProto;

    for (auto &el : nhit.m_hitboxes)
    {
        el.second = el.second.getHitboxAtOffset(offset_, ownOrientation_);
    }
    return nhit;
}
