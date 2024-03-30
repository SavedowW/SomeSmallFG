#include "Character.h"
#include "Application.h"
#include "ActionResolver.h"

Character::Character(Application &application_, Vector2<float> pos_, float maxHealth_, float baseGravity_, Camera *cam_, ParticleManager *particleManager_, int maxAirdashes_, int maxDJumps_,
    int framesBeforeAirdash_, int framesBeforeAirjump_, StateMarker autoRealignAfter_, int stateCnt_) :
    InteractableStateMachine(application_, pos_, stateCnt_, cam_, particleManager_),
    m_healthHandler(maxHealth_),
    m_comboPhysHandler(baseGravity_),
    m_jumpsAvailable(maxDJumps_),
    m_airdashesAvailable(maxAirdashes_),
    m_framesBeforeAirdash(framesBeforeAirdash_),
    m_framesBeforeAirjump(framesBeforeAirjump_),
    m_autoRealignAfter(std::move(autoRealignAfter_)),
    m_genericCharacterData(stateCnt_)
{
    setPos(pos_);
}

void Character::setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_)
{
    InteractableStateMachine::setOnStage(application_, playerId_, otherCharacter_, priorityHandler_);

    for (auto &el : m_animations)
    {
        el.second->generateWhite(*application_.getRenderer());
    }
}

CharacterUpdateRes Character::update()
{
    auto initPos = m_pos;

    auto hstopTimerRes = m_hitstopTimer.update();
    if (hstopTimerRes)
    {
        m_inHitstop = false;
        m_hitstopTimer.begin(0);
    }

    if (!m_inHitstop)
    {
        updateDirToEnemy();
    }


    if (m_shineLockedTimer.update())
    {
        if (m_shineAlphaTimer.update())
        {
            m_shineAlphaTimer.begin(0);
            m_shineLockedTimer.begin(0);
        }
    }

    m_throwInvulTimer.update();

    if (m_airborne)
    {
        m_airdashTimer.update();
        m_airjumpTimer.update();
    }

    proceedCurrentState();

    updateState();

    updatePosition();

    if (!m_inHitstop && m_extendedBuffer)
        m_extendedBuffer = 0;

    CharacterUpdateRes res;
    res.moveOffset = m_pos - initPos;
    res.newPos = m_pos;
    res.pushbox = getPushbox();
    return res;
}

void Character::updatePosition()
{
    if (m_inHitstop)
        return;

    // Apply velocity and inertia
    auto posAddition = m_velocity;
    if (canBeDraggedByInertia())
        posAddition += m_inertia;
    setPos(m_pos + posAddition);

    auto pb = getPushbox();

    // Define if character is airborne
    if (m_pos.y < gamedata::stages::levelOfGround)
    {
        m_airborne = true;
    }
    else if (m_airborne && m_pos.y >= gamedata::stages::levelOfGround)
    {
        m_inertia.y = 0;
        m_airborne = false;
        m_pos.y = gamedata::stages::levelOfGround;
        land();
    }

    if (m_airborne)
    {
        if (canApplyGravity())
        {
            auto gravity = m_comboPhysHandler.getGravity();
            m_inertia.y += gravity;
        }
    }

    // Apply drag for grounded character
    if (m_inertia.x != 0.0f && !m_airborne && canApplyDrag())
    {
        auto absInertia = abs(m_inertia.x);
        auto m_inertiaSign = m_inertia.x / abs(m_inertia.x);
        absInertia = std::max(absInertia - m_inertiaDrag, 0.0f);
        m_inertia.x = m_inertiaSign * absInertia;
    }

    if ((m_pushback.x != 0 || m_pushback.y != 0) && !(isInHitstun() && m_airborne))
    {
        auto carry = utils::limitVectorLength(m_pushback, m_pushbackCurrentCarry);
        m_pos += carry;
        m_pushback -= carry;
        if (m_pushback.getLen() < m_pushbackTreshold)
        {
            m_pushbackCurrentCarry = utils::clamp(m_pushbackCurrentCarry - m_pushbackDeterioration, 3.0f, m_pushbackMaxCarry);
        }
    }
    
    m_currentAnimation->update();
}

ORIENTATION Character::getInputDir() const
{
    auto res = m_ownOrientation;
    if (m_genericCharacterData.m_useDirToEnemyForInputs.getMark(m_currentState))
        res = m_dirToEnemy;
    return res;
}

