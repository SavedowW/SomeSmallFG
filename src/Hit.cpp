#include "Hit.h"
#include "Char1.h"

Hit::Hit(const HitData &hitData_, const std::vector<std::pair<FrameWindow, Collider>> &hitboxes_) :
    HitData(hitData_),
    m_hitboxes(hitboxes_)
{
        
}

std::vector<std::pair<FrameWindow, Collider>> Hit::getHitboxes() const
{
    return m_hitboxes;
}

HitData Hit::getHitData() const
{
    return *static_cast<const HitData*>(this);
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_jab()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.proratio = 0.8f;
    hdata.damage = 5.0f / hdata.proratio;
    hdata.opponentPushbackOnBlock = 7.0f;
    hdata.opponentImpulseOnHit = {6.0f, 0.0f};
    hdata.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.cornerPushbackMaxRange = 390.0f;
    hdata.cornerPushbackMinImpulse = 8.0f;
    hdata.cornerPushbackMaxImpulse = 25.0f;
    hdata.blockstun = 7;
    hdata.hitstun = 12;
    hdata.chAdditionalHitstun = 20;
    hdata.hitstop = 10;
    hdata.chAdditionalHitstop = 12;
    hdata.hitBlockShakeAmp = 0;
    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    Hit hit(hdata, {{{5, 7}, {50.0f, -300.0f, 120.0f, 30.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 8;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_214C};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_214C};

    return {{5, 7}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveB()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.proratio = 0.9f;
    hdata.damage = 15.0f / hdata.proratio;
    hdata.opponentPushbackOnBlock = 5.6f;
    hdata.opponentImpulseOnHit = {3.0f, 0.0f};
    hdata.opponentImpulseOnAirHit = {10.0f, -5.0f};
    hdata.cornerPushbackMaxRange = 390.0f;
    hdata.cornerPushbackMinImpulse = 8.0f;
    hdata.cornerPushbackMaxImpulse = 30.0f;
    hdata.blockstun = 8;
    hdata.hitstun = 15;
    hdata.chAdditionalHitstun = 25;
    hdata.hitstop = 11;
    hdata.chAdditionalHitstop = 13;
    hdata.hitBlockShakeAmp = 20;
    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    Hit hit(hdata, {{{7, 10}, {50.0f, -265.0f, 192.0f, 70.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 8;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_214C};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_214C};

    return {{7, 10}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveC()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.proratio = 1.0f;
    hdata.damage = 20.0f / hdata.proratio;
    hdata.chipDamage = 2.0f;
    hdata.opponentPushbackOnBlock = 3.0f;
    hdata.opponentImpulseOnHit = {10.0f, -35.0f};
    hdata.opponentImpulseOnAirHit = {5.0f, -20.0f};
    hdata.cornerPushbackMaxRange = 300.0f;
    hdata.cornerPushbackMinImpulse = 8.0f;
    hdata.cornerPushbackMaxImpulse = 30.0f;
    hdata.blockstun = 15;
    hdata.hitstun = 21;
    hdata.chAdditionalHitstun = 25;
    hdata.hitstop = 13;
    hdata.chAdditionalHitstop = 12;
    hdata.hitBlockShakeAmp = 40;
    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    Hit hit(hdata, {{{13, 16}, {60.0f, -400.0f, 80.0f, 220.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 1;
    hit.cancelsOnHit.second = {};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 1;
    hit.cancelsOnBlock.second = {};

    return {{13, 16}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_move2B()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.proratio = 0.7f;
    hdata.damage = 13.0f / hdata.proratio;
    hdata.opponentPushbackOnBlock = 3.0f;
    hdata.opponentImpulseOnHit = {3.0f, 0.0f};
    hdata.opponentImpulseOnAirHit = {10.0f, -5.0f};
    hdata.cornerPushbackMaxRange = 390.0f;
    hdata.cornerPushbackMinImpulse = 15.0f;
    hdata.cornerPushbackMaxImpulse = 31.0f;
    hdata.blockstun = 8;
    hdata.hitstun = 12;
    hdata.chAdditionalHitstun = 25;
    hdata.hitstop = 11;
    hdata.chAdditionalHitstop = 13;
    hdata.hitBlockShakeAmp = 15;
    hdata.canBeBlockedAs = {BLOCK_STATE::LOW, BLOCK_STATE::AIR};

    Hit hit(hdata, {{{10, 12}, {10.0f, -100.0f, 110.0f, 100.0f}}, {{10, 12}, {120.0f, -40.0f, 70.0f, 40.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 15;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_214C};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 15;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_214C};

    return {{10, 12}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JA()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.proratio = 0.7f;
    hdata.damage = 5.0f / hdata.proratio;
    hdata.opponentPushbackOnBlock = 7.0f;
    hdata.opponentImpulseOnHit = {6.0f, 0.0f};
    hdata.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.cornerPushbackMaxRange = 390.0f;
    hdata.cornerPushbackMinImpulse = 8.0f;
    hdata.cornerPushbackMaxImpulse = 25.0f;
    hdata.blockstun = 7;
    hdata.hitstun = 12;
    hdata.chAdditionalHitstun = 20;
    hdata.hitstop = 10;
    hdata.chAdditionalHitstop = 12;
    hdata.hitBlockShakeAmp = 0;
    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::AIR};

    Hit hit(hdata, {{{6, 8}, {50.0f, -280.0f, 80.0f, 80.0f}}, {{6, 8}, {110.0f, -220.0f, 50.0f, 50.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 9;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JA, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 9;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JA, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    return {{6, 8}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JC()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.proratio = 1.0f;
    hdata.damage = 25.0f / hdata.proratio;
    hdata.chipDamage = 2.0f;
    hdata.opponentPushbackOnBlock = 10.0f;
    hdata.opponentImpulseOnHit = {13.0f, -35.0f};
    hdata.opponentImpulseOnAirHit = {13.0f, -33.0f};
    hdata.cornerPushbackMaxRange = 400.0f;
    hdata.cornerPushbackMinImpulse = 5.0f;
    hdata.cornerPushbackMaxImpulse = 15.0f;
    hdata.blockstun = 15;
    hdata.hitstun = 12;
    hdata.chAdditionalHitstun = 20;
    hdata.hitstop = 10;
    hdata.chAdditionalHitstop = 12;
    hdata.hitBlockShakeAmp = 15;
    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AUTO, BLOCK_STATE::AIR};

    Hit hit(hdata, {{{12, 17}, {0.0f, -280.0f, 180.0f, 130.0f}}, {{12, 17}, {-30.0f, -160.0f, 130.0f, 130.0f}}, {{12, 17}, {100.0f, -385.0f, 40.0f, 300.0f}}, {{12, 17}, {-30.0f, -440.0f, 130.0f, 160.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 9;
    hit.cancelsOnHit.second = {};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 9;
    hit.cancelsOnBlock.second = {};

    return {{12, 17}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_214C()
{
    HitData hdata;
    hdata.m_hitId = 1;
    hdata.proratio = 1.0f;
    hdata.damage = 40.0f / hdata.proratio;
    hdata.chipDamage = 4.0f;
    hdata.opponentPushbackOnBlock = 5.6f;
    hdata.opponentImpulseOnHit = {3.0f, 0.0f};
    hdata.opponentImpulseOnAirHit = {10.0f, 40.0f};
    hdata.cornerPushbackMaxRange = 300.0f;
    hdata.cornerPushbackMinImpulse = 5.0f;
    hdata.cornerPushbackMaxImpulse = 8.0f;
    hdata.blockstun = 12;
    hdata.hitstun = 28;
    hdata.chAdditionalHitstun = 25;
    hdata.hitstop = 11;
    hdata.chAdditionalHitstop = 13;
    hdata.hardKnd = true;
    hdata.hitBlockShakeAmp = 20;
    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AUTO};

    hdata.groundBounce = true;
    hdata.groundBounceStrength = 15.0f;

    Hit hit(hdata, {{{16, 17}, {50.0f, -400.0f, 100.0f, 150.0f}}, {{18, 21}, {50.0f, -400.0f, 130.0f, 400.0f}}});

    hit.cancelsOnHit.first.first = 0;
    hit.cancelsOnHit.first.second = 0;
    hit.cancelsOnHit.second = {};

    hit.cancelsOnBlock.first.first = 0;
    hit.cancelsOnBlock.first.second = 0;
    hit.cancelsOnBlock.second = {};

    return {{16, 21}, hit};
}