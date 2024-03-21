#include "Actions.h"
#include "Char1.h"
#include <stdexcept>

/* ============================
 *
 *       UNIVERSAL ABSTRACT ACTIONS
 *
 *========================== */

template <typename Char_t>
Action<Char_t>::Action(int actionState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, bool isAttack_, bool isCrouchState_, bool isThrowStartup_) :
    actionState(actionState_),
    m_hurtboxes(std::move(hurtboxes_)),
    m_anim(anim_),
    m_isAttack(isAttack_),
    m_isCrouchState(isCrouchState_),
    m_isThrowStartup(isThrowStartup_),
    m_counterWindow(std::move(counterWindow_)),
    m_gravityWindow(std::move(gravityWindow_)),
    m_blockWindow(std::move(blockWindow_))
{
    incmp = std::move(incmp_);
}

template <typename Char_t>
bool Action<Char_t>::isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_, int extendBuffer_) const
{
    if (incmp)
        return (*incmp)(inputQueue_, ownDirection_, extendBuffer_);
    else
        return false;
}

template <typename Char_t>
const HurtboxVec Action<Char_t>::getCurrentHurtboxes(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const
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

template <typename Char_t>
void Action<Char_t>::switchTo(Char_t &character_) const
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

template <typename Char_t>
bool Action<Char_t>::isInCounterState(uint32_t currentFrame_) const
{
    return m_counterWindow[currentFrame_];
}

template <typename Char_t>
bool Action<Char_t>::applyGravity(uint32_t currentFrame_) const
{
    return m_gravityWindow[currentFrame_];
}

template <typename Char_t>
bool Action<Char_t>::canBlock(uint32_t currentFrame_) const
{
    return m_blockWindow[currentFrame_];
}

// ABSTRACT PROLONGED ACTION
template <typename Char_t>
Action_prolonged<Char_t>::Action_prolonged(int actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_) :
    Action<Char_t>(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, std::move(counterWindow_), std::move(gravityWindow_), std::move(blockWindow_), false, false, false)
{
    incmp_prolonged = std::move(incmp_prolonged_);
}

template <typename Char_t>
int Action_prolonged<Char_t>::isPossibleToProlong(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const
{
    return (*incmp_prolonged)(inputQueue_, ownDirection_);
}



// ABSTRACT JUMP ACTION
template <typename Char_t>
Action_jump<Char_t>::Action_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_) :
    Action<Char_t>(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, std::move(counterWindow_), TimelineProperty(true), std::move(blockWindow_), false, false, false),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
}




// ABSTRACT ATTACK ACTION
template <typename Char_t>
Action_attack<Char_t>::Action_attack(int actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, TimelineProperty<Vector2<float>> &&velocity_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, bool isCrouchState_, bool stepOnly_) :
    Action<Char_t>(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, hitutils::getRegularCounterTimeline(hits_), std::move(gravityWindow_), TimelineProperty(false), true, isCrouchState_, false),
    m_fullDuration(fullDuration_),
    m_hits(hits_),
    m_velocity(std::move(velocity_)),
    m_stepOnly(stepOnly_)
{
}

template <typename Char_t>
const HitsVec Action_attack<Char_t>::getCurrentHits(uint32_t currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const
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

template <typename Char_t>
void Action_attack<Char_t>::switchTo(Char_t &character_) const
{
    character_.turnVelocityToInertia();
    Action<Char_t>::switchTo(character_);
    character_.m_timer.begin(m_fullDuration);
}

template <typename Char_t>
void Action_attack<Char_t>::update(Char_t &character_) const
{
    if (!m_velocity.isEmpty())
    {
        character_.m_velocity = m_velocity[character_.m_timer.getCurrentFrame() + 1].mulComponents(Vector2{character_.getOwnHorDir().x, 1.0f});
    }
}


// ABSTRACT THROW STARTUP
template <typename Char_t>
Action_throw_startup<Char_t>::Action_throw_startup(int actionState_, int whiffState_, int holdState_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float range_, FrameWindow activeWindow_, bool requiredAirborne_, THROW_LIST throw_) :
    Action<Char_t>(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(true), std::move(gravityWindow_), TimelineProperty(false), false, false, true),
    m_whiffState(whiffState_),
    m_holdState(holdState_),
    m_range(range_),
    m_activeWindow(activeWindow_),
    m_requiredAirborne(requiredAirborne_),
    m_throw(throw_)
{
}

template <typename Char_t>
void Action_throw_startup<Char_t>::switchTo(Char_t &character_) const
{
    character_.turnVelocityToInertia();
    Action<Char_t>::switchTo(character_);
    character_.m_timer.begin(m_activeWindow.second);
    if (character_.m_airborne)
        character_.m_usedAirAttack = true;
}

template <typename Char_t>
void Action_throw_startup<Char_t>::attemptThrow(Char_t &character_) const
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

template <typename Char_t>
void Action_throw_startup<Char_t>::outdated(Char_t &character_) const
{
    character_.m_actionResolver.getAction(m_whiffState)->switchTo(character_);
}

// ABSTRACT THROW HOLD
template <typename Char_t>
Action_throw_hold<Char_t>::Action_throw_hold(int actionState_, int throwState_, float setRange_, float duration_, bool sideSwitch_) :
    Action<Char_t>(actionState_, nullptr, {}, ANIMATIONS::NONE, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false), false, false, false),
    m_setRange(setRange_),
    m_duration(duration_),
    m_throwState(throwState_),
    m_sideSwitch(sideSwitch_)
{
}

