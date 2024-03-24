#include "Actions.h"
#include "Char1.h"
#include <stdexcept>

/* ============================
 *
 *       UNIVERSAL ABSTRACT ACTIONS
 *
 *========================== */

Action::Action(int actionState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isAttack_, bool isCrouchState_, bool isThrowStartup_) :
    actionState(actionState_),
    m_hurtboxes(std::move(hurtboxes_)),
    m_anim(anim_),
    m_isAttack(isAttack_),
    m_isCrouchState(isCrouchState_),
    m_isThrowStartup(isThrowStartup_),
    m_counterWindow(std::move(counterWindow_)),
    m_gravityWindow(std::move(gravityWindow_)),
    m_blockWindow(std::move(blockWindow_)),
    m_transitionableFrom(std::move(transitionableFrom_))
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

void Action::switchTo(Character &character_) const
{
    character_.m_timer.begin(0);
    character_.m_currentState = actionState;

    if (m_anim != ANIMATIONS::NONE)
    {
        character_.m_currentAnimation = character_.m_animations[m_anim].get();
        character_.m_currentAnimation->reset();
    }

    character_.m_currentAction = this;
    character_.applyCancelWindow({{0, 0}, {}});
    character_.framesInState = 0;
    character_.m_appliedHits.clear();
    character_.m_hitstunAnimation = HITSTUN_ANIMATION::NONE;
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
    // TODO:
    // Airdash resource check
    // Airjump resource check
    // Airdash timer check
    // Airjump timer check
    // IB cancel check
    // Airborne check is probably not necessary since there is no overlap between cancellable grounded actions and cancellable airborne actions

    if (char_->isInHitstop())
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

// ABSTRACT PROLONGED ACTION
Action_prolonged::Action_prolonged(int actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, StateMarker transitionableFrom_, bool isCrouchState_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, std::move(counterWindow_), std::move(gravityWindow_), std::move(blockWindow_), std::move(transitionableFrom_), false, isCrouchState_, false)
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
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, std::move(counterWindow_), TimelineProperty(true), std::move(blockWindow_), std::move(transitionableFrom_), false, false, false),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
}


// ABSTRACT AIR JUMP ACTION
Action_airjump::Action_airjump(int actionState_, const Vector2<float> &impulse_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, StateMarker transitionableFrom_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(false), TimelineProperty(true), std::move(true), std::move(transitionableFrom_), false, false, false),
    m_impulse(impulse_)
{
}

int Action_airjump::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (!char_->m_jumpsAvailable.canConsume() || !char_->m_airjumpTimer.isOver() || !char_->isAirborne())
        return 0;

    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    return Action::isPossible(inputQueue_, char_, extendBuffer_);
}

