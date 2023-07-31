#include "HealthWidget.h"

HealthWidget::HealthWidget(Application &application_, bool rightSide_) :
    InputReactor(application_.getInputSystem())
{
    m_rightSide = rightSide_;
    auto &tm = *application_.getTextureManager();
    m_bar = tm.getTexture(TEXTURES::HEALTHBAR);
    m_back = tm.getTexture(TEXTURES::HEALTHBAR_BACK);
    m_front = tm.getTexture(TEXTURES::HEALTHBAR_FRONT);

    updateRects();

    m_barBackFrontDstRect.w = m_back->w;
    m_barBackFrontDstRect.h = m_back->h;
    m_barBackFrontDstRect.x = m_barDstRect.x - (m_back->w - m_barDstRect.w) / 2;
    m_barBackFrontDstRect.y = m_barDstRect.y - (m_back->h - m_barDstRect.h) / 2;

    if (rightSide_)
        m_redHealthDir = 1.0f;
    else
        m_redHealthDir = -1.0f;

    subscribe(EVENTS::FN1);
    subscribe(EVENTS::FN2);
    setInputEnabled(true);
}

void HealthWidget::updateHealth(float scale_)
{
    auto newScale = utils::clamp(scale_, 0.0f, 1.0f);;
    float delta = m_scale - newScale;
    if (delta > 0)
    {
        m_redHealthLen += m_barLen * delta;
        m_redHealthBaseX = m_screenCenter + m_redHealthDir * (m_centerOffset + scale_ * m_barLen);
    }

    m_scale = newScale;

    m_resetedRedHealth = false;
}

void HealthWidget::updateRects()
{
    m_barSrcRect.x = m_barLen * (1.0f - m_displayedScale);
    m_barSrcRect.y = 0;
    m_barSrcRect.w = (m_bar->w - 1) * m_displayedScale;
    m_barSrcRect.h = m_bar->h - 1;

    m_barDstRect.y = m_topOffset;
    m_barDstRect.w = m_barLen * m_displayedScale;
    m_barDstRect.h = m_barHeight;

    if (m_rightSide)
    {
        m_barDstRect.x = m_centerOffset + m_screenCenter;
    }
    else
    {
        m_barDstRect.x = m_screenCenter - m_centerOffset - m_barLen * m_displayedScale;
    }
}

void HealthWidget::update()
{
    if (m_redHealthLen > 0 && m_resetedRedHealth)
    {
        m_redHealthLen -= (m_redHealthLen > 5.0f ? 5.0f : m_redHealthLen);
    }

    if (m_displayedScale == m_scale)
        return;
    auto diff = (m_displayedScale - m_scale) / 6.0f;
    auto diffAbs = abs(diff);
    if (diffAbs < 0.0001f)
    {
        std::cout << "END ======================\n";
        diff = m_displayedScale - m_scale;
    }
    m_displayedScale -= diff;

    updateRects();

}

void HealthWidget::draw(Renderer &renderer_, Camera &camera_)
{
    auto flip = (m_rightSide ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    
    renderer_.renderTexture(m_back->getSprite(), m_barBackFrontDstRect.x, m_barBackFrontDstRect.y, m_barBackFrontDstRect.w, m_barBackFrontDstRect.h, 0, nullptr, flip);

    if (m_redHealthLen > 0)
    {
        if (m_redHealthDir > 0.0f)
        {
            renderer_.fillRectangle({m_redHealthBaseX, m_topOffset}, {m_redHealthLen, m_barHeight}, {200, 0, 0, 150});
        }
        else
        {
            renderer_.fillRectangle({m_redHealthBaseX - m_redHealthLen, m_topOffset}, {m_redHealthLen, m_barHeight}, {200, 0, 0, 150});
        }
    }

    renderer_.renderTexture(m_bar->getSprite(), &m_barSrcRect, &m_barDstRect, 0, nullptr, flip);
    renderer_.renderTexture(m_front->getSprite(), m_barBackFrontDstRect.x, m_barBackFrontDstRect.y, m_barBackFrontDstRect.w, m_barBackFrontDstRect.h, 0, nullptr, flip);
}

void HealthWidget::receiveInput(EVENTS event, const float scale_)
{
    if (scale_ < 1.0f)
        return;

    if (event == EVENTS::FN1)
    {
        updateHealth(m_scale - 0.1f);
    }
    else
    {
        ResetRedHealth();
    }
}

void HealthWidget::ResetRedHealth()
{
    m_resetedRedHealth = true;
}