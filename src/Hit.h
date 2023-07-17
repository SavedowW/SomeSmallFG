#ifndef HIT_H_
#define HIT_H_

#include "Vector2.h"
#include <array>
#include <utility>
#include <vector>

using FrameWindow = std::pair<int, int>;

enum class HIT_RESULT {HIT, BLOCK}; // Potentially more options with guard points, parries, etc
enum class HITSTUN_ANIMATION {HIGH, MID, LOW};

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
};

struct Hit : public HitData
{
    Hit(const HitData &hitData_, const std::vector<Hitbox> &hitboxes_);

    std::vector<Hitbox> getHitboxes() const;
    HitData getHitData() const;
    
    std::vector<Hitbox> m_hitboxes;
};

using ActiveFramesVec = std::vector<std::pair<FrameWindow, Hit>>;
using HitsVec = std::vector<Hit>;

using HurtboxFramesVec = std::vector<std::pair<FrameWindow, Hitbox>>;
using HurtboxVec = std::vector<Hitbox>;

struct HitEvent
{
    int m_hittingPlayerId = 0;
    HitData m_hitData;
    HIT_RESULT m_hitRes = HIT_RESULT::HIT;
};

namespace hitgeneration
{
    inline Hit generate_char1_jab()
    {
        HitData hdata;
        hdata.m_hitId = 1;
        hdata.damage = 5.0f;
        hdata.opponentPushbackOnBlock = 2.0f;
        hdata.opponentPushbackOnHit = 1.0f;
        hdata.blockstun = 15;
        hdata.hitstun = 15;
        hdata.chHitstun = 20;
        hdata.hitstop = 10;
        hdata.chHitstop = 12;

        Hit hit(hdata, {{50.0f, -300.0f, 120.0f, 30.0f}});
        return hit;
    }
}

#endif