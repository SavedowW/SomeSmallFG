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
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.hitProps.hitstop = 10;
    hdata.hitProps.hitstun = 12;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.hardKnd = true;
    hdata.hitProps.groundBounce = true;
    hdata.hitProps.groundBounceStrength = 5.0f;
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;

    hdata.hitBlockShakeAmp = 35;

    hdata.damage = 50.0f / hdata.hitProps.proratio;

    return hdata;
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_jab(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.5f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_scale = 0.4f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 110.0f;

    hdata.hitProps.hitstop = 10;
    hdata.hitProps.hitstun = 12;
    hdata.hitProps.proratio = 0.8f;
    hdata.hitProps.opponentImpulseOnHit = {90.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::HIGH;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 2;
    hdata.chProps.hitstop += 3;
    hdata.chProps.proratio = 0.9;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 7;

    hdata.hitBlockShakeAmp = 0;

    hdata.damage = 5.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{5, 7}, {50.0f, -300.0f, 120.0f, 30.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 8;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    return {{5, 7}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveB(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 80.0f;

    hdata.hitProps.hitstop = 11;
    hdata.hitProps.hitstun = 15;
    hdata.hitProps.proratio = 0.9f;
    hdata.hitProps.opponentImpulseOnHit = {50.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {10.0f, -7.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 3;
    hdata.chProps.hitstop += 4;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 13;

    hdata.hitBlockShakeAmp = 20;

    hdata.damage = 15.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{7, 10}, {50.0f, -265.0f, 192.0f, 70.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 8;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::STEP};

    return {{7, 10}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveC(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 100.0f;

    hdata.hitProps.hitstop = 13;
    hdata.hitProps.hitstun = 20;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.opponentImpulseOnHit = {80.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {5.0f, -15.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 4;
    hdata.chProps.hitstop += 5;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 16;
    hdata.chipDamage = 0.0f;

    hdata.hitBlockShakeAmp = 40;

    hdata.damage = 30.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{11, 16}, {60.0f, -250.0f, 90.0f, 150.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 10;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 10;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    return {{11, 16}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveS(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 100.0f;

    hdata.hitProps.hitstop = 13;
    hdata.hitProps.wallBounce = true;
    hdata.hitProps.wallbounceInertiaMultiplierX = 1.0f;
    hdata.hitProps.wallbounceInertiaMultiplierY = 0.5f;
    hdata.hitProps.hitstun = 28;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.opponentImpulseOnHit = {120.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {20.0f, -15.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 4;
    hdata.chProps.hitstop += 5;
    hdata.chProps.groundBounce = true;
    hdata.chProps.opponentImpulseOnHit = {15.0f, 10.0f};
    hdata.chProps.groundBounceStrength = 25.0f;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 16;
    hdata.chipDamage = 0.0f;

    hdata.hitBlockShakeAmp = 40;

    hdata.damage = 50.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{8, 13}, {90.0f, -350.0f, 150.0f, 100.0f}},
                    {{12, 13}, {120.0f, -250.0f, 120.0f, 100.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 10;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 10;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    return {{8, 14}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveStepC(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 150.0f;

    hdata.hitProps.hitstop = 13;
    hdata.hitProps.hitstun = 20;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.opponentImpulseOnHit = {5.0f, -30.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {5.0f, -30.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 4;
    hdata.chProps.hitstop += 5;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 16;
    hdata.chipDamage = 0.0f;

    hdata.hitBlockShakeAmp = 40;

    hdata.damage = 40.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{6, 13}, {60.0f, -450.0f, 200.0f, 400.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 20;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 20;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR};

    return {{6, 13}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_move2B(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 60.0f;

    hdata.hitProps.hitstop = 11;
    hdata.hitProps.hitstun = 12;
    hdata.hitProps.proratio = 0.7f;
    hdata.hitProps.opponentImpulseOnHit = {40.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {10.0f, -5.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::LOW;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
    
    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 3;
    hdata.chProps.hitstop += 4;

    hdata.canBeBlockedAs = {BLOCK_STATE::LOW, BLOCK_STATE::AIR};

    hdata.blockstun = 10;

    hdata.hitBlockShakeAmp = 15;

    hdata.damage = 13.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{10, 12}, {10.0f, -100.0f, 110.0f, 100.0f}}, {{10, 12}, {120.0f, -40.0f, 70.0f, 40.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 15;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 15;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_S_STARTUP, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    return {{10, 12}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_move4A(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 120.0f;
    hdata.forcedClash = true;

    hdata.hitProps.hitstop = 11;
    hdata.hitProps.hitstun = 15;
    hdata.hitProps.proratio = 0.9f;
    hdata.hitProps.opponentImpulseOnHit = {100.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {8.0f, -15.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 3;
    hdata.chProps.hitstop += 4;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 13;

    hdata.hitBlockShakeAmp = 20;

    hdata.damage = 15.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{8, 11}, {60.0f, -420.0f, 80.0f, 170.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 8;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

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
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_angle = 0.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 110.0f;

    hdata.hitProps.hitstop = 10;
    hdata.hitProps.hitstun = 12;
    hdata.hitProps.proratio = 0.7f;
    hdata.hitProps.opponentImpulseOnHit = {90.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::HIGH;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 2;
    hdata.chProps.hitstop += 3;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::AIR};

    hdata.blockstun = 9;

    hdata.hitBlockShakeAmp = 0;

    hdata.damage = 5.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{6, 8}, {50.0f, -280.0f, 80.0f, 80.0f}}, {{6, 8}, {110.0f, -220.0f, 50.0f, 50.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 9;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JA, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 9;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::JUMP, (int)CHAR1_STATE::MOVE_JA, (int)CHAR1_STATE::MOVE_JC, (int)CHAR1_STATE::AIR_DASH};

    return {{6, 8}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JC(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.9f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_angle = -60.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 15;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 150.0f;
    
    hdata.hitProps.hitstun = 12;
    hdata.hitProps.hitstop = 10;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.opponentImpulseOnHit = {18.0f, -40.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {18.0f, -40.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::HIGH;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
    hdata.hitProps.wallBounce = true;
    hdata.hitProps.wallbounceInertiaMultiplierX = 1.0f;
    hdata.hitProps.wallbounceInertiaMultiplierY = 0.5f;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 4;
    hdata.chProps.hitstop += 5;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AUTO, BLOCK_STATE::AIR};

    hdata.blockstun = 15;
    hdata.chipDamage = 2.0f;

    hdata.hitBlockShakeAmp = 15;

    hdata.damage = 25.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{12, 17}, {0.0f, -280.0f, 180.0f, 130.0f}}, {{12, 17}, {-30.0f, -160.0f, 130.0f, 130.0f}}, {{12, 17}, {100.0f, -385.0f, 40.0f, 300.0f}}, {{12, 17}, {-30.0f, -440.0f, 130.0f, 160.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 9;
    hit.cancelsOnHit.second = {};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 9;
    hit.cancelsOnBlock.second = {};

    return {{12, 17}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_214C(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_angle = 90.0f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 20;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 50.0f;

    hdata.hitProps.hitstun = 28;
    hdata.hitProps.hitstop = 11;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.opponentImpulseOnHit = {10.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {10.0f, 40.0f};
    hdata.hitProps.hardKnd = true;
    hdata.hitProps.groundBounce = true;
    hdata.hitProps.forceCrouch = true;
    hdata.hitProps.groundBounceStrength = 15.0f;
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 5;
    hdata.chProps.hitstop += 6;
    hdata.chProps.opponentImpulseOnHit = {3.0f, 10.0f};
    hdata.chProps.groundBounceStrength = 25.0f;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AUTO};

    hdata.blockstun = 20;
    hdata.chipDamage = 4.0f;

    hdata.hitBlockShakeAmp = 20;

    hdata.damage = 40.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{16, 17}, {50.0f, -400.0f, 100.0f, 150.0f}}, {{18, 21}, {50.0f, -400.0f, 130.0f, 400.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 8;
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::STEP};

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
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_angle = -90.0;
    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 15;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 50.0f;

    hdata.hitProps.hitstop = 13;
    hdata.hitProps.hitstun = 21;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.opponentImpulseOnHit = {5.0f, -40.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {5.0f, -30.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 4;
    hdata.chProps.hitstop += 5;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 16;
    hdata.chipDamage = 2.0f;

    hdata.hitBlockShakeAmp = 40;

    hdata.damage = 20.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{13, 16}, {60.0f, -400.0f, 80.0f, 220.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 1;
    hit.cancelsOnHit.second = {};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 1;
    hit.cancelsOnBlock.second = {};

    return {{13, 16}, hit};
}

Hit hitgeneration::generate_char1_projectile_hit(int animIdHit_, int animIdBlock_)
{
    HitData hdata;

    ParticlesSpawnData pdatatemp;

    pdatatemp.m_animation = animIdBlock_;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_animation = animIdHit_;
    pdatatemp.m_minLifeTime = pdatatemp.m_maxLifeTime = 10;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.opponentPushbackOnBlock = 80.0f;

    hdata.hitProps.hitstop = 11;
    hdata.hitProps.hitstun = 15;
    hdata.hitProps.proratio = 0.9f;
    hdata.hitProps.opponentImpulseOnHit = {50.0f, 0.0f};
    hdata.hitProps.opponentImpulseOnAirHit = {10.0f, -7.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::MID;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;
    hdata.chProps.hitstun += 3;
    hdata.chProps.hitstop += 4;

    hdata.canBeBlockedAs = {BLOCK_STATE::HIGH, BLOCK_STATE::LOW, BLOCK_STATE::AIR, BLOCK_STATE::AUTO};

    hdata.blockstun = 13;

    hdata.hitBlockShakeAmp = 20;

    hdata.damage = 15.0f / hdata.hitProps.proratio;

    Hit hit(hdata, {{{1, 100}, {-50.0f, -400.0f, 130.0f, 400.0f}}});

    hit.cancelsOnHit.first.first = 1;
    hit.cancelsOnHit.first.second = 9;
    hit.cancelsOnHit.second = {};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 9;
    hit.cancelsOnBlock.second = {};

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