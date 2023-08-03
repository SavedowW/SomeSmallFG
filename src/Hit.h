#ifndef HIT_H_
#define HIT_H_

#include "Vector2.h"
#include <array>
#include <utility>
#include <vector>
#include <set>
#include "BlockHandler.h"

using FrameWindow = std::pair<int, int>;

enum class HIT_RESULT {HIT, COUNTER, BLOCK_HIGH, BLOCK_LOW, BLOCK_AIR, NONE}; // Potentially more options with guard points, parries, etc
enum class HITSTUN_ANIMATION {HIGH, MID, LOW};

// Integers in set are values of character-specific state ENUMs
using CancelWindow = std::pair<FrameWindow, std::set<int>>;

struct PostHitProperties
{
    bool hardKnd = false;

    bool groundBounce = false;
    float groundBounceStrength = 0.0f;

    int hitstun = 0;
    int hitstop = 0;
    float proratio = 1.0f;

    Vector2<float> opponentImpulseOnHit = 0;
    Vector2<float> opponentImpulseOnAirHit = 0;
};

struct HitData
{
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

    // TODO: move animation data into PostHitProperties and separating ground and air hits before adding new animations
    HITSTUN_ANIMATION hitstunAnimation = HITSTUN_ANIMATION::MID;
    std::set<BLOCK_STATE> canBeBlockedAs;

    CancelWindow cancelsOnHit;
    CancelWindow cancelsOnBlock;
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
    std::pair<FrameWindow, Hit> generate_char1_jab();
    std::pair<FrameWindow, Hit> generate_char1_moveB();
    std::pair<FrameWindow, Hit> generate_char1_moveC();
    std::pair<FrameWindow, Hit> generate_char1_move2B();
    std::pair<FrameWindow, Hit> generate_char1_JA();
    std::pair<FrameWindow, Hit> generate_char1_JC();
    std::pair<FrameWindow, Hit> generate_char1_214C();
}

namespace hitutils
{
    int getLastActiveFrame(const ActiveFramesVec &hits_);
}

#endif