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
