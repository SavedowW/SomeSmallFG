#include "Character.h"
#include "Application.h"
#include "ActionResolver.h"

Character::Character(Application &application_, Vector2<float> pos_, float maxHealth_, float baseGravity_, Camera *cam_, ParticleManager *particleManager_) :
    m_playerId(0),
    m_currentAnimation(nullptr),
    m_healthHandler(maxHealth_),
    m_cam(cam_),
    m_comboPhysHandler(baseGravity_),
    m_particleManager(particleManager_)
{
    setPos(pos_);
}

void Character::setOnStage(Application &application_, int playerId_, Character *otherCharacter_)
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
    loadAnimations(application_);
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
        auto vecToEnemy = getHorDirToEnemy();
        if (vecToEnemy.x > 0)
            m_dirToEnemy = ORIENTATION::RIGHT;
        else if (vecToEnemy.x < 0)
            m_dirToEnemy = ORIENTATION::LEFT;
    }

    if (m_shineLockedTimer.update())
    {
        if (m_shineAlphaTimer.update())
        {
            m_shineAlphaTimer.begin(0);
            m_shineLockedTimer.begin(0);
        }
    }

    proceedCurrentState();

    updateState();

    updatePosition();

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

    // Define if character is airborne
    if (m_pos.y < gamedata::stages::levelOfGround)
    {
        m_airborne = true;
    }
    else if (m_airborne)
    {
        m_pos.y = gamedata::stages::levelOfGround;
        m_airborne = false;
        m_inertia.y = 0;
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
    
    m_currentAnimation->update();
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

        renderer_.renderTexture(spr, texPos.x + xoffset, texPos.y, camera_, flip);
        if (shining)
        {
            // TODO: maybe its better to create white versions during the creation of the main animations?
            float alpha = 1 - m_shineAlphaTimer.getProgressNormalized();

            auto pw = renderer_.createTexture(m_currentAnimation->getSize());
            renderer_.setRenderTarget(pw);
            renderer_.fillRenderer({255, 255, 255, 0});

            auto blendmode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MAXIMUM, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
            SDL_SetTextureBlendMode(spr, blendmode);
            renderer_.renderTexture(spr, 0, 0);
            SDL_SetTextureBlendMode(spr, SDL_BLENDMODE_BLEND);
            SDL_SetTextureBlendMode(pw, SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(pw, m_colorShine.r, m_colorShine.g, m_colorShine.b);
            SDL_SetTextureAlphaMod(pw, alpha * m_colorShine.a);
            
            renderer_.setRenderTarget(nullptr);
            renderer_.renderTexture(pw, texPos.x + xoffset, texPos.y, camera_, flip);
            SDL_DestroyTexture(pw);
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

Collider Character::getPushbox() const
{
    auto pb = (!m_airborne ? m_pushbox : m_airbornePushbox);
    pb.x += m_pos.x;
    pb.y += m_pos.y;
    return pb;
}

Vector2<float> Character::getHorDirToEnemy() const
{
    return Vector2(m_otherCharacter->getPos().x - getPos().x, 0.0f).normalised();
}

Vector2<float> Character::getOwnHorDir() const
{
    return Vector2<float>{(m_ownOrientation == ORIENTATION::RIGHT ? 1.0f : -1.0f), 0.0f};
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

bool Character::canApplyDrag() const
{
    return true;
}

bool Character::canApplyGravity() const
{
    return true;
}

bool Character::canBeDraggedByInertia() const
{
    return true;
}

void Character::turnVelocityToInertia()
{
    m_inertia += m_velocity;
    m_velocity = {0.0f, 0.0f};
}

void Character::applyHitstop(int hitstopLength)
{
    m_inHitstop = true;
    m_hitstopTimer.begin(hitstopLength);
}

HitData Character::getCurrentTakenHit()
{
    auto temp = m_currentTakenHit;
    m_currentTakenHit.m_hitId = -1;
    return temp;
}

void Character::takeCornerPushback(HitData fromHit_, float rangeToCorner_, const Vector2<int> dirFromCorner_)
{
    if (fromHit_.m_hitId != -1 && fromHit_.cornerPushbackMaxRange > rangeToCorner_)
    {
        if (utils::sameSign((int)m_inertia.x, dirFromCorner_.x))
            m_inertia.x = 0;

        float pbimpulse = std::max((1 - rangeToCorner_ / fromHit_.cornerPushbackMaxRange) * fromHit_.cornerPushbackMaxImpulse, fromHit_.cornerPushbackMinImpulse);

        m_inertia += dirFromCorner_ * pbimpulse;
    }
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