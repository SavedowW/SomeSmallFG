#include "Application.h"
#include "Char1.h"
#include <memory>
#include "Actions.h"
#include <stdexcept>

ActionResolver_Char1::ActionResolver_Char1(InputSystem *input_) :
    ActionResolver(input_)
{

}

void ActionResolver_Char1::createActions()
{
    m_actions.push_back(std::make_unique<Action_char1_air_dash>());
    m_actions.push_back(std::make_unique<Action_char1_air_backdash>());
    m_actions.push_back(std::make_unique<Action_char1_backward_doublejump>());
    m_actions.push_back(std::make_unique<Action_char1_forward_doublejump>());
    m_actions.push_back(std::make_unique<Action_char1_neutral_doublejump>());
    m_actions.push_back(std::make_unique<Action_char1_move_JA>());
    m_actions.push_back(std::make_unique<Action_char1_move_2B>());
    m_actions.push_back(std::make_unique<Action_char1_move_C>());
    m_actions.push_back(std::make_unique<Action_char1_move_B>());
    m_actions.push_back(std::make_unique<Action_char1_jab>());
    m_actions.push_back(std::make_unique<Action_char1_ground_backdash>());
    m_actions.push_back(std::make_unique<Action_char1_ground_dash>());
    m_actions.push_back(std::make_unique<Action_char1_backward_jump>());
    m_actions.push_back(std::make_unique<Action_char1_forward_jump>());
    m_actions.push_back(std::make_unique<Action_char1_neutral_jump>());
    m_actions.push_back(std::make_unique<Action_char1_walk_bwd>());
    m_actions.push_back(std::make_unique<Action_char1_walk_fwd>());
    m_actions.push_back(std::make_unique<Action_char1_ground_dash_recovery>());
    m_actions.push_back(std::make_unique<Action_char1_crouch>());
    m_actions.push_back(std::make_unique<Action_char1_idle>());
    m_actions.push_back(std::make_unique<Action_char1_soft_landing_recovery>());
    m_actions.push_back(std::make_unique<Action_char1_hard_knockdown>());
    m_actions.push_back(std::make_unique<Action_char1_knockdown_recovery>());
    m_actions.push_back(std::make_unique<Action_char1_float>());
    m_actions.push_back(std::make_unique<Action_char1_air_dash_extention>());
}

Char1::Char1(Application &application_, Vector2<float> pos_) :
    Character(application_, pos_),
    m_actionResolver(application_.getInputSystem()),
    m_currentAction(nullptr)
{

}

void Char1::loadAnimations(Application &application_)
{
    m_animations[ANIMATIONS::CHAR1_IDLE] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_IDLE, LOOPMETHOD::SWITCH_DIR_LOOP);
    m_animations[ANIMATIONS::CHAR1_CROUCH_IDLE] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_CROUCH_IDLE, LOOPMETHOD::JUMP_LOOP);
    m_animations[ANIMATIONS::CHAR1_WALK_FWD] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_WALK_FWD, LOOPMETHOD::JUMP_LOOP);
    m_animations[ANIMATIONS::CHAR1_WALK_BWD] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_WALK_BWD, LOOPMETHOD::JUMP_LOOP, 65, -1);
    m_animations[ANIMATIONS::CHAR1_PREJUMP] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_PREJUMP);
    m_animations[ANIMATIONS::CHAR1_JUMP] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_JUMP);
    m_animations[ANIMATIONS::CHAR1_LANDING_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_LANDING_RECOVERY);
    m_animations[ANIMATIONS::CHAR1_GROUND_DASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_GROUND_DASH, LOOPMETHOD::JUMP_LOOP);
    m_animations[ANIMATIONS::CHAR1_GROUND_DASH_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_GROUND_DASH_RECOVERY);
    m_animations[ANIMATIONS::CHAR1_BACKDASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_BACKDASH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_AIRDASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_AIRDASH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_AIR_BACKDASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_AIR_BACKDASH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_A] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_A, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_B] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_B, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_C] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_C, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_2B] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_2B, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_JA] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_JA, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_LOW] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_LOW, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_MID] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_MID, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_HIGH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_HIGH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_AIR] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_AIR, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_STANDING] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_BLOCKSTUN_STANDING, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_CROUCHING] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_BLOCKSTUN_CROUCHING, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_KNOCKDOWN] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_KNOCKDOWN, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_KNOCKDOWN_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_KNOCKDOWN_RECOVERY, LOOPMETHOD::NOLOOP);

    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_IDLE].get();
    m_currentAnimation->reset();
}