void Character::drawGroundProjection(Renderer &renderer_, Camera &camera_, float angle_)
{
    auto texSize = m_currentAnimation->getSize();
    auto texPos = m_pos - Vector2{texSize.x / 2, texSize.y};
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (m_ownOrientation == ORIENTATION::LEFT)
            flip = SDL_FLIP_HORIZONTAL;

    float xoffset = 0;

    if (isInHitstun() && m_inHitstop)
    {
        xoffset = rand() % 13 - 6;
    }

    auto spr = m_currentAnimation->getSprite();

    auto charBottomHeight = (m_pos.y - gamedata::stages::levelOfGround);
    auto legposy = gamedata::stages::levelOfGround - charBottomHeight / 4.0f - 10.0f;
    auto llegposx = texPos.x + xoffset;
    auto rlegposx = texPos.x + xoffset + texSize.x;

    float angleOffset = texSize.x * sin(angle_);
    float sizeExtention = angleOffset / 2;

    Vector2<float> tl = {llegposx, legposy};
    Vector2<float> tr = {rlegposx, legposy};
    Vector2<float> br = {rlegposx + angleOffset + sizeExtention, legposy + texSize.y / 4.0f};
    Vector2<float> bl = {llegposx + angleOffset - sizeExtention, legposy + texSize.y / 4.0f};

    auto camSize = camera_.getSize();

    tl = ((tl - camera_.getTopLeft()) / camera_.getScale());
    tr = ((tr - camera_.getTopLeft()) / camera_.getScale());
    br = ((br - camera_.getTopLeft()) / camera_.getScale());
    bl = ((bl - camera_.getTopLeft()) / camera_.getScale());

    float leftpoint = (flip == SDL_FLIP_HORIZONTAL ? 1 : 0);
    float rightpoint = 1 - leftpoint;

    SDL_Vertex vertices[] = {
        {{tl.x, tl.y}, {255, 255, 255, 255}, {leftpoint, 1}},
        {{tr.x, tr.y}, {255, 255, 255, 255}, {rightpoint, 1}},
        {{br.x, br.y}, {255, 255, 255, 255}, {rightpoint, 0}},
        {{bl.x, bl.y}, {255, 255, 255, 255}, {leftpoint, 0}}
    };
    int indices1[] = {0, 1, 2};
    int indices2[] = {0, 2, 3};



    renderer_.drawGeometry(spr, vertices, 4, indices1, 3);
    renderer_.drawGeometry(spr, vertices, 4, indices2, 3);
}

void Character::draw(Renderer &renderer_, Camera &camera_)
{
    if (m_currentAnimation != nullptr)
    {
        auto texSize = m_currentAnimation->getSize();
        auto texPos = m_pos - Vector2{texSize.x / 2, texSize.y};
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (m_ownOrientation == ORIENTATION::LEFT)
            flip = SDL_FLIP_HORIZONTAL;

        float xoffset = 0;

        if (isInHitstun() && m_inHitstop)
        {
            xoffset = rand() % 13 - 6;
        }

        auto spr = m_currentAnimation->getSprite();
        bool shining = m_shineLockedTimer.isActive() || m_shineAlphaTimer.isActive();

        auto sprsize = m_currentAnimation->getSize();
        renderer_.renderTexture(spr, texPos.x + xoffset, texPos.y, sprsize.x , sprsize.y, camera_, 0.0f, flip);
        if (shining)
        {
            float alpha = 1 - m_shineAlphaTimer.getProgressNormalized();

            auto pw = m_currentAnimation->getWhiteSprite();
            SDL_SetTextureColorMod(pw, m_colorShine.r, m_colorShine.g, m_colorShine.b);
            SDL_SetTextureAlphaMod(pw, alpha * m_colorShine.a);
            
            renderer_.renderTexture(pw, texPos.x + xoffset, texPos.y, sprsize.x, sprsize.y, camera_, 0, flip);

            SDL_SetTextureColorMod(pw, 255, 255, 255);
            SDL_SetTextureAlphaMod(pw, 255);
        }
    }

    // render pushboxes with macro DEBUG
    #ifdef DEBUG_
    {
        auto pb = getPushbox();
        renderer_.fillRectangle({pb.x, pb.y}, {pb.w, pb.h}, gamedata::characters::pushboxColor, camera_);
        renderer_.drawRectangle({pb.x, pb.y}, {pb.w, pb.h}, gamedata::characters::pushboxColor, camera_);

        auto hurtboxes = getHurtboxes();
        for (const auto &hb : hurtboxes)
        {
            renderer_.fillRectangle({hb.x, hb.y}, {hb.w, hb.h}, gamedata::characters::hurtboxColor, camera_);
            renderer_.drawRectangle({hb.x, hb.y}, {hb.w, hb.h}, gamedata::characters::hurtboxColor, camera_);
        }

        auto hits = getHits(true);
        for (const auto &el : hits)
        {
            auto hitboxes = el.getHitboxes();
            for (const auto &hb : hitboxes)
            {
                renderer_.drawRectangle({hb.second.x, hb.second.y}, {hb.second.w, hb.second.h}, gamedata::characters::hitboxColor, camera_);
                renderer_.fillRectangle({hb.second.x, hb.second.y}, {hb.second.w, hb.second.h}, gamedata::characters::hitboxColor, camera_);
            }
        }

        renderer_.fillRectangle({m_pos.x - 2.0f, m_pos.y - 25.0f}, {4.0f, 50.0f}, {255, 255, 255, 200}, camera_);
        renderer_.fillRectangle({m_pos.x - 25.0f, m_pos.y - 2.0f}, {50.0f, 4.0f}, {255, 255, 255, 200}, camera_);
    }
    #endif
}