int Action_airjump::responseOnOwnState(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const
{
    return (isInputPossible(inputQueue_, ownDirection_, extendBuffer_) ? 1 : 0);
}

// ABSTRACT ATTACK ACTION
Action_attack::Action_attack(int actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, TimelineProperty<Vector2<float>> &&velocity_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, StateMarker transitionableFrom_, bool isCrouchState_, bool stepOnly_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, hitutils::getRegularCounterTimeline(hits_), std::move(gravityWindow_), TimelineProperty(false), std::move(transitionableFrom_), true, isCrouchState_, false),
    m_fullDuration(fullDuration_),
    m_hits(hits_),
    m_velocity(std::move(velocity_)),
    m_stepOnly(stepOnly_)
{
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

void Action_attack::switchTo(Character &character_) const
{
    character_.turnVelocityToInertia();
    Action::switchTo(character_);
    character_.m_timer.begin(m_fullDuration);
}

void Action_attack::update(Character &character_) const
{
    if (!m_velocity.isEmpty())
    {
        character_.m_velocity = m_velocity[character_.m_timer.getCurrentFrame() + 1].mulComponents(Vector2{character_.getOwnHorDir().x, 1.0f});
    }
}


// ABSTRACT THROW STARTUP
Action_throw_startup::Action_throw_startup(int actionState_, int whiffState_, int holdState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float range_, FrameWindow activeWindow_, bool requiredAirborne_, THROW_LIST throw_, StateMarker transitionableFrom_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(true), std::move(gravityWindow_), TimelineProperty(false), std::move(transitionableFrom_), false, false, true),
    m_whiffState(whiffState_),
    m_holdState(holdState_),
    m_range(range_),
    m_activeWindow(activeWindow_),
    m_requiredAirborne(requiredAirborne_),
    m_throw(throw_)
{
}

void Action_throw_startup::switchTo(Character &character_) const
{
    character_.turnVelocityToInertia();
    Action::switchTo(character_);
    character_.m_timer.begin(m_activeWindow.second);
    if (character_.m_airborne)
        character_.m_usedAirAttack = true;
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

void Action_throw_startup::outdated(Character &character_) const
{
    character_.m_actionResolver.getAction(m_whiffState)->switchTo(character_);
}

// ABSTRACT THROW HOLD
Action_throw_hold::Action_throw_hold(int actionState_, int throwState_, float setRange_, float duration_, bool sideSwitch_) :
    Action(actionState_, nullptr, {}, ANIMATIONS::NONE, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false),
    m_setRange(setRange_),
    m_duration(duration_),
    m_throwState(throwState_),
    m_sideSwitch(sideSwitch_)
{
}

void Action_throw_hold::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);

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

void Action_throw_hold::update(Character &character_) const
{
}

void Action_throw_hold::outdated(Character &character_) const
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

    character_.m_actionResolver.getAction(m_throwState)->switchTo(character_);
}

// ABSTRACT THROW WHIFF
Action_throw_whiff::Action_throw_whiff(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float duration_, HurtboxFramesVec &&hurtboxes_) :
    Action(actionState_, nullptr, std::move(hurtboxes_), anim_, TimelineProperty(false), std::move(gravityWindow_), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false),
    m_duration(duration_)
{
}

void Action_throw_whiff::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);
}

void Action_throw_whiff::outdated(Character &character_) const
{
    if (!character_.m_airborne)
        character_.switchToIdle();
    else
        character_.switchToFloat();
}

// ABSTRACT THROWN HOLD
Action_thrown_hold::Action_thrown_hold(int actionState_, int thrownState_, ANIMATIONS anim_, float duration_) :
    Action(actionState_, nullptr, {}, anim_, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false),
    m_duration(duration_),
    m_thrownState(thrownState_)
{
}

void Action_thrown_hold::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);

    character_.lockInAnimation();
    character_.tieAnimWithOpponent();
}

void Action_thrown_hold::outdated(Character &character_) const
{
    //character_.switchToIdle();
    character_.m_actionResolver.getAction(m_thrownState)->switchTo(character_);
}

// ABSTRACT THROW TECH
Action_throw_tech::Action_throw_tech(int actionState_, InputComparator_ptr incmp_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, float duration_, HurtboxFramesVec &&hurtboxes_, THROW_TECHS_LIST throwTech_, StateMarker transitionableFrom_) :
    Action(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(false), std::move(gravityWindow_), std::move(blockWindow_), std::move(transitionableFrom_), false, false, false),
    m_duration(duration_),
    m_throwTech(throwTech_)
{
}

void Action_throw_tech::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);
    
    character_.releaseFromAnimation();
    if (character_.m_tiedAnimWithOpponent)
    {
        character_.untieAnimWithOpponent();
        character_.m_otherCharacter->throwTeched(m_throwTech);
    }
}

void Action_throw_tech::outdated(Character &character_) const
{
    if (!character_.m_airborne)
        character_.switchToIdle();
    else
        character_.switchToFloat();
}

// ABSTRACT LOCKED ANIMATION
Action_locked_animation::Action_locked_animation(int actionState_, int quitState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, float duration_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_) :
    Action(actionState_, nullptr, std::move(hurtboxes_), anim_, std::move(counterWindow_), TimelineProperty(false), std::move(blockWindow_), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false),
    m_duration(duration_),
    m_quitState(quitState_)
{
}

void Action_locked_animation::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);
    character_.lockInAnimation();
}

void Action_locked_animation::outdated(Character &character_) const
{
    character_.releaseFromAnimation();
    character_.untieAnimWithOpponent();
    character_.m_actionResolver.getAction(m_quitState)->switchTo(character_);
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
    false, false, false)
{
}

