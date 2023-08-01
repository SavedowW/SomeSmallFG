#ifndef HEALTH_DAMAGE_HANDLER_H_
#define HEALTH_DAMAGE_HANDLER_H_

#include "HealthWidget.h"
#include "Hit.h"
#include <array>

class HealthHandler
{
public:
    HealthHandler(float maxHealth_, HealthWidget *widget_ = nullptr);
    void takeDamage(HitEvent &ev);
    void resetScaling();
    void setWidget(HealthWidget *widget_);


private:
    float m_maxHealth;
    float m_currentHealth;

    const std::array<float, 7> m_fixedScalingValues{1.0f, 0.95f, 0.85f, 0.75f, 0.6f, 0.45f, 0.25f};
    const float m_scalingMultiplier = 0.9f;
    float m_currentScaling = 1.0f;
    float m_proratio = 1.0f;
    int m_currentHit = 0;

    HealthWidget *m_widget;
};

#endif