bool Character::canApplyDrag() const
{
    return !m_genericCharacterData.m_noDrag.getMark(m_currentState);
}

bool Character::canApplyGravity() const
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);

    return act->applyGravity(m_timer.getCurrentFrame() + 1);
}

bool Character::canBeDraggedByInertia() const
{
    return !m_genericCharacterData.m_noInertia.getMark(m_currentState);
}

void Character::turnVelocityToInertia(float horMultiplier_)
{
    if (m_genericCharacterData.m_dontConvertVelocityToInertia.getMark(m_currentState))
        m_velocity = {0, 0};

    m_inertia.x += m_velocity.x * horMultiplier_;
    m_inertia.y += m_velocity.y;
    m_velocity = {0.0f, 0.0f};
}

HIT_RESULT Character::applyHit(HitEvent &hitEvent_)
{
    float range = (m_pos - m_otherCharacter->getPos()).getLen();
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);

    // Attacker's side
    if (hitEvent_.m_hittingPlayerId == m_playerId)
    {
        if (hitEvent_.m_hitRes == HIT_RESULT::NONE)
            return HIT_RESULT::NONE;

        callForPriority();
        if (hitEvent_.m_hitRes == HIT_RESULT::COUNTER)
        {
            m_notifyWidget->addNotification("COUNTER!");
        }
        m_notifyWidget->addNotification(std::to_string(hitEvent_.realDamage));

        applyHitstop(hitEvent_.m_hitData.hitProps.hitstop);

        if (!isInHitstun())
        {
            if (hitEvent_.m_hitRes == HIT_RESULT::HIT || hitEvent_.m_hitRes == HIT_RESULT::COUNTER)
                applyCancelWindow(hitEvent_.m_hitData.cancelsOnHit);
            else
                applyCancelWindow(hitEvent_.m_hitData.cancelsOnBlock);

        }

        return HIT_RESULT::NONE;
    }
    // Defender's side
    else
    {
        if (isHitTaken(hitEvent_.m_hitData.m_hitId))
            return HIT_RESULT::NONE;
            
        m_currentTakenHit = hitEvent_.m_hitData;
        m_takenHits.insert(hitEvent_.m_hitData.m_hitId);

        auto blockState = m_blockHandler.getBlockState();
        bool blocked = hitEvent_.m_hitData.canBeBlockedAs.contains(blockState);

        // Took hit
        if (!blocked)
        {
            turnVelocityToInertia();
            m_inertia.x *= gamedata::characters::takenHitInertiaCarry;
            std::cout << "Took hit!\n";

            auto hitres = HIT_RESULT::HIT;
            bool isCounter = false;

            if (act->isInCounterState(m_timer.getCurrentFrame() + 1))
            {
                isCounter = true;
            }

            if (isCounter)
            {
                m_hitProps = hitEvent_.m_hitData.chProps;
                hitres = HIT_RESULT::COUNTER;
                startShine({150, 0, 0, 225}, std::max(hitEvent_.m_hitData.chProps.hitstop - 3, 5), 3);
            }
            else
            {
                m_hitProps = hitEvent_.m_hitData.hitProps;
            }

            hitEvent_.m_hitRes = hitres;
            hitEvent_.realDamage = m_healthHandler.takeDamage(hitEvent_);
            m_comboPhysHandler.takeHit(hitEvent_);

            if (!m_lockedInAnimation)
            {
                Vector2<float> newImpulse;
                auto hordir = getHorDirToEnemy();
                
                if (m_airborne)
                {
                    newImpulse = m_hitProps.opponentImpulseOnAirHit.mulComponents(Vector2{hordir.x * -1.0f, 1.0f});
                    newImpulse = m_comboPhysHandler.getImpulseScaling(newImpulse, hordir);
                    newImpulse += m_inertia * gamedata::characters::airborneImpulseIntoPushbackCarry;
                    m_inertia = newImpulse;

                    newImpulse.x *= gamedata::characters::airbornePushbackMultiplier;
                }
                else
                {
                    if (m_hitProps.opponentImpulseOnHit.y == 0)
                    {
                    newImpulse = m_hitProps.opponentImpulseOnHit.mulComponents(Vector2{hordir.x * -1.0f, 1.0f});
                    newImpulse = m_comboPhysHandler.getImpulseScaling(newImpulse, hordir);
                    }
                    else
                    {
                        m_airborne = true;
    
                        newImpulse = m_hitProps.opponentImpulseOnHit.mulComponents(Vector2{hordir.x * -1.0f, 1.0f});
                        newImpulse = m_comboPhysHandler.getImpulseScaling(newImpulse, hordir);
                        newImpulse += m_inertia * gamedata::characters::airborneImpulseIntoPushbackCarry;

                        m_inertia += newImpulse;

                        newImpulse.x *= gamedata::characters::airbornePushbackMultiplier;
                        
                    }
                }

                newImpulse.y = 0;
                takePushback(newImpulse);
            }


            enterHitstunAnimation(m_hitProps);

            applyCancelWindow({{0, 0}, {}});
            applyHitstop(m_hitProps.hitstop);

            return hitres;
        }
        // Blocked
        else
        {
            bool inBlockstun = isInBlockstun();

            bool isInstant = false;
            if ((!inBlockstun || m_blockstunType == BLOCK_FRAME::INSTANT) && m_blockHandler.getBlockFrame() == BLOCK_FRAME::INSTANT)
            {
                isInstant = true;
                m_blockstunType = BLOCK_FRAME::INSTANT;
                startShine({255, 255, 255, 255}, 4, 8);
                m_notifyWidget->addNotification("INSTANT");
            }

            applyHitstop(hitEvent_.m_hitData.hitProps.hitstop);
            if (!isInstant)
            {
                turnVelocityToInertia();
                m_inertia.x *= gamedata::characters::pushblockInertiaCarry;
                takePushback(getHorDirToEnemy() * -1.0f * hitEvent_.m_hitData.opponentPushbackOnBlock);
            }
            else if (m_airborne)
            {
                turnVelocityToInertia();
                if (!isInstant)
                    takePushback(getHorDirToEnemy() * -1.0f * hitEvent_.m_hitData.opponentPushbackOnBlock);
            }
            else
            {
                m_velocity = {0, 0};
                m_inertia = {0, 0};
            }
            auto blockstunDuration = m_blockHandler.getBlockstunDuration(hitEvent_.m_hitData.blockstun);
            m_notifyWidget->addNotification(std::to_string(blockstunDuration));
            HIT_RESULT res = HIT_RESULT::BLOCK_HIGH;

            switch (blockState)
            {
                case (BLOCK_STATE::AUTO):
                    [[fallthrough]];
                case (BLOCK_STATE::HIGH):
                    switchTo(m_genericCharacterData.m_blockstunStanding);
                    res = HIT_RESULT::BLOCK_HIGH;

                    break;

                case (BLOCK_STATE::LOW):
                    switchTo(m_genericCharacterData.m_blockstunCrouching);
                    res = HIT_RESULT::BLOCK_LOW;
                    break;

                case (BLOCK_STATE::AIR):
                    switchTo(m_genericCharacterData.m_blockstunAir);
                    res = HIT_RESULT::BLOCK_AIR;
                    break;
            }

            m_timer.begin(blockstunDuration);

            hitEvent_.m_hitRes = res;
            if (!isInstant)
            {
                hitEvent_.realDamage = m_healthHandler.takeDamage(hitEvent_);
            }
            else
            {
                CancelWindow tempwindow;
                tempwindow.first = {1, 20};
                tempwindow.second = {
                    m_genericCharacterData.m_step,
                    m_genericCharacterData.m_airdash
                };

                applyCancelWindow(tempwindow);
            }

            return res;
        }
    }
}

