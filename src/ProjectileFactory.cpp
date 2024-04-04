#include "ProjectileFactory.h"

ProjectileFactory::ProjectileFactory(Application *application_, Camera *cam_, ParticleManager *particleManager_, ProjectileManager *ptManager_) :
    m_application(application_),
    m_cam(cam_),
    m_particleManager(particleManager_),
    m_projectileManager(ptManager_)
{
}

void ProjectileFactory::setPlayerID(int playerId_)
{
    m_playerID = playerId_;
}

std::unique_ptr<Projectile> ProjectileFactory::createProjectile(PTRecipe &recipe_)
{
    AnimationManager animmgmgt = *m_application->getAnimationManager();

    auto actpt1 = std::unique_ptr<Action>((new ActionProjectile(0, {}, animmgmgt.getAnimID("Char1/ProjectileFirewallStartup"), StateMarker(3, {}), false, false))->setSwitchData(false, 29, false, false,
    {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})->setOutdatedTransition(1));
    auto actpt2 = std::unique_ptr<Action>((new ActionProjectileHitProvider(1, {}, animmgmgt.getAnimID("Char1/ProjectileFirewallFiller"), StateMarker(3, {}), false, hitgeneration::generate_char1_projectile_hit(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block")), 3))->setSwitchData(false, 360, false, false,
    {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})->setOutdatedTransition(2));
    auto actpt3 = std::unique_ptr<Action>((new ActionProjectile(2, {}, animmgmgt.getAnimID("Char1/ProjectileFirewallFadeout"), StateMarker(3, {}), false, false))->setOutdatedExpire(true)->setSwitchData(false, 16, false, false,
    {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}));

    auto pos = utils::clamp(recipe_.m_starterPos, recipe_.m_minPos, recipe_.m_maxPos);
    auto pt = std::unique_ptr<Projectile>(new Projectile(*m_application, pos, 3, m_cam, m_particleManager, recipe_.m_initialState, recipe_.m_starterOrientation, m_projectileManager, recipe_.m_ptTypeID));
    pt->addAction(std::move(actpt1));
    pt->addAction(std::move(actpt2));
    pt->addAction(std::move(actpt3));

    return std::move(pt);
}
