#include "ParticleManager.h"

Particle::Particle(Animation &&anim_) :
    m_anim(std::forward<Animation>(anim_))
{

}

void Particle::update()
{
    if (m_lifetime >= 0)
    m_lifetime--;

    m_pos += m_velocity;
    m_anim.update();

    m_velocity += m_accel;
}

ParticleManager::ParticleManager(Renderer *renderer_, AnimationManager *animManager_) :
    m_renderer(renderer_),
    m_animationManager(animManager_)
{

}

void ParticleManager::update()
{
    int i = 0;
    while (i < m_particles.size())
    {
        m_particles[i].update();
        if (m_particles[i].m_lifetime < 0)
            m_particles.erase(m_particles.begin() + i);
        else
            i++;
    }
}

void ParticleManager::draw(Camera &camera_)
{
    for (auto &particle : m_particles)
    {
        auto tex = particle.m_anim.getSprite();
        auto size = particle.m_anim.getSize() * particle.m_scale;
        auto pos = particle.m_pos - (size / 2.0f);
        m_renderer->renderTexture(tex, pos.x, pos.y, size.x, size.y, camera_, particle.m_angle, particle.m_flip);
    }
}

void ParticleManager::spawnParticles(const ParticleSpawnData &partData_)
{
    switch (partData_.m_particleType)
    {
        case (PARTICLE_TYPES::HIT_1):
        {
            Animation anim(*m_animationManager, ANIMATIONS::PARTICLE_HIT1, LOOPMETHOD::NOLOOP);
            Particle part{std::move(anim)};
            part.m_pos = partData_.m_pos;
            part.m_velocity = partData_.m_velocity;
            part.m_lifetime = 10;
            part.m_flip = partData_.m_flip;
            part.m_scale = partData_.m_scale;
            part.m_angle = partData_.m_angle;
            part.m_accel = partData_.m_accel;
            if (part.m_flip != SDL_FLIP_NONE)
                part.m_angle *= -1;
            m_particles.push_back(std::move(part));
            break;
        }

        case (PARTICLE_TYPES::HIT_2):
        {
            Animation anim(*m_animationManager, ANIMATIONS::PARTICLE_HIT2, LOOPMETHOD::NOLOOP);
            Particle part{std::move(anim)};
            part.m_pos = partData_.m_pos;
            part.m_velocity = partData_.m_velocity;
            part.m_lifetime = 15;
            part.m_flip = partData_.m_flip;
            part.m_scale = partData_.m_scale;
            part.m_angle = partData_.m_angle;
            part.m_accel = partData_.m_accel;
            if (part.m_flip != SDL_FLIP_NONE)
                part.m_angle *= -1;
            m_particles.push_back(std::move(part));
            break;
        }

        case (PARTICLE_TYPES::HIT_2_SLOWED):
        {
            Animation anim(*m_animationManager, ANIMATIONS::PARTICLE_HIT2_SLOWED, LOOPMETHOD::NOLOOP);
            Particle part{std::move(anim)};
            part.m_pos = partData_.m_pos;
            part.m_velocity = partData_.m_velocity;
            part.m_lifetime = 20;
            part.m_flip = partData_.m_flip;
            part.m_scale = partData_.m_scale;
            part.m_angle = partData_.m_angle;
            part.m_accel = partData_.m_accel;
            if (part.m_flip != SDL_FLIP_NONE)
                part.m_angle *= -1;
            m_particles.push_back(std::move(part));
            break;
        }

        case (PARTICLE_TYPES::BLOCK):
        {
            Animation anim(*m_animationManager, ANIMATIONS::PARTICLE_BLOCK, LOOPMETHOD::NOLOOP);
            Particle part{std::move(anim)};
            part.m_pos = partData_.m_pos;
            part.m_velocity = partData_.m_velocity;
            part.m_lifetime = 10;
            part.m_flip = partData_.m_flip;
            part.m_scale = partData_.m_scale;
            part.m_angle = partData_.m_angle;
            part.m_accel = partData_.m_accel;
            if (part.m_flip != SDL_FLIP_NONE)
                part.m_angle *= -1;
            m_particles.push_back(std::move(part));
            break;
        }

        case (PARTICLE_TYPES::CLASH):
        {
            Animation anim(*m_animationManager, ANIMATIONS::PARTICLE_CLASH, LOOPMETHOD::NOLOOP);
            Particle part{std::move(anim)};
            part.m_pos = partData_.m_pos;
            part.m_velocity = partData_.m_velocity;
            part.m_lifetime = 24;
            part.m_flip = partData_.m_flip;
            part.m_scale = partData_.m_scale;
            part.m_angle = partData_.m_angle;
            part.m_accel = partData_.m_accel;
            if (part.m_flip != SDL_FLIP_NONE)
                part.m_angle *= -1;
            m_particles.push_back(std::move(part));
            break;
        }
        
    }
}