void Character::applyHitstop(int hitstopLength)
{
    m_inHitstop = true;
    m_hitstopTimer.begin(hitstopLength);
    m_extendedBuffer = hitstopLength - gamedata::global::inputBufferLength;
}

void Character::takePushback(const Vector2<float> pushback_)
{
    if (m_lockedInAnimation || m_tiedAnimWithOpponent)
        return;

    /*if (fromHit_.m_hitId != -1 && fromHit_.cornerPushbackMaxRange > rangeToCorner_)
    {
        if (utils::sameSign((int)m_inertia.x, dirFromCorner_.x))
            m_inertia.x = 0;

        float pbimpulse = std::max((1 - rangeToCorner_ / fromHit_.cornerPushbackMaxRange) * fromHit_.cornerPushbackMaxImpulse, fromHit_.cornerPushbackMinImpulse);

        m_inertia += dirFromCorner_ * pbimpulse;
    }*/

    m_pushback = pushback_;
    m_pushbackCurrentCarry = m_pushbackMaxCarry;
    //m_inertia += dirFromCorner_ * pushback_;
    std::cout << m_pushback << std::endl;
    
}

void Character::generateWidgets(Application &application_, HUD &hud_)
{
    bool rs = m_playerId == 2;

    auto hptr = std::make_unique<HealthWidget>(application_, rs);
    m_healthWidget = hptr.get();
    hud_.addWidget(std::move(hptr));
    m_healthHandler.setWidget(m_healthWidget);

    auto nptr = std::make_unique<NotifyWidget>(application_, rs);
    m_notifyWidget = nptr.get();
    hud_.addWidget(std::move(nptr));
}

