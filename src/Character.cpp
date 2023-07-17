#include "Character.h"
#include "Application.h"
#include "ActionResolver.h"

Character::Character(Application &application_, Vector2<float> pos_) :
    m_playerId(0),
    m_currentAnimation(nullptr)
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
}

CharacterUpdateRes Character::update()
{
    auto initPos = m_pos;

    auto vecToEnemy = getHorDirToEnemy();
    if (vecToEnemy.x > 0)
        m_dirToEnemy = ORIENTATION::RIGHT;
    else if (vecToEnemy.x < 0)
        m_dirToEnemy = ORIENTATION::LEFT;

    updateState();


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
            m_inertia.y += m_gravity;
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

    CharacterUpdateRes res;
    res.moveOffset = m_pos - initPos;
    res.newPos = m_pos;
    res.pushbox = getPushbox();;
    return res;
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

        renderer_.renderTexture(m_currentAnimation->getSprite(), texPos.x, texPos.y, camera_, flip);
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

        auto hits = getHits();
        for (const auto &el : hits)
        {
            auto hitboxes = el.getHitboxes();
            for (const auto &hb : hitboxes)
            {
                renderer_.fillRectangle({hb.x, hb.y}, {hb.w, hb.h}, gamedata::characters::hitboxColor, camera_);
                renderer_.drawRectangle({hb.x, hb.y}, {hb.w, hb.h}, gamedata::characters::hitboxColor, camera_);
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

Hitbox Character::getPushbox() const
{
    auto pb = m_pushbox;
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