template <typename Char_t>
int Action_throw_hold<Char_t>::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

template <typename Char_t>
void Action_throw_hold<Char_t>::switchTo(Char_t &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action<Char_t>::switchTo(character_);
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

template <typename Char_t>
void Action_throw_hold<Char_t>::update(Char_t &character_) const
{
}

template <typename Char_t>
void Action_throw_hold<Char_t>::outdated(Char_t &character_) const
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
template <typename Char_t>
Action_throw_whiff<Char_t>::Action_throw_whiff(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, float duration_, HurtboxFramesVec &&hurtboxes_) :
    Action<Char_t>(actionState_, nullptr, std::move(hurtboxes_), anim_, TimelineProperty(false), std::move(gravityWindow_), TimelineProperty(false), false, false, false),
    m_duration(duration_)
{
}

template <typename Char_t>
int Action_throw_whiff<Char_t>::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

template <typename Char_t>
void Action_throw_whiff<Char_t>::switchTo(Char_t &character_) const
{
    Action<Char_t>::switchTo(character_);
    character_.m_timer.begin(m_duration);
}

template <typename Char_t>
void Action_throw_whiff<Char_t>::outdated(Char_t &character_) const
{
    if (!character_.m_airborne)
        character_.switchToIdle();
    else
        character_.switchToFloat();
}

// ABSTRACT THROWN HOLD
template <typename Char_t>
Action_thrown_hold<Char_t>::Action_thrown_hold(int actionState_, int thrownState_, ANIMATIONS anim_, float duration_) :
    Action<Char_t>(actionState_, nullptr, {}, anim_, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false), false, false, false),
    m_duration(duration_),
    m_thrownState(thrownState_)
{
}

template <typename Char_t>
int Action_thrown_hold<Char_t>::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

template <typename Char_t>
void Action_thrown_hold<Char_t>::switchTo(Char_t &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action<Char_t>::switchTo(character_);
    character_.m_timer.begin(m_duration);

    character_.lockInAnimation();
    character_.tieAnimWithOpponent();
}

template <typename Char_t>
void Action_thrown_hold<Char_t>::outdated(Char_t &character_) const
{
    //character_.switchToIdle();
    character_.m_actionResolver.getAction(m_thrownState)->switchTo(character_);
}

// ABSTRACT THROW TECH
template <typename Char_t>
Action_throw_tech<Char_t>::Action_throw_tech(int actionState_, InputComparator_ptr incmp_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, TimelineProperty<bool> &&blockWindow_, float duration_, HurtboxFramesVec &&hurtboxes_, THROW_TECHS_LIST throwTech_) :
    Action<Char_t>(actionState_, std::move(incmp_), std::move(hurtboxes_), anim_, TimelineProperty(false), std::move(gravityWindow_), std::move(blockWindow_), false, false, false),
    m_duration(duration_),
    m_throwTech(throwTech_)
{
}

template <typename Char_t>
void Action_throw_tech<Char_t>::switchTo(Char_t &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action<Char_t>::switchTo(character_);
    character_.m_timer.begin(m_duration);
    
    character_.releaseFromAnimation();
    if (character_.m_tiedAnimWithOpponent)
    {
        character_.untieAnimWithOpponent();
        character_.m_otherCharacter->throwTeched(m_throwTech);
    }
}

template <typename Char_t>
void Action_throw_tech<Char_t>::outdated(Char_t &character_) const
{
    if (!character_.m_airborne)
        character_.switchToIdle();
    else
        character_.switchToFloat();
}

// ABSTRACT LOCKED ANIMATION
template <typename Char_t>
Action_locked_animation<Char_t>::Action_locked_animation(int actionState_, int quitState_, HurtboxFramesVec &&hurtboxes_, ANIMATIONS anim_, float duration_, TimelineProperty<bool> &&counterWindow_, TimelineProperty<bool> &&blockWindow_) :
    Action<Char_t>(actionState_, nullptr, std::move(hurtboxes_), anim_, std::move(counterWindow_), TimelineProperty(false), std::move(blockWindow_), false, false, false),
    m_duration(duration_),
    m_quitState(quitState_)
{
}

template <typename Char_t>
int Action_locked_animation<Char_t>::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

template <typename Char_t>
void Action_locked_animation<Char_t>::switchTo(Char_t &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    Action<Char_t>::switchTo(character_);
    character_.m_timer.begin(m_duration);
    character_.lockInAnimation();
}

template <typename Char_t>
void Action_locked_animation<Char_t>::outdated(Char_t &character_) const
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
    Action((int)CHAR1_STATE::IDLE, std::make_unique<InputComparatorIdle>(), {{TimelineProperty(true), gamedata::characters::char1::standingHurtbox}}, ANIMATIONS::CHAR1_IDLE, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false)
{
}

