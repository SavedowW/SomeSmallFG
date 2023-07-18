#include "Hit.h"
#include "Char1.h"

Hit::Hit(const HitData &hitData_, const std::vector<Collider> &hitboxes_) :
    HitData(hitData_),
    m_hitboxes(hitboxes_)
{
        
}

std::vector<Collider> Hit::getHitboxes() const
{
    return m_hitboxes;
}

HitData Hit::getHitData() const
{
    return *static_cast<const HitData*>(this);
}

Hit hitgeneration::generate_char1_jab()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.damage = 5.0f;
    hdata.opponentPushbackOnBlock = 7.0f;
    hdata.opponentPushbackOnHit = 5.0f;
    hdata.ownPushbackOnBlock = 7.0f;
    hdata.ownPushbackOnHit = 5.0f;
    hdata.blockstun = 15;
    hdata.hitstun = 15;
    hdata.chHitstun = 20;
    hdata.hitstop = 10;
    hdata.chHitstop = 12;

    Hit hit(hdata, {{50.0f, -300.0f, 120.0f, 30.0f}});

    hit.hitCancel.first.first = 1;
    hit.hitCancel.first.second = 5;
    hit.hitCancel.second = {(int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_A};

    return hit;
}