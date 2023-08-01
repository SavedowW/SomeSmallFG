#include "Stage1.h"
#include "Application.h"
#include "Char1.h"

Stage1::Stage1(Application *application_, const Vector2<float>& size_, int lvlId_) :
    BattleLevel(application_, size_, lvlId_)
{
    m_background = std::make_unique<Stage1Background>(application_->getTextureManager());
}

void Stage1::update()
{
    BattleLevel::update();
}

void Stage1::enter()
{
    BattleLevel::enter();

    m_characters[0] = std::make_unique<Char1>(*m_application, gamedata::stages::spawnPoints[0], &m_camera);
    m_characters[1] = std::make_unique<Char1>(*m_application, gamedata::stages::spawnPoints[1], &m_camera);

    m_characters[0]->setOnStage(*m_application, 1, m_characters[1].get());
    m_characters[1]->setOnStage(*m_application, 2, m_characters[0].get());

    m_characters[0]->generateWidgets(*m_application, m_hud);
    m_characters[1]->generateWidgets(*m_application, m_hud);
}