#ifndef HIT_H_
#define HIT_H_

#include "Vector2.h"
#include <array>
#include <utility>
#include <vector>
#include <set>
#include "BlockHandler.h"

using FrameWindow = std::pair<int, int>;

enum class HIT_RESULT {HIT, BLOCK_HIGH, BLOCK_LOW, BLOCK_AIR, NONE}; // Potentially more options with guard points, parries, etc
enum class HITSTUN_ANIMATION {HIGH, MID, LOW};

// Integers in set are values of character-specific state ENUMs
using CancelWindow = std::pair<FrameWindow, std::set<int>>;

struct HitData
{
    int m_hitId;
    float damage = 0;
    float chipDamage = 0;
    float proratio = 1.0f;

    // TODO: change pushback logic
    // There should be no "own pushback"
    // Character should be pushed back only if he pushes opponent into the corner
    float cornerPushbackMaxRange;
    float cornerPushbackMinImpulse;
    float cornerPushbackMaxImpulse;
    float opponentPushbackOnBlock = 0;
    Vector2<float> opponentImpulseOnHit = 0;
    Vector2<float> opponentImpulseOnAirHit = 0;

    int blockstun = 0;
    int hitstun = 0;
    int hitstop = 0;
    int chHitstun = 0;
    int chHitstop = 0;
    int hitBlockShakeAmp;
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
}

#endif