void Character::enterHitstunAnimation(const PostHitProperties &props_)
{
    m_blockstunType = BLOCK_FRAME::NONE;
    if (m_lockedInAnimation)
        return;

    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);

    if (m_airborne)
    {
        switchTo(m_genericCharacterData.m_hitstunAnimToStates[(int)props_.airHitstunAnimation]);
    }
    else
    {
        int targetAnim_;
        if (act->m_isCrouchState || (int)m_hitstunAnimation == m_genericCharacterData.m_crouchHitstunAnim || props_.forceCrouch)
        {
            targetAnim_ = m_genericCharacterData.m_crouchHitstunAnim;
        }
        else
        {
            targetAnim_ = (int)props_.groundHitstunAnimation;
        }

        switchTo(m_genericCharacterData.m_hitstunAnimToStates[(int)targetAnim_]);
        m_hitstunAnimation = (HITSTUN_ANIMATION)targetAnim_;

        m_timer.begin(m_hitProps.hitstun);
    }
}

void Character::startShine(const SDL_Color &col_, int lockedDuration_, int alphaDuration_)
{
    m_colorShine = col_;
    m_shineAlphaTimer.begin(alphaDuration_);
    m_shineLockedTimer.begin(lockedDuration_);
}

void Character::jumpUsingAction()
{
    turnVelocityToInertia();

    auto ownOrientationVector = getOwnHorDir();
    ownOrientationVector.y = 1;
    auto jumpAction = dynamic_cast<const Action_jump*>(m_currentAction);
    m_velocity = (jumpAction)->m_impulse.mulComponents(ownOrientationVector);
    if (m_inertia.y > 0)
        m_inertia.y = 0;

    auto currentXInertia = m_inertia.x;
    currentXInertia *= ownOrientationVector.x;
    if (currentXInertia > jumpAction->m_maxHorInertia)
    {
        m_inertia.x = jumpAction->m_maxHorInertia * ownOrientationVector.x;
    }

    switchTo(m_genericCharacterData.m_float);
    m_currentAnimation->reset();
}