int Action_char1_idle::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    auto *chr1 = dynamic_cast<Char1*>(char_);

    switch (chr1->m_currentState)
    {
        case ((int)CHAR1_STATE::IDLE):
            return -1;
            break;

        case ((int)CHAR1_STATE::CROUCH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_BWD):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_idle::switchTo(Char1 &character_) const
{
    character_.turnVelocityToInertia();
    Action<Char1>::switchTo(character_);
    character_.updateOwnOrientation();
    character_.m_healthHandler.resetScaling();
    character_.m_comboPhysHandler.reset();
    character_.m_blockstunType = BLOCK_FRAME::NONE;
}

void Action_char1_idle::update(Char1 &character_) const
{
    character_.updateOwnOrientation();
}

// CROUCH ACTION
Action_char1_crouch::Action_char1_crouch() :
    Action((int)CHAR1_STATE::CROUCH, std::make_unique<InputComparatorDownHold>(), {{TimelineProperty(true), gamedata::characters::char1::crouchingHurtbox}}, ANIMATIONS::CHAR1_CROUCH_IDLE, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true),  false, false, false)
{
}

int Action_char1_crouch::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    auto *chr1 = dynamic_cast<Char1*>(char_);

    switch (chr1->m_currentState)
    {
        case ((int)CHAR1_STATE::CROUCH):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? -1 : 0);
            break;

        case ((int)CHAR1_STATE::IDLE):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_BWD):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_crouch::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
}

void Action_char1_crouch::update(Char1 &character_) const
{
    character_.updateOwnOrientation();
}


// WALK FORWARD ACTION
Action_char1_walk_fwd::Action_char1_walk_fwd() :
    Action((int)CHAR1_STATE::WALK_FWD, std::make_unique<InputComparatorForward>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_WALK_FWD, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false)
{
}

int Action_char1_walk_fwd::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    auto *chr1 = dynamic_cast<Char1*>(char_);

    switch (chr1->m_currentState)
    {
        case ((int)CHAR1_STATE::WALK_FWD):
            return (isInputPossible(inputQueue_, chr1->getInputDir(), extendBuffer_) ? -1 : 0);
            break;

        case ((int)CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, chr1->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_walk_fwd::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
    character_.m_velocity = character_.getHorDirToEnemy().mulComponents(Vector2{6.0f, 0.0f});
}

void Action_char1_walk_fwd::update(Char1 &character_) const
{
    character_.updateOwnOrientation();
}

// WALK BACKWARD ACTION
Action_char1_walk_bwd::Action_char1_walk_bwd() :
    Action((int)CHAR1_STATE::WALK_BWD, std::make_unique<InputComparatorBackward>(), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_WALK_BWD, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false)
{
}

int Action_char1_walk_bwd::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    auto *chr1 = dynamic_cast<Char1*>(char_);

    switch (chr1->m_currentState)
    {
        case ((int)CHAR1_STATE::WALK_BWD):
            return (isInputPossible(inputQueue_, chr1->getInputDir(), extendBuffer_) ? -1 : 0);
            break;

        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, chr1->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_walk_bwd::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
    character_.m_currentAnimation->reset(65, -1);
    character_.m_velocity = character_.getHorDirToEnemy().mulComponents(Vector2{-6.0f, 0.0f});
}

void Action_char1_walk_bwd::update(Char1 &character_) const
{
    character_.updateOwnOrientation();
}


// ABSTRACT CHAR1 JUMP ACTION
Action_char1_jump::Action_char1_jump(int actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec &&hurtboxes_) :
    Action_jump<Char1>((int)actionState_, impulse_, prejumpLen_, maxHorInertia_, std::move(incmp_), std::move(hurtboxes_), ANIMATIONS::CHAR1_PREJUMP, TimelineProperty(false), TimelineProperty(true)),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
}

int Action_char1_jump::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne())
        return 0;


    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {
        case ((int)CHAR1_STATE::PREJUMP):
            return -1;
            break;

        case ((int)CHAR1_STATE::GROUND_DASH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::GROUND_DASH_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_jump::switchTo(Char1 &character_) const
{
    character_.turnVelocityToInertia();
    Action<Char1>::switchTo(character_);
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
    }, ANIMATIONS::CHAR1_JUMP, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false)
{
}

int Action_char1_float::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_float::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
    character_.m_currentAnimation->reset(20);

    //character_.turnVelocityToInertia();
    character_.m_currentState = (int)CHAR1_STATE::JUMP;
}



