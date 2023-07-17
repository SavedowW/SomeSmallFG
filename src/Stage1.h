#ifndef STAGE_1_H_
#define STAGE_1_H_

#include "BattleLevel.h"

class Stage1Background : public Background
{
public:
    Stage1Background(TextureManager *textureManager_)
    {
        BackgroundSegment seg1(textureManager_->getTexture(TEXTURES::STAGE1_BACKGROUND_2));
        m_backgroundSegments.push_back(seg1);

        BackgroundSegment seg2(textureManager_->getTexture(TEXTURES::STAGE1_BACKGROUND_1));
        m_backgroundSegments.push_back(seg2);
    }

    virtual ~Stage1Background() = default;
};

class Stage1 : public BattleLevel<Stage1Background>
{
public:
    Stage1(Application *application_, const Vector2<float>& size_, int lvlId_);

    virtual void enter() override;

    virtual void update() override;

    virtual ~Stage1() = default;
};

#endif
