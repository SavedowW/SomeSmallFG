#include "Actions.h"
#include "Char1.h"
#include <stdexcept>

/* ============================
 *
 *       UNIVERSAL ABSTRACT ACTIONS
 *
 *========================== */

Action::Action(int actionState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isAttack_, bool isCrouchState_, bool isThrowStartup_,
        int consumeAirdash_, int consumeAirjump_, bool waitAirdashTimer_, bool waitAirjumpTimer_, bool isAirborne_) :
    actionState(actionState_),
    m_hurtboxes(std::move(hurtboxes_)),
    m_anim(anim_),
    m_isAttack(isAttack_),
    m_isCrouchState(isCrouchState_),
    m_isThrowStartup(isThrowStartup_),
    m_counterWindow(std::move(counterWindow_)),
    m_gravityWindow(std::move(gravityWindow_)),
    m_blockWindow(std::move(blockWindow_)),
    m_transitionableFrom(std::move(transitionableFrom_)),
    m_consumeAirdash(consumeAirdash_),
    m_consumeAirjump(consumeAirjump_),
    m_waitAirdashTimer(waitAirdashTimer_),
    m_waitAirjumpTimer(waitAirjumpTimer_),
    m_isAirborne(isAirborne_)
{
    incmp = std::move(incmp_);
}

bool Action::isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const
{
    if (incmp)
        return (*incmp)(inputQueue_, ownDirection_, extendBuffer_);
    else
        return false;
}

const HurtboxVec Action::getCurrentHurtboxes(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const
{
    HurtboxVec vec;
    for (const auto &el : m_hurtboxes)
    {
        if (el.first[currentFrame_])
        {
            auto hbox = el.second;
            if (ownOrientation_ == ORIENTATION::RIGHT)
            {
                hbox.x += offset_.x;
                hbox.y += offset_.y;
            }
            else
            {
                hbox.x = offset_.x - hbox.x - hbox.w;
                hbox.y += offset_.y;
            }
            vec.push_back(hbox);
        }
    }

    return vec;
}

void Action::outdated(Character &character_)
{
    if (m_setRealignOtd)
        character_.updateOwnOrientation();

    if (m_setThrowInvulOtd)
        character_.setThrowInvul();

    if (m_setAirborneOtd)
        character_.m_airborne = true;

    if (m_enterHitstunOtd)
        character_.enterHitstunAnimation(character_.m_hitProps);

    if (m_setAboveGroundOtd)
        character_.m_pos.y = gamedata::stages::levelOfGround - 1;

    character_.m_velocity = character_.m_velocity.mulComponents(m_mulOwnVelOtd) + character_.getOwnHorDir().mulComponents(m_mulOwnDirVelOtd) + m_rawAddVelOtd;
    character_.m_inertia = character_.m_inertia.mulComponents(m_mulOwnInrOtd) + character_.getOwnHorDir().mulComponents(m_mulOwnDirInrOtd) + m_rawAddInrOtd;

    if (m_targetStateOutdated != -1)
        character_.m_actionResolver.getAction(m_targetStateOutdated)->switchTo(character_);
}

void Action::switchTo(Character &character_)
{
    if (character_.m_currentAction && character_.m_currentAction->m_isAttack)
        dynamic_cast<Action_attack*>(character_.m_currentAction)->resetOpponentsHits(character_);

    auto oldState = character_.m_currentState;

    if (m_velToInertia)
        character_.turnVelocityToInertia();

    character_.m_currentAction = this;
    character_.applyCancelWindow({{0, 0}, {}});
    character_.framesInState = 0;
    if (m_hitstunAnimation != -1)
        character_.m_hitstunAnimation = (HITSTUN_ANIMATION)m_hitstunAnimation;
    character_.m_blockstunType = BLOCK_FRAME::NONE;
    character_.m_currentState = actionState;

    if (m_anim != ANIMATIONS::NONE)
    {
        character_.m_currentAnimation = character_.m_animations[m_anim].get();
        character_.m_currentAnimation->reset(m_animResetFrame, m_animResetDirection);
    }

    character_.m_timer.begin(m_timerValue);

    if (m_realign || character_.m_autoRealignAfter.getMark(oldState))
        character_.updateOwnOrientation();

    if (m_setAirAttackFlag && character_.isAirborne())
        character_.m_usedAirAttack = true;

    if (m_resetDefenseState)
    {
        character_.m_healthHandler.resetScaling();
        character_.m_comboPhysHandler.reset();
    }

    if (m_consumeAirdash)
        character_.m_airdashesAvailable.consume(m_consumeAirdash);

    if (m_consumeAirjump)
        character_.m_jumpsAvailable.consume(m_consumeAirjump);

    if (m_resetPushback)
        character_.takePushback({0.0f, 0.0f});

    if (m_callForPriority)
        character_.callForPriority();

    character_.m_velocity = character_.m_velocity.mulComponents(m_mulOwnVel) + character_.getOwnHorDir().mulComponents(m_mulOwnDirVel) + m_rawAddVel;
    character_.m_inertia = character_.m_inertia.mulComponents(m_mulOwnInr) + character_.getOwnHorDir().mulComponents(m_mulOwnDirInr) + m_rawAddInr;
}

// TODO: Add particle generation
void Action::update(Character &character_)
{
    if (character_.m_inHitstop)
        return;
        
    auto frame = character_.m_timer.getCurrentFrame() + 1;

    if (m_updRealign[frame])
        character_.updateOwnOrientation();

    if (m_usingUpdateMovement)
    {
        character_.m_velocity = character_.m_velocity.mulComponents(m_mulOwnVelUpd[frame]) + character_.getOwnHorDir().mulComponents(m_mulOwnDirVelUpd[frame]) + m_rawAddVelUpd[frame];
        character_.m_inertia = character_.m_inertia.mulComponents(m_mulOwnInrUpd[frame]) + character_.getOwnHorDir().mulComponents(m_mulOwnDirInrUpd[frame]) + m_rawAddInrUpd[frame];
    }
    
    if (!m_ownVelLimitUpd.isEmpty())
        character_.m_velocity = utils::limitVectorLength(character_.m_velocity, m_ownVelLimitUpd[frame]);

    if (!m_ownInrLimitUpd.isEmpty())
        character_.m_inertia = utils::limitVectorLength(character_.m_inertia, m_ownInrLimitUpd[frame]);

    auto camShakeData = m_camShakeUpd[frame];
    if (camShakeData.y != 0)
        character_.m_cam->startShake(camShakeData.x, camShakeData.y);
}

bool Action::onLand(Character &character_)
{
    character_.m_velocity = character_.m_velocity.mulComponents(m_mulOwnVelLand) + character_.getOwnHorDir().mulComponents(m_mulOwnDirVelLand) + m_rawAddVelLand;
    character_.m_inertia = character_.m_inertia.mulComponents(m_mulOwnInrLand) + character_.getOwnHorDir().mulComponents(m_mulOwnDirInrLand) + m_rawAddInrLand;
    if (m_recoveryOnLand >= 0)
    {
        character_.switchTo(m_recoveryOnLand);
        return true;
    }

    return m_recoveryOnLand != -1;
}

bool Action::isInCounterState(uint32_t currentFrame_) const
{
    return m_counterWindow[currentFrame_];
}

bool Action::applyGravity(uint32_t currentFrame_) const
{
    return m_gravityWindow[currentFrame_];
}

bool Action::canBlock(uint32_t currentFrame_) const
{
    return m_blockWindow[currentFrame_];
}

int Action::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne() != m_isAirborne)
        return 0;

    if (m_waitAirdashTimer)
        if (!char_->m_airdashTimer.isOver())
            return 0;

    if (m_waitAirjumpTimer)
        if (!char_->m_airjumpTimer.isOver())
            return 0;

    if (m_consumeAirdash)
        if (!char_->m_airdashesAvailable.canConsume(m_consumeAirdash))
            return 0;

    if (m_consumeAirjump)
        if (!char_->m_jumpsAvailable.canConsume(m_consumeAirjump))
            return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    if (actionState == char_->m_currentState)
        return responseOnOwnState(inputQueue_, char_->getInputDir(), extendBuffer_);

    if (m_transitionableFrom.getMark(char_->m_currentState))
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    return 0;
}

