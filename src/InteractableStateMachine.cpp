#include "InteractableStateMachine.h"
#include "Character.h"
#include "Projectile.h"

bool InteractableStateMachine::isAirborne() const
{
    return m_airborne;
}

void InteractableStateMachine::turnVelocityToInertia(float horMultiplier_)
{
    m_inertia.x += m_velocity.x * horMultiplier_;
    m_inertia.y += m_velocity.y;
    m_velocity = {0.0f, 0.0f};
}

bool InteractableStateMachine::canApplyDrag() const
{
    return true;
}

bool InteractableStateMachine::canBeDraggedByInertia() const
{
    return true;
}

InteractableStateMachine::InteractableStateMachine(Application &application_, Vector2<float> pos_, int stateCnt_, Camera *cam_, ParticleManager *particleManager_) :
    m_pos(pos_),
    m_actionResolver(application_.getInputSystem()),
    m_currentAction(nullptr),
    m_currentState(0),
    m_playerId(0),
    m_currentAnimation(nullptr),
    m_cam(cam_),
    m_particleManager(particleManager_)
{
}

Vector2<float> InteractableStateMachine::getPos() const
{
    return m_pos;
}


void InteractableStateMachine::setPos(Vector2<float> pos_)
{
    //std::cout << "setPos to " << pos_ << std::endl;
    m_pos = pos_;
}

Vector2<float> InteractableStateMachine::getVelocity() const
{
    return m_velocity;
}

Vector2<float> InteractableStateMachine::getFullVelocity() const
{
    return m_velocity + m_inertia;
}

Vector2<float> InteractableStateMachine::getInertia() const
{
    return m_inertia;
}

void InteractableStateMachine::setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_)
{
    m_timer.begin(0);

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

    initiate();
}

void InteractableStateMachine::updateOwnOrientation()
{
    m_ownOrientation = m_dirToEnemy;
}

void InteractableStateMachine::updateDirToEnemy()
{
    auto vecToEnemy = getHorDirToEnemy();
    if (vecToEnemy.x > 0)
        m_dirToEnemy = ORIENTATION::RIGHT;
    else if (vecToEnemy.x < 0)
        m_dirToEnemy = ORIENTATION::LEFT;
}

Vector2<float> InteractableStateMachine::getHorDirToEnemy() const
{
    return Vector2(m_otherCharacter->getPos().x - getPos().x, 0.0f).normalised();
}

Vector2<float> InteractableStateMachine::getOwnHorDir() const
{
    return Vector2<float>{(m_ownOrientation == ORIENTATION::RIGHT ? 1.0f : -1.0f), 0.0f};
}

ORIENTATION InteractableStateMachine::getOwnOrientation() const
{
    return m_ownOrientation;
}

ORIENTATION InteractableStateMachine::getOrientationToEnemy() const
{
    return m_dirToEnemy;
}

ORIENTATION InteractableStateMachine::getInputDir() const
{
    auto res = m_ownOrientation;
    return m_ownOrientation;
}

void InteractableStateMachine::generateHitParticles(HitEvent &ev_, const Vector2<float> hitpos_)
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

CharacterUpdateRes InteractableStateMachine::update()
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

    proceedCurrentState();

    updateState();

    updatePosition();

    if (!m_inHitstop && m_extendedBuffer)
        m_extendedBuffer = 0;

    CharacterUpdateRes res;
    res.moveOffset = m_pos - initPos;
    res.newPos = m_pos;
    return res;
}

void InteractableStateMachine::drawGroundProjection(Renderer &renderer_, Camera &camera_, float angle_)
{
    auto texSize = m_currentAnimation->getSize();
    auto texPos = m_pos - Vector2{texSize.x / 2, texSize.y};
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (m_ownOrientation == ORIENTATION::LEFT)
            flip = SDL_FLIP_HORIZONTAL;

    float xoffset = 0;

    if (m_inHitstop)
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

void InteractableStateMachine::draw(Renderer &renderer_, Camera &camera_)
{
    if (m_currentAnimation != nullptr)
    {
        auto texSize = m_currentAnimation->getSize();
        auto texPos = m_pos - Vector2{texSize.x / 2, texSize.y};
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (m_ownOrientation == ORIENTATION::LEFT)
            flip = SDL_FLIP_HORIZONTAL;

        float xoffset = 0;

        if (m_inHitstop)
        {
            xoffset = rand() % 13 - 6;
        }

        auto spr = m_currentAnimation->getSprite();

        auto sprsize = m_currentAnimation->getSize();
        renderer_.renderTexture(spr, texPos.x + xoffset, texPos.y, sprsize.x , sprsize.y, camera_, 0.0f, flip);
    }

    // render pushboxes with macro DEBUG
    #ifdef DEBUG_
    {
        auto hurtboxes = getHurtboxes();
        for (const auto &hb : hurtboxes)
        {
            renderer_.fillRectangle({hb.x, hb.y}, {hb.w, hb.h}, gamedata::characters::hurtboxColor, camera_);
            renderer_.drawRectangle({hb.x, hb.y}, {hb.w, hb.h}, gamedata::characters::hurtboxColor, camera_);
        }

        auto hits = getHits();
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

int InteractableStateMachine::getPlayerID()
{
    return m_playerId;
}

void InteractableStateMachine::switchTo(int state_)
{
    m_actionResolver.getAction(state_)->switchTo(*this);
}

void InteractableStateMachine::callForPriority()
{
    m_priorityHandler->callForPriority(m_playerId);
}

bool InteractableStateMachine::isInHitstop() const
{
    return m_inHitstop;
}

void InteractableStateMachine::applyCancelWindow(CancelWindow cw_)
{
    if (cw_.first.first == 0 && cw_.first.second == 0)
    {
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

bool InteractableStateMachine::isCancelAllowed(int cancelTarget_)
{
    return m_cancelAvailable && m_currentCancelWindow.second.contains(cancelTarget_);
}

void InteractableStateMachine::proceedCurrentState()
{
    if (!m_inHitstop)
    {
        auto timerRes = m_timer.update();

        if (timerRes)
        {
            if (m_currentAction)
                    m_currentAction->outdated(*this);

        }
    }
}

void InteractableStateMachine::updateState()
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

HurtboxVec InteractableStateMachine::getHurtboxes()
{
    auto currentFrame = m_timer.getCurrentFrame() + 1;
    return m_currentAction->getCurrentHurtboxes(currentFrame, m_pos, m_ownOrientation);
}

void InteractableStateMachine::updatePosition()
{
    if (m_inHitstop)
        return;

    // Apply velocity and inertia
    auto posAddition = m_velocity;
    if (canBeDraggedByInertia())
        posAddition += m_inertia;
    setPos(m_pos + posAddition);

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
    }

    // Apply drag for grounded character
    if (m_inertia.x != 0.0f && !m_airborne && canApplyDrag())
    {
        auto absInertia = abs(m_inertia.x);
        auto m_inertiaSign = m_inertia.x / abs(m_inertia.x);
        absInertia = std::max(absInertia - m_inertiaDrag, 0.0f);
        m_inertia.x = m_inertiaSign * absInertia;
    }
    
    m_currentAnimation->update();
}

float InteractableStateMachine::touchedWall(int sideDir_)
{
    return 0.0f;
}
