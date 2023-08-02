#ifndef HEALTH_WIDGET_H_
#define HEALTH_WIDGET_H_

#include "HUD.h"
#include "Application.h"
#include "GameData.h"

class HealthWidget : public Widget
{
public:
    HealthWidget(Application &application_, bool rightSide = false);
    virtual void update() final;
    virtual void draw(Renderer &renderer_, Camera &camera_) final;
    virtual void updateHealth(float scale_);
    virtual void ResetRedHealth();
    virtual ~HealthWidget() = default;

private:
    void updateRects();

    Texture_t m_bar;
    Texture_t m_back;
    Texture_t m_front;

    SDL_FRect m_barSrcRect;
    SDL_FRect m_barDstRect;
    SDL_FRect m_barBackFrontDstRect;

    bool m_rightSide;

    const float m_screenCenter = gamedata::global::cameraWidth / 2;
    const float m_topOffset = 50.0f;
    const float m_centerOffset = 100.0f;
    const float m_barLen = 600.0f;
    const float m_barHeight = 50.0f;
    float m_displayedScale = 1.0f;
    float m_scale = 1.0f;

    float m_redHealthLen = 0.0f;
    float m_redHealthBaseX = 0.0f;
    float m_redHealthDir = 1.0f;
    bool m_resetedRedHealth = false;

    float m_healthFlash = 0.0f;
    float m_healthFlashMax = 255.0f;
    uint8_t m_rbFlashValue = 255;
    
};

#endif