void Character::lockInAnimation()
{
    m_lockedInAnimation = true;
}

void Character::addTakenHit(int hitId_)
{
    m_takenHits.insert(hitId_);
}

void Character::removeTakenHit(int hitId_)
{
    m_takenHits.erase(hitId_);
}

bool Character::isHitTaken(int hitId_)
{
    return m_takenHits.contains(hitId_);
}

void Character::releaseFromAnimation()
{
    m_lockedInAnimation = false;
}

void Character::tieAnimWithOpponent()
{
    if (m_tiedAnimWithOpponent)
        return;

    m_tiedAnimWithOpponent = true;
    m_otherCharacter->tieAnimWithOpponent();
}  

void Character::untieAnimWithOpponent()
{
    if (!m_tiedAnimWithOpponent)
        return;

    m_tiedAnimWithOpponent = false;
    m_otherCharacter->untieAnimWithOpponent();
}

Collider Character::getUntiedPushbox() const
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);

    Collider pb;
    if (m_airborne && m_hitstunAnimation != HITSTUN_ANIMATION::NONE)
        pb = m_genericCharacterData.m_airHitstunPushbox;
    else if (act->m_isCrouchState || m_hitstunAnimation == HITSTUN_ANIMATION::CROUCH)
        pb = m_genericCharacterData.m_crouchingPushbox;
    else if (m_airborne)
        pb = m_genericCharacterData.m_airPushbox;
    else
        pb = m_genericCharacterData.m_standingPushbox;

    pb.x += m_pos.x;
    pb.y += m_pos.y;
    return pb;
}
void Character::enterThrown(THROW_LIST throw_)
{
    lockInAnimation();

    switchTo(m_genericCharacterData.m_thrownStates[(int)throw_]);
}

void Character::throwTeched(THROW_TECHS_LIST tech_)
{
    switchTo(m_genericCharacterData.m_throwTechedStates[(int)tech_]);
}

Collider Character::getPushbox() const
{
    auto pb = getUntiedPushbox();
    if (!m_tiedAnimWithOpponent)
        return pb;

    auto pb2 = m_otherCharacter->getUntiedPushbox();
    Vector2<float> tl, br;

    tl.x = std::min(pb.x, pb2.x);
    tl.y = std::min(pb.y, pb2.y);

    br.x = std::max(pb.x + pb.w, pb2.x + pb2.w);
    br.y = std::max(pb.y + pb.h, pb2.y + pb2.h);

    Collider realpb;
    realpb.x = tl.x;
    realpb.y = tl.y;
    realpb.w = br.x - tl.x;
    realpb.h = br.y - tl.y;
    return realpb;
}

bool Character::passableThrough() const
{
    return m_tiedAnimWithOpponent || m_lockedInAnimation;
}

bool Character::canBeThrown(THROW_LIST throw_) const
{
    return (!m_throwInvulTimer.isActive() && !isInBlockstun() && !isInHitstun() && !isKnockedDown());
}

void Character::setThrowInvul()
{
    m_throwInvulTimer.begin(5);
}

void Character::attemptThrow()
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);

    if (act->m_isThrowStartup)
    {
        auto action = dynamic_cast<const Action_throw_startup*>(m_currentAction);
        action->attemptThrow(*this);
    }
}

float Character::touchedWall(int sideDir_)
{
    auto fw = getFullVelocity();

    if (sideDir_ * fw.x > 0 && isInHitstun() && m_airborne && m_hitProps.wallBounce)
    {
        m_velocity.x *= -1 * m_hitProps.wallbounceInertiaMultiplierX;
        m_inertia.x *= -1 * m_hitProps.wallbounceInertiaMultiplierX;
        
        if (m_inertia.y < 0)
            m_inertia.y *= m_hitProps.wallbounceInertiaMultiplierY;

        if (m_velocity.y < 0)
            m_velocity.y *= m_hitProps.wallbounceInertiaMultiplierY;

        m_hitProps.wallBounce = false;
    }

    if ((isInHitstun() || isInBlockstun()) && utils::sameSign<float>(m_pushback.x, sideDir_))
    {
        auto pbx = m_pushback.x;
        m_pushback.x = 0;
        return abs(pbx + fw.x * gamedata::characters::inertiaIntoCornerPushbackCarry);
    }

    return 0;
}

