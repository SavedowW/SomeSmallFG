#ifndef HIT_H_
#define HIT_H_

#include "Vector2.h"
#include <array>
#include <utility>
#include <vector>
#include <set>
#include "BlockHandler.h"
#include "ParticleManager.h"
#include "TimelineProperty.h"

using FrameWindow = std::pair<uint32_t, uint32_t>;

enum class HIT_RESULT {HIT, THROWN, COUNTER, BLOCK_HIGH, BLOCK_LOW, BLOCK_AIR, NONE}; // Potentially more options with guard points, parries, etc
enum class HITSTUN_ANIMATION {HIGH, MID, LOW, CROUCH, FLOAT, NONE};

// Integers in set are values of character-specific state ENUMs
using CancelWindow = std::pair<FrameWindow, std::set<int>>;

struct PostHitProperties
{
    bool hardKnd = false;

    bool groundBounce = false;
    float groundBounceStrength = 0.0f;

    bool wallBounce = false;
    float wallbounceInertiaMultiplierX = 1.0f;
    float wallbounceInertiaMultiplierY = 1.0f;

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
    
    bool forcedClash = false;

    float opponentPushbackOnBlock = 0;

    PostHitProperties hitProps;
    PostHitProperties chProps;

    int blockstun = 0;
    float chipDamage = 0;

    int hitBlockShakeAmp;

    std::set<BLOCK_STATE> canBeBlockedAs;

    CancelWindow cancelsOnHit;
    CancelWindow cancelsOnBlock;

    std::vector<ParticlesSpawnData> particlesOnBlock;
    std::vector<ParticlesSpawnData> particlesOnHit;
    std::vector<ParticlesSpawnData> particlesOnCH;

    static int nextHitID;

    void initializeHitID();
    void setHitID(int id_);
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

using HurtboxFramesVec = std::vector<std::pair<TimelineProperty<bool>, Collider>>;
using HurtboxVec = std::vector<Collider>;

struct HitEvent
{
    int m_hittingPlayerId = 0;
    bool m_isHitByProjectile = false;
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
    std::pair<FrameWindow, Hit> generate_char1_moveStepC();
    std::pair<FrameWindow, Hit> generate_char1_move2B();
    std::pair<FrameWindow, Hit> generate_char1_move4A();
    std::pair<FrameWindow, Hit> generate_char1_JA();
    std::pair<FrameWindow, Hit> generate_char1_JC();
    std::pair<FrameWindow, Hit> generate_char1_214C();
    std::pair<FrameWindow, Hit> generate_char1_236C();

    Hit generate_char1_projectile_hit();
}

namespace hitutils
{
    TimelineProperty<bool> getRegularCounterTimeline(const ActiveFramesVec &hits_);
    Vector2<float> getHitPosition(const std::vector<std::pair<FrameWindow, Collider>> &hits_, const HurtboxVec &hurtboxes_);
    std::pair<bool, Vector2<float>> checkCollision(const Hit &hit1_, const Hit &hit2_);
    std::pair<bool, Vector2<float>> checkCollision(const Hit &hit_, const HurtboxVec &hurtboxes_);
}

#endif