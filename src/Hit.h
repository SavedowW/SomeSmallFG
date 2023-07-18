#ifndef HIT_H_
#define HIT_H_

#include "Vector2.h"
#include <array>
#include <utility>
#include <vector>
#include <set>

using FrameWindow = std::pair<int, int>;

enum class HIT_RESULT {HIT, BLOCK}; // Potentially more options with guard points, parries, etc
enum class HITSTUN_ANIMATION {HIGH, MID, LOW};

// Integers in set are values of character-specific state ENUMs
using CancelWindow = std::pair<FrameWindow, std::set<int>>;

struct HitData
{
    int m_hitId;
    float damage = 0;
    float chipDamage = 0;

    float ownPushbackOnBlock = 0;
    float ownPushbackOnHit = 0;
    float opponentPushbackOnBlock = 0;
    float opponentPushbackOnHit = 0;

    int blockstun = 0;
    int hitstun = 0;
    int hitstop = 0;
    int chHitstun = 0;
    int chHitstop = 0;
    HITSTUN_ANIMATION hitstunAnimation = HITSTUN_ANIMATION::MID;

    CancelWindow hitCancel;
};

struct Hit : public HitData
{
    Hit(const HitData &hitData_, const std::vector<Collider> &hitboxes_);

    std::vector<Collider> getHitboxes() const;
    HitData getHitData() const;
    
    std::vector<Collider> m_hitboxes;
};

using ActiveFramesVec = std::vector<std::pair<FrameWindow, Hit>>;
using HitsVec = std::vector<Hit>;

using HurtboxFramesVec = std::vector<std::pair<FrameWindow, Collider>>;
using HurtboxVec = std::vector<Collider>;

struct HitEvent
{
    int m_hittingPlayerId = 0;
    HitData m_hitData;
    HIT_RESULT m_hitRes = HIT_RESULT::HIT;
};

namespace hitgeneration
{
    Hit generate_char1_jab();
}

#endif