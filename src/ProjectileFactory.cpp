#include "ProjectileFactory.h"

ProjectileFactory::ProjectileFactory(Application *application_, Camera *cam_, ParticleManager *particleManager_) :
    m_application(application_),
    m_cam(cam_),
    m_particleManager(particleManager_)
{
}

void ProjectileFactory::setPlayerID(int playerId_)
{
    m_playerID = playerId_;
}

std::unique_ptr<Projectile> ProjectileFactory::createProjectile(PTRecipe &recipe_)
{
    auto actpt1 = std::unique_ptr<Action>((new ActionProjectile(0, {}, ANIMATIONS::CHAR1_PROJECTILE_STARTUP, StateMarker(3, {}), false, false))->setSwitchData(false, 29, false, false,
    {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})->setOutdatedTransition(1));
    auto actpt2 = std::unique_ptr<Action>((new ActionProjectileHitProvider(1, {}, ANIMATIONS::CHAR1_PROJECTILE_FILLER, StateMarker(3, {}), false, hitgeneration::generate_char1_projectile_hit(), 3))->setSwitchData(false, 60, false, false,
    {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})->setOutdatedTransition(2));
    auto actpt3 = std::unique_ptr<Action>((new ActionProjectile(2, {}, ANIMATIONS::CHAR1_PROJECTILE_FADEOUT, StateMarker(3, {}), false, false))->setOutdatedExpire(true)->setSwitchData(false, 16, false, false,
    {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}));

    auto pt = std::unique_ptr<Projectile>(new Projectile(*m_application, recipe_.m_starterPos, 3, m_cam, m_particleManager, 0, recipe_.m_starterOrientation));
    pt->addAction(std::move(actpt1));
    pt->addAction(std::move(actpt2));
    pt->addAction(std::move(actpt3));

    return std::move(pt);
}