// ABSTRACT AIR JUMP ACTION
Action_char1_airjump::Action_char1_airjump(const Vector2<float> &impulse_, InputComparator_ptr incmp_) :
    Action((int)CHAR1_STATE::JUMP, std::move(incmp_), {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_JUMP, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false),
    m_impulse(impulse_)
{
}

int Action_char1_airjump::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    auto *chr1 = dynamic_cast<Char1*>(char_);

    if (char_->isInHitstop() || !char_->m_jumpsAvailable.canConsume() || !char_->m_airjumpTimer.isOver() || !char_->isAirborne())
        return 0;

    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {
        case ((int)CHAR1_STATE::AIR_DASH_EXTENTION):
            [[fallthrough]];
        case ((int)CHAR1_STATE::JUMP):
            return ((isInputPossible(inputQueue_, char_->getOrientationToEnemy(), extendBuffer_)) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_airjump::switchTo(Char1 &character_) const
{
    character_.updateOwnOrientation();

    if (character_.m_currentState == (int)CHAR1_STATE::AIR_DASH_EXTENTION || (character_.m_currentAction && character_.m_currentAction->m_isAttack))
    {
        character_.turnVelocityToInertia();
        character_.m_inertia.x /= gamedata::characters::char1::doubleJumpInertiaDivider;
    }

    Action<Char1>::switchTo(character_);

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
    }, ANIMATIONS::CHAR1_GROUND_DASH, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true)),
    m_accel(gamedata::characters::char1::dashAccel),
    m_maxspd(gamedata::characters::char1::dashMaxSpeed)
{
}

int Action_char1_ground_dash::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    auto *chr1 = dynamic_cast<Char1*>(char_);

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (chr1->m_currentState)
    {
        case ((int)CHAR1_STATE::GROUND_DASH):
            return (isPossibleToProlong(inputQueue_, char_->getInputDir()) ? -1 : 0);
            break;

        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return ((isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_)) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_ground_dash::update(Char1 &character_) const
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
    }, ANIMATIONS::CHAR1_STEP, TimelineProperty(true), TimelineProperty(true), TimelineProperty(false), false, false, false),
    m_duration(gamedata::characters::char1::stepDuration)
{
}

int Action_char1_step::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne())
        return 0;
    
    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    return false;
}

void Action_char1_step::outdated(Char1 &character_) const
{
    //character_.m_velocity.x = 0;
    character_.m_actionResolver.getAction((int)CHAR1_STATE::STEP_RECOVERY)->switchTo(character_);
}

void Action_char1_step::switchTo(Char1 &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{gamedata::characters::char1::stepSpeed, 0.0f});
    Action<Char1>::switchTo(character_);
    character_.m_timer.begin(m_duration);
}

// STEP RECOVERY ACTION
Action_char1_step_recovery::Action_char1_step_recovery() :
    Action((int)CHAR1_STATE::STEP_RECOVERY, std::move(std::make_unique<InputComparator66>()), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_STEP_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false),
    m_recoveryLen(gamedata::characters::char1::stepRecoveryDuration)
{
}

int Action_char1_step_recovery::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    auto *chr1 = dynamic_cast<Char1*>(char_);

    switch (chr1->m_currentState)
    {
        case ((int)CHAR1_STATE::STEP_RECOVERY):
            return -1;
            break;

        default:
            return 0;
            break;
    }
}

void Action_char1_step_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_step_recovery::switchTo(Char1 &character_) const
{
    character_.turnVelocityToInertia();
    Action<Char1>::switchTo(character_);
    character_.m_timer.begin(m_recoveryLen);
}

void Action_char1_step_recovery::update(Char1 &character_) const
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
    }, ANIMATIONS::CHAR1_BACKDASH, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false),
    m_totalDuration(gamedata::characters::char1::backdashDuration)
{
}

int Action_char1_ground_backdash::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {
        case ((int)CHAR1_STATE::GROUND_BACKDASH):
            return -1;
            break;

        case ((int)CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::SOFT_LANDING_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::CROUCH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_ground_backdash::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_ground_backdash::switchTo(Char1 &character_) const
{
    if (character_.m_currentState == (int)CHAR1_STATE::SOFT_LANDING_RECOVERY)
    {
        character_.updateOwnOrientation();
    }

    Action<Char1>::switchTo(character_);

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
    }, ANIMATIONS::CHAR1_AIRDASH, TimelineProperty(false), TimelineProperty(false), TimelineProperty(false), false, false, false),
    m_duration(gamedata::characters::char1::airdashDuration)
{
}

int Action_char1_air_dash::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || !char_->m_airdashesAvailable.canConsume() || !(char_->m_airdashTimer.isOver()))
        return 0;

    if (char_->isInInstantBlockstun())
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {
        case ((int)CHAR1_STATE::AIR_DASH):
            return -1;
            break;

        case ((int)CHAR1_STATE::JUMP):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_air_dash::outdated(Char1 &character_) const
{
    character_.m_actionResolver.getAction((int)CHAR1_STATE::AIR_DASH_EXTENTION)->switchTo(character_);
    character_.m_currentAnimation = character_.m_animations[m_anim].get();
}

void Action_char1_air_dash::switchTo(Char1 &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{gamedata::characters::char1::airdashSpeed, 0.0f});
    Action<Char1>::switchTo(character_);
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
    }, ANIMATIONS::CHAR1_AIR_BACKDASH, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false),
    m_duration(gamedata::characters::char1::airBackdashDuration)
{
}