void Action_char1_idle::switchTo(Character &character_) const
{
    character_.turnVelocityToInertia();
    Action::switchTo(character_);
    character_.updateOwnOrientation();
    character_.m_healthHandler.resetScaling();
    character_.m_comboPhysHandler.reset();
    character_.m_blockstunType = BLOCK_FRAME::NONE;
}

void Action_char1_idle::update(Character &character_) const
{
    character_.updateOwnOrientation();
}

// CROUCH ACTION
Action_char1_crouch::Action_char1_crouch() :
    Action_prolonged((int)CHAR1_STATE::CROUCH, std::make_unique<InputComparatorDownHold>(), std::make_unique<InputComparatorDownHold>(), {{TimelineProperty(true), gamedata::characters::char1::crouchingHurtbox}}, ANIMATIONS::CHAR1_CROUCH_IDLE, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::IDLE, (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::WALK_BWD}),
    true)
{
}

void Action_char1_crouch::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
}

void Action_char1_crouch::update(Character &character_) const
{
    character_.updateOwnOrientation();
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
    false)
{
}

void Action_char1_walk_fwd::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_velocity = character_.getHorDirToEnemy().mulComponents(Vector2{6.0f, 0.0f});
}

void Action_char1_walk_fwd::update(Character &character_) const
{
    character_.updateOwnOrientation();
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
    true)
{
}

void Action_char1_walk_bwd::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_currentAnimation->reset(65, -1);
    character_.m_velocity = character_.getHorDirToEnemy().mulComponents(Vector2{-6.0f, 0.0f});
}

void Action_char1_walk_bwd::update(Character &character_) const
{
    character_.updateOwnOrientation();
}


// ABSTRACT CHAR1 JUMP ACTION
Action_char1_jump::Action_char1_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_) :
    Action_jump((int)actionState_, impulse_, prejumpLen_, maxHorInertia_, std::move(incmp_), std::move(hurtboxes_), ANIMATIONS::CHAR1_PREJUMP, TimelineProperty(false), TimelineProperty(true), StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD, (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::IDLE})),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
}

int Action_char1_jump::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    return Action::isPossible(inputQueue_, char_, extendBuffer_);
}

void Action_char1_jump::switchTo(Character &character_) const
{
    character_.turnVelocityToInertia();
    Action::switchTo(character_);
    character_.m_timer.begin(m_prejumpLen);
    character_.m_airjumpTimer.begin(5);
    character_.m_airdashTimer.begin(6);
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


// FLOAT ACTION
Action_char1_float::Action_char1_float() :
    Action((int)CHAR1_STATE::FLOAT, std::move(std::make_unique<InputComparatorUpHold>()), {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_JUMP, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), StateMarker(gamedata::characters::totalStateCount, {}), false, false, false)
{
}

void Action_char1_float::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_currentAnimation->reset(20);

    //character_.turnVelocityToInertia();
    character_.m_currentState = (int)CHAR1_STATE::JUMP;
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
}

void Action_char1_airjump::switchTo(Character &character_) const
{
    character_.updateOwnOrientation();

    if (character_.m_currentState == (int)CHAR1_STATE::AIR_DASH_EXTENTION || (character_.m_currentAction && character_.m_currentAction->m_isAttack))
    {
        character_.turnVelocityToInertia();
        character_.m_inertia.x /= gamedata::characters::char1::doubleJumpInertiaDivider;
    }

    Action::switchTo(character_);

    auto ownOrientationVector = character_.getOwnHorDir();
    ownOrientationVector.y = 1;
    character_.m_velocity = m_impulse.mulComponents(ownOrientationVector);

    character_.m_inertia.y = 0;

    character_.m_jumpsAvailable.consume();
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
    false),
    m_accel(gamedata::characters::char1::dashAccel),
    m_maxspd(gamedata::characters::char1::dashMaxSpeed)
{
}

void Action_char1_ground_dash::update(Character &character_) const
{
    auto newXVelocity = (abs(character_.m_velocity.x) + m_accel);
    character_.m_velocity.x = character_.getOwnHorDir().x * std::min(abs(newXVelocity), abs(m_maxspd));
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
    false, false, false),
    m_duration(gamedata::characters::char1::stepDuration)
{
}

int Action_char1_step::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{  
    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    return Action::isPossible(inputQueue_, char_, extendBuffer_);
}

