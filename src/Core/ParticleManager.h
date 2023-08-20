#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Renderer.h"
#include "Vector2.h"
#include <vector>
#include "AnimationManager.h"

struct Particle
{
    Particle(Animation &&anim_);
    void update();

    Vector2<float> m_pos;
    Vector2<float> m_velocity;
    Vector2<float> m_accel;

    SDL_RendererFlip m_flip;
    float m_angle;
    float m_scale;

    Animation m_anim;
    int m_lifetime;
};

enum class PARTICLE_TYPES {
    HIT_1,
    HIT_2,
    HIT_2_SLOWED,
    BLOCK,
    CLASH
};

struct ParticleSpawnData
{
    Vector2<float> m_pos;
    Vector2<float> m_velocity;
    Vector2<float> m_accel;
    PARTICLE_TYPES m_particleType;
    SDL_RendererFlip m_flip = SDL_FLIP_NONE;
    float m_angle = 0;
    float m_scale = 1;
    int m_forcedLifeTime = -1;

};

class ParticleManager
{
public:
    ParticleManager(Renderer *renderer_, AnimationManager *animManager_);
    void update();
    void draw(Camera &camera_);
    void spawnParticles(const ParticleSpawnData &partData_);

private:
    std::vector<Particle> m_particles;
    Renderer* m_renderer;
    AnimationManager* m_animationManager;
};

#endif