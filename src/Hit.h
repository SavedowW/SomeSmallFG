#ifndef HIT_H_
#define HIT_H_

#include "Vector2.h"
#include <array>
#include <utility>
#include <vector>
#include <set>
#include "BlockHandler.h"
#include "ParticleManager.h"

using FrameWindow = std::pair<int, int>;

enum class HIT_RESULT {HIT, THROWN, COUNTER, BLOCK_HIGH, BLOCK_LOW, BLOCK_AIR, NONE}; // Potentially more options with guard points, parries, etc
enum class HITSTUN_ANIMATION {HIGH, MID, LOW, CROUCH, FLOAT, NONE};

// Integers in set are values of character-specific state ENUMs
using CancelWindow = std::pair<FrameWindow, std::set<int>>;

struct HitParticleData
{
    int count = 1;
    PARTICLE_TYPES m_partType;
    float m_angle = 0.0f;
    float m_scale = 1.0f;

    float minRange = 1;
    float maxRange = 1;
    Vector2<float> m_baseOffsetMin;
    Vector2<float> m_baseOffsetMax;

    Vector2<float> m_baseVelocity;
    bool m_randVelocity = false;
    Vector2<int> m_velocityRange;

    float m_reverseAccel = 0.0f;
    Vector2<float> m_additionalAccel;

    bool m_randLifeTime = false;
    int m_minLifeTime = 1;
    int m_maxLifeTime = 1;
};

struct PostHitProperties
{
    bool hardKnd = false;

    bool groundBounce = false;
    float groundBounceStrength = 0.0f;

    bool wallBounce = false;

    int hitstun = 0;
    int hitstop = 0;
    float proratio = 1.0f;
    bool forceCrouch = false;

    Vector2<float> opponentImpulseOnHit = 0;
    Vector2<float> opponentImpulseOnAirHit = 0;

    HITSTUN_ANIMATION groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    HITSTUN_ANIMATION airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
};

struct HitData
{
    bool partOfThrow = false;

    int m_hitId;
    float damage = 0;

    float cornerPushbackMaxRange;
    float cornerPushbackMinImpulse;
    float cornerPushbackMaxImpulse;
    float opponentPushbackOnBlock = 0;

    PostHitProperties hitProps;
    PostHitProperties chProps;

    int blockstun = 0;
    float chipDamage = 0;

    int hitBlockShakeAmp;

    std::set<BLOCK_STATE> canBeBlockedAs;

    CancelWindow cancelsOnHit;
    CancelWindow cancelsOnBlock;

    std::vector<HitParticleData> particlesOnBlock;
    std::vector<HitParticleData> particlesOnHit;
    std::vector<HitParticleData> particlesOnCH;
};

struct Hit : public HitData
{
    Hit(const HitData &hitData_, const std::vector<std::pair<FrameWindow, Collider>> &hitboxes_);

    std::vector<std::pair<FrameWindow, Collider>> getHitboxes() const;
    HitData getHitData() const;
    
    std::vector<std::pair<FrameWindow, Collider>> m_hitboxes;
};

using ActiveFramesVec = std::vector<std::pair<FrameWindow, Hit>>;
using HitsVec = std::vector<Hit>;

using HurtboxFramesVec = std::vector<std::pair<FrameWindow, Collider>>;
using HurtboxVec = std::vector<Collider>;

struct HitEvent
{
    int m_hittingPlayerId = 0;
    HitData m_hitData;
    HIT_RESULT m_hitRes = HIT_RESULT::NONE;
    float realDamage;
    int hitInSequence;
};

namespace hitgeneration
{
    HitData generate_char1_normal_throw();
    std::pair<FrameWindow, Hit> generate_char1_jab();
    std::pair<FrameWindow, Hit> generate_char1_moveB();
    std::pair<FrameWindow, Hit> generate_char1_moveC();
    std::pair<FrameWindow, Hit> generate_char1_move2B();
    std::pair<FrameWindow, Hit> generate_char1_JA();
    std::pair<FrameWindow, Hit> generate_char1_JC();
    std::pair<FrameWindow, Hit> generate_char1_214C();
    std::pair<FrameWindow, Hit> generate_char1_236C();
}

namespace hitutils
{
    int getLastActiveFrame(const ActiveFramesVec &hits_);
}

#endif