int Action_char1_air_backdash::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{

    if (char_->isInHitstop() || !char_->m_airdashesAvailable.canConsume() || !char_->m_airdashTimer.isOver() || !char_->isAirborne())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {
        case ((int)CHAR1_STATE::AIR_BACKDASH):
            return -1;
            break;

        case ((int)CHAR1_STATE::JUMP):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_air_backdash::outdated(Char1 &character_) const
{
    character_.m_velocity.x /= 5.0f;
    character_.switchToFloat();
}

void Action_char1_air_backdash::switchTo(Char1 &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, -10.0f};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{-gamedata::characters::char1::airBackdashSpeed, 0.0f});
    Action<Char1>::switchTo(character_);
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
    }, ANIMATIONS::CHAR1_AIRDASH, TimelineProperty(false), TimelineProperty(false), TimelineProperty(true), false, false, false),
    m_duration(gamedata::characters::char1::airdashExtentionDuration),
    m_baseSpd(gamedata::characters::char1::airdashExtentionMaxSpeed),
    m_spdMultiplier((gamedata::characters::char1::airdashExtentionMaxSpeed - gamedata::characters::char1::airdashExtentionMinSpeed) / gamedata::characters::char1::airdashExtentionDuration)
{
}

int Action_char1_air_dash_extention::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_air_dash_extention::outdated(Char1 &character_) const
{
    character_.switchToFloat();
    character_.updateOwnOrientation();
}

void Action_char1_air_dash_extention::switchTo(Char1 &character_) const
{
    character_.m_velocity = {0, 0};
    character_.m_inertia = {0, 0};
    character_.m_velocity = character_.getOwnHorDir().mulComponents(Vector2{(float)m_baseSpd, 0.0f});
    Action<Char1>::switchTo(character_);
    character_.m_timer.begin(m_duration);
}

void Action_char1_air_dash_extention::setVelocity(Char1 &character_) const
{
    character_.m_velocity.x = character_.getOwnHorDir().x * (m_baseSpd - m_spdMultiplier * (character_.m_timer.getCurrentFrame() + 1));
}

void Action_char1_air_dash_extention::update(Char1 &character_) const
{
    setVelocity(character_);
}


// GROUND DASH RECOVERY ACTION
Action_char1_ground_dash_recovery::Action_char1_ground_dash_recovery() :
    Action((int)CHAR1_STATE::GROUND_DASH_RECOVERY, std::move(std::make_unique<InputComparator66>()), {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_GROUND_DASH_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false),
    m_recoveryLen(gamedata::characters::char1::dashRecovery)
{
}

int Action_char1_ground_dash_recovery::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    auto *chr1 = dynamic_cast<Char1*>(char_);

    switch (chr1->m_currentState)
    {
        case ((int)CHAR1_STATE::GROUND_DASH_RECOVERY):
            return -1;
            break;

        case ((int)CHAR1_STATE::GROUND_DASH):
            return 1;
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_ground_dash_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_ground_dash_recovery::switchTo(Char1 &character_) const
{
    character_.turnVelocityToInertia();
    Action<Char1>::switchTo(character_);
    character_.m_timer.begin(m_recoveryLen);
}

// SOFT LANDING RECOVERY
Action_char1_soft_landing_recovery::Action_char1_soft_landing_recovery() :
    Action((int)CHAR1_STATE::SOFT_LANDING_RECOVERY, nullptr, {
        {
            TimelineProperty(true),
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false),
    m_recoveryLen(gamedata::characters::char1::softLandingRecovery)
{
}

int Action_char1_soft_landing_recovery::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_soft_landing_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_soft_landing_recovery::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
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
    }, ANIMATIONS::CHAR1_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), false, false, false),
    m_recoveryLen(gamedata::characters::char1::hardLandingRecovery)
{
}

int Action_char1_hard_landing_recovery::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_hard_landing_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_hard_landing_recovery::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
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
    }, ANIMATIONS::CHAR1_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false),
    m_recoveryLen(gamedata::characters::char1::vulnerableLandingRecovery)
{
}

int Action_char1_vulnerable_landing_recovery::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_vulnerable_landing_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_vulnerable_landing_recovery::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
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
    }, ANIMATIONS::CHAR1_JC_LANDING_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false),
    m_recoveryLen(gamedata::characters::char1::jcLandingRecovery)
{
}

int Action_char1_jc_landing_recovery::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_jc_landing_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_jc_landing_recovery::switchTo(Char1 &character_) const
{
    Action<Char1>::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
    character_.m_inertia = {0.0f, 0.0f};
    character_.m_timer.begin(m_recoveryLen);
}

// SOFT KNOCKDOWN ACTION
Action_char1_soft_knockdown::Action_char1_soft_knockdown() :
    Action((int)CHAR1_STATE::SOFT_KNOCKDOWN, nullptr, {}, ANIMATIONS::CHAR1_SOFT_KNOCKDOWN, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false)
{
}

int Action_char1_soft_knockdown::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_soft_knockdown::outdated(Char1 &character_) const
{
    character_.enterKndRecovery();
}

void Action_char1_soft_knockdown::switchTo(Char1 &character_) const
{
    character_.takePushback({0, 0});
    character_.turnVelocityToInertia();
    Action<Char1>::switchTo(character_);
    character_.m_timer.begin(8);
}