void Character::applyClash(const Hit &clashedHit_, int opponentsHitId_)
{
    applyHitstop(20);

    m_takenHits.insert(opponentsHitId_);

    CancelWindow tempwindow;

    tempwindow.first = {1, 20};
    tempwindow.second = m_genericCharacterData.m_clashCancelOptions;

    applyCancelWindow(tempwindow);
}

void Character::updateBlockState()
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);

    bool inBlockstun = isInBlockstun();

    bool canBlock = (inBlockstun || act->canBlock(m_timer.getCurrentFrame() + 1));

    auto backButton = (m_dirToEnemy == ORIENTATION::RIGHT ? INPUT_BUTTON::LEFT : INPUT_BUTTON::RIGHT);

    m_blockHandler.update(m_actionResolver.getCurrentInputDir(), m_actionResolver.getPostFrameButtonState(backButton), m_airborne, getHorDirToEnemy(), inBlockstun, canBlock, m_inHitstop);
}

bool Character::isInHitstun() const
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);
    return act->m_isHitstun;
}

bool Character::isKnockedDown() const
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);
    return act->m_isKnockdown;
}


bool Character::isInBlockstun() const
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);
    return act->m_isBlockstun;
}

bool Character::isInInstantBlockstun() const
{
    auto *act = dynamic_cast<ActionCharacter*>(m_currentAction);
    return act->m_isBlockstun && m_blockstunType == BLOCK_FRAME::INSTANT;
}

void Character::proceedCurrentState()
{
    if (!m_inHitstop)
    {
        auto timerRes = m_timer.update();


        if (timerRes)
        {
            if (isInHitstun() || isInBlockstun())
            {
                setThrowInvul();
            }

            if (m_genericCharacterData.m_prejums.getMark(m_currentState))
            {
                jumpUsingAction();
            }
            else
            {
                if (m_currentAction)
                        m_currentAction->outdated(*this);
            }

        }
    }
}

void Character::initiate()
{
    if (m_playerId == 1)
        m_actionResolver.subscribe_p1();
    else if (m_playerId == 2)
        m_actionResolver.subscribe_p2();
    else
        throw std::runtime_error("Trying to initiate character without player ID");

    m_actionResolver.setInputEnabled(true);

    m_currentState = m_genericCharacterData.m_idle;
    switchTo(m_genericCharacterData.m_idle);    
}

void Character::land()
{
    m_blockstunType = BLOCK_FRAME::NONE;
    bool vulnerable = m_usedAirAttack;
    m_jumpsAvailable.free();
    m_airdashesAvailable.free();
    m_airjumpTimer.begin(0);
    m_airdashTimer.begin(0);
    m_usedAirAttack = false;

    if (m_currentAction->onLand(*this))
        return;

    if (m_lockedInAnimation)
        return;

    if (isInHitstun())
    {
        if (m_hitProps.groundBounce)
        {
            switchTo(m_genericCharacterData.m_groundBounceHitstun);
            m_inertia.y = 0;
            m_velocity.y = -m_comboPhysHandler.getGroundBounceForce(m_hitProps.groundBounceStrength);
            m_hitProps.groundBounce = false;
        }
        else if (m_hitProps.hardKnd)
            switchTo(m_genericCharacterData.m_hardKD);
        else
            switchTo(m_genericCharacterData.m_softKD);

        return;
    }

    switch (m_currentState)
    {            
        default:
            if (vulnerable)
                switchTo(m_genericCharacterData.m_vulnerableLandingRecovery);
            else
                switchTo(m_genericCharacterData.m_softLandingRecovery);
    }
}

HitsVec Character::getHits(bool allHits_)
{
    if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto hits = dynamic_cast<const Action_attack*>(m_currentAction)->getCurrentHits(m_timer.getCurrentFrame() + 1, m_pos, m_ownOrientation);

        return hits;
    }
    return {};
}

GenericCharacterData::GenericCharacterData(int statecnt_) :
    m_dontConvertVelocityToInertia(statecnt_),
    m_prejums(statecnt_),
    m_noDrag(statecnt_),
    m_noInertia(statecnt_),
    m_useDirToEnemyForInputs(statecnt_)
{
}
