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

HitData hitgeneration::generate_char1_normal_throw()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    pdatatemp.m_scale = 0.6f;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.partOfThrow = true;
    hdata.m_hitId = 1;

    //hdata.cornerPushbackMaxRange = 390.0f;
    //hdata.cornerPushbackMinImpulse = 8.0f;
    //hdata.cornerPushbackMaxImpulse = 25.0f;
    //hdata.opponentPushbackOnBlock = 7.0f;

    hdata.hitProps.hitstop = 10;
    hdata.hitProps.hitstun = 12;
    hdata.hitProps.proratio = 1.0f;
    hdata.hitProps.hardKnd = true;
    hdata.hitProps.groundBounce = true;
    hdata.hitProps.groundBounceStrength = 5.0f;
    //hdata.hitProps.opponentImpulseOnHit = {6.0f, 0.0f};
    //hdata.hitProps.opponentImpulseOnAirHit = {8.0f, -5.0f};
    hdata.hitProps.groundHitstunAnimation = HITSTUN_ANIMATION::FLOAT;
    hdata.hitProps.airHitstunAnimation = HITSTUN_ANIMATION::FLOAT;

    hdata.chProps = hdata.hitProps;

    hdata.hitBlockShakeAmp = 35;

    hdata.damage = 50.0f / hdata.hitProps.proratio;

    return hdata;
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_jab()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.5f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    pdatatemp.m_scale = 0.4f;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);


    hdata.m_hitId = 1;

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
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_A, (int)CHAR1_STATE::MOVE_B, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    return {{5, 7}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveB()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.6f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 8;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_2B, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::STEP};

    return {{7, 10}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveC()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.8f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 10;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::STEP};

    return {{11, 16}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_moveStepC()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.8f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_move2B()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.6f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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
    hit.cancelsOnHit.second = {(int)CHAR1_STATE::PREJUMP, (int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    hit.cancelsOnBlock.first.first = 1;
    hit.cancelsOnBlock.first.second = 15;
    hit.cancelsOnBlock.second = {(int)CHAR1_STATE::MOVE_C, (int)CHAR1_STATE::MOVE_4A, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::MOVE_236C, (int)CHAR1_STATE::PROJECTILE_CHAR, (int)CHAR1_STATE::MOVE_214C, (int)CHAR1_STATE::STEP};

    return {{10, 12}, hit};
}

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_move4A()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.6f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JA()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.6f;
    pdatatemp.m_angle = 45.0f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_1;
    pdatatemp.m_angle = 0.0f;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

    hdata.m_hitId = 1;

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

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_JC()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.9f;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_2;
    pdatatemp.m_angle = -60.0f;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_214C()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_2_SLOWED;
    pdatatemp.m_angle = 90.0f;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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

std::pair<FrameWindow, Hit> hitgeneration::generate_char1_236C()
{
    HitData hdata;

    HitParticleData pdatatemp;

    pdatatemp.m_partType = PARTICLE_TYPES::BLOCK;
    pdatatemp.m_scale = 0.8f;
    pdatatemp.m_angle = -45.0;
    hdata.particlesOnBlock.push_back(pdatatemp);

    pdatatemp.m_partType = PARTICLE_TYPES::HIT_2;
    pdatatemp.m_angle = -90.0;
    hdata.particlesOnHit.push_back(pdatatemp);
    hdata.particlesOnCH.push_back(pdatatemp);

    hdata.m_hitId = 1;

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