#ifndef NOTIFY_WIDGET_H_
#define NOTIFY_WIDGET_H_

#include "HUD.h"
#include "Application.h"
#include "GameData.h"
#include "TextManager.h"

class NotifyWidget : public Widget, public InputReactor
{
public:
    NotifyWidget(Application &application_, bool rightSide = false);
    virtual void update() final;
    virtual void draw(Renderer &renderer_, Camera &camera_) final;
    virtual void addNotification(const std::string &notification);
    void receiveInput(EVENTS event, const float scale_) final;
    virtual ~NotifyWidget() = default;

private:
    struct Field
    {
        std::string str;
        FrameTimer timer;

    };

    int m_maxNotifications = 10;
    std::vector<Field> m_fields;
    float m_spacing = 40.0f;

    bool m_rightSide;

    const float m_notifyOffset = 20.0f;
    const Vector2<float> m_basePos;
    const fonts::HOR_ALIGN m_baseAlign;

    TextManager *m_textManager;
    
};

#endif