#ifndef PHYSICS_HANDLER_H_
#define PHYSICS_HANDLER_H_

#include "Hit.h"

class ComboPhysicsHandler
{
public:
    ComboPhysicsHandler(float baseGravity_);
    float getGravity();
    void takeHit(const HitEvent &ev);
    float getGroundBounceForce(float bounceForce);
    Vector2<float> getImpulseScaling(const Vector2<float> &impulse, const Vector2<int> &vecToEnemy_);
    void reset();

private:
    float m_baseGravity = 0;

    float m_bounceScaling = 1.0f;
    float m_gravityScaling = 1.0f;
    Vector2<float> m_impulseScaling = {1.0f, 1.0f};

};

#endif