void Char1::initiate()
{
    m_timer.begin(0);

    if (m_playerId == 1)
        m_actionResolver.subscribe_p1();
    else if (m_playerId == 2)
        m_actionResolver.subscribe_p2();
    else
        throw std::runtime_error("Trying to initiate character without player ID");

    m_actionResolver.createActions();
    m_actionResolver.setInputEnabled(true);

    m_pushbox.w = gamedata::characters::char1::standingPushboxWidth;
    m_pushbox.h = gamedata::characters::char1::standingPushboxHeight;
    m_pushbox.x = -m_pushbox.w / 2;
    m_pushbox.y = -m_pushbox.h;

    m_crouchingPushbox.w = gamedata::characters::char1::crouchingPushboxWidth;
    m_crouchingPushbox.h = gamedata::characters::char1::crouchingPushboxHeight;
    m_crouchingPushbox.x = -m_crouchingPushbox.w / 2;
    m_crouchingPushbox.y = -m_crouchingPushbox.h;

    m_airbornePushbox.w = gamedata::characters::char1::airbornePushboxWidth;
    m_airbornePushbox.h = gamedata::characters::char1::airbornePushboxHeight;
    m_airbornePushbox.x = -m_airbornePushbox.w / 2;
    m_airbornePushbox.y = -m_airbornePushbox.h - gamedata::characters::char1::airbornePushboxOffset;

    m_inertiaDrag = gamedata::characters::char1::inertiaDrag;
    m_gravity = gamedata::characters::char1::gravity;

    m_standingHurtbox = {-70, -375, 140, 375};
    m_airHitstunHurtbox = {-350/2, -160, 350, 120};
    m_crouchingHurtbox = {-70, -200, 140, 200};

    switchToIdle();
}

void Char1::proceedCurrentState()
{
    if (m_jumpFramesCounter && m_airborne)
        m_jumpFramesCounter--;

    if (m_airadashFramesCounter && m_airborne)
        m_airadashFramesCounter--;

    if (!m_inHitstop)
    {
        auto timerRes = m_timer.update();

        if (timerRes)
        {
            switch (m_currentState)
            {
                case (CHAR1_STATE::PREJUMP):
                    jumpUsingAction();
                    break;
                
                case (CHAR1_STATE::HITSTUN):
                    [[fallthrough]];
                case (CHAR1_STATE::BLOCKSTUN_CROUCHING):
                    [[fallthrough]];
                case (CHAR1_STATE::BLOCKSTUN_STANDING):                
                    switchToIdle();
                    m_currentTakenHit.m_hitId = -1;
                    break;

                default:
                    if (m_currentAction)
                        m_currentAction->outdated(*this);
            }
        }
    }
}