void Action_char1_step::outdated(Character &character_) const
{
    //character_.m_velocity.x = 0;
    character_.m_actionResolver.getAction((int)CHAR1_STATE::STEP_RECOVERY)->switchTo(character_);
}

void Action_char1_step::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{gamedata::characters::char1::stepSpeed, 0.0f});
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);
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
    false, false, false),
    m_recoveryLen(gamedata::characters::char1::stepRecoveryDuration)
{
}

void Action_char1_step_recovery::outdated(Character &character_) const
{
    character_.switchToIdle();
}

void Action_char1_step_recovery::switchTo(Character &character_) const
{
    character_.turnVelocityToInertia();
    Action::switchTo(character_);
    character_.m_timer.begin(m_recoveryLen);
}

void Action_char1_step_recovery::update(Character &character_) const
{
    //character_.m_inertia.x /= 4.0f;
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
    false, false, false),
    m_totalDuration(gamedata::characters::char1::backdashDuration)
{
}

void Action_char1_ground_backdash::outdated(Character &character_) const
{
    character_.switchToIdle();
}

void Action_char1_ground_backdash::switchTo(Character &character_) const
{
    if (character_.m_currentState == (int)CHAR1_STATE::SOFT_LANDING_RECOVERY)
    {
        character_.updateOwnOrientation();
    }

    Action::switchTo(character_);

    character_.m_timer.begin(m_totalDuration);
    auto dir = character_.getOwnHorDir();
    dir.y = 1;
    character_.m_velocity = dir.mulComponents(Vector2{-gamedata::characters::char1::backdashSpd, -14.0f});
    character_.m_inertia = {0.0f, 0.0f};
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
    false, false, false),
    m_duration(gamedata::characters::char1::airdashDuration)
{
}

int Action_char1_air_dash::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (!char_->m_airdashesAvailable.canConsume() || !(char_->m_airdashTimer.isOver()))
        return 0;

    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    return Action::isPossible(inputQueue_, char_, extendBuffer_);
}

void Action_char1_air_dash::outdated(Character &character_) const
{
    character_.m_actionResolver.getAction((int)CHAR1_STATE::AIR_DASH_EXTENTION)->switchTo(character_);
    character_.m_currentAnimation = character_.m_animations[m_anim].get();
}

void Action_char1_air_dash::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{gamedata::characters::char1::airdashSpeed, 0.0f});
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);
    character_.m_airdashesAvailable.consume();
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
    false, false, false),
    m_duration(gamedata::characters::char1::airBackdashDuration)
{
}

int Action_char1_air_backdash::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{

    if (!char_->m_airdashesAvailable.canConsume() || !char_->m_airdashTimer.isOver())
        return 0;

    return Action::isPossible(inputQueue_, char_, extendBuffer_);
}

void Action_char1_air_backdash::outdated(Character &character_) const
{
    character_.m_velocity.x /= 5.0f;
    character_.switchToFloat();
}

void Action_char1_air_backdash::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, -10.0f};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{-gamedata::characters::char1::airBackdashSpeed, 0.0f});
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);
    character_.m_airdashesAvailable.consume();
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
    false, false, false),
    m_duration(gamedata::characters::char1::airdashExtentionDuration),
    m_baseSpd(gamedata::characters::char1::airdashExtentionMaxSpeed),
    m_spdMultiplier((gamedata::characters::char1::airdashExtentionMaxSpeed - gamedata::characters::char1::airdashExtentionMinSpeed) / gamedata::characters::char1::airdashExtentionDuration)
{
}

void Action_char1_air_dash_extention::outdated(Character &character_) const
{
    character_.switchToFloat();
    character_.updateOwnOrientation();
}

void Action_char1_air_dash_extention::switchTo(Character &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{(float)m_baseSpd, 0.0f});
    Action::switchTo(character_);
    character_.m_timer.begin(m_duration);
}

void Action_char1_air_dash_extention::setVelocity(Character &character_) const
{
    character_.m_velocity.x = character_.getOwnHorDir().x * (m_baseSpd - m_spdMultiplier * (character_.m_timer.getCurrentFrame() + 1));
}

void Action_char1_air_dash_extention::update(Character &character_) const
{
    setVelocity(character_);
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
    false, false, false),
    m_recoveryLen(gamedata::characters::char1::dashRecovery)
{
}

