#include "Hit.h"
#include "Char1.h"

int HitData::nextHitID = 1;

Hit::Hit(const HitData &hitData_, const std::vector<std::pair<FrameWindow, Collider>> &hitboxes_) :
    HitData(hitData_),
    m_hitboxes(hitboxes_)
{
        
}

Hit::Hit(const Hit &hit_) :
    HitData(*(HitData*)(&hit_)),
    m_hitboxes(hit_.m_hitboxes)
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

HitData hitgeneration::generate_char1_normal_throw(int animIdHit_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_hitProps.hitstop = 10;
    hdata.m_hitProps.hitstun = 12;
    hdata.m_hitProps.proratio = 1.0f;
    hdata.m_hitProps.hardKnd = true;
    hdata.m_hitProps.groundBounce = true;
    hdata.m_hitProps.groundBounceStrength = 5.0f;
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;

    hdata.m_hitBlockShakeAmp = 35;

    hdata.m_damage = 50.0f / hdata.m_hitProps.proratio;

    return hdata;
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_jab(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.5f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_scale = 0.4f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 110.0f;

    hdata.m_hitProps.hitstop = 10;
    hdata.m_hitProps.hitstun = 12;
    hdata.m_hitProps.proratio = 0.8f;
    hdata.m_hitProps.opponentImpulseOnHit = {90.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::HIGH;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 2;
    hdata.m_chProps.hitstop += 3;
    hdata.m_chProps.proratio = 0.9;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 7;

    hdata.m_hitBlockShakeAmp = 0;

    hdata.m_damage = 5.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{5, 7}, {50.0f, -300.0f, 120.0f, 30.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 8;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 8;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    return {{5, 7}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveB(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 80.0f;

    hdata.m_hitProps.hitstop = 11;
    hdata.m_hitProps.hitstun = 15;
    hdata.m_hitProps.proratio = 0.9f;
    hdata.m_hitProps.opponentImpulseOnHit = {50.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {10.0f, -7.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 3;
    hdata.m_chProps.hitstop += 4;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 13;

    hdata.m_hitBlockShakeAmp = 20;

    hdata.m_damage = 15.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{7, 10}, {50.0f, -265.0f, 192.0f, 70.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 8;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 8;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::STEP};

    return {{7, 10}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveC(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 100.0f;

    hdata.m_hitProps.hitstop = 13;
    hdata.m_hitProps.hitstun = 20;
    hdata.m_hitProps.proratio = 1.0f;
    hdata.m_hitProps.opponentImpulseOnHit = {80.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {5.0f, -15.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 4;
    hdata.m_chProps.hitstop += 5;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 16;
    hdata.m_chipDamage = 0.0f;

    hdata.m_hitBlockShakeAmp = 40;

    hdata.m_damage = 30.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{11, 16}, {60.0f, -250.0f, 90.0f, 150.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 10;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 10;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    return {{11, 16}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveS(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 100.0f;

    hdata.m_hitProps.hitstop = 13;
    hdata.m_hitProps.wallBounce = true;
    hdata.m_hitProps.wallbounceInertiaMultiplierX = 1.0f;
    hdata.m_hitProps.wallbounceInertiaMultiplierY = 0.5f;
    hdata.m_hitProps.hitstun = 28;
    hdata.m_hitProps.proratio = 1.0f;
    hdata.m_hitProps.opponentImpulseOnHit = {120.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {20.0f, -15.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 4;
    hdata.m_chProps.hitstop += 5;
    hdata.m_chProps.groundBounce = true;
    hdata.m_chProps.opponentImpulseOnHit = {15.0f, 10.0f};
    hdata.m_chProps.groundBounceStrength = 25.0f;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 16;
    hdata.m_chipDamage = 0.0f;

    hdata.m_hitBlockShakeAmp = 40;

    hdata.m_damage = 50.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{8, 13}, {90.0f, -350.0f, 150.0f, 100.0f}},
                    {{12, 13}, {120.0f, -250.0f, 120.0f, 100.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 10;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 10;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    return {{8, 14}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveStepC(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 150.0f;

    hdata.m_hitProps.hitstop = 13;
    hdata.m_hitProps.hitstun = 20;
    hdata.m_hitProps.proratio = 1.0f;
    hdata.m_hitProps.opponentImpulseOnHit = {5.0f, -30.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {5.0f, -30.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 4;
    hdata.m_chProps.hitstop += 5;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 16;
    hdata.m_chipDamage = 0.0f;

    hdata.m_hitBlockShakeAmp = 40;

    hdata.m_damage = 40.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{6, 13}, {60.0f, -450.0f, 200.0f, 400.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 20;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 20;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR};

    return {{6, 13}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_move2B(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 60.0f;

    hdata.m_hitProps.hitstop = 11;
    hdata.m_hitProps.hitstun = 12;
    hdata.m_hitProps.proratio = 0.7f;
    hdata.m_hitProps.opponentImpulseOnHit = {40.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {10.0f, -5.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::LOW;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
    
    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 3;
    hdata.m_chProps.hitstop += 4;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::LOW, BLOCK_STATE::AIR};

    hdata.m_blockstun = 10;

    hdata.m_hitBlockShakeAmp = 15;

    hdata.m_damage = 13.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{10, 12}, {10.0f, -100.0f, 110.0f, 100.0f}}, {{10, 12}, {120.0f, -40.0f, 70.0f, 40.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 15;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 15;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    return {{10, 12}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_move4A(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 120.0f;
    hdata.m_forcedClash = true;

    hdata.m_hitProps.hitstop = 11;
    hdata.m_hitProps.hitstun = 15;
    hdata.m_hitProps.proratio = 0.9f;
    hdata.m_hitProps.opponentImpulseOnHit = {100.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {8.0f, -15.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 3;
    hdata.m_chProps.hitstop += 4;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 13;

    hdata.m_hitBlockShakeAmp = 20;

    hdata.m_damage = 15.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{8, 11}, {60.0f, -420.0f, 80.0f, 170.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 8;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 8;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    return {{7, 10}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JA(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_angle = 45.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_angle = 0.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 110.0f;

    hdata.m_hitProps.hitstop = 10;
    hdata.m_hitProps.hitstun = 12;
    hdata.m_hitProps.proratio = 0.7f;
    hdata.m_hitProps.opponentImpulseOnHit = {90.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::HIGH;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 2;
    hdata.m_chProps.hitstop += 3;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::AIR};

    hdata.m_blockstun = 9;

    hdata.m_hitBlockShakeAmp = 0;

    hdata.m_damage = 5.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{6, 8}, {50.0f, -280.0f, 80.0f, 80.0f}}, {{6, 8}, {110.0f, -220.0f, 50.0f, 50.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 9;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JA, (int)CHAR1_STATE::MOVE_JB, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 9;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JA, (int)CHAR1_STATE::MOVE_JB, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    return {{6, 8}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JB(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_angle = 45.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_angle = 0.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 110.0f;

    hdata.m_hitProps.hitstop = 11;
    hdata.m_hitProps.hitstun = 15;
    hdata.m_hitProps.proratio = 0.8f;
    hdata.m_hitProps.opponentImpulseOnHit = {90.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {13.0f, -20.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::HIGH;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 3;
    hdata.m_chProps.hitstop += 4;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::AIR};

    hdata.m_blockstun = 13;

    hdata.m_hitBlockShakeAmp = 20;

    hdata.m_damage = 20.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{8, 13}, {50.0f, -300.0f, 200.0f, 60.0f}}, {{8, 13}, {200.0f, -330.0f, 160.0f, 60.0f}}, {{8, 13}, {0.0f, -200.0f, 100.0f, 80.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 9;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 9;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    return {{8, 13}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JC(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.9f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_angle = -60.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 15;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 150.0f;
    
    hdata.m_hitProps.hitstun = 12;
    hdata.m_hitProps.hitstop = 10;
    hdata.m_hitProps.proratio = 1.0f;
    hdata.m_hitProps.opponentImpulseOnHit = {18.0f, -40.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {18.0f, -40.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::HIGH;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
    hdata.m_hitProps.wallBounce = true;
    hdata.m_hitProps.wallbounceInertiaMultiplierX = 0.75f;
    hdata.m_hitProps.wallbounceInertiaMultiplierY = 0.5f;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 4;
    hdata.m_chProps.hitstop += 5;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AUTO, BLOCK_STATE::AIR};

    hdata.m_blockstun = 15;
    hdata.m_chipDamage = 2.0f;

    hdata.m_hitBlockShakeAmp = 15;

    hdata.m_damage = 25.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{12, 17}, {0.0f, -280.0f, 180.0f, 130.0f}}, {{12, 17}, {-30.0f, -160.0f, 130.0f, 130.0f}}, {{12, 17}, {100.0f, -385.0f, 40.0f, 300.0f}}, {{12, 17}, {-30.0f, -440.0f, 130.0f, 160.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 9;
    hit.m_cancelsOnHit.second = {};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 9;
    hit.m_cancelsOnBlock.second = {};

    return {{12, 17}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_214C(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_angle = 90.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 20;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 50.0f;

    hdata.m_hitProps.hitstun = 28;
    hdata.m_hitProps.hitstop = 11;
    hdata.m_hitProps.proratio = 1.0f;
    hdata.m_hitProps.opponentImpulseOnHit = {10.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {10.0f, 40.0f};
    hdata.m_hitProps.hardKnd = true;
    hdata.m_hitProps.groundBounce = true;
    hdata.m_hitProps.forceCrouch = true;
    hdata.m_hitProps.groundBounceStrength = 15.0f;
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 5;
    hdata.m_chProps.hitstop += 6;
    hdata.m_chProps.opponentImpulseOnHit = {3.0f, 10.0f};
    hdata.m_chProps.groundBounceStrength = 25.0f;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 20;
    hdata.m_chipDamage = 4.0f;

    hdata.m_hitBlockShakeAmp = 20;

    hdata.m_damage = 40.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{16, 17}, {50.0f, -400.0f, 100.0f, 150.0f}}, {{18, 21}, {50.0f, -400.0f, 130.0f, 400.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 8;
    hit.m_cancelsOnHit.second = {(int)CHAR1_STATE::STEP};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 8;
    hit.m_cancelsOnBlock.second = {(int)CHAR1_STATE::STEP};

    return {{16, 21}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_236C(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_angle = -45.0;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_angle = -90.0;
    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 15;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 50.0f;

    hdata.m_hitProps.hitstop = 13;
    hdata.m_hitProps.hitstun = 21;
    hdata.m_hitProps.proratio = 1.0f;
    hdata.m_hitProps.opponentImpulseOnHit = {5.0f, -40.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {5.0f, -30.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 4;
    hdata.m_chProps.hitstop += 5;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 16;
    hdata.m_chipDamage = 2.0f;

    hdata.m_hitBlockShakeAmp = 40;

    hdata.m_damage = 20.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{13, 16}, {60.0f, -400.0f, 80.0f, 220.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 1;
    hit.m_cancelsOnHit.second = {};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 1;
    hit.m_cancelsOnBlock.second = {};

    return {{13, 16}, hit};
}

Hit hitgeneration::generate_char1_projectile_hit(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.m_particlesOnHit.push_back(pdatatemp);
    hdata.m_particlesOnCH.push_back(pdatatemp);

    hdata.m_opponentPushbackOnBlock = 80.0f;

    hdata.m_hitProps.hitstop = 11;
    hdata.m_hitProps.hitstun = 15;
    hdata.m_hitProps.proratio = 0.9f;
    hdata.m_hitProps.opponentImpulseOnHit = {50.0f, 0.0f};
    hdata.m_hitProps.opponentImpulseOnAirHit = {10.0f, -7.0f};
    hdata.m_hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.m_hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.m_chProps = hdata.m_hitProps;
    hdata.m_chProps.hitstun += 3;
    hdata.m_chProps.hitstop += 4;

    hdata.m_canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.m_blockstun = 13;

    hdata.m_hitBlockShakeAmp = 20;

    hdata.m_damage = 15.0f / hdata.m_hitProps.proratio;

    Hit hit(hdata, {{{1, 100}, {-50.0f, -400.0f, 130.0f, 400.0f}}});

    hit.m_cancelsOnHit.first.first = 1;
    hit.m_cancelsOnHit.first.second = 9;
    hit.m_cancelsOnHit.second = {};

    hit.m_cancelsOnBlock.first.first = 1;
    hit.m_cancelsOnBlock.first.second = 9;
    hit.m_cancelsOnBlock.second = {};

    return hit;
}

TimelineProperty<bool> hitutils::getRegularCounterTimeline(const ActiveFramesVec &hits_)
{
    int max = 0;
    for (const auto &el : hits_)
    {
        if (el.first.second > max)
            max = el.first.second;
    }

    return TimelineProperty<bool>({{0, true}, {max, false}});
}

Vector2<float> hitutils::getHitPosition(const std::vector<std::pair<FrameWindow, Collider>> &hits_, const HurtboxVec &hurtboxes_)
{
    if (hits_.empty() || hurtboxes_.empty())
        return {0, 0};

    Vector2<float> hitTL = {hits_[0].second.x, hits_[0].second.y};
    Vector2<float> hitBR = {hits_[0].second.x + hits_[0].second.w, hits_[0].second.y + hits_[0].second.h};
    for (const auto &hit : hits_)
    {
        if (hit.second.x < hitTL.x)
            hitTL.x = hit.second.x;

        if (hit.second.y < hitTL.y)
            hitTL.y = hit.second.y;

        if (hit.second.x + hit.second.w > hitBR.x)
            hitBR.x = hit.second.x + hit.second.w;

        if (hit.second.y + hit.second.h > hitBR.y)
            hitBR.y = hit.second.y + hit.second.h;
    }
    Collider hitFullCollider = {hitTL.x, hitTL.y, hitBR.x - hitTL.x, hitBR.y - hitTL.y};

    Vector2<float> hurtTL = {hurtboxes_[0].x, hurtboxes_[0].y};
    Vector2<float> hurtBR = {hurtboxes_[0].x + hurtboxes_[0].w, hurtboxes_[0].y + hurtboxes_[0].h};
    for (const auto &hurtbox : hurtboxes_)
    {
        if (hurtbox.x < hurtTL.x)
            hurtTL.x = hurtbox.x;

        if (hurtbox.y < hurtTL.y)
            hurtTL.y = hurtbox.y;

        if (hurtbox.x + hurtbox.w > hurtBR.x)
            hurtBR.x = hurtbox.x + hurtbox.w;

        if (hurtbox.y + hurtbox.h > hurtBR.y)
            hurtBR.y = hurtbox.y + hurtbox.h;
    }
    Collider hurtFullCollider = {hurtTL.x, hurtTL.y, hurtBR.x - hurtTL.x, hurtBR.y - hurtTL.y};

    return hitFullCollider.getOverlapArea(hurtFullCollider).getCenter();
}

std::pair<bool, Vector2<float>> hitutils::checkCollision(const Hit &hit1_, const Hit &hit2_)
{
    auto hbs1 = hit1_.getHitboxes();
    auto hbs2 = hit2_.getHitboxes();

    for (auto &hb1 : hbs1)
    {
        for (auto &hb2 : hbs2)
        {
            if (hb1.second.isCollideWith(hb2.second))
            {
                auto pos = hb1.second.getOverlapArea(hb2.second).getCenter();
                return {true, pos};
            }
        }
    }

    return {false, {0.0f, 0.0f}};
}

std::pair<bool, Vector2<float>> hitutils::checkCollision(const Hit &hit_, const HurtboxVec &hurtboxes_)
{
    auto hbs = hit_.getHitboxes();

    for (auto &hb1 : hbs)
    {
        for (auto &hb2 : hurtboxes_)
        {
            if (hb1.second.isCollideWith(hb2))
            {
                auto pos = getHitPosition(hbs, hurtboxes_);
                return {true, pos};
            }
        }
    }

    return {false, {0.0f, 0.0f}};
}

void HitData::initializeHitID()
{
    setHitID(nextHitID++);
}

void HitData::setHitID(int id_)
{
    m_hitId = id_;
    std::cout << "Initialized hit with ID " << id_ << std::endl;
}