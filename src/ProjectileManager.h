#ifndef PROJECTILE_MANAGER_H_
#define PROJECTILE_MANAGER_H_

#include "Projectile.h"

class ProjectileManager
{
public:
    ProjectileManager(Application *application_, PriorityHandler *priorityHandler_);
    void addProjectile(std::unique_ptr<Projectile> &&pt_, int playerId_, Character *otherCharacter_);
    std::vector<std::unique_ptr<Projectile>> &getProjectiles();
    void update();
    bool containsProjectile(int playerID_, int ptTypeID_);

private:
    Application *m_application;
    PriorityHandler *m_priorityHandler;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;

};


#endif