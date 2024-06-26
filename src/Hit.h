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
    int m_hitId;
    int m_damage = 0;
    
    bool m_forcedClash = false;

    float m_opponentPushbackOnBlock = 0;

    PostHitProperties m_hitProps;
    PostHitProperties m_chProps;

    int m_blockstun = 0;
    float m_chipDamage = 0;

    int m_hitBlockShakeAmp;

    std::set<BLOCK_STATE> m_canBeBlockedAs;

    CancelWindow m_cancelsOnHit;
    CancelWindow m_cancelsOnBlock;

    std::vector<ParticlesSpawnData> m_particlesOnBlock;
    std::vector<ParticlesSpawnData> m_particlesOnHit;
    std::vector<ParticlesSpawnData> m_particlesOnCH;

    static int nextHitID;

    void initializeHitID();
    void setHitID(int id_);
};

struct Hit : public HitData
{
    Hit(const HitData &hitData_, const std::vector<std::pair<FrameWindow, Collider>> &hitboxes_);
    Hit(const Hit &hit_);

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
    float m_realDamage;
    int m_hitInSequence;
};

namespace hitgeneration
{
    HitData generate_char1_normal_throw(int animIdHit_);
    std::pair<FrameWindow, Hit> generate_char1_jab(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_moveB(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_moveC(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_moveS(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_moveStepC(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_move2B(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_move4A(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_JA(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_JB(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_JC(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_214C(int animIdHit_, int animIdBlock_);
    std::pair<FrameWindow, Hit> generate_char1_236C(int animIdHit_, int animIdBlock_);

    Hit generate_char1_projectile_hit(int animIdHit_, int animIdBlock_);
}

namespace hitutils
{
    TimelineProperty<bool> getRegularCounterTimeline(const ActiveFramesVec &hits_);
    Vector2<float> getHitPosition(const std::vector<std::pair<FrameWindow, Collider>> &hits_, const HurtboxVec &hurtboxes_);
    std::pair<bool, Vector2<float>> checkCollision(const Hit &hit1_, const Hit &hit2_);
    std::pair<bool, Vector2<float>> checkCollision(const Hit &hit_, const HurtboxVec &hurtboxes_);
}

#endif