void Char1::updateState()
{
    framesInState++;

    if (!m_currentCancelWindow.second.empty() && !m_inHitstop)
    {
        m_cancelTimer.update();
        int timer = m_cancelTimer.getCurrentFrame();
        if (timer > m_currentCancelWindow.first.second)
        {
            std::cout << "Cancel window outdated\n";
            m_cancelTimer.begin(0);
            m_currentCancelWindow = {};
        }
    }

    auto resolverRes = m_actionResolver.update(generateCharData());

    if (resolverRes)
    {
        resolverRes->switchTo(*this);
    }

    /*if (m_currentState == CHAR1_STATE::IDLE ||
    m_currentState == CHAR1_STATE::WALK_BWD ||
    m_currentState == CHAR1_STATE::WALK_FWD ||
    m_currentState == CHAR1_STATE::CROUCH)
        updateOwnOrientation();

    if (m_currentState == CHAR1_STATE::GROUND_DASH)
    {
        auto dashAction = dynamic_cast<const Action_char1_ground_dash*>(m_currentAction);
        auto newXVelocity = (abs(m_velocity.x) + dashAction->m_accel);
        m_velocity.x = getOwnHorDir().x * std::min(abs(newXVelocity), abs(dashAction->m_maxspd));
    }
    else if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto atkAction = dynamic_cast<const Action_attack<CHAR1_STATE, Char1Data, Char1>*>(m_currentAction);
        auto newVelocity = atkAction->getCurrentVelocity(m_timer.getCurrentFrame() + 1);
        if (newVelocity)
            m_velocity = *newVelocity * getOwnHorDir().x;
    }
    else if (m_currentState == CHAR1_STATE::AIR_DASH_EXTENTION)
    {
        dynamic_cast<const Action_char1_air_dash_extention*>(m_currentAction)->setVelocity(*this);
    }*/
    if (m_currentAction)
        m_currentAction->update(*this);
}

void Char1::switchToIdle()
{
    m_actionResolver.getAction(CHAR1_STATE::IDLE)->switchTo(*this);
}

void Char1::jumpUsingAction()
{
    turnVelocityToInertia();

    auto ownOrientationVector = getOwnHorDir();
    ownOrientationVector.y = 1;
    auto jumpAction = dynamic_cast<const Action_jump<CHAR1_STATE, Char1Data, Char1>*>(m_currentAction);
    m_velocity = (jumpAction)->m_impulse.mulComponents(ownOrientationVector);
    if (m_inertia.y > 0)
        m_inertia.y = 0;

    auto currentXInertia = m_inertia.x;
    currentXInertia *= ownOrientationVector.x;
    if (currentXInertia > jumpAction->m_maxHorInertia)
    {
        std::cout << "LIMIT INERTIA\n";
        m_inertia.x = jumpAction->m_maxHorInertia * ownOrientationVector.x;
    }

    m_currentState = CHAR1_STATE::JUMP;
    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_JUMP].get();
    m_currentAnimation->reset();
}

void Char1::switchToSoftLandingRecovery()
{
    m_actionResolver.getAction(CHAR1_STATE::SOFT_LANDING_RECOVERY)->switchTo(*this);
}

Char1Data Char1::generateCharData()
{
    Char1Data charData;
    charData.pos = m_pos;
    charData.airborne = m_airborne;
    charData.velocity = m_velocity;
    charData.inertia = m_inertia;
    charData.dirToEnemy = m_dirToEnemy;
    charData.ownDirection = m_ownOrientation;
    charData.dirToEnemyVec = getHorDirToEnemy();
    charData.ownDirectionVec = getOwnHorDir();
    charData.state = m_currentState;
    charData.usedDoubleJump = m_usedDoubleJump;
    charData.usedAirDash = m_usedAirDash;
    charData.inHitstop = m_inHitstop;
    charData.canDoubleJumpAfterPrejump = (m_jumpFramesCounter == 0);
    charData.canAirdashAfterPrejump = (m_airadashFramesCounter == 0);

    if (!m_currentCancelWindow.second.empty())
        charData.cancelOptions = &m_currentCancelWindow.second;

    return charData;
}

void Char1::land()
{
    m_usedDoubleJump = false;
    m_usedAirDash = false;

    if (m_currentAction && m_currentAction->m_noLandTransition)
        return;

    switch (m_currentState)
    {
        case (CHAR1_STATE::JUMP):
            switchToSoftLandingRecovery();
            break;

        case (CHAR1_STATE::GROUND_BACKDASH):
            m_velocity = {0.0f, 0.0f};
            m_inertia = {0.0f, 0.0f};
            break;

        // TODO: airborne hitstun to knd
        case (CHAR1_STATE::HITSTUN_AIR):
            enterKndRecovery();
            m_currentTakenHit.m_hitId = -1;
            break;

        default:
            switchToSoftLandingRecovery();
    }
}

bool Char1::canApplyDrag() const
{
    switch (m_currentState)
    {
        case (CHAR1_STATE::PREJUMP):
            return false;

        default:
            return true;
    }
}