int Action::responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const
{
    return 0;
}

Action *Action::setSwitchData(bool realign_, int timerValue_, bool velToInertia_, bool resetDefenseState_, bool setAirAttackFlag_, bool resetPushback_, bool callForPriority_, Vector2<float> mulOwnVel_, Vector2<float> mulOwnInr_, Vector2<float> mulOwnDirVel_, Vector2<float> mulOwnDirInr_, Vector2<float> rawAddVel_, Vector2<float> rawAddInr_)
{
    m_realign = realign_;
    m_timerValue = timerValue_;
    m_velToInertia = velToInertia_;
    m_resetDefenseState = resetDefenseState_;
    m_setAirAttackFlag = setAirAttackFlag_;
    m_mulOwnVel = mulOwnVel_;
    m_mulOwnInr = mulOwnInr_;
    m_mulOwnDirVel = mulOwnDirVel_;
    m_mulOwnDirInr = mulOwnDirInr_;
    m_rawAddVel = rawAddVel_;
    m_rawAddInr = rawAddInr_;
    m_resetPushback = resetPushback_;
    m_callForPriority = callForPriority_;

    return this;
}

Action *Action::setHitstunAnimation(int hitstunAnim_)
{
    m_hitstunAnimation = hitstunAnim_;

    return this;
}

Action *Action::setAnimResetData(int animResetFrame_, int animResetDirection_)
{
    m_animResetFrame = animResetFrame_;
    m_animResetDirection = animResetDirection_;

    return this;
}

Action *Action::setUpdateMovementData(TimelineProperty<Vector2<float>> &&mulOwnVelUpd_, TimelineProperty<Vector2<float>> &&mulOwnInrUpd_, TimelineProperty<Vector2<float>> &&mulOwnDirVelUpd_,
TimelineProperty<Vector2<float>> &&mulOwnDirInrUpd_, TimelineProperty<Vector2<float>> &&rawAddVelUpd_, TimelineProperty<Vector2<float>> &&rawAddInrUpd_)
{
    m_mulOwnVelUpd = std::move(mulOwnVelUpd_);
    m_mulOwnInrUpd = std::move(mulOwnInrUpd_);
    m_mulOwnDirVelUpd = std::move(mulOwnDirVelUpd_);
    m_mulOwnDirInrUpd = std::move(mulOwnDirInrUpd_);
    m_rawAddVelUpd = std::move(rawAddVelUpd_);
    m_rawAddInrUpd = std::move(rawAddInrUpd_);

    m_usingUpdateMovement = !m_mulOwnVelUpd.isEmpty() || !m_mulOwnInrUpd.isEmpty() || !m_mulOwnDirVelUpd.isEmpty() ||
    !m_mulOwnDirInrUpd.isEmpty() || !m_rawAddVelUpd.isEmpty() || !m_rawAddInrUpd.isEmpty();

    return this;
}

Action *Action::setUpdateSpeedLimitData(TimelineProperty<float> &&ownVelLimitUpd_, TimelineProperty<float> &&ownInrLimitUpd_)
{
    m_ownVelLimitUpd = std::move(ownVelLimitUpd_);
    m_ownInrLimitUpd = std::move(ownInrLimitUpd_);

    return this;
}

Action *Action::setUpdateCamShakeData(TimelineProperty<Vector2<int>> &&camShakeUpd_)
{
    m_camShakeUpd = std::move(camShakeUpd_);

    return this;
}

Action *Action::setUpdateRealignData(TimelineProperty<bool> &&updRealign_)
{
    m_updRealign = std::move(updRealign_);

    return this;
}

Action *Action::setOutdatedFlags(bool setRealign_, bool setThrowInvul_, bool setAirborne_, bool enterHitstun_, bool setAboveGroundOtd_)
{
    m_setRealignOtd = setRealign_;
    m_setThrowInvulOtd = setThrowInvul_;
    m_setAirborneOtd = setAirborne_;
    m_enterHitstunOtd = enterHitstun_;
    m_setAboveGroundOtd = setAboveGroundOtd_;

    return this;
}

Action *Action::setOutdatedTransition(int targetState_)
{
    m_targetStateOutdated = targetState_;

    return this;
}

Action *Action::setOutdatedMovementData(Vector2<float> mulOwnVel_, Vector2<float> mulOwnInr_, Vector2<float> mulOwnDirVel_, Vector2<float> mulOwnDirInr_, Vector2<float> rawAddVel_, Vector2<float> rawAddInr_)
{
    m_mulOwnVelOtd = mulOwnVel_;
    m_mulOwnInrOtd = mulOwnInr_;
    m_mulOwnDirVelOtd = mulOwnDirVel_;
    m_mulOwnDirInrOtd = mulOwnDirInr_;
    m_rawAddVelOtd = rawAddVel_;
    m_rawAddInrOtd = rawAddInr_;

    return this;
}

Action *Action::setDisadvantageFlags(bool isBlockstun_, bool isHitstun_, bool isKnockdown_)
{
    m_isBlockstun = isBlockstun_;
    m_isHitstun = isHitstun_;
    m_isKnockdown = isKnockdown_;

    return this;
}

Action *Action::setLandingMovementData(Vector2<float> mulOwnVel_, Vector2<float> mulOwnInr_, Vector2<float> mulOwnDirVel_, Vector2<float> mulOwnDirInr_, Vector2<float> rawAddVel_, Vector2<float> rawAddInr_)
{
    m_mulOwnVelLand = mulOwnVel_;
    m_mulOwnInrLand = mulOwnInr_;
    m_mulOwnDirVelLand = mulOwnDirVel_;
    m_mulOwnDirInrLand = mulOwnDirInr_;
    m_rawAddVelLand = rawAddVel_;
    m_rawAddInrLand = rawAddInr_;

    return this;
}

