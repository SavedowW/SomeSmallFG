#include "Projectile.h"
#include "ActionProjectile.h"

Projectile::Projectile(Application &application_, Vector2<float> pos_, int stateCnt_, Camera *cam_, ParticleManager *particleManager_, int initialState_, ORIENTATION initialOrientation_) :
    InteractableStateMachine(application_, pos_, stateCnt_, cam_, particleManager_),
    m_initialState(initialState_)
{
    m_ownOrientation = initialOrientation_;
}

void Projectile::addAction(std::unique_ptr<Action> &&action_)
{
    m_actionResolver.addAction(std::move(action_));
}

void Projectile::setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_)
{
    m_timer.begin(0);

    m_playerId = playerId_;

    m_dirToEnemy = m_ownOrientation;

    m_otherCharacter = otherCharacter_;
    m_priorityHandler = priorityHandler_;

    loadAnimations(application_);

    initiate();
}

void Projectile::expireSelf()
{
    m_expired = true;
}

bool Projectile::isExpired() const
{
    return m_expired;
}

HitsVec Projectile::getHits()
{
    if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto hit = dynamic_cast<ActionProjectileHitProvider*>(m_currentAction)->getCurrentHit(m_pos, m_ownOrientation, *this);

        return {hit};
    }
    return {};
}

void Projectile::initiate()
{
    m_currentState = m_initialState;
    switchTo(m_currentState);
}

void Projectile::loadAnimations(Application &application_)
{
    m_animations[ANIMATIONS::CHAR1_PROJECTILE_STARTUP] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_PROJECTILE_STARTUP, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_PROJECTILE_FILLER] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_PROJECTILE_FILLER, LOOPMETHOD::JUMP_LOOP);
    m_animations[ANIMATIONS::CHAR1_PROJECTILE_FADEOUT] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_PROJECTILE_FADEOUT, LOOPMETHOD::NOLOOP);

    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_PROJECTILE_STARTUP].get();
    m_currentAnimation->reset();
}

HIT_RESULT Projectile::applyHit(HitEvent &hitEvent_)
{
    auto *act = dynamic_cast<ActionProjectile*>(m_currentAction);

    // Attacker's side
    if (hitEvent_.m_hittingPlayerId == m_playerId && hitEvent_.m_isHitByProjectile)
    {
        if (hitEvent_.m_hitRes == HIT_RESULT::NONE)
            return HIT_RESULT::NONE;

        act->ownHitApplied(hitEvent_, *this);

        callForPriority();

        applyHitstop(hitEvent_.m_hitData.hitProps.hitstop - 2);
            
        if (hitEvent_.m_hitRes == HIT_RESULT::HIT || hitEvent_.m_hitRes == HIT_RESULT::COUNTER)
            applyCancelWindow(hitEvent_.m_hitData.cancelsOnHit);
        else
            applyCancelWindow(hitEvent_.m_hitData.cancelsOnBlock);


        return HIT_RESULT::NONE;
    }

    return HIT_RESULT::NONE;
}

void Projectile::applyClash(const Hit &clashedHit_, int opponentsHitId_)
{
    auto *act = dynamic_cast<ActionProjectile*>(m_currentAction);

    applyHitstop(20);
    m_takenHits.insert(opponentsHitId_);

    act->ownHitClashed(clashedHit_, *this);
}
