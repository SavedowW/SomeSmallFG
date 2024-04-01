#ifndef PROJECTILE_FACTORY_H_
#define PROJECTILE_FACTORY_H_
#include "Application.h"
#include "ProjectileManager.h"
#include "ActionProjectile.h"
#include "PTRecipe.h"

class Projectile;

class ProjectileFactory
{
public:
    ProjectileFactory(Application *application_, Camera *cam_, ParticleManager *particleManager_, ProjectileManager *ptManager_);

    void setPlayerID(int playerId_);

    std::unique_ptr<Projectile> createProjectile(PTRecipe &recipe_);

    virtual ~ProjectileFactory() { }


    Application *m_application;
    Camera *m_cam;
    int m_playerID;
    ParticleManager *m_particleManager;
    ProjectileManager *m_projectileManager;

};

#endif