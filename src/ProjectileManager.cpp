#include "ProjectileManager.h"
#include "Character.h"

ProjectileManager::ProjectileManager(Application *application_, PriorityHandler *priorityHandler_) :
    m_application(application_),
    m_priorityHandler(priorityHandler_)
{
}

void ProjectileManager::addProjectile(std::unique_ptr<Projectile> &&pt_, int playerId_, Character *otherCharacter_)
{
    m_projectiles.push_back(std::move(pt_));
    m_projectiles[m_projectiles.size() - 1]->setOnStage(*m_application, playerId_, otherCharacter_, m_priorityHandler);
}

std::vector<std::unique_ptr<Projectile>> &ProjectileManager::getProjectiles()
{
    return m_projectiles;
}

void ProjectileManager::update()
{
    for (auto &pt : m_projectiles)
        pt->update();

    for (int i = 0; i < m_projectiles.size();)
    {
        if (m_projectiles[i]->isExpired())
            m_projectiles.erase(m_projectiles.begin() + i);
        else
            i++;
    }
}

bool ProjectileManager::containsProjectile(int playerID_, int ptTypeID_)
{
    for (auto &pt : m_projectiles)
        if (pt->getPlayerID() == playerID_ && pt->getPtTypeID() == ptTypeID_)
            return true;

    return false;
}
