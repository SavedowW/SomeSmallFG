#ifndef PROJECTILE_FACTORY_H_
#define PROJECTILE_FACTORY_H_
#include "Application.h"
#include "Projectile.h"
#include "ActionProjectile.h"

class Projectile;

struct PTRecipe
{
    Vector2<float> m_starterPos;
    ORIENTATION m_starterOrientation;
};

class ProjectileFactory
{
public:
    ProjectileFactory(Application *application_, Camera *cam_, ParticleManager *particleManager_);

    void setPlayerID(int playerId_);

    std::unique_ptr<Projectile> createProjectile(PTRecipe &recipe_);

    virtual ~ProjectileFactory() { }


    Application *m_application;
    Camera *m_cam;
    int m_playerID;
    ParticleManager *m_particleManager;

};

#endif