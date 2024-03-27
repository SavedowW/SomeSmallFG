#include "Character.h"
#include "Application.h"
#include "ActionResolver.h"

Character::Character(Application &application_, Vector2<float> pos_, float maxHealth_, float baseGravity_, Camera *cam_, ParticleManager *particleManager_, int maxAirdashes_, int maxDJumps_,
    int framesBeforeAirdash_, int framesBeforeAirjump_, StateMarker autoRealignAfter_) :
    m_playerId(0),
    m_currentAnimation(nullptr),
    m_healthHandler(maxHealth_),
    m_cam(cam_),
    m_comboPhysHandler(baseGravity_),
    m_particleManager(particleManager_),
    m_jumpsAvailable(maxDJumps_),
    m_airdashesAvailable(maxAirdashes_),
    m_framesBeforeAirdash(framesBeforeAirdash_),
    m_framesBeforeAirjump(framesBeforeAirjump_),
    m_actionResolver(application_.getInputSystem()),
    m_currentAction(nullptr),
    m_autoRealignAfter(std::move(autoRealignAfter_))
{
    setPos(pos_);
}

void Character::setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_)
{
    m_playerId = playerId_;
    if (m_playerId == 1)
    {
        m_dirToEnemy = ORIENTATION::RIGHT;
        m_ownOrientation = ORIENTATION::RIGHT;
    }
    else
    {
        m_dirToEnemy = ORIENTATION::LEFT;
        m_ownOrientation = ORIENTATION::LEFT;
    }

    m_otherCharacter = otherCharacter_;
    m_priorityHandler = priorityHandler_;

    loadAnimations(application_);
    for (auto &el : m_animations)
    {
        el.second->generateWhite(*application_.getRenderer());
    }

    initiate();
    m_currentTakenHit.m_hitId = -1;
    m_hitstunAnimation = HITSTUN_ANIMATION::NONE;
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

Vector2<float> Character::getPos() const
{
    return m_pos;
}


void Character::setPos(Vector2<float> pos_)
{
    //std::cout << "setPos to " << pos_ << std::endl;
    m_pos = pos_;
}

Vector2<float> Character::getHorDirToEnemy() const
{
    return Vector2(m_otherCharacter->getPos().x - getPos().x, 0.0f).normalised();
}

Vector2<float> Character::getOwnHorDir() const
{
    return Vector2<float>{(m_ownOrientation == ORIENTATION::RIGHT ? 1.0f : -1.0f), 0.0f};
}

ORIENTATION Character::getOwnOrientation() const
{
    return m_ownOrientation;
}

ORIENTATION Character::getOrientationToEnemy() const
{
    return m_dirToEnemy;
}

Vector2<float> Character::getVelocity() const
{
    return m_velocity;
}

Vector2<float> Character::getFullVelocity() const
{
    return m_velocity + m_inertia;
}

Vector2<float> Character::getInertia() const
{
    return m_inertia;
}

void Character::updateOwnOrientation()
{
    m_ownOrientation = m_dirToEnemy;
}

void Character::updateDirToEnemy()
{
    auto vecToEnemy = getHorDirToEnemy();
    if (vecToEnemy.x > 0)
        m_dirToEnemy = ORIENTATION::RIGHT;
    else if (vecToEnemy.x < 0)
        m_dirToEnemy = ORIENTATION::LEFT;
}

bool Character::canApplyDrag() const
{
    return true;
}

bool Character::canApplyGravity() const
{
    if (m_lockedInAnimation)
        return false;

    return true;
}

bool Character::canBeDraggedByInertia() const
{
    return true;
}

void Character::turnVelocityToInertia(float horMultiplier_)
{
    m_inertia.x += m_velocity.x * horMultiplier_;
    m_inertia.y += m_velocity.y;
    m_velocity = {0.0f, 0.0f};
}

void Character::applyHitstop(int hitstopLength)
{
    m_inHitstop = true;
    m_hitstopTimer.begin(hitstopLength);
    m_extendedBuffer = hitstopLength - gamedata::global::inputBufferLength;
}

HitData Character::getCurrentTakenHit()
{
    auto temp = m_currentTakenHit;
    m_currentTakenHit.m_hitId = -1;
    return temp;
}

void Character::applyCancelWindow(CancelWindow cw_)
{
    if (cw_.first.first == 0 && cw_.first.second == 0)
    {
        if (m_playerId == 1)
            std::cout << "Drop cancels\n";
        m_currentCancelWindow = cw_;
        m_cancelAvailable = false;
        m_cancelTimer.begin(0);
    }

    m_currentCancelWindow.first = std::move(cw_.first);
    m_currentCancelWindow.second = std::move(cw_.second);
    m_cancelAvailable = false;
    if (m_currentCancelWindow.second.size() > 0)
        m_cancelTimer.begin(m_currentCancelWindow.first.first);
    else
        m_cancelTimer.begin(0);
}

bool Character::isCancelAllowed(int cancelTarget_)
{
    return m_cancelAvailable && m_currentCancelWindow.second.contains(cancelTarget_);
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

void Character::startShine(const SDL_Color &col_, int lockedDuration_, int alphaDuration_)
{
    m_colorShine = col_;
    m_shineAlphaTimer.begin(alphaDuration_);
    m_shineLockedTimer.begin(lockedDuration_);
}

void Character::switchTo(int state_)
{
    m_actionResolver.getAction(state_)->switchTo(*this);
}

void Character::lockInAnimation()
{
    m_lockedInAnimation = true;
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

bool Character::isAirborne()
{
    return m_airborne;
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

void Character::callForPriority()
{
    m_priorityHandler->callForPriority(m_playerId);
}

void Character::applyClash(const Hit &clashedHit_)
{
    applyHitstop(20);
}

void Character::generateHitParticles(HitEvent &ev_, const Vector2<float> hitpos_)
{
    auto hordir = getOwnHorDir();
    hordir.y = 1;
    const std::vector<HitParticleData> *pdata;
    if (ev_.m_hitRes == HIT_RESULT::COUNTER)
        pdata = &ev_.m_hitData.particlesOnCH;
    else if (ev_.m_hitRes == HIT_RESULT::HIT || ev_.m_hitRes == HIT_RESULT::THROWN)
        pdata = &ev_.m_hitData.particlesOnHit;
    else
        pdata = &ev_.m_hitData.particlesOnBlock;

    for (const auto &el : *pdata)
    {
        ParticleSpawnData psd;
        psd.m_angle = el.m_angle;
        psd.m_pos = hitpos_;
        psd.m_particleType = el.m_partType;
        psd.m_scale = el.m_scale;
        psd.m_flip = (hordir.x > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
        for (int i = 0; i < el.count; ++i)
        {
            psd.m_velocity = el.m_baseVelocity;
            if (el.m_randVelocity)
            {
                psd.m_velocity.x += el.m_baseVelocity.x + ((rand() % (el.m_velocityRange.x * 100)) - el.m_velocityRange.x * 100 / 2.0f) / 100.0f;
                psd.m_velocity.y += el.m_baseVelocity.y + ((rand() % (el.m_velocityRange.y * 100)) - el.m_velocityRange.y * 100 / 2.0f) / 100.0f;
            }

            psd.m_accel = el.m_additionalAccel + (psd.m_velocity * (-el.m_reverseAccel));

            if (el.m_randLifeTime)
            {
                psd.m_forcedLifeTime = (rand() % (el.m_maxLifeTime - el.m_minLifeTime + 1)) + el.m_minLifeTime;
            }

            m_particleManager->spawnParticles(psd);
        }
    }
}

bool Character::isInHitstop() const
{
    return m_inHitstop;
}

void Character::updateState()
{
    framesInState++;

    // Update cancel window
    if (!m_inHitstop)
    {
        auto timerres = m_cancelTimer.update();
        if (timerres)
        {
            if (!m_cancelAvailable)
            {
                m_cancelTimer.begin(m_currentCancelWindow.first.second - m_currentCancelWindow.first.first + 1);
                std::cout << "Cancel window opened\n";
                m_cancelAvailable = true;
            }
            else
            {
                std::cout << "Cancel window outdated\n";
                m_cancelTimer.begin(0);
                m_cancelAvailable = false;
            }
        }
    }

    auto resolverRes = m_actionResolver.update(this, m_extendedBuffer);

    if (resolverRes)
    {
        resolverRes->switchTo(*this);
    }

    if (m_currentAction)
        m_currentAction->update(*this);
}

HitsVec Character::getHits(bool allHits_)
{
    if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto hits = dynamic_cast<const Action_attack*>(m_currentAction)->getCurrentHits(m_timer.getCurrentFrame() + 1, m_pos, m_ownOrientation);
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