bool Char1::canBeDraggedByInertia() const
{
    switch (m_currentState)
    {
        case (CHAR1_STATE::PREJUMP):
            return false;

        default:
            return true;
    }
}

HitsVec Char1::getHits(bool allHits_)
{
    if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto hits = dynamic_cast<const Action_attack<CHAR1_STATE, Char1Data, Char1>*>(m_currentAction)->getCurrentHits(m_timer.getCurrentFrame() + 1, m_pos, m_ownOrientation);
        int i = 0;
        while (i < hits.size())
        {
            if (!allHits_ && m_appliedHits.contains(hits[i].m_hitId))
            {
                hits.erase(hits.begin() + i);
            }
            i++;
        }

        return hits;
    }
    return {};
}

HurtboxVec Char1::getHurtboxes()
{
    if (m_currentState == CHAR1_STATE::IDLE ||
    m_currentState == CHAR1_STATE::CROUCH ||
    m_currentState == CHAR1_STATE::HITSTUN ||
    m_currentState == CHAR1_STATE::HITSTUN_AIR ||
    m_currentState == CHAR1_STATE::SOFT_LANDING_RECOVERY ||
    m_currentState == CHAR1_STATE::BLOCKSTUN_CROUCHING ||
    m_currentState == CHAR1_STATE::BLOCKSTUN_STANDING ||
    m_currentState == CHAR1_STATE::BLOCKSTUN_AIR)
    {
        auto currentHBox = m_standingHurtbox;
        if (m_currentState == CHAR1_STATE::HITSTUN_AIR || m_currentState == CHAR1_STATE::BLOCKSTUN_AIR)
            currentHBox = m_airHitstunHurtbox;
        else if (m_currentState == CHAR1_STATE::CROUCH || m_currentState == CHAR1_STATE::BLOCKSTUN_CROUCHING)
            currentHBox = m_crouchingHurtbox;

        currentHBox.y += m_pos.y;
        if (m_ownOrientation == ORIENTATION::RIGHT)
            currentHBox.x += m_pos.x;
        else
            currentHBox.x = m_pos.x - currentHBox.x - currentHBox.w;

        return {currentHBox};
    }
    else if (m_currentAction)
    {
        auto currentFrame = m_timer.getCurrentFrame() + 1;
        if (m_timer.isOver())
            currentFrame = 1;
        if (m_currentState == CHAR1_STATE::JUMP)
            currentFrame = 0;
        return m_currentAction->getCurrentHurtboxes(currentFrame, m_pos, m_ownOrientation);
    }

    return {};

}

