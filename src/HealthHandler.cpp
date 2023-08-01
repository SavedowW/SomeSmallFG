#include "HealthHandler.h"

HealthHandler::HealthHandler(float maxHealth_, HealthWidget *widget_) :
    m_maxHealth(maxHealth_),
    m_currentHealth(maxHealth_),
    m_widget(widget_)
{

}

void HealthHandler::takeDamage(HitEvent &ev)
{
    if (ev.m_hitRes == HIT_RESULT::HIT)
    {
        if (m_currentHit == 0)
            m_proratio = ev.m_hitData.proratio;

        if (m_currentHit < m_fixedScalingValues.size())
            m_currentScaling = m_fixedScalingValues[m_currentHit];
        else
            m_currentScaling *= m_scalingMultiplier;

        m_currentHit++;

        float damage = m_currentScaling * ev.m_hitData.damage * m_proratio;
        std::cout << damage << std::endl;

        m_currentHealth -= damage;

        ev.hitInSequence = m_currentHit;

    }
    else // Block options
    {
        ev.hitInSequence = 0;
        
        float damage = ev.m_hitData.chipDamage;
        std::cout << damage << std::endl;

        m_currentHealth -= damage;

        ev.hitInSequence = m_currentHit;
    }

    if (m_widget)
        m_widget->updateHealth(m_currentHealth / m_maxHealth);
}

void HealthHandler::resetScaling()
{
    if (m_widget)
        m_widget->ResetRedHealth();

    m_currentHit = 0;
}

void HealthHandler::setWidget(HealthWidget *widget_)
{
    m_widget = widget_;
}