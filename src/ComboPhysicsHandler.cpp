#include "ComboPhysicsHandler.h"

ComboPhysicsHandler::ComboPhysicsHandler(float baseGravity_) :
    m_baseGravity(baseGravity_)
{
    reset();
}

float ComboPhysicsHandler::getGravity()
{
    return m_baseGravity * m_gravityScaling;
}

void ComboPhysicsHandler::takeHit(const HitEvent &ev)
{
    m_gravityScaling *= 1.02;
}

float ComboPhysicsHandler::getGroundBounceForce(float bounceForce_)
{
    auto res = bounceForce_ / m_bounceScaling;
    m_bounceScaling *= 1.02f;
    return res;
}

Vector2<float> ComboPhysicsHandler::getImpulseScaling(const Vector2<float> &impulse, const Vector2<int> &vecToEnemy_)
{
    auto res = impulse;

    if (impulse.x * vecToEnemy_.x < 0)
    {
        res.x *= m_impulseScaling.x;
        m_impulseScaling.x *= 1.02f;
    }
    else if (impulse.x * vecToEnemy_.x > 0)
    {
        res.x /= m_impulseScaling.x;
        m_impulseScaling.x *= 1.02f;
    }

    if (impulse.y < 0)
    {
        res.y /= m_impulseScaling.y;
        m_impulseScaling.y *= 1.03f;
    }
    else if (impulse.x > 0)
    {
        res.x *= m_impulseScaling.y;
        m_impulseScaling.y *= 1.03f;
    }

    return res;
}

void ComboPhysicsHandler::reset()
{
    m_bounceScaling = 1.0f;
    m_gravityScaling = 1.0f;
    m_impulseScaling = {1.0f, 1.0f};
}