// HARD KNOCKDOWN ACTION
Action_char1_hard_knockdown::Action_char1_hard_knockdown() :
    Action((int)CHAR1_STATE::HARD_KNOCKDOWN, nullptr, {}, ANIMATIONS::CHAR1_KNOCKDOWN, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false)
{
}

int Action_char1_hard_knockdown::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_hard_knockdown::outdated(Char1 &character_) const
{
    character_.enterKndRecovery();
}

void Action_char1_hard_knockdown::switchTo(Char1 &character_) const
{
    character_.takePushback({0, 0});
    Action<Char1>::switchTo(character_);
    character_.m_timer.begin(30);
    character_.m_velocity = {0.0f, 0.0f};
}

// KNOCKDOWN RECOVERY ACTION
Action_char1_knockdown_recovery::Action_char1_knockdown_recovery() :
    Action((int)CHAR1_STATE::KNOCKDOWN_RECOVERY, nullptr, {}, ANIMATIONS::CHAR1_KNOCKDOWN_RECOVERY, TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), false, false, false)
{
}

int Action_char1_knockdown_recovery::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_knockdown_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
    character_.setThrowInvul();
}

void Action_char1_knockdown_recovery::switchTo(Char1 &character_) const
{
    character_.turnVelocityToInertia();
    character_.takePushback({0, 0});
    Action<Char1>::switchTo(character_);
    character_.m_timer.begin(21);
    character_.m_healthHandler.resetScaling();
    character_.m_comboPhysHandler.reset();
}

// ABSTRACT CHAR1 GROUND ATTACK ACTION
Action_char1_ground_attack::Action_char1_ground_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_, TimelineProperty<Vector2<float>> &&velocity_, bool isCrouchState_, bool stepOnly_) :
    Action_attack<Char1>(actionState_, std::move(incmp_), fullDuration_, hits_, std::move(hurtboxes_), std::move(velocity_), anim_, std::move(gravityWindow_), isCrouchState_, stepOnly_)
{
}

int Action_char1_ground_attack::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    if (m_stepOnly)
    {
        if (char_->m_currentState == (int)CHAR1_STATE::STEP_RECOVERY)
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
        else
            return 0;
    }

    switch (char_->m_currentState)
    {

        case ((int)CHAR1_STATE::SOFT_LANDING_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::GROUND_DASH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::GROUND_DASH_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::CROUCH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::STEP_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_ground_attack::outdated(Char1 &character_) const
{
    character_.m_velocity = {0.0f, 0.0f};
    character_.switchToIdle();
}

void Action_char1_ground_attack::switchTo(Char1 &character_) const
{
    if (character_.m_currentState == (int)CHAR1_STATE::SOFT_LANDING_RECOVERY)
                    character_.updateOwnOrientation();

    Action_attack<Char1>::switchTo(character_);
}

// ABSTRACT CHAR1 AIR ATTACK ACTION
Action_char1_air_attack::Action_char1_air_attack(int actionState_, ANIMATIONS anim_, TimelineProperty<bool> &&gravityWindow_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec &&hurtboxes_) :
    Action_attack<Char1>(actionState_, std::move(incmp_), fullDuration_, hits_, std::move(hurtboxes_), TimelineProperty<Vector2<float>>{}, anim_, std::move(gravityWindow_), false, false)
{
}

int Action_char1_air_attack::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || !char_->isAirborne())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {

        case ((int)CHAR1_STATE::AIR_DASH_EXTENTION):
            [[fallthrough]];
        case ((int)CHAR1_STATE::JUMP):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_air_attack::outdated(Char1 &character_) const
{
    character_.switchToFloat();
}

void Action_char1_air_attack::switchTo(Char1 &character_) const
{
    character_.m_usedAirAttack = true;
    if (character_.m_currentState == (int)CHAR1_STATE::AIR_DASH_EXTENTION)
    {
        character_.turnVelocityToInertia();
    }

    Action<Char1>::switchTo(character_);
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
    })
{
}

void Action_char1_move_JC::update(Char1 &character_) const
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
        }), false, false)
    /*{
        {
            {1, 4},
            {6.0f, 0.0f}
        },
        {
            {10, 15},
            {2.0f, 0.0f}
        },
        {
            {18, 18},
            {80.0f, 0.0f}
        }
    })*/
{
}

void Action_char1_move_214C::update(Char1 &character_) const
{
    Action_attack<Char1>::update(character_);

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
        }), false, false)
{
}

// Normal throw startup
Action_char1_normal_throw_startup::Action_char1_normal_throw_startup() :
    Action_throw_startup<Char1>((int)CHAR1_STATE::THROW_NORMAL_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_WHIFF, (int)CHAR1_STATE::THROW_NORMAL_HOLD,
    std::make_unique<InputComparatorBCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, false, THROW_LIST::CHAR1_NORMAL_THROW)
{
}