HIT_RESULT Char1::applyHit(const HitEvent &hitEvent, HIT_RESULT hitRes_)
{
    if (m_playerId == 1)
        std::cout << "";
    applyHitstop(hitEvent.m_hitData.hitstop);

    if (hitEvent.m_hittingPlayerId == m_playerId)
    {
        if (m_currentState != CHAR1_STATE::HITSTUN && m_currentState != CHAR1_STATE::HITSTUN)
        {
            m_appliedHits.insert(hitEvent.m_hitData.m_hitId);

            if (hitRes_ == HIT_RESULT::HIT)
            {
                m_currentCancelWindow = hitEvent.m_hitData.cancelsOnHit;
                if (!m_currentCancelWindow.second.empty())
                    m_cancelTimer.begin(m_currentCancelWindow.first.second + 1);
            }
            else
            {
                m_currentCancelWindow = hitEvent.m_hitData.cancelsOnBlock;
                if (!m_currentCancelWindow.second.empty())
                    m_cancelTimer.begin(m_currentCancelWindow.first.second + 1);
            }

        }

        return HIT_RESULT::NONE;
    }
    else
    {
        auto blockState = m_blockHandler.getBlockState();
        m_currentTakenHit = hitEvent.m_hitData;
        bool blocked = hitEvent.m_hitData.canBeBlockedAs.contains(blockState);
        if (!blocked)
        {
            m_velocity = {0.0f, 0.0f};
            std::cout << "Took hit!\n";

            if (m_airborne)
            {
                m_inertia = hitEvent.m_hitData.opponentImpulseOnAirHit.mulComponents(Vector2{getHorDirToEnemy().x * -1.0f, 1.0f});
            }
            else
            {
                m_inertia = hitEvent.m_hitData.opponentImpulseOnHit.mulComponents(Vector2{getHorDirToEnemy().x * -1.0f, 1.0f});
                if (m_inertia.y < 0)
                    m_airborne = true;
            }


            if (m_airborne)
            {
                m_currentState = CHAR1_STATE::HITSTUN_AIR;
                m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_AIR].get();
            }
            else
            {
                m_currentState = CHAR1_STATE::HITSTUN;
                m_timer.begin(hitEvent.m_hitData.hitstun);
                if (hitEvent.m_hitData.hitstunAnimation == HITSTUN_ANIMATION::HIGH)
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_HIGH].get();
                else if (hitEvent.m_hitData.hitstunAnimation == HITSTUN_ANIMATION::MID)
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_MID].get();
                else
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_LOW].get();
            }
            m_currentAnimation->reset(0);
            m_currentAction = nullptr;
            m_currentCancelWindow = {};
            m_cancelTimer.begin(0);

            return HIT_RESULT::HIT;
        }
        else
        {
            turnVelocityToInertia();
            m_inertia += getHorDirToEnemy() * -1.0f * hitEvent.m_hitData.opponentPushbackOnBlock;
            m_timer.begin(hitEvent.m_hitData.blockstun);
            HIT_RESULT res = HIT_RESULT::BLOCK_HIGH;

            switch (blockState)
            {
                case (BLOCK_STATE::AUTO):
                    [[fallthrough]];
                case (BLOCK_STATE::HIGH):
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_STANDING].get();
                    m_currentState = CHAR1_STATE::BLOCKSTUN_STANDING;
                    res = HIT_RESULT::BLOCK_HIGH;
                    break;

                case (BLOCK_STATE::LOW):
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_CROUCHING].get();
                    m_currentState = CHAR1_STATE::BLOCKSTUN_CROUCHING;
                    res = HIT_RESULT::BLOCK_LOW;
                    break;

                case (BLOCK_STATE::AIR):
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_STANDING].get();
                    m_currentState = CHAR1_STATE::BLOCKSTUN_AIR;
                    res = HIT_RESULT::BLOCK_AIR;
                    break;
            }

            m_currentAnimation->reset(0);
            m_currentAction = nullptr;

            return res;
        }
    }
}

void Char1::updateBlockState()
{
    bool inBlockstun = (m_currentState == CHAR1_STATE::BLOCKSTUN_STANDING ||
                        m_currentState == CHAR1_STATE::BLOCKSTUN_CROUCHING ||
                        m_currentState == CHAR1_STATE::BLOCKSTUN_AIR);

    // TODO: turn into action property
    bool canBlock = !(m_currentState == CHAR1_STATE::HITSTUN ||
                    m_currentState == CHAR1_STATE::HITSTUN_AIR ||
                    m_currentState == CHAR1_STATE::GROUND_BACKDASH ||
                    m_currentState == CHAR1_STATE::MOVE_A ||
                    m_currentState == CHAR1_STATE::MOVE_B ||
                    m_currentState == CHAR1_STATE::MOVE_2B ||
                    m_currentState == CHAR1_STATE::MOVE_C);

    m_blockHandler.update(m_actionResolver.getCurrentInputDir(), m_airborne, getHorDirToEnemy(), inBlockstun, canBlock);
}