Action *Action::setLandingRecoveryState(int recoveryState_)
{
    m_recoveryOnLand = recoveryState_;
    return this;
}

// ABSTRACT PROLONGED ACTION
Action_prolonged::Action_prolonged(int actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isCrouchState_, int consumeAirdash_, int consumeAirjump_, bool waitAirdashTimer_, bool waitAirjumpTimer_, bool isAirborne_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, std::move(counterWindow_), std::move(gravityWindow_), std::move(blockWindow_), std::move(transitionableFrom_), false, isCrouchState_, false, consumeAirdash_, consumeAirjump_, waitAirdashTimer_, waitAirjumpTimer_, isAirborne_)
{
    incmp_prolonged = std::move(incmp_prolonged_);
}

int Action_prolonged::isPossibleToProlong(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const
{
    return (*incmp_prolonged)(inputQueue_, ownDirection_);
}

int Action_prolonged::responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const
{
    return (isPossibleToProlong(inputQueue_, ownDirection_) ? -1 : 0);
}

// ABSTRACT JUMP ACTION
Action_jump::Action_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, std::move(counterWindow_), TimelineProperty(true), std::move(blockWindow_), std::move(transitionableFrom_), false, false, false, 0, 0, false, false, false),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
    setSwitchData(false, prejumpLen_, true, false, false, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}

Action_jump *Action_jump::setAirActionTimers(int airjumpTimerValue_, int airdashTimerValue_)
{
    m_airjumpTimerValue = airjumpTimerValue_;
    m_airdashTimerValue = airdashTimerValue_;
    return this;
}

void Action_jump::switchTo(Character &character_)
{
    Action::switchTo(character_);

    character_.m_airjumpTimer.begin(m_airjumpTimerValue);
    character_.m_airdashTimer.begin(m_airdashTimerValue);
}


// ABSTRACT AIR JUMP ACTION
Action_airjump::Action_airjump(int actionState_, const Vector2<float> &impulse_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, StateMarker transitionableFrom_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(false), TimelineProperty(true), std::move(true), std::move(transitionableFrom_), false, false, false,
    0, 1, false, true, true),
    m_impulse(impulse_)
{
}

int Action_airjump::responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const
{
    return (isInputPossible(inputQueue_, ownDirection_, extendBuffer_) ? 1 : 0);
}

int Action_airjump::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne() != m_isAirborne)
        return 0;

    if (!char_->m_airjumpTimer.isOver())
        return 0;

    if (!char_->m_jumpsAvailable.canConsume(m_consumeAirjump))
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getOrientationToEnemy(), extendBuffer_) ? 1 : 0);
    }

    if (actionState == char_->m_currentState)
        return responseOnOwnState(inputQueue_, char_->getOrientationToEnemy(), extendBuffer_);

    if (m_transitionableFrom.getMark(char_->m_currentState))
        return (isInputPossible(inputQueue_, char_->getOrientationToEnemy(), extendBuffer_) ? 1 : 0);

    return 0;
}

