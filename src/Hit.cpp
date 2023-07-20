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
    hdata.opponentImpulseOnHit = {6.0f, 0.0f};
    hdata.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.ownPushbackOnBlock = 7.0f;
    hdata.ownPushbackOnHit = 6.0f;
    hdata.blockstun = 15;
    hdata.hitstun = 12;
    hdata.chHitstun = 20;
    hdata.hitstop = 10;
    hdata.chHitstop = 12;

    Hit hit(hdata, {{50.0f, -300.0f, 120.0f, 30.0f}});

    hit.hitCancel.first.first = 1;
    hit.hitCancel.first.second = 8;
    hit.hitCancel.second = {(int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_C};

    return hit;
}

Hit hitgeneration::generate_char1_moveC()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.damage = 20.0f;
    hdata.opponentPushbackOnBlock = 7.0f;
    hdata.opponentImpulseOnHit = {10.0f, -35.0f};
    hdata.opponentImpulseOnAirHit = {5.0f, -20.0f};
    hdata.ownPushbackOnBlock = 1.0f;
    hdata.ownPushbackOnHit = 1.0f;
    hdata.blockstun = 15;
    hdata.hitstun = 21;
    hdata.chHitstun = 25;
    hdata.hitstop = 13;
    hdata.chHitstop = 12;

    Hit hit(hdata, {{60.0f, -400.0f, 80.0f, 220.0f}});

    hit.hitCancel.first.first = 1;
    hit.hitCancel.first.second = 5;
    hit.hitCancel.second = {};

    return hit;
}