std::string Char1::CharStateData() const
{
    std::string res;
    res += "P" + std::to_string(m_playerId) + ": ";

    std::string stateName = "";
    switch (m_currentState)
    {
        case (CHAR1_STATE::IDLE):
            stateName = "IDLE";
            break;
        
        case (CHAR1_STATE::CROUCH):
            stateName = "CROUCH";
            break;

        case (CHAR1_STATE::HITSTUN):
            stateName = "HITSTUN";
            break;

        case (CHAR1_STATE::HITSTUN_AIR):
            stateName = "HITSTUN_AIR";
            break;

        case (CHAR1_STATE::WALK_FWD):
            stateName = "WALK_FWD";
            break;

        case (CHAR1_STATE::WALK_BWD):
            stateName = "WALK_BWD";
            break;

        case (CHAR1_STATE::PREJUMP):
            stateName = "PREJUMP";
            break;
        
        case (CHAR1_STATE::JUMP):
            stateName = "JUMP";
            break;

        case (CHAR1_STATE::SOFT_LANDING_RECOVERY):
            stateName = "SOFT_LANDING_RECOVERY";
            break;

        case (CHAR1_STATE::GROUND_DASH):
            stateName = "GROUND_DASH";
            break;

        case (CHAR1_STATE::GROUND_DASH_RECOVERY):
            stateName = "GROUND_DASH_RECOVERY";
            break;
        
        case (CHAR1_STATE::MOVE_A):
            stateName = "MOVE_A";
            break;

        case (CHAR1_STATE::MOVE_B):
            stateName = "MOVE_B";
            break;

        case (CHAR1_STATE::MOVE_C):
            stateName = "MOVE_C";
            break;

        case (CHAR1_STATE::MOVE_2B):
            stateName = "MOVE_2B";
            break;
        
        case (CHAR1_STATE::BLOCKSTUN_STANDING):
            stateName = "BLOCKSTUN_STANDING";
            break;

        case (CHAR1_STATE::BLOCKSTUN_CROUCHING):
            stateName = "BLOCKSTUN_CROUCHING";
            break;
        
        case (CHAR1_STATE::BLOCKSTUN_AIR):
            stateName = "BLOCKSTUN_AIR";
            break;

        case (CHAR1_STATE::HARD_KNOCKDOWN):
            stateName = "HARD_KNOCKDOWN";
            break;

        case (CHAR1_STATE::KNOCKDOWN_RECOVERY):
            stateName = "KNOCKDOWN_RECOVERY";
            break;

        case (CHAR1_STATE::AIR_DASH):
            stateName = "AIR_DASH";
            break;

        case (CHAR1_STATE::AIR_DASH_EXTENTION):
            stateName = "AIR_DASH_EXTENTION";
            break;

        default:
            stateName = "OTHER_STATE";
    }

    res += "(" + std::to_string(framesInState) + ") ";
    res += stateName;

    if (m_inHitstop)
        res += "[##] ";
    else
        res += "[>>] ";
    
    if (m_airborne)
        res += "[^] ";
    else
        res += "[_] ";

    bool isActive = isInActiveFrames();
    if (isActive)
        res += "[!] ";
    else
        res += "[-] ";


    return res;
}

bool Char1::isInActiveFrames() const
{
    if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto atkAction = dynamic_cast<const Action_attack<CHAR1_STATE, Char1Data, Char1>*>(m_currentAction);
        return atkAction->getCurrentHits(m_timer.getCurrentFrame() + 1, m_pos, m_ownOrientation).size();
    }

    return false;
}

bool Char1::isInHitstun() const
{
    return (m_currentState == CHAR1_STATE::HITSTUN || m_currentState == CHAR1_STATE::HITSTUN_AIR);
}

void Char1::enterKndRecovery()
{
    m_actionResolver.getAction(CHAR1_STATE::KNOCKDOWN_RECOVERY)->switchTo(*this);
}

void Char1::switchToFloat()
{
    m_actionResolver.getAction(CHAR1_STATE::FLOAT)->switchTo(*this);
}

bool Char1::canApplyGravity() const
{
    // TODO: make property of state
    if (m_currentState == CHAR1_STATE::AIR_DASH || m_currentState == CHAR1_STATE::AIR_DASH_EXTENTION ||
    m_currentState == CHAR1_STATE::AIR_BACKDASH)
        return false;
    return true;
}

Collider Char1::getPushbox() const
{
    Collider pb;
    if (m_currentAction && m_currentAction->m_isCrouchState)
        pb = m_crouchingPushbox;
    else if (m_airborne)
        pb = m_airbornePushbox;
    else
        pb = m_pushbox;

    pb.x += m_pos.x;
    pb.y += m_pos.y;
    return pb;
}