// ABSTRACT ATTACK ACTION
Action_attack::Action_attack(int actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, StateMarker transitionableFrom_, bool isCrouchState_, bool isAirborne_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, hitutils::getRegularCounterTimeline(hits_), std::move(gravityWindow_), TimelineProperty(false), std::move(transitionableFrom_), true, isCrouchState_, false,
    0, 0, false, false, isAirborne_),
    m_fullDuration(fullDuration_),
    m_hits(hits_)
{
    setSwitchData(false, m_fullDuration, true, true, true, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}

const HitsVec Action_attack::getCurrentHits(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const
{
    HitsVec vec;
    for (const auto &el : m_hits)
    {
        if (el.first.first <= currentFrame_ && el.first.second >= currentFrame_)
        {
            auto hit = el.second;

            int i = 0;
            while (i < hit.m_hitboxes.size())
            {
                if (currentFrame_ < hit.m_hitboxes[i].first.first || currentFrame_ > hit.m_hitboxes[i].first.second)
                {
                    hit.m_hitboxes.erase(hit.m_hitboxes.begin() + i);
                }
                else
                {
                    hit.m_hitboxes[i].second = hit.m_hitboxes[i].second.getHitboxAtOffset(offset_, ownOrientation_);
                    i++;
                }
            }
        
            vec.push_back(hit);
        }
    }

    return vec;
}

void Action_attack::resetOpponentsHits(Character &character_)
{
    for (auto &el : m_hits)
        character_.m_otherCharacter->removeTakenHit(el.second.m_hitId);
}

void Action_attack::switchTo(Character &character_)
{
    Action::switchTo(character_);

    for (auto &el : m_hits)
        el.second.initializeHitID();
}

const bool Action_attack::isActive(uint32_t currentFrame_) const
{
    for (const auto &el : m_hits)
    {
        if (el.first.first <= currentFrame_ && el.first.second >= currentFrame_)
        {
            auto hit = el.second;

            for (auto &hb : hit.m_hitboxes)
                if (currentFrame_ >= hb.first.first && currentFrame_ <= hb.first.second)
                    return true;
    
        }
    }

    return false;
}

// ABSTRACT THROW STARTUP
Action_throw_startup::Action_throw_startup(int actionState_, int whiffState_, int holdState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float range_, FrameWindow activeWindow_, bool requiredAirborne_, THROW_LIST throw_, StateMarker transitionableFrom_, bool isAirborne_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(true), std::move(gravityWindow_), TimelineProperty(false), std::move(transitionableFrom_), false, false, true,
    0, 0, false, false, isAirborne_),
    m_holdState(holdState_),
    m_range(range_),
    m_activeWindow(activeWindow_),
    m_requiredAirborne(requiredAirborne_),
    m_throw(throw_)
{
    setSwitchData(false, activeWindow_.second, true, true, true, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition(whiffState_);
}

void Action_throw_startup::attemptThrow(Character &character_) const
{
    auto currentFrame = character_.m_timer.getCurrentFrame() + 1;
    if (currentFrame < m_activeWindow.first || currentFrame > m_activeWindow.second)
        return;

    if (character_.m_dirToEnemy != character_.m_ownOrientation || character_.m_otherCharacter->m_airborne != m_requiredAirborne)
        return;

    float realrange = (character_.m_pos - character_.m_otherCharacter->m_pos).getLen();
    if (realrange <= m_range && character_.m_otherCharacter->canBeThrown(m_throw))
    {
        character_.m_actionResolver.getAction(m_holdState)->switchTo(character_);
        character_.m_otherCharacter->enterThrown(m_throw);
        character_.callForPriority();
    }
}

// ABSTRACT THROW HOLD
Action_throw_hold::Action_throw_hold(int actionState_, int throwState_, float setRange_, float duration_, bool sideSwitch_) :
    Action(actionState_, nullptr, {}, ANIMATIONS::NONE, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false,
    0, 0, false, false, false),
    m_setRange(setRange_),
    m_duration(duration_),
    m_sideSwitch(sideSwitch_)
{
    setSwitchData(false, duration_, true, true, true, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition(throwState_);
}

void Action_throw_hold::switchTo(Character &character_)
{
    Action::switchTo(character_);

    Character &otherChar = *character_.m_otherCharacter;
    if (character_.m_ownOrientation == ORIENTATION::RIGHT)
        otherChar.m_pos.x = character_.m_pos.x + m_setRange;
    else
        otherChar.m_pos.x = character_.m_pos.x - m_setRange;

    otherChar.m_pos.y = character_.m_pos.y;

    otherChar.lockInAnimation();
    character_.lockInAnimation();
    character_.tieAnimWithOpponent();

    character_.updateDirToEnemy();
    character_.updateOwnOrientation();
    otherChar.updateDirToEnemy();
    otherChar.updateOwnOrientation();
}

void Action_throw_hold::outdated(Character &character_)
{
    //character_.switchToIdle();
    if (m_sideSwitch)
    {
        Character &otherChar = *character_.m_otherCharacter;
        if (character_.m_ownOrientation == ORIENTATION::RIGHT)
            otherChar.m_pos.x = character_.m_pos.x - m_setRange;
        else
            otherChar.m_pos.x = character_.m_pos.x + m_setRange;

        character_.updateDirToEnemy();
        character_.updateOwnOrientation();
        otherChar.updateDirToEnemy();
        otherChar.updateOwnOrientation();
    }

    Action::outdated(character_);
}

// ABSTRACT THROW WHIFF
Action_throw_whiff::Action_throw_whiff(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float duration_, HurtboxFramesVec &&hurtboxes_, int idleState_, int floatState_) :
    Action(actionState_, nullptr, std::move(hurtboxes_), anim_, TimelineProperty(false), std::move(gravityWindow_), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false,
    0, 0, false, false, false),
    m_duration(duration_),
    m_idleState(idleState_),
    m_floatState(floatState_)
{
    setSwitchData(false, duration_, false, true, false, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}

void Action_throw_whiff::outdated(Character &character_)
{
    if (!character_.m_airborne)
        character_.m_actionResolver.getAction(m_idleState)->switchTo(character_);
    else
        character_.m_actionResolver.getAction(m_floatState)->switchTo(character_);

    Action::outdated(character_);
}

// ABSTRACT THROWN HOLD
Action_thrown_hold::Action_thrown_hold(int actionState_, int thrownState_, ANIMATIONS anim_, float duration_) :
    Action(actionState_, nullptr, {}, anim_, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false,
    0, 0, false, false, false),
    m_duration(duration_)
{
    setSwitchData(false, duration_, true, true, true, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition(thrownState_);
}

void Action_thrown_hold::switchTo(Character &character_)
{
    Action::switchTo(character_);

    character_.lockInAnimation();
    character_.tieAnimWithOpponent();
}

// ABSTRACT THROW TECH
Action_throw_tech::Action_throw_tech(int actionState_, InputComparator_ptr incmp_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, float duration_, HurtboxFramesVec &&hurtboxes_, THROW_TECHS_LIST throwTech_, StateMarker transitionableFrom_, bool isAirborne_, int idleState_, int floatState_) :    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(false), std::move(gravityWindow_), std::move(blockWindow_), std::move(transitionableFrom_), false, false, false,
    0, 0, false, false, isAirborne_),
    m_duration(duration_),
    m_throwTech(throwTech_),
    m_idleState(idleState_),
    m_floatState(floatState_)
{
}

void Action_throw_tech::switchTo(Character &character_)
{
    Action::switchTo(character_);
    
    character_.releaseFromAnimation();
    if (character_.m_tiedAnimWithOpponent)
    {
        character_.untieAnimWithOpponent();
        character_.m_otherCharacter->throwTeched(m_throwTech);
    }
}

void Action_throw_tech::outdated(Character &character_)
{
    if (!character_.m_airborne)
        character_.m_actionResolver.getAction(m_idleState)->switchTo(character_);
    else
        character_.m_actionResolver.getAction(m_floatState)->switchTo(character_);

    Action::outdated(character_);
}

// ABSTRACT LOCKED ANIMATION
Action_locked_animation::Action_locked_animation(int actionState_, int quitState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, float duration_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_) :
    Action(actionState_, nullptr, std::move(hurtboxes_), anim_, std::move(counterWindow_), TimelineProperty(false), std::move(blockWindow_), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false,
    0, 0, false, false, false),
    m_duration(duration_)
{
    setSwitchData(false, duration_, true, true, true, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition(quitState_);
}

Action_locked_animation *Action_locked_animation::setUpdateHitsToOpponent(TimelineProperty<HitData*> &&hitsToOpponent_)
{
    m_hitsToOpponent = std::move(hitsToOpponent_);

    return this;
}

void Action_locked_animation::switchTo(Character &character_)
{
    Action::switchTo(character_);
    character_.lockInAnimation();
}

void Action_locked_animation::update(Character &character_)
{
    Action::update(character_);

    auto frame = character_.m_timer.getCurrentFrame() + 1;

    auto curHit = m_hitsToOpponent[frame];

    if (curHit && !character_.m_inHitstop)
    {
        curHit->initializeHitID();
        HitEvent ev;
        ev.m_hitData = *curHit;
        ev.m_hitRes = HIT_RESULT::THROWN;
        ev.m_hittingPlayerId = character_.m_playerId;
        character_.m_otherCharacter->applyHit(ev);
        character_.applyHit(ev);
        character_.m_cam->startShake(ev.m_hitData.hitBlockShakeAmp, ev.m_hitData.hitProps.hitstop + 1);
    }
}


void Action_locked_animation::outdated(Character &character_)
{
    character_.releaseFromAnimation();
    character_.untieAnimWithOpponent();
    Action::outdated(character_);
}


// FLOAT ACTION
Action_float::Action_float(int actionState_, int realState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_) :
    Action(actionState_, std::move(std::make_unique<InputComparatorUpHold>()), std::move(hurtboxes_), anim_, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false,
    0, 0, false, false, true),
    m_realState(realState_)
{
}

void Action_float::switchTo(Character &character_)
{
    Action::switchTo(character_);
    character_.m_currentState = m_realState;
}



/* ============================
 *
 *       CHAR 1 ACTIONS
 *
 *========================== */

// IDLE ACTION
Action_char1_idle::Action_char1_idle() :
    Action((int)CHAR1_STATE::IDLE, std::make_unique<InputComparatorIdle>(), {{TimelineProperty(true), gamedata::characters::char1::standingHurtbox}}, ANIMATIONS::CHAR1_IDLE, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::WALK_BWD}),
    false, false, false,
    0, 0, false, false, false)
{
    setSwitchData(true, 0, true, true, false, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setUpdateRealignData(
        TimelineProperty(true)
    );
}

// CROUCH ACTION
Action_char1_crouch::Action_char1_crouch() :
    Action_prolonged((int)CHAR1_STATE::CROUCH, std::make_unique<InputComparatorDownHold>(), std::make_unique<InputComparatorDownHold>(), {{TimelineProperty(true), gamedata::characters::char1::crouchingHurtbox}}, ANIMATIONS::CHAR1_CROUCH_IDLE, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::WALK_BWD}),
    true,
    0, 0, false, false, false)
{
    setSwitchData(true, 0, true, true, false, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setUpdateRealignData(
        TimelineProperty(true)
    );
}


// WALK FORWARD ACTION
Action_char1_walk_fwd::Action_char1_walk_fwd() :
    Action_prolonged((int)CHAR1_STATE::WALK_FWD, std::make_unique<InputComparatorForward>(), std::make_unique<InputComparatorForward>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_WALK_FWD, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::WALK_BWD}),
    false,
    0, 0, false, false, false)
{
    setSwitchData(false, 0, true, true, false, false, false, {0.0f, 0.0f}, {1.0f, 1.0f}, {6.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setUpdateRealignData(
        TimelineProperty(true)
    );
}

// WALK BACKWARD ACTION
Action_char1_walk_bwd::Action_char1_walk_bwd() :
    Action_prolonged((int)CHAR1_STATE::WALK_BWD, std::make_unique<InputComparatorBackward>(), std::make_unique<InputComparatorBackward>(), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_WALK_BWD, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::WALK_FWD}),
    false,
    0, 0, false, false, false)
{
    setSwitchData(false, 0, true, true, false, false, false, {0.0f, 0.0f}, {1.0f, 1.0f}, {-6.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setUpdateRealignData(
        TimelineProperty(true)
    );
}

// ABSTRACT CHAR1 JUMP ACTION
Action_char1_jump::Action_char1_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_) :
    Action_jump((int)actionState_, impulse_, prejumpLen_, maxHorInertia_, std::move(incmp_), std::move(hurtboxes_), ANIMATIONS::CHAR1_PREJUMP, TimelineProperty(false), TimelineProperty(true), StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD, (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::IDLE})),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
    setAirActionTimers(5, 6);
}

// NEUTRAL JUMP ACTION
Action_char1_neutral_jump::Action_char1_neutral_jump() :
    Action_char1_jump((int)CHAR1_STATE::PREJUMP,
    {0.0f, -gamedata::characters::char1::groundJumpVerticalImpulse},
    gamedata::characters::char1::prejumpFrames, gamedata::characters::char1::groundJumpMaxHorizontalInertia,
    std::make_unique<InputComparatorUpHold>(),
    {
        {
            TimelineProperty<bool>({{0, true}, {5, false}}),
            {-70, -50, 140, 50}
        },
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    })
{
}

// FORWARD JUMP ACTION
Action_char1_forward_jump::Action_char1_forward_jump() :
    Action_char1_jump((int)CHAR1_STATE::PREJUMP,
    {gamedata::characters::char1::groundJumpHorizontalImpulse, -gamedata::characters::char1::groundJumpVerticalImpulse},
    gamedata::characters::char1::prejumpFrames, gamedata::characters::char1::groundJumpMaxHorizontalInertia,
    std::make_unique<InputComparatorUpForwardHold>(),
    {
        {
            TimelineProperty<bool>({{0, true}, {5, false}}),
            {-70, -50, 140, 50}
        },
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    })
{
}

// BACKWARD JUMP ACTION
Action_char1_backward_jump::Action_char1_backward_jump() :
    Action_char1_jump((int)CHAR1_STATE::PREJUMP,
    {-gamedata::characters::char1::groundJumpHorizontalImpulse, -gamedata::characters::char1::groundJumpVerticalImpulse},
    gamedata::characters::char1::prejumpFrames, gamedata::characters::char1::groundJumpMaxHorizontalInertia,
    std::make_unique<InputComparatorUpBackwardHold>(),
    {
        {
            TimelineProperty<bool>({{0, true}, {5, false}}),
            {-70, -50, 140, 50}
        },
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    })
{
}


// ABSTRACT CHAR1 AIR JUMP ACTION
Action_char1_airjump::Action_char1_airjump(const Vector2<float> &impulse_, InputComparator_ptr incmp_) :
    Action_airjump((int)CHAR1_STATE::JUMP, impulse_, std::move(incmp_), {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_JUMP,
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}))
{
    setSwitchData(true, 0, false, true, false, false, false, {0.0f, 0.0f}, {1.0f, 0.0f}, {impulse_.x, 0.0f}, {0.0f, 0.0f}, {0.0f, impulse_.y}, {0.0f, 0.0f});
}