void Action_char1_ground_dash_recovery::outdated(Character &character_) const
{
    character_.switchToIdle();
}

void Action_char1_ground_dash_recovery::switchTo(Character &character_) const
{
    character_.turnVelocityToInertia();
    Action::switchTo(character_);
    character_.m_timer.begin(m_recoveryLen);
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
    false, false, false),
    m_recoveryLen(gamedata::characters::char1::softLandingRecovery)
{
}

void Action_char1_soft_landing_recovery::outdated(Character &character_) const
{
    character_.switchToIdle();
}

void Action_char1_soft_landing_recovery::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
    character_.m_inertia = {0.0f, 0.0f};
    character_.m_timer.begin(m_recoveryLen);
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
    false, false, false),
    m_recoveryLen(gamedata::characters::char1::hardLandingRecovery)
{
}

void Action_char1_hard_landing_recovery::outdated(Character &character_) const
{
    character_.switchToIdle();
}

void Action_char1_hard_landing_recovery::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
    character_.m_inertia = {0.0f, 0.0f};
    character_.m_timer.begin(m_recoveryLen);
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
    false, false, false),
    m_recoveryLen(gamedata::characters::char1::vulnerableLandingRecovery)
{
}

void Action_char1_vulnerable_landing_recovery::outdated(Character &character_) const
{
    character_.switchToIdle();
}

void Action_char1_vulnerable_landing_recovery::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
    character_.m_inertia = {0.0f, 0.0f};
    character_.m_timer.begin(m_recoveryLen);
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
    false, false, false),
    m_recoveryLen(gamedata::characters::char1::jcLandingRecovery)
{
}

void Action_char1_jc_landing_recovery::outdated(Character &character_) const
{
    character_.switchToIdle();
}

void Action_char1_jc_landing_recovery::switchTo(Character &character_) const
{
    Action::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
    character_.m_inertia = {0.0f, 0.0f};
    character_.m_timer.begin(m_recoveryLen);
}

// SOFT KNOCKDOWN ACTION
Action_char1_soft_knockdown::Action_char1_soft_knockdown() :
    Action((int)CHAR1_STATE::SOFT_KNOCKDOWN, nullptr, {}, ANIMATIONS::CHAR1_SOFT_KNOCKDOWN, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false)
{
}

void Action_char1_soft_knockdown::outdated(Character &character_) const
{
    character_.enterKndRecovery();
}

void Action_char1_soft_knockdown::switchTo(Character &character_) const
{
    character_.takePushback({0, 0});
    character_.turnVelocityToInertia();
    Action::switchTo(character_);
    character_.m_timer.begin(8);
}

// HARD KNOCKDOWN ACTION
Action_char1_hard_knockdown::Action_char1_hard_knockdown() :
    Action((int)CHAR1_STATE::HARD_KNOCKDOWN, nullptr, {}, ANIMATIONS::CHAR1_KNOCKDOWN, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false)
{
}

void Action_char1_hard_knockdown::outdated(Character &character_) const
{
    character_.enterKndRecovery();
}

void Action_char1_hard_knockdown::switchTo(Character &character_) const
{
    character_.takePushback({0, 0});
    Action::switchTo(character_);
    character_.m_timer.begin(30);
    character_.m_velocity = {0.0f, 0.0f};
}

// KNOCKDOWN RECOVERY ACTION
Action_char1_knockdown_recovery::Action_char1_knockdown_recovery() :
    Action((int)CHAR1_STATE::KNOCKDOWN_RECOVERY, nullptr, {}, ANIMATIONS::CHAR1_KNOCKDOWN_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false)
{
}

void Action_char1_knockdown_recovery::outdated(Character &character_) const
{
    character_.switchToIdle();
    character_.setThrowInvul();
}

void Action_char1_knockdown_recovery::switchTo(Character &character_) const
{
    character_.turnVelocityToInertia();
    character_.takePushback({0, 0});
    Action::switchTo(character_);
    character_.m_timer.begin(21);
    character_.m_healthHandler.resetScaling();
    character_.m_comboPhysHandler.reset();
}