int Action_char1_normal_throw_startup::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {

        case ((int)CHAR1_STATE::SOFT_LANDING_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::GROUND_DASH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::GROUND_DASH_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::CROUCH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::STEP_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

// Normal throw hold
Action_char1_normal_throw_hold::Action_char1_normal_throw_hold() :
    Action_throw_hold<Char1>((int)CHAR1_STATE::THROW_NORMAL_HOLD, (int)CHAR1_STATE::THROW_NORMAL_ANIM, 90, 10, false)
{
}

// Back throw startup
Action_char1_back_throw_startup::Action_char1_back_throw_startup() :
    Action_throw_startup<Char1>((int)CHAR1_STATE::THROW_BACK_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_WHIFF, (int)CHAR1_STATE::THROW_BACK_HOLD,
    std::make_unique<InputComparator4BCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, false, THROW_LIST::CHAR1_NORMAL_THROW)
{
}

int Action_char1_back_throw_startup::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || char_->isAirborne())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {

        case ((int)CHAR1_STATE::SOFT_LANDING_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::GROUND_DASH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::GROUND_DASH_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case ((int)CHAR1_STATE::CROUCH):
            [[fallthrough]];
        case ((int)CHAR1_STATE::STEP_RECOVERY):
            [[fallthrough]];
        case ((int)CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

// Back throw hold
Action_char1_back_throw_hold::Action_char1_back_throw_hold() :
    Action_throw_hold<Char1>((int)CHAR1_STATE::THROW_BACK_HOLD, (int)CHAR1_STATE::THROW_NORMAL_ANIM, 90, 10, true)
{
}

// Normal throw whiff
Action_char1_normal_throw_whiff::Action_char1_normal_throw_whiff() :
    Action_throw_whiff<Char1>((int)CHAR1_STATE::THROW_NORMAL_WHIFF, ANIMATIONS::CHAR1_NORMAL_THROW_WHIFF, TimelineProperty(true), 30, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    })
{
}

// NORMAL THROW ACTION
Action_char1_normal_throw::Action_char1_normal_throw() :
    Action_locked_animation<Char1>((int)CHAR1_STATE::THROW_NORMAL_ANIM, (int)CHAR1_STATE::IDLE, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_NORMAL_THROW, 50, TimelineProperty(false), TimelineProperty(false))
{
}

// TODO: turn attacks during animation into timeline property
void Action_char1_normal_throw::update(Char1 &character_) const
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
    Action_throw_startup<Char1>((int)CHAR1_STATE::THROW_NORMAL_AIR_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_AIR_WHIFF, (int)CHAR1_STATE::THROW_NORMAL_AIR_HOLD,
    std::make_unique<InputComparatorBCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::floatingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, true, THROW_LIST::CHAR1_AIR_THROW)
{
}

int Action_char1_normal_air_throw_startup::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || !char_->isAirborne())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {
        case ((int)CHAR1_STATE::AIR_DASH_EXTENTION):
            [[fallthrough]];
        case ((int)CHAR1_STATE::JUMP):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

// Air throw hold
Action_char1_normal_air_throw_hold::Action_char1_normal_air_throw_hold() :
    Action_throw_hold<Char1>((int)CHAR1_STATE::THROW_NORMAL_AIR_HOLD, (int)CHAR1_STATE::THROW_NORMAL_AIR_ANIM, 90, 10, false)
{
}

// Air back throw startup
Action_char1_back_air_throw_startup::Action_char1_back_air_throw_startup() :
    Action_throw_startup<Char1>((int)CHAR1_STATE::THROW_BACK_AIR_STARTUP, (int)CHAR1_STATE::THROW_NORMAL_AIR_WHIFF, (int)CHAR1_STATE::THROW_BACK_AIR_HOLD,
    std::make_unique<InputComparator4BCPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::floatingHurtbox
        }
    },
    ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, TimelineProperty(true), 200.0f, {2, 5}, true, THROW_LIST::CHAR1_AIR_THROW)
{
}

int Action_char1_back_air_throw_startup::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop() || !char_->isAirborne())
        return 0;

    auto *chr1 = dynamic_cast<Char1*>(char_);

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {
        case ((int)CHAR1_STATE::AIR_DASH_EXTENTION):
            [[fallthrough]];
        case ((int)CHAR1_STATE::JUMP):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

// Air back throw hold
Action_char1_back_air_throw_hold::Action_char1_back_air_throw_hold() :
    Action_throw_hold<Char1>((int)CHAR1_STATE::THROW_BACK_AIR_HOLD, (int)CHAR1_STATE::THROW_NORMAL_AIR_ANIM, 90, 10, true)
{
}

// Air throw whiff
Action_char1_normal_air_throw_whiff::Action_char1_normal_air_throw_whiff() :
    Action_throw_whiff<Char1>((int)CHAR1_STATE::THROW_NORMAL_AIR_WHIFF, ANIMATIONS::CHAR1_NORMAL_THROW_WHIFF, TimelineProperty(true), 200, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::floatingHurtbox
        }
    })
{
}