// NEUTRAL DOUBLEJUMP ACTION
Action_char1_neutral_doublejump::Action_char1_neutral_doublejump() :
    Action_char1_airjump({0.0f, -gamedata::characters::char1::airJumpVerticalImpulse}, std::make_unique<InputComparatorUpPress>())
{
}

// FORWARD DOUBLEJUMP ACTION
Action_char1_forward_doublejump::Action_char1_forward_doublejump() :
    Action_char1_airjump({gamedata::characters::char1::airJumpHorizontalImpulse, -gamedata::characters::char1::airJumpVerticalImpulse}, std::make_unique<InputComparatorUpForwardPress>())
{
}

// BACKWARD DOUBLEJUMP ACTION
Action_char1_backward_doublejump::Action_char1_backward_doublejump() :
    Action_char1_airjump({-gamedata::characters::char1::airJumpHorizontalImpulse, -gamedata::characters::char1::airJumpVerticalImpulse}, std::make_unique<InputComparatorUpBackwardPress>())
{
}




// GROUND DASH ACTION
Action_char1_ground_dash::Action_char1_ground_dash() :
    Action_prolonged((int)CHAR1_STATE::GROUND_DASH, std::move(std::make_unique<InputComparator66>()), std::move(std::make_unique<InputComparatorForward>()), 
    {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_GROUND_DASH, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::WALK_BWD}),
    false,
    0, 0, false, false, false),
    m_accel(gamedata::characters::char1::dashAccel),
    m_maxspd(gamedata::characters::char1::dashMaxSpeed)
{
    setUpdateMovementData(
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{m_accel, 0.0f}}
        }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    );

    setUpdateSpeedLimitData(
        TimelineProperty<float>(m_maxspd),
        TimelineProperty<float>()
    );
}