// ABSTRACT CHAR1 GROUND ATTACK ACTION
Action_char1_ground_attack::Action_char1_ground_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, TimelineProperty<Vector2<float>> &&velocity_, StateMarker transitionableFrom_, bool isCrouchState_, bool stepOnly_) :
    Action_attack(actionState_, std::move(incmp_), fullDuration_, hits_, std::move(hurtboxes_), std::move(velocity_), anim_, std::move(gravityWindow_), std::move(transitionableFrom_), isCrouchState_, stepOnly_)
{
}


void Action_char1_ground_attack::outdated(Character &character_) const
{
    character_.m_velocity = {0.0f, 0.0f};
    character_.switchToIdle();
}

void Action_char1_ground_attack::switchTo(Character &character_) const
{
    if (character_.m_currentState == (int)CHAR1_STATE::SOFT_LANDING_RECOVERY)
                    character_.updateOwnOrientation();

    Action_attack::switchTo(character_);
}

// ABSTRACT CHAR1 AIR ATTACK ACTION
Action_char1_air_attack::Action_char1_air_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, StateMarker transitionableFrom_) :
    Action_attack(actionState_, std::move(incmp_), fullDuration_, hits_, std::move(hurtboxes_), TimelineProperty<Vector2<float>>{}, anim_, std::move(gravityWindow_), std::move(transitionableFrom_), false, false)
{
}

void Action_char1_air_attack::outdated(Character &character_) const
{
    character_.switchToFloat();
}

void Action_char1_air_attack::switchTo(Character &character_) const
{
    character_.m_usedAirAttack = true;
    if (character_.m_currentState == (int)CHAR1_STATE::AIR_DASH_EXTENTION)
    {
        character_.turnVelocityToInertia();
    }

    Action::switchTo(character_);
    character_.m_timer.begin(m_fullDuration);
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
}

void Action_char1_move_JC::update(Character &character_) const
{
    auto frame = character_.m_timer.getCurrentFrame() + 1;
    if (frame <= 9)
    {
        //character_.m_inertia.y /= 10.0f - character_.m_gravity / 2.0f;
        character_.m_inertia.y /= 3.0f;
        character_.m_velocity.y /= 8.0f;

        character_.m_velocity.x /= 1.1f;
        character_.m_inertia.x /= 1.1f;
    }
    else if (frame >= 10 && frame <= 11)
    {
        character_.m_inertia.y = 0;
    }
    else if (frame == 12)
    {
        character_.m_velocity.y = -20.0f;
        character_.m_velocity.x = character_.getOwnHorDir().x * 3.0f;
    }

    std::cout << "(" << character_.m_velocity << ") : (" << character_.m_inertia << ")\n";
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
    }, TimelineProperty<Vector2<float>>(
        {
            {1, {6.0f, 0.0f}},
            {5, {0.0f, 0.0f}},
            {10, {2.0f, 0.0f}},
            {16, {0.0f, 0.0f}},
            {18, {80.0f, 0.0f}},
            {19, {0.0f, 0.0f}}
        }),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false)
{
}

void Action_char1_move_214C::update(Character &character_) const
{
    Action_attack::update(character_);

    auto frame = character_.m_timer.getCurrentFrame() + 1;
    if (frame == 18)
    {
        character_.m_cam->startShake(35, 10);
    }
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
    },
    TimelineProperty<Vector2<float>>(
        {
            {1, {-4.0f, 0.0f}},
            {4, {-7.0f, 0.0f}},
            {15, {-3.0f, 0.0f}},
            {25, {0.0f, 0.0f}},
        }), StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false)
{
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
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}))
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
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}))
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
    })
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
}

// TODO: turn attacks during animation into timeline property
void Action_char1_normal_throw::update(Character &character_) const
{
    //character_.updateOwnOrientation();
    if (character_.m_timer.getCurrentFrame() == 12 && !character_.m_inHitstop)
    {
        HitEvent ev;
        ev.m_hitData = hitgeneration::generate_char1_normal_throw();
        ev.m_hitRes = HIT_RESULT::THROWN;
        ev.m_hittingPlayerId = character_.m_playerId;
        character_.applyHit(ev);
        character_.m_otherCharacter->applyHit(ev);
        character_.m_cam->startShake(35, 10);
    }
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
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}))
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
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}))
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
    })
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
}

