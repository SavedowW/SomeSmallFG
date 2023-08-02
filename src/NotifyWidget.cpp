#include "NotifyWidget.h"

NotifyWidget::NotifyWidget(Application &application_, bool rightSide) :
    InputReactor(application_.getInputSystem()),
    m_basePos(rightSide ? Vector2{gamedata::global::cameraWidth - m_notifyOffset, 250.0f} : Vector2{m_notifyOffset, 250.0f}),
    m_baseAlign(rightSide ? fonts::HOR_ALIGN::RIGHT : fonts::HOR_ALIGN::LEFT),
    m_textManager(application_.getTextManager())
{
    subscribe(EVENTS::FN1);
    setInputEnabled(true);
}

void NotifyWidget::update()
{
    int i = 0;
    while (i < m_fields.size())
    {
        if (m_fields[i].timer.update())
            m_fields.erase(m_fields.begin() + i);
        else
            i++;
    }
}

void NotifyWidget::draw(Renderer &renderer_, Camera &camera_)
{
    auto pos = m_basePos;
    for (auto &el : m_fields)
    {
        m_textManager->renderText(el.str, 2, pos, m_baseAlign);
        pos.y += m_spacing;
    }
}

void NotifyWidget::addNotification(const std::string &notification)
{
    Field field;
    field.str = notification;
    field.timer.begin(180);
    m_fields.insert(m_fields.begin(), field);
    if (m_fields.size() > m_maxNotifications)
        m_fields.erase(m_fields.begin() + (m_fields.size() - 1));
}

void NotifyWidget::receiveInput(EVENTS event, const float scale_)
{
    if (scale_ > 0)
    addNotification("Generated notification " + std::to_string(rand() % 1000));
}