// STEP ACTION
Action_char1_step::Action_char1_step() :
    Action((int)CHAR1_STATE::STEP, std::move(std::make_unique<InputComparator66>()), {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_STEP, TimelineProperty(true), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false),
    m_duration(gamedata::characters::char1::stepDuration)
{
    setSwitchData(true, m_duration, false, true, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, Vector2{gamedata::characters::char1::stepSpeed, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::STEP_RECOVERY);
}


// STEP RECOVERY ACTION
Action_char1_step_recovery::Action_char1_step_recovery() :
    Action((int)CHAR1_STATE::STEP_RECOVERY, std::move(std::make_unique<InputComparator66>()), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_STEP_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false),
    m_recoveryLen(gamedata::characters::char1::stepRecoveryDuration)
{
    setSwitchData(false, m_recoveryLen, true, false, false, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
}


// GROUND BACKDASH ACTION
Action_char1_ground_backdash::Action_char1_ground_backdash() :
    Action((int)CHAR1_STATE::GROUND_BACKDASH, std::move(std::make_unique<InputComparator44>()), {
        {
            TimelineProperty<bool>({{gamedata::characters::char1::backdashInvulDuration + 1, true}}),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_BACKDASH, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::WALK_BWD, (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::IDLE}),
    false, false, false,
    0, 0, false, false, false),
    m_totalDuration(gamedata::characters::char1::backdashDuration)
{
    setSwitchData(false, m_totalDuration, true, false, false, false, false, {1.0f, 1.0f}, {0.0f, 0.0f}, {-gamedata::characters::char1::backdashSpd, 0.0f}, {0.0f, 0.0f}, {0.0f, -14.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
    setLandingMovementData({0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -0.0f}, {0.0f, 0.0f});
    setLandingRecoveryState(-2);
}


// AIR DASH ACTION
Action_char1_air_dash::Action_char1_air_dash() :
    Action((int)CHAR1_STATE::AIR_DASH, std::move(std::make_unique<InputComparator66>()), {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_AIRDASH, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::JUMP}),
    false, false, false,
    1, 0, true, false, true),
    m_duration(gamedata::characters::char1::airdashDuration)
{
    setSwitchData(false, m_duration, false, false, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {gamedata::characters::char1::airdashSpeed, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::AIR_DASH_EXTENTION);
}


// AIR BACKDASH ACTION
Action_char1_air_backdash::Action_char1_air_backdash() :
    Action((int)CHAR1_STATE::AIR_BACKDASH, std::move(std::make_unique<InputComparator44>()), {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_AIR_BACKDASH, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::JUMP}),
    false, false, false,
    1, 0, true, false, true),
    m_duration(gamedata::characters::char1::airBackdashDuration)
{
    setSwitchData(false, m_duration, false, false, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {-gamedata::characters::char1::airBackdashSpeed, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -10.0f});
    setOutdatedTransition((int)CHAR1_STATE::FLOAT);
    setOutdatedMovementData({0.2f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}


// AIR DASH EXTENTION ACTION
Action_char1_air_dash_extention::Action_char1_air_dash_extention() :
    Action((int)CHAR1_STATE::AIR_DASH_EXTENTION, std::move(std::make_unique<InputComparatorIdle>()), {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_AIRDASH, TimelineProperty(false), TimelineProperty(false), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, true),
    m_duration(gamedata::characters::char1::airdashExtentionDuration),
    m_baseSpd(gamedata::characters::char1::airdashExtentionMaxSpeed),
    m_spdMultiplier(0.66f)
{
    setSwitchData(false, m_duration, false, false, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {(float)m_baseSpd, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});

    setUpdateMovementData(
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{-m_spdMultiplier, 0.0f}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    );

    setOutdatedTransition((int)CHAR1_STATE::FLOAT);
    setOutdatedFlags(true, false, false, false, false);
}

// GROUND DASH RECOVERY ACTION
Action_char1_ground_dash_recovery::Action_char1_ground_dash_recovery() :
    Action((int)CHAR1_STATE::GROUND_DASH_RECOVERY, std::move(std::make_unique<InputComparatorIdle>()), {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_GROUND_DASH_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::GROUND_DASH}),
    false, false, false,
    0, 0, false, false, false),
    m_recoveryLen(gamedata::characters::char1::dashRecovery)
{
    setSwitchData(false, m_recoveryLen, true, false, false, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
}


// SOFT LANDING RECOVERY
Action_char1_soft_landing_recovery::Action_char1_soft_landing_recovery() :
    Action((int)CHAR1_STATE::SOFT_LANDING_RECOVERY, nullptr, {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false),
    m_recoveryLen(gamedata::characters::char1::softLandingRecovery)
{
    setSwitchData(false, m_recoveryLen, false, false, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
}


// HARD LANDING RECOVERY
Action_char1_hard_landing_recovery::Action_char1_hard_landing_recovery() :
    Action((int)CHAR1_STATE::HARD_LANDING_RECOVERY, nullptr, {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false),
    m_recoveryLen(gamedata::characters::char1::hardLandingRecovery)
{
    setSwitchData(false, m_recoveryLen, false, false, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
    setDisadvantageFlags(true, false, false);
}


// VULNERABLE LANDING RECOVERY
Action_char1_vulnerable_landing_recovery::Action_char1_vulnerable_landing_recovery() :
    Action((int)CHAR1_STATE::VULNERABLE_LANDING_RECOVERY, nullptr, {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false),
    m_recoveryLen(gamedata::characters::char1::vulnerableLandingRecovery)
{
    setSwitchData(false, m_recoveryLen, false, false, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
}


// JC LANDING RECOVERY
Action_char1_jc_landing_recovery::Action_char1_jc_landing_recovery() :
    Action((int)CHAR1_STATE::MOVE_JC_LANDING_RECOVERY, nullptr, {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_JC_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false),
    m_recoveryLen(gamedata::characters::char1::jcLandingRecovery)
{
    setSwitchData(false, m_recoveryLen, false, false, false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
}


// SOFT KNOCKDOWN ACTION
Action_char1_soft_knockdown::Action_char1_soft_knockdown() :
    Action((int)CHAR1_STATE::SOFT_KNOCKDOWN, nullptr, {}, ANIMATIONS::CHAR1_SOFT_KNOCKDOWN, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false)
{
    setSwitchData(false, 8, true, false, false, true, false, {1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::KNOCKDOWN_RECOVERY);
    setDisadvantageFlags(false, false, true);
}


// HARD KNOCKDOWN ACTION
Action_char1_hard_knockdown::Action_char1_hard_knockdown() :
    Action((int)CHAR1_STATE::HARD_KNOCKDOWN, nullptr, {}, ANIMATIONS::CHAR1_KNOCKDOWN, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false)
{
    setSwitchData(false, 30, true, false, false, true, false, {1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::KNOCKDOWN_RECOVERY);
    setDisadvantageFlags(false, false, true);
}


// KNOCKDOWN RECOVERY ACTION
Action_char1_knockdown_recovery::Action_char1_knockdown_recovery() :
    Action((int)CHAR1_STATE::KNOCKDOWN_RECOVERY, nullptr, {}, ANIMATIONS::CHAR1_KNOCKDOWN_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false,
    0, 0, false, false, false)
{
    setSwitchData(false, 21, true, true, false, true, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
    setOutdatedFlags(false, true, false, false, false);
    setDisadvantageFlags(false, false, true);
}


// ABSTRACT CHAR1 GROUND ATTACK ACTION
Action_char1_ground_attack::Action_char1_ground_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, StateMarker transitionableFrom_, bool isCrouchState_) :
    Action_attack(actionState_, std::move(incmp_), fullDuration_, hits_, std::move(hurtboxes_), anim_, std::move(gravityWindow_), std::move(transitionableFrom_), isCrouchState_, false)
{
    setOutdatedTransition((int)CHAR1_STATE::IDLE);
    setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}


// ABSTRACT CHAR1 AIR ATTACK ACTION
Action_char1_air_attack::Action_char1_air_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, StateMarker transitionableFrom_) :
    Action_attack(actionState_, std::move(incmp_), fullDuration_, hits_, std::move(hurtboxes_), anim_, std::move(gravityWindow_), std::move(transitionableFrom_), false, true)
{
    setSwitchData(false, fullDuration_, false, true, true, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setOutdatedTransition((int)CHAR1_STATE::FLOAT);
}


// j.C ACTION
Action_char1_move_JC::Action_char1_move_JC() :
    Action_char1_air_attack((int)CHAR1_STATE::MOVE_JC, ANIMATIONS::CHAR1_MOVE_JC, TimelineProperty<bool>({{0, false}, {10, true}}), std::make_unique<InputComparatorCPress>(), 200,
    {
        hitgeneration::generate_char1_JC()
    },
    {
        {
            TimelineProperty<bool>({{1, true}, {201, false}}),
            {-70, -350, 140, 300}
        },
        {
            TimelineProperty<bool>({{10, true}, {22, false}}),
            {0, -450, 120, 250}
        }
    },
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}))
{
    setUpdateMovementData(
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{0.9f, 0.125f}},
                {10, Vector2{1.0f, 1.0f}},
                {12, Vector2{1.0f, 0.0f}},
                {13, Vector2{1.0f, 1.0f}}
            }), // Vel mul
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{0.9f, 0.33f}},
                {10, Vector2{1.0f, 0.0f}},
                {11, Vector2{1.0f, 1.0f}}
            }), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {12, Vector2{3.0f, 0.0f}},
                {13, Vector2{0.0f, 0.0f}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>(
            {
                {12, Vector2{0.0f, -20.0f}},
                {13, Vector2{0.0f, 0.0f}}
            }), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    );

    setLandingRecoveryState((int)CHAR1_STATE::MOVE_JC_LANDING_RECOVERY);
}


// MOVE 214C ACTION
Action_char1_move_214C::Action_char1_move_214C() :
    Action_char1_ground_attack((int)CHAR1_STATE::MOVE_214C, ANIMATIONS::CHAR1_MOVE_214C, TimelineProperty(true), std::make_unique<InputComparator214CPress>(), 40,
    {
        hitgeneration::generate_char1_214C()
    },
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        },
        {
            TimelineProperty<bool>({{16, true}, {18, false}}),
            {50.0f, -350.0f, 80.0f, 75.0f}
        },
        {
            TimelineProperty<bool>({{18, true}, {36, false}}),
            {50.0f, -120.0f, 30.0f, 120.0f}
        }
    },
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false)
{
    setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, {6.0f, 0.0f}},
                {5, {0.0f, 0.0f}},
                {10, {2.0f, 0.0f}},
                {16, {0.0f, 0.0f}},
                {18, {80.0f, 0.0f}},
                {19, {0.0f, 0.0f}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    );

    setUpdateCamShakeData(
        TimelineProperty<Vector2<int>>(
            {
                {18, {35, 10}},
                {19, {0, 0}}
            }
    ));
}

// MOVE PROJECTILE ACTION
Action_char1_move_projectile::Action_char1_move_projectile() :
    Action_char1_ground_attack((int)CHAR1_STATE::PROJECTILE_CHAR, ANIMATIONS::CHAR1_PROJECTILE_CHAR_ANIM, TimelineProperty(true), std::make_unique<InputComparator214APress>(), 60,
    {
    },
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        },
        {
            TimelineProperty<bool>({{31, true}, {53, false}}),
            {60.0f, -420.0f, 60.0f, 350.0f}
        },
        {
            TimelineProperty<bool>({{31, true}, {35, false}}),
            {120.0f, -370.0f, 60.0f, 250.0f}
        }
    }, StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false)
{
    setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, {-4.0f, 0.0f}},
            {4, {-7.0f, 0.0f}},
            {15, {-3.0f, 0.0f}},
            {25, {0.0f, 0.0f}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    );
}

// Normal throw startup
Action_char1_normal_throw_startup::Action_char1_normal_throw_startup() :
    Action_throw_startup((int)CHAR1_STATE::THROW_NORMAL_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_WHIFF, (int)CHAR1_STATE::THROW_NORMAL_HOLD,
    std::make_unique<InputComparatorBCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, false, THROW_LIST::CHAR1_NORMAL_THROW,
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false)
{
}

// Normal throw hold
Action_char1_normal_throw_hold::Action_char1_normal_throw_hold() :
    Action_throw_hold((int)CHAR1_STATE::THROW_NORMAL_HOLD, (int)CHAR1_STATE::THROW_NORMAL_ANIM, 90, 10, false)
{
}

// Back throw startup
Action_char1_back_throw_startup::Action_char1_back_throw_startup() :
    Action_throw_startup((int)CHAR1_STATE::THROW_BACK_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_WHIFF, (int)CHAR1_STATE::THROW_BACK_HOLD,
    std::make_unique<InputComparator4BCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, false, THROW_LIST::CHAR1_NORMAL_THROW,
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false)
{
}

// Back throw hold
Action_char1_back_throw_hold::Action_char1_back_throw_hold() :
    Action_throw_hold((int)CHAR1_STATE::THROW_BACK_HOLD, (int)CHAR1_STATE::THROW_NORMAL_ANIM, 90, 10, true)
{
}

// Normal throw whiff
Action_char1_normal_throw_whiff::Action_char1_normal_throw_whiff() :
    Action_throw_whiff((int)CHAR1_STATE::THROW_NORMAL_WHIFF, ANIMATIONS::CHAR1_NORMAL_THROW_WHIFF, TimelineProperty(true), 30, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, (int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::FLOAT)
{
}

// NORMAL THROW ACTION
Action_char1_normal_throw::Action_char1_normal_throw() :
    Action_locked_animation((int)CHAR1_STATE::THROW_NORMAL_ANIM, (int)CHAR1_STATE::IDLE, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_NORMAL_THROW, 50, TimelineProperty(false), TimelineProperty(false))
{
    setUpdateHitsToOpponent(TimelineProperty<HitData*>(
            {
                {13, new HitData(hitgeneration::generate_char1_normal_throw())},
                {14, nullptr}
            })
    );
}

// Air throw startup
Action_char1_normal_air_throw_startup::Action_char1_normal_air_throw_startup() :
    Action_throw_startup((int)CHAR1_STATE::THROW_NORMAL_AIR_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_AIR_WHIFF, (int)CHAR1_STATE::THROW_NORMAL_AIR_HOLD,
    std::make_unique<InputComparatorBCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::floatingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, true, THROW_LIST::CHAR1_AIR_THROW,
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}),
    true)
{
}

// Air throw hold
Action_char1_normal_air_throw_hold::Action_char1_normal_air_throw_hold() :
    Action_throw_hold((int)CHAR1_STATE::THROW_NORMAL_AIR_HOLD, (int)CHAR1_STATE::THROW_NORMAL_AIR_ANIM, 90, 10, false)
{
}

// Air back throw startup
Action_char1_back_air_throw_startup::Action_char1_back_air_throw_startup() :
    Action_throw_startup((int)CHAR1_STATE::THROW_BACK_AIR_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_AIR_WHIFF, (int)CHAR1_STATE::THROW_BACK_AIR_HOLD,
    std::make_unique<InputComparator4BCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::floatingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, true, THROW_LIST::CHAR1_AIR_THROW,
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}),
    true)
{
}

// Air back throw hold
Action_char1_back_air_throw_hold::Action_char1_back_air_throw_hold() :
    Action_throw_hold((int)CHAR1_STATE::THROW_BACK_AIR_HOLD, (int)CHAR1_STATE::THROW_NORMAL_AIR_ANIM, 90, 10, true)
{
}

// Air throw whiff
Action_char1_normal_air_throw_whiff::Action_char1_normal_air_throw_whiff() :
    Action_throw_whiff((int)CHAR1_STATE::THROW_NORMAL_AIR_WHIFF, ANIMATIONS::CHAR1_NORMAL_THROW_WHIFF, TimelineProperty(true), 200, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::floatingHurtbox
        }
    }, (int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::FLOAT)
{
}

// AIR THROW ACTION
Action_char1_normal_air_throw::Action_char1_normal_air_throw() :
    Action_locked_animation((int)CHAR1_STATE::THROW_NORMAL_AIR_ANIM, (int)CHAR1_STATE::IDLE, {
        {
            TimelineProperty<bool>({{1, true}, {51, false}}),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_NORMAL_THROW, 20, TimelineProperty(false), TimelineProperty(false))
{
    setUpdateHitsToOpponent(TimelineProperty<HitData*>(
            {
                {13, new HitData(hitgeneration::generate_char1_normal_throw())},
                {14, nullptr}
            })
    );
}

// Throw tech
Action_char1_throw_tech::Action_char1_throw_tech() :
    Action_throw_tech((int)CHAR1_STATE::THROW_TECH_OWN,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 15,
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, THROW_TECHS_LIST::CHAR1_GROUND, StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD}),
    false, (int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::FLOAT)
{
    setSwitchData(false, m_duration, true, true, true, false, true, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {-5.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}

// Throw tech state when Char1 breaks throw
Action_char1_throw_tech_char1::Action_char1_throw_tech_char1() :
    Action_throw_tech((int)CHAR1_STATE::THROW_TECH_CHAR1,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 25,
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, THROW_TECHS_LIST::NONE, StateMarker(gamedata::characters::totalStateCount, {}),
    false, (int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::FLOAT)
{
    setSwitchData(false, m_duration, true, true, true, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {-20.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}

// Air throw tech
Action_char1_air_throw_tech::Action_char1_air_throw_tech() :
    Action_throw_tech((int)CHAR1_STATE::AIR_THROW_TECH_OWN,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 15,
    {
        {
            TimelineProperty<bool>({{1, true}, {16, false}}),
            {-70, -350, 140, 300}
        }
    }, THROW_TECHS_LIST::CHAR1_AIR, StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD}),
    true, (int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::FLOAT)
{
    setSwitchData(false, m_duration, true, true, true, false, true, {1.0f, 1.0f}, {1.0f, 1.0f}, {-3.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
}

// Throw tech state when Char1 breaks air throw
Action_char1_air_throw_tech_char1::Action_char1_air_throw_tech_char1() :
    Action_throw_tech((int)CHAR1_STATE::AIR_THROW_TECH_CHAR1,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 200,
    {
        {
            TimelineProperty<bool>({{1, true}, {200, false}}),
            {-70, -350, 140, 300}
        }
    }, THROW_TECHS_LIST::NONE, StateMarker(gamedata::characters::totalStateCount, {}),
    true, (int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::FLOAT)
{
    setSwitchData(false, m_duration, true, true, true, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {-10.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f});
    setLandingRecoveryState((int)CHAR1_STATE::HARD_LANDING_RECOVERY);
}


// Thrown by char1 normal throw - hold
Action_char1_thrown_char1_normal_hold::Action_char1_thrown_char1_normal_hold() :
    Action_thrown_hold((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD, (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_ANIM, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD, 10)
{
}

// Thrown by char1 normal throw - animation
Action_char1_thrown_char1_normal::Action_char1_thrown_char1_normal() :
    Action_locked_animation((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_ANIM, -1, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL, 14, TimelineProperty(false), TimelineProperty(false))
{
    setUpdateMovementData(
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{1.0f, 1.0f}},
                {10, Vector2{0.0f, 0.0f}},
        }), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{0.8f, 0.0f}},
                {2, Vector2{0.0f, 0.0f}},
        }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{0.0f, 5.0f}},
                {2, Vector2{0.0f, 0.0f}}
        }), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    );

    setOutdatedFlags(false, false, true, true, true);
    setOutdatedMovementData({0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {-5.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -20.0f});
}


// Thrown by char1 normal air throw - hold
Action_char1_thrown_char1_normal_air_hold::Action_char1_thrown_char1_normal_air_hold() :
    Action_thrown_hold((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD, (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_ANIM, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD, 10)
{
}

// Thrown by char1 normal air throw - animation
Action_char1_thrown_char1_normal_air::Action_char1_thrown_char1_normal_air() :
    Action_locked_animation((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_ANIM, -1, {
        {
            TimelineProperty<bool>({{1, true}, {51, false}}),
            {-70, -375, 140, 375}
        }
    }, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL, 14, TimelineProperty(false), TimelineProperty(false))
{
    setUpdateMovementData(
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{1.0f, 1.0f}},
                {10, Vector2{0.0f, 0.0f}},
        }), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{0.8f, 0.0f}},
                {2, Vector2{0.0f, 0.0f}},
        }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, Vector2{0.0f, 5.0f}},
                {2, Vector2{0.0f, 0.0f}}
        }), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    );
    setOutdatedFlags(false, false, true, true, false);
    setOutdatedMovementData({0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {-5.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -10.0f});
}