void Action_char1_normal_air_throw::update(Character &character_) const
{
    //character_.updateOwnOrientation();
    if (character_.m_timer.getCurrentFrame() == 12 && !character_.m_inHitstop)
    {
        HitEvent ev;
        ev.m_hitData = hitgeneration::generate_char1_normal_throw();
        ev.m_hitRes = HIT_RESULT::THROWN;
        ev.m_hittingPlayerId = character_.m_playerId;
        character_.applyHit(ev);
        character_.m_otherCharacter->applyHit(ev);
        character_.m_cam->startShake(35, 10);
    }
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
    }, THROW_TECHS_LIST::CHAR1_GROUND, StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD}))
{
}

void Action_char1_throw_tech::switchTo(Character &character_) const
{
    Action_throw_tech::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 5.0f;
    character_.callForPriority();
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
    }, THROW_TECHS_LIST::NONE, StateMarker(gamedata::characters::totalStateCount, {}))
{
}

void Action_char1_throw_tech_char1::switchTo(Character &character_) const
{
    Action_throw_tech::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 20.0f;
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
    }, THROW_TECHS_LIST::CHAR1_AIR, StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD}))
{
}

void Action_char1_air_throw_tech::switchTo(Character &character_) const
{
    Action_throw_tech::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 3.0f;
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
    }, THROW_TECHS_LIST::NONE, StateMarker(gamedata::characters::totalStateCount, {}))
{
}

void Action_char1_air_throw_tech_char1::switchTo(Character &character_) const
{
    Action_throw_tech::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 10.0f;
}

// Thrown by char1 normal throw - hold
Action_char1_thrown_char1_normal_hold::Action_char1_thrown_char1_normal_hold() :
    Action_thrown_hold((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD, (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_ANIM, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD, 10)
{
}

// Thrown by char1 normal throw - animation
Action_char1_thrown_char1_normal::Action_char1_thrown_char1_normal() :
    Action_locked_animation((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_ANIM, (int)CHAR1_STATE::HITSTUN_AIR, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL, 14, TimelineProperty(false), TimelineProperty(false))
{
}

void Action_char1_thrown_char1_normal::update(Character &character_) const
{
    auto curFrame = character_.m_timer.getCurrentFrame();
    if (curFrame == 1)
        character_.m_velocity = {character_.getOwnHorDir().x * 0.8f, 5.0f};
    else if (curFrame == 10)
        character_.m_velocity = {0, 0};
}

void Action_char1_thrown_char1_normal::outdated(Character &character_) const
{
    character_.releaseFromAnimation();
    character_.untieAnimWithOpponent();
    character_.m_inertia = {-character_.getOwnHorDir().x * 5.0f, -20.0f};
    character_.m_pos.y = gamedata::stages::levelOfGround - 1;
    character_.m_airborne = true;
    character_.enterHitstunAnimation(character_.m_hitProps);
}

// Thrown by char1 normal air throw - hold
Action_char1_thrown_char1_normal_air_hold::Action_char1_thrown_char1_normal_air_hold() :
    Action_thrown_hold((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD, (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_ANIM, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD, 10)
{
}

// Thrown by char1 normal air throw - animation
Action_char1_thrown_char1_normal_air::Action_char1_thrown_char1_normal_air() :
    Action_locked_animation((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_ANIM, (int)CHAR1_STATE::HITSTUN_AIR, {
        {
            TimelineProperty<bool>({{1, true}, {51, false}}),
            {-70, -375, 140, 375}
        }
    }, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL, 14, TimelineProperty(false), TimelineProperty(false))
{
}

void Action_char1_thrown_char1_normal_air::update(Character &character_) const
{
    //8-16
    auto curFrame = character_.m_timer.getCurrentFrame();
    if (curFrame == 1)
        character_.m_velocity = {character_.getOwnHorDir().x * 0.8f, 5.0f};
    else if (curFrame == 10)
        character_.m_velocity = {0, 0};
}

void Action_char1_thrown_char1_normal_air::outdated(Character &character_) const
{
    character_.releaseFromAnimation();
    character_.untieAnimWithOpponent();
    character_.m_inertia = {-character_.getOwnHorDir().x * 5.0f, -10.0f};
    character_.m_airborne = true;
    character_.enterHitstunAnimation(character_.m_hitProps);
}