// AIR THROW ACTION
Action_char1_normal_air_throw::Action_char1_normal_air_throw() :
    Action_locked_animation<Char1>((int)CHAR1_STATE::THROW_NORMAL_AIR_ANIM, (int)CHAR1_STATE::IDLE, {
        {
            TimelineProperty<bool>({{1, true}, {51, false}}),
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_NORMAL_THROW, 20, TimelineProperty(false), TimelineProperty(false))
{
}

void Action_char1_normal_air_throw::update(Char1 &character_) const
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
    Action_throw_tech<Char1>((int)CHAR1_STATE::THROW_TECH_OWN,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 15,
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, THROW_TECHS_LIST::CHAR1_GROUND)
{
}

int Action_char1_throw_tech::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {

        case ((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_throw_tech::switchTo(Char1 &character_) const
{
    Action_throw_tech<Char1>::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 5.0f;
    character_.callForPriority();
}

// Throw tech state when Char1 breaks throw
Action_char1_throw_tech_char1::Action_char1_throw_tech_char1() :
    Action_throw_tech<Char1>((int)CHAR1_STATE::THROW_TECH_CHAR1,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 25,
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, THROW_TECHS_LIST::NONE)
{
}

int Action_char1_throw_tech_char1::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_throw_tech_char1::switchTo(Char1 &character_) const
{
    Action_throw_tech<Char1>::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 20.0f;
}

// Air throw tech
Action_char1_air_throw_tech::Action_char1_air_throw_tech() :
    Action_throw_tech<Char1>((int)CHAR1_STATE::AIR_THROW_TECH_OWN,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 15,
    {
        {
            TimelineProperty<bool>({{1, true}, {16, false}}),
            {-70, -350, 140, 300}
        }
    }, THROW_TECHS_LIST::CHAR1_AIR)
{
}

int Action_char1_air_throw_tech::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    if (char_->isInHitstop())
        return 0;

    if (char_->isCancelAllowed(actionState))
    {
        return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
    }

    switch (char_->m_currentState)
    {

        case ((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD):
            return (isInputPossible(inputQueue_, char_->getInputDir(), extendBuffer_) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_air_throw_tech::switchTo(Char1 &character_) const
{
    Action_throw_tech<Char1>::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 3.0f;
}

// Throw tech state when Char1 breaks air throw
Action_char1_air_throw_tech_char1::Action_char1_air_throw_tech_char1() :
    Action_throw_tech<Char1>((int)CHAR1_STATE::AIR_THROW_TECH_CHAR1,
    std::make_unique<InputComparatorBCPress>(), ANIMATIONS::CHAR1_THROW_TECH, TimelineProperty(true), TimelineProperty(true), 200,
    {
        {
            TimelineProperty<bool>({{1, true}, {200, false}}),
            {-70, -350, 140, 300}
        }
    }, THROW_TECHS_LIST::NONE)
{
}

int Action_char1_air_throw_tech_char1::isPossible(const InputQueue &inputQueue_, Character *char_, int extendBuffer_) const
{
    return false;
}

void Action_char1_air_throw_tech_char1::switchTo(Char1 &character_) const
{
    Action_throw_tech<Char1>::switchTo(character_);
    character_.m_inertia.x += -character_.getOwnHorDir().x * 10.0f;
}

// Thrown by char1 normal throw - hold
Action_char1_thrown_char1_normal_hold::Action_char1_thrown_char1_normal_hold() :
    Action_thrown_hold<Char1>((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD, (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_ANIM, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD, 10)
{
}

// Thrown by char1 normal throw - animation
Action_char1_thrown_char1_normal::Action_char1_thrown_char1_normal() :
    Action_locked_animation<Char1>((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_ANIM, (int)CHAR1_STATE::HITSTUN_AIR, {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL, 14, TimelineProperty(false), TimelineProperty(false))
{
}

void Action_char1_thrown_char1_normal::update(Char1 &character_) const
{
    auto curFrame = character_.m_timer.getCurrentFrame();
    if (curFrame == 1)
        character_.m_velocity = {character_.getOwnHorDir().x * 0.8f, 5.0f};
    else if (curFrame == 10)
        character_.m_velocity = {0, 0};
}

void Action_char1_thrown_char1_normal::outdated(Char1 &character_) const
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
    Action_thrown_hold<Char1>((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD, (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_ANIM, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD, 10)
{
}

// Thrown by char1 normal air throw - animation
Action_char1_thrown_char1_normal_air::Action_char1_thrown_char1_normal_air() :
    Action_locked_animation<Char1>((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_ANIM, (int)CHAR1_STATE::HITSTUN_AIR, {
        {
            TimelineProperty<bool>({{1, true}, {51, false}}),
            {-70, -375, 140, 375}
        }
    }, ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL, 14, TimelineProperty(false), TimelineProperty(false))
{
}

void Action_char1_thrown_char1_normal_air::update(Char1 &character_) const
{
    //8-16
    auto curFrame = character_.m_timer.getCurrentFrame();
    if (curFrame == 1)
        character_.m_velocity = {character_.getOwnHorDir().x * 0.8f, 5.0f};
    else if (curFrame == 10)
        character_.m_velocity = {0, 0};
}

void Action_char1_thrown_char1_normal_air::outdated(Char1 &character_) const
{
    character_.releaseFromAnimation();
    character_.untieAnimWithOpponent();
    character_.m_inertia = {-character_.getOwnHorDir().x * 5.0f, -10.0f};
    character_.m_airborne = true;
    character_.enterHitstunAnimation(character_.m_hitProps);
}


template Action<Char1>;
template Action_jump<Char1>;
template Action_prolonged<Char1>;