#include "Application.h"
#include "Char1.h"
#include <memory>
#include "Actions.h"
#include <stdexcept>

void Char1::provideActions(Application &application_)
{
    AnimationManager &animmgmgt = *application_.getAnimationManager();

    m_actionResolver.addAction(std::make_unique<Action_char1_back_air_throw_startup>(animmgmgt.getAnimID("Char1/ThrowStartup")));
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw_startup>(animmgmgt.getAnimID("Char1/ThrowStartup")));
    m_actionResolver.addAction(std::make_unique<Action_char1_back_throw_startup>(animmgmgt.getAnimID("Char1/ThrowStartup")));
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw_startup>(animmgmgt.getAnimID("Char1/ThrowStartup")));
    m_actionResolver.addAction(std::make_unique<Action_char1_air_throw_tech>(animmgmgt.getAnimID("Char1/ThrowTech")));
    m_actionResolver.addAction(std::make_unique<Action_char1_throw_tech>(animmgmgt.getAnimID("Char1/ThrowTech")));
    m_actionResolver.addAction(std::make_unique<Action_char1_step>(animmgmgt.getAnimID("Char1/Step")));
    m_actionResolver.addAction(std::make_unique<Action_char1_air_dash>(animmgmgt.getAnimID("Char1/Airdash")));
    m_actionResolver.addAction(std::make_unique<Action_char1_air_backdash>(animmgmgt.getAnimID("Char1/AirBackdash")));
    m_actionResolver.addAction(std::make_unique<Action_char1_backward_doublejump>(animmgmgt.getAnimID("Char1/Jump")));
    m_actionResolver.addAction(std::make_unique<Action_char1_forward_doublejump>(animmgmgt.getAnimID("Char1/Jump")));
    m_actionResolver.addAction(std::make_unique<Action_char1_neutral_doublejump>(animmgmgt.getAnimID("Char1/Jump")));
    m_actionResolver.addAction(std::make_unique<Action_char1_move_JC>(animmgmgt.getAnimID("Char1/MoveJC"), animmgmgt.getAnimID("Particles/Hit2"), animmgmgt.getAnimID("Particles/Block")));

    // j.A
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_JA, std::make_unique<InputComparatorAPress>(), 17,
    {
        hitgeneration::generate_char1_JA(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty<bool>({{1, true}, {18, false}}),
            {-70, -350, 140, 300}
        },
        {
            TimelineProperty<bool>({{5, true}, {12, false}}),
            {40.0f, -290.0f, 100.0f, 100.0f}
        }
    }, animmgmgt.getAnimID("Char1/MoveJA"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}), false, true))->setSwitchData(false, 17, false, true, true, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})->setOutdatedTransition((int)CHAR1_STATE::FLOAT)));

    m_actionResolver.addAction(std::make_unique<Action_char1_move_214C>(animmgmgt.getAnimID("Char1/Move214C"), animmgmgt.getAnimID("Particles/Hit2_Slowed"), animmgmgt.getAnimID("Particles/Block")));
    m_actionResolver.addAction(std::make_unique<Action_char1_move_projectile>(animmgmgt.getAnimID("Char1/MoveProjectileCharAnim")));

    //4A
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_4A, std::make_unique<InputComparator4APress>(), 24,
    {
        hitgeneration::generate_char1_move4A(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        },
        {
            TimelineProperty<bool>({{8, true}, {18, false}}),
            {60.0f, -420.0f, 60.0f, 150.0f}
        }
    }, animmgmgt.getAnimID("Char1/Move4A"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}), false, false))->setOutdatedTransition((int)CHAR1_STATE::IDLE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})));

    // 2B
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_2B, std::make_unique<InputComparator2BPress>(), 29,
    {
        hitgeneration::generate_char1_move2B(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty<bool>({{1, true}, {30, false}}),
            {-70, -200, 140, 200}
        },
        {
            TimelineProperty<bool>({{9, true}, {22, false}}),
            {10.0f, -110.0f, 200.0f, 110.0f}
        }
    }, animmgmgt.getAnimID("Char1/Move2B"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    true, false))->setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, {23.0f, 0.0f}},
                {5, {7.0f, 0.0f}},
                {9, {0.0f, 0.0f}},
                {22, {-7.0f, 0.0f}},
                {26, {-23.0f, 0.0f}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    )->setOutdatedTransition((int)CHAR1_STATE::IDLE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})));

    // 236C
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_236C, std::make_unique<InputComparator236CPress>(), 40,
    {
        hitgeneration::generate_char1_236C(animmgmgt.getAnimID("Particles/Hit2"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty<bool>({{1, true}, {41, false}}),
            {-70, -200, 140, 200}
        },
        {
            TimelineProperty<bool>({{1, true}, {5, false}}),
            {-70.0f, -300.0f, 140.0f, 100.0f}
        },
        {
            TimelineProperty<bool>({{13, true}, {41, false}}),
            {-70.0f, -300.0f, 140.0f, 100.0f}
        },
        {
            TimelineProperty<bool>({{17, true}, {34, false}}),
            {60.0f, -350.0f, 40.0f, 150.0f}
        },
    }, animmgmgt.getAnimID("Char1/Move236C"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false))->setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, {20.0f, 0.0f}},
                {6, {15.0f, 0.0f}},
                {9, {10.0f, 0.0f}},
                {12, {6.0f, 0.0f}},
                {13, {0, 0}},
                {30, {-1.5f, 0.0f}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    )->setOutdatedTransition((int)CHAR1_STATE::IDLE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})));

    // s.5C
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_STEP_C, std::make_unique<InputComparatorCPress>(), 53,
    {
        hitgeneration::generate_char1_moveStepC(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        },
        {
            TimelineProperty<bool>({{6, true}, {32, false}}),
            {60.0f, -450.0f, 200.0f, 400.0f}
        }
    }, animmgmgt.getAnimID("Char1/MoveStepC"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::STEP_RECOVERY}),
    false, false))->setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, {30.0f, 0.0f}},
                {4, {0, 0}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    )->setOutdatedTransition((int)CHAR1_STATE::IDLE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})));

    // 5C
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_C, std::make_unique<InputComparatorCPress>(), 26,
    {
        hitgeneration::generate_char1_moveC(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        },
        {
            TimelineProperty<bool>({{11, true}, {23, false}}),
            {60.0f, -220.0f, 60.0f, 100.0f}
        }
    }, animmgmgt.getAnimID("Char1/MoveC"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false))->setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {3, {20.0f, 0.0f}},
                {6, {0, 0}},
                {22, {20.0f, 0.0f}},
                {27, {0, 0}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    )->setOutdatedTransition((int)CHAR1_STATE::IDLE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})));

    // 5B
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_B, std::make_unique<InputComparatorBPress>(), 22,
    {
        hitgeneration::generate_char1_moveB(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        },
        {
            TimelineProperty<bool>({{7, true}, {20, false}}),
            {50.0f, -275.0f, 175.0f, 80.0f}
        }
    }, animmgmgt.getAnimID("Char1/MoveB"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false))->setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, {3.5f, 0.0f}},
                {7, {0, 0}},
                {20, {-2.0f, 0.0f}},
                {23, {0, 0}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    )->setOutdatedTransition((int)CHAR1_STATE::IDLE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})));

    // 5S startup
    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter((int)CHAR1_STATE::MOVE_S_STARTUP, std::make_unique<InputComparatorSPress>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, animmgmgt.getAnimID("Char1/MoveSStartup"), TimelineProperty(true), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false, false, false, false, false, false, false))->setSwitchData(
        false, 20, true, true, true, false, false, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}
    )->setOutdatedTransition((int)CHAR1_STATE::MOVE_S_CHARGE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})->setUpdateMovementData(
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Vel mul
        TimelineProperty<Vector2<float>>({1.0f, 1.0f}), // Inr mul
        TimelineProperty<Vector2<float>>(
            {
                {1, {-4.0f, 0.0f}},
                {3, {-8.0f, 0.0f}},
                {7, {-10.0f, 0}},
                {14, {-4.0f, 0.0f}},
                {17, {-2, 0}}
            }),  // Dir vel mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}),  // Dir inr mul
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}), // Raw vel
        TimelineProperty<Vector2<float>>({0.0f, 0.0f}) // Raw inr
    )));

    // 5S startup
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_prolonged((int)CHAR1_STATE::MOVE_S_CHARGE, std::make_unique<InputComparatorIdle>(), std::make_unique<InputComparatorSHold>(),
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, animmgmgt.getAnimID("Char1/MoveSCharge"), TimelineProperty(true), TimelineProperty(true), TimelineProperty(false),
    StateMarker(gamedata::characters::totalStateCount, {}),
    false, 0, 0, false, false, false))));

    // 5A
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_attack((int)CHAR1_STATE::MOVE_A, std::make_unique<InputComparatorAPress>(), 16,
    {
        hitgeneration::generate_char1_jab(animmgmgt.getAnimID("Particles/Hit1"), animmgmgt.getAnimID("Particles/Block"))
    },
    {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        },
        {
            TimelineProperty<bool>({{4, true}, {13, false}}),
            {40.0f, -310.0f, 140.0f, 50.0f}
        }
    }, animmgmgt.getAnimID("Char1/Jab"), TimelineProperty(true),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false))->setOutdatedTransition((int)CHAR1_STATE::IDLE)->setOutdatedMovementData({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f})));

    m_actionResolver.addAction(std::make_unique<Action_char1_ground_backdash>(animmgmgt.getAnimID("Char1/Backdash")));
    m_actionResolver.addAction(std::make_unique<Action_char1_ground_dash>(animmgmgt.getAnimID("Char1/GroundDash")));
    m_actionResolver.addAction(std::make_unique<Action_char1_backward_jump>(animmgmgt.getAnimID("Char1/Prejump")));
    m_actionResolver.addAction(std::make_unique<Action_char1_forward_jump>(animmgmgt.getAnimID("Char1/Prejump")));
    m_actionResolver.addAction(std::make_unique<Action_char1_neutral_jump>(animmgmgt.getAnimID("Char1/Prejump")));
    m_actionResolver.addAction(std::make_unique<Action_char1_walk_bwd>(animmgmgt.getAnimID("Char1/WalkBackward")));
    m_actionResolver.addAction(std::make_unique<Action_char1_walk_fwd>(animmgmgt.getAnimID("Char1/WalkForward")));
    m_actionResolver.addAction(std::make_unique<Action_char1_ground_dash_recovery>(animmgmgt.getAnimID("Char1/GroundDashRecovery")));
    m_actionResolver.addAction(std::make_unique<Action_char1_crouch>(animmgmgt.getAnimID("Char1/CrouchIdle")));
    m_actionResolver.addAction(std::make_unique<Action_char1_idle>(animmgmgt.getAnimID("Char1/Idle")));
    m_actionResolver.addAction(std::make_unique<Action_char1_soft_landing_recovery>(animmgmgt.getAnimID("Char1/LandingRecovery")));
    m_actionResolver.addAction(std::make_unique<Action_char1_vulnerable_landing_recovery>(animmgmgt.getAnimID("Char1/LandingRecovery")));
    m_actionResolver.addAction(std::make_unique<Action_char1_hard_landing_recovery>(animmgmgt.getAnimID("Char1/LandingRecovery")));
    m_actionResolver.addAction(std::make_unique<Action_char1_jc_landing_recovery>(animmgmgt.getAnimID("Char1/JCLandingRecovery")));
    m_actionResolver.addAction(std::make_unique<Action_char1_hard_knockdown>(animmgmgt.getAnimID("Char1/Knockdown")));
    m_actionResolver.addAction(std::make_unique<Action_char1_soft_knockdown>(animmgmgt.getAnimID("Char1/SoftKnockdown")));
    m_actionResolver.addAction(std::make_unique<Action_char1_knockdown_recovery>(animmgmgt.getAnimID("Char1/KnockdownRecovery")));
    
    // Float state
    m_actionResolver.addAction(std::unique_ptr<Action>((new Action_float((int)CHAR1_STATE::FLOAT, (int)CHAR1_STATE::JUMP, {
        {
            TimelineProperty(true),
            {-70, -350, 140, 300}
        }
    }, animmgmgt.getAnimID("Char1/Jump")))->setAnimResetData(20, 1)));


    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::HITSTUN_HIGH, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            {-100, -375, 200, 375}
        }
    }, animmgmgt.getAnimID("Char1/HighHitstun"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false, 0, 0, false, false, false ))->setHitstunAnimation((int)HITSTUN_ANIMATION::HIGH)->setDisadvantageFlags(false, true, false)->setOutdatedTransition((int)CHAR1_STATE::IDLE)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::HITSTUN_MID, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            {-100, -375, 200, 375}
        }
    }, animmgmgt.getAnimID("Char1/MidHitstun"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false, 0, 0, false, false, false ))->setHitstunAnimation((int)HITSTUN_ANIMATION::MID)->setDisadvantageFlags(false, true, false)->setOutdatedTransition((int)CHAR1_STATE::IDLE)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::HITSTUN_LOW, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            {-100, -375, 200, 375}
        }
    }, animmgmgt.getAnimID("Char1/LowHitstun"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false, 0, 0, false, false, false ))->setHitstunAnimation((int)HITSTUN_ANIMATION::LOW)->setDisadvantageFlags(false, true, false)->setOutdatedTransition((int)CHAR1_STATE::IDLE)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::HITSTUN_CROUCH, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            {-100, -200, 200, 200}
        }
    }, animmgmgt.getAnimID("Char1/CrouchHitstun"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}),
    false, true, false, 0, 0, false, false, false ))->setHitstunAnimation((int)HITSTUN_ANIMATION::CROUCH)->setDisadvantageFlags(false, true, false)->setOutdatedTransition((int)CHAR1_STATE::IDLE)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::HITSTUN_FLOAT, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::airHitstunHurtbox
        }
    }, animmgmgt.getAnimID("Char1/AirHitstun"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false, 0, 0, false, false, true ))->setHitstunAnimation((int)HITSTUN_ANIMATION::FLOAT)->setDisadvantageFlags(false, true, false)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::HITSTUN_GROUND_BOUNCE, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::airHitstunHurtbox
        }
    }, animmgmgt.getAnimID("Char1/AirHitstun"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(false), StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false, 0, 0, false, false, true ))->setHitstunAnimation((int)HITSTUN_ANIMATION::FLOAT)->setDisadvantageFlags(false, true, false)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::BLOCKSTUN_STANDING, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, animmgmgt.getAnimID("Char1/StandingBlock"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false, 0, 0, false, false, false ))->setDisadvantageFlags(true, false, false)->setOutdatedTransition((int)CHAR1_STATE::IDLE)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::BLOCKSTUN_CROUCHING, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::crouchingHurtbox
        }
    }, animmgmgt.getAnimID("Char1/CrouchBlock"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), StateMarker(gamedata::characters::totalStateCount, {}),
    false, true, false, 0, 0, false, false, false ))->setDisadvantageFlags(true, false, false)->setOutdatedTransition((int)CHAR1_STATE::IDLE)));

    m_actionResolver.addAction(std::unique_ptr<Action>((new ActionCharacter( (int)CHAR1_STATE::BLOCKSTUN_AIR, std::make_unique<InputComparatorIdle>(), {
        {
            TimelineProperty(true),
            gamedata::characters::char1::standingHurtbox
        }
    }, animmgmgt.getAnimID("Char1/StandingBlock"), TimelineProperty(false), TimelineProperty(true), TimelineProperty(true), StateMarker(gamedata::characters::totalStateCount, {}),
    false, false, false, 0, 0, false, false, true ))->setDisadvantageFlags(true, false, false)->setLandingRecoveryState((int)CHAR1_STATE::HARD_LANDING_RECOVERY)->setOutdatedTransition((int)CHAR1_STATE::FLOAT)));


    m_actionResolver.addAction(std::make_unique<Action_char1_air_dash_extention>(animmgmgt.getAnimID("Char1/Airdash")));
    m_actionResolver.addAction(std::make_unique<Action_char1_step_recovery>(animmgmgt.getAnimID("Char1/StepRecovery")));

    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw_hold>(animmgmgt.getAnimID("Char1/ThrowStartup")));
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw_whiff>(animmgmgt.getAnimID("Char1/ThrowWhiff")));
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw>(animmgmgt.getAnimID("Char1/Throw"), animmgmgt.getAnimID("Particles/Hit1")));

    m_actionResolver.addAction(std::make_unique<Action_char1_back_throw_hold>(animmgmgt.getAnimID("Char1/ThrowStartup")));

    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal_hold>(animmgmgt.getAnimID("Char1/ThrownChar1Hold")));
    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal>(animmgmgt.getAnimID("Char1/ThrownChar1")));

    m_actionResolver.addAction(std::make_unique<Action_char1_throw_tech_char1>(animmgmgt.getAnimID("Char1/ThrowTech")));


    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw_hold>(animmgmgt.getAnimID("Char1/ThrowStartup")));
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw_whiff>(animmgmgt.getAnimID("Char1/ThrowWhiff")));
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw>(animmgmgt.getAnimID("Char1/Throw"), animmgmgt.getAnimID("Particles/Hit1")));

    m_actionResolver.addAction(std::make_unique<Action_char1_back_air_throw_hold>(animmgmgt.getAnimID("Char1/ThrowStartup")));

    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal_air_hold>(animmgmgt.getAnimID("Char1/ThrownChar1Hold")));
    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal_air>(animmgmgt.getAnimID("Char1/ThrownChar1")));

    m_actionResolver.addAction(std::make_unique<Action_char1_air_throw_tech_char1>(animmgmgt.getAnimID("Char1/ThrowTech")));
}

Char1::Char1(Application &application_, Vector2<float> pos_, Camera *cam_, ParticleManager *particleManager_, ProjectileManager *ptManager_) :
    Character(application_, pos_, 400.0f, gamedata::characters::char1::gravity, cam_, particleManager_, 1, 1, 6, 5, StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY}), 1000, ptManager_)
{

    provideActions(application_);
    m_genericCharacterData.m_idle = (int)CHAR1_STATE::IDLE;
    m_genericCharacterData.m_float = (int)CHAR1_STATE::FLOAT;

    m_genericCharacterData.m_dontConvertVelocityToInertia.toggleMark((int)CHAR1_STATE::WALK_FWD);
    m_genericCharacterData.m_dontConvertVelocityToInertia.toggleMark((int)CHAR1_STATE::WALK_BWD);

    m_genericCharacterData.m_prejums.toggleMark((int)CHAR1_STATE::PREJUMP);

    m_genericCharacterData.m_noDrag.toggleMark((int)CHAR1_STATE::PREJUMP);
    m_genericCharacterData.m_noInertia.toggleMark((int)CHAR1_STATE::PREJUMP);

    m_genericCharacterData.m_hitstunAnimToStates[(int)HITSTUN_ANIMATION::CROUCH] = (int)CHAR1_STATE::HITSTUN_CROUCH;
    m_genericCharacterData.m_hitstunAnimToStates[(int)HITSTUN_ANIMATION::HIGH] = (int)CHAR1_STATE::HITSTUN_HIGH;
    m_genericCharacterData.m_hitstunAnimToStates[(int)HITSTUN_ANIMATION::MID] = (int)CHAR1_STATE::HITSTUN_MID;
    m_genericCharacterData.m_hitstunAnimToStates[(int)HITSTUN_ANIMATION::LOW] = (int)CHAR1_STATE::HITSTUN_LOW;
    m_genericCharacterData.m_hitstunAnimToStates[(int)HITSTUN_ANIMATION::FLOAT] = (int)CHAR1_STATE::HITSTUN_FLOAT;

    m_genericCharacterData.m_groundBounceHitstun = (int)CHAR1_STATE::HITSTUN_GROUND_BOUNCE;

    m_genericCharacterData.m_blockstunStanding = (int)CHAR1_STATE::BLOCKSTUN_STANDING;
    m_genericCharacterData.m_blockstunCrouching = (int)CHAR1_STATE::BLOCKSTUN_CROUCHING;
    m_genericCharacterData.m_blockstunAir = (int)CHAR1_STATE::BLOCKSTUN_AIR;

    m_genericCharacterData.m_softKD = (int)CHAR1_STATE::SOFT_KNOCKDOWN;
    m_genericCharacterData.m_hardKD = (int)CHAR1_STATE::HARD_KNOCKDOWN;
    m_genericCharacterData.m_softLandingRecovery = (int)CHAR1_STATE::SOFT_LANDING_RECOVERY;
    m_genericCharacterData.m_vulnerableLandingRecovery = (int)CHAR1_STATE::VULNERABLE_LANDING_RECOVERY;

    m_genericCharacterData.m_step = (int)CHAR1_STATE::STEP;
    m_genericCharacterData.m_airdash = (int)CHAR1_STATE::AIR_DASH;

    m_genericCharacterData.m_thrownStates[(int)THROW_LIST::CHAR1_NORMAL_THROW] = (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD;
    m_genericCharacterData.m_thrownStates[(int)THROW_LIST::CHAR1_AIR_THROW] = (int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD;

    m_genericCharacterData.m_throwTechedStates[(int)THROW_TECHS_LIST::CHAR1_GROUND] = (int)CHAR1_STATE::THROW_TECH_CHAR1;
    m_genericCharacterData.m_throwTechedStates[(int)THROW_TECHS_LIST::CHAR1_AIR] = (int)CHAR1_STATE::AIR_THROW_TECH_CHAR1;

    m_genericCharacterData.m_standingPushbox = gamedata::characters::char1::standingPushbox;
    m_genericCharacterData.m_crouchingPushbox = gamedata::characters::char1::crouchingPushbox;
    m_genericCharacterData.m_airPushbox = gamedata::characters::char1::airbornePushbox;
    m_genericCharacterData.m_airHitstunPushbox = gamedata::characters::char1::airborneHitstunPushbox;

    m_genericCharacterData.m_clashCancelOptions = {
        (int)CHAR1_STATE::PREJUMP,
        (int)CHAR1_STATE::GROUND_BACKDASH,
        (int)CHAR1_STATE::STEP,
        (int)CHAR1_STATE::JUMP,
        (int)CHAR1_STATE::AIR_DASH,
        (int)CHAR1_STATE::AIR_BACKDASH,
        (int)CHAR1_STATE::MOVE_A,
        (int)CHAR1_STATE::MOVE_B,
        (int)CHAR1_STATE::MOVE_C,
        (int)CHAR1_STATE::MOVE_2B,
        (int)CHAR1_STATE::MOVE_4A,
        (int)CHAR1_STATE::MOVE_236C,
        (int)CHAR1_STATE::MOVE_214C,
        (int)CHAR1_STATE::MOVE_JA,
        (int)CHAR1_STATE::MOVE_JC,
        (int)CHAR1_STATE::THROW_NORMAL_STARTUP,
        (int)CHAR1_STATE::THROW_BACK_STARTUP,
        (int)CHAR1_STATE::THROW_NORMAL_AIR_STARTUP,
        (int)CHAR1_STATE::THROW_BACK_AIR_STARTUP
    };

    m_genericCharacterData.m_useDirToEnemyForInputs.toggleMark((int)CHAR1_STATE::SOFT_LANDING_RECOVERY);
    m_genericCharacterData.m_useDirToEnemyForInputs.toggleMark((int)CHAR1_STATE::GROUND_DASH_RECOVERY);
    
    m_inertiaDrag = gamedata::characters::char1::inertiaDrag;
}

void Char1::loadAnimations(Application &application_)
{
    AnimationManager animmgmgt = *application_.getAnimationManager();

    m_animations[animmgmgt.getAnimID("Char1/Idle")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Idle"), LOOPMETHOD::SWITCH_DIR_LOOP);
    m_animations[animmgmgt.getAnimID("Char1/CrouchIdle")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/CrouchIdle"), LOOPMETHOD::JUMP_LOOP);
    m_animations[animmgmgt.getAnimID("Char1/WalkForward")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/WalkForward"), LOOPMETHOD::JUMP_LOOP);
    m_animations[animmgmgt.getAnimID("Char1/WalkBackward")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/WalkBackward"), LOOPMETHOD::JUMP_LOOP, 65, -1);
    m_animations[animmgmgt.getAnimID("Char1/Prejump")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Prejump"));
    m_animations[animmgmgt.getAnimID("Char1/Jump")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Jump"));
    m_animations[animmgmgt.getAnimID("Char1/LandingRecovery")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/LandingRecovery"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/GroundDash")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/GroundDash"), LOOPMETHOD::JUMP_LOOP);
    m_animations[animmgmgt.getAnimID("Char1/GroundDashRecovery")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/GroundDashRecovery"));
    m_animations[animmgmgt.getAnimID("Char1/Backdash")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Backdash"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Airdash")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Airdash"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/AirBackdash")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/AirBackdash"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Jab")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Jab"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveB")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveB"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveC")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveC"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveStepC")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveStepC"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Move2B")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Move2B"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Move4A")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Move4A"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveJA")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveJA"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveJC")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveJC"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Move214C")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Move214C"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Move236C")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Move236C"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/JCLandingRecovery")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/JCLandingRecovery"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/LowHitstun")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/LowHitstun"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MidHitstun")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MidHitstun"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/HighHitstun")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/HighHitstun"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/AirHitstun")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/AirHitstun"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/CrouchHitstun")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/CrouchHitstun"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/StandingBlock")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/StandingBlock"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/CrouchBlock")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/CrouchBlock"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Knockdown")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Knockdown"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/SoftKnockdown")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/SoftKnockdown"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/KnockdownRecovery")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/KnockdownRecovery"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Step")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Step"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/StepRecovery")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/StepRecovery"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/ThrowStartup")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/ThrowStartup"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/Throw")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/Throw"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/ThrownChar1Hold")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/ThrownChar1Hold"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/ThrownChar1")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/ThrownChar1"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/ThrowTech")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/ThrowTech"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/ThrowWhiff")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/ThrowWhiff"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveProjectileCharAnim")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveProjectileCharAnim"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveSStartup")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveSStartup"), LOOPMETHOD::NOLOOP);
    m_animations[animmgmgt.getAnimID("Char1/MoveSCharge")] = std::make_unique<Animation>(animmgmgt, animmgmgt.getAnimID("Char1/MoveSCharge"), LOOPMETHOD::JUMP_LOOP);

    /*
    CHAR1_NORMAL_THROW_STARTUP,
	CHAR1_NORMAL_THROW,
	CHAR1_THROWN_CHAR1_NORMAL_HOLD,
	CHAR1_THROWN_CHAR1_NORMAL,
	CHAR1_THROW_TECH,
	CHAR1_NORMAL_THROW_WHIFF
    */

   application_.getAnimationManager()->preload("Char1/ProjectileFirewallStartup");
   application_.getAnimationManager()->preload("Char1/ProjectileFirewallFiller");
   application_.getAnimationManager()->preload("Char1/ProjectileFirewallFadeout");

    m_currentAnimation = m_animations[animmgmgt.getAnimID("Char1/Idle")].get();
    m_currentAnimation->reset();
}

std::string Char1::CharStateData() const
{
    std::string res;
    res += "P" + std::to_string(m_playerId) + ": ";

    std::string stateName = "";
    switch (m_currentState)
    {
        case ((int)CHAR1_STATE::IDLE):
            stateName = "IDLE";
            break;
        
        case ((int)CHAR1_STATE::CROUCH):
            stateName = "CROUCH";
            break;

        case ((int)CHAR1_STATE::HITSTUN_GROUND_BOUNCE):
            stateName = "HITSTUN_GROUND_BOUNCE";
            break;

        case ((int)CHAR1_STATE::HITSTUN_HIGH):
            stateName = "HITSTUN_HIGH";
            break;

        case ((int)CHAR1_STATE::HITSTUN_MID):
            stateName = "HITSTUN_MID";
            break;

        case ((int)CHAR1_STATE::HITSTUN_LOW):
            stateName = "HITSTUN_LOW";
            break;

        case ((int)CHAR1_STATE::HITSTUN_CROUCH):
            stateName = "HITSTUN_CROUCH";
            break;

        case ((int)CHAR1_STATE::HITSTUN_FLOAT):
            stateName = "HITSTUN_FLOAT";
            break;

        case ((int)CHAR1_STATE::WALK_FWD):
            stateName = "WALK_FWD";
            break;

        case ((int)CHAR1_STATE::WALK_BWD):
            stateName = "WALK_BWD";
            break;

        case ((int)CHAR1_STATE::PREJUMP):
            stateName = "PREJUMP";
            break;
        
        case ((int)CHAR1_STATE::JUMP):
            stateName = "JUMP";
            break;

        case ((int)CHAR1_STATE::GROUND_BACKDASH):
            stateName = "GROUND_BACKDASH";
            break;

        case ((int)CHAR1_STATE::SOFT_LANDING_RECOVERY):
            stateName = "SOFT_LANDING_RECOVERY";
            break;
        
        case ((int)CHAR1_STATE::VULNERABLE_LANDING_RECOVERY):
            stateName = "VULNERABLE_LANDING_RECOVERY";
            break;

        case ((int)CHAR1_STATE::HARD_LANDING_RECOVERY):
            stateName = "HARD_LANDING_RECOVERY";
            break;

        case ((int)CHAR1_STATE::GROUND_DASH):
            stateName = "GROUND_DASH";
            break;

        case ((int)CHAR1_STATE::GROUND_DASH_RECOVERY):
            stateName = "GROUND_DASH_RECOVERY";
            break;
        
        case ((int)CHAR1_STATE::MOVE_A):
            stateName = "MOVE_A";
            break;

        case ((int)CHAR1_STATE::MOVE_4A):
            stateName = "MOVE_4A";
            break;

        case ((int)CHAR1_STATE::MOVE_B):
            stateName = "MOVE_B";
            break;

        case ((int)CHAR1_STATE::MOVE_C):
            stateName = "MOVE_C";
            break;

        case ((int)CHAR1_STATE::MOVE_STEP_C):
            stateName = "MOVE_STEP_C";
            break;

        case ((int)CHAR1_STATE::MOVE_236C):
            stateName = "MOVE_236C";
            break;

        case ((int)CHAR1_STATE::MOVE_2B):
            stateName = "MOVE_2B";
            break;
        
        case ((int)CHAR1_STATE::BLOCKSTUN_STANDING):
            stateName = "BLOCKSTUN_STANDING";
            break;

        case ((int)CHAR1_STATE::BLOCKSTUN_CROUCHING):
            stateName = "BLOCKSTUN_CROUCHING";
            break;
        
        case ((int)CHAR1_STATE::BLOCKSTUN_AIR):
            stateName = "BLOCKSTUN_AIR";
            break;

        case ((int)CHAR1_STATE::HARD_KNOCKDOWN):
            stateName = "HARD_KNOCKDOWN";
            break;

        case ((int)CHAR1_STATE::KNOCKDOWN_RECOVERY):
            stateName = "KNOCKDOWN_RECOVERY";
            break;

        case ((int)CHAR1_STATE::AIR_DASH):
            stateName = "AIR_DASH";
            break;

        case ((int)CHAR1_STATE::AIR_DASH_EXTENTION):
            stateName = "AIR_DASH_EXTENTION";
            break;

        case ((int)CHAR1_STATE::AIR_BACKDASH):
            stateName = "AIR_BACKDASH";
            break;

        case ((int)CHAR1_STATE::MOVE_JA):
            stateName = "MOVE_JA";
            break;
        
        case ((int)CHAR1_STATE::MOVE_JC):
            stateName = "MOVE_JC";
            break;

        case ((int)CHAR1_STATE::MOVE_JC_LANDING_RECOVERY):
            stateName = "MOVE_JC_LANDING_RECOVERY";
            break;

        case ((int)CHAR1_STATE::MOVE_214C):
            stateName = "MOVE_214C";
            break;

        case ((int)CHAR1_STATE::STEP):
            stateName = "STEP";
            break;

        case ((int)CHAR1_STATE::STEP_RECOVERY):
            stateName = "STEP_RECOVERY";
            break;

        case ((int)CHAR1_STATE::THROW_BACK_AIR_HOLD):
            stateName = "THROW_BACK_AIR_HOLD";
            break;

        case ((int)CHAR1_STATE::THROW_BACK_AIR_STARTUP):
            stateName = "THROW_BACK_AIR_STARTUP";
            break;

        case ((int)CHAR1_STATE::THROW_BACK_HOLD):
            stateName = "THROW_BACK_HOLD";
            break;

        case ((int)CHAR1_STATE::THROW_BACK_STARTUP):
            stateName = "THROW_BACK_STARTUP";
            break;

        case ((int)CHAR1_STATE::THROW_NORMAL_AIR_ANIM):
            stateName = "THROW_NORMAL_AIR_ANIM";
            break;

        case ((int)CHAR1_STATE::THROW_NORMAL_AIR_HOLD):
            stateName = "THROW_NORMAL_AIR_HOLD";
            break;

        case ((int)CHAR1_STATE::THROW_NORMAL_AIR_STARTUP):
            stateName = "THROW_NORMAL_AIR_STARTUP";
            break;

        case ((int)CHAR1_STATE::THROW_NORMAL_AIR_WHIFF):
            stateName = "THROW_NORMAL_AIR_WHIFF";
            break;

        case ((int)CHAR1_STATE::THROW_NORMAL_ANIM):
            stateName = "THROW_NORMAL_ANIM";
        break;

        case ((int)CHAR1_STATE::THROW_NORMAL_HOLD):
            stateName = "THROW_NORMAL_HOLD";
        break;

        case ((int)CHAR1_STATE::THROW_NORMAL_STARTUP):
            stateName = "THROW_NORMAL_STARTUP";
        break;

        case ((int)CHAR1_STATE::THROW_NORMAL_WHIFF):
            stateName = "THROW_NORMAL_WHIFF";
        break;

        case ((int)CHAR1_STATE::THROW_TECH_CHAR1):
            stateName = "THROW_TECH_CHAR1";
        break;

        case ((int)CHAR1_STATE::THROW_TECH_OWN):
            stateName = "THROW_TECH_OWN";
        break;

        case ((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_ANIM):
            stateName = "THROWN_CHAR1_NORMAL_AIR_ANIM";
        break;

        case ((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD):
            stateName = "THROWN_CHAR1_NORMAL_AIR_HOLD";
        break;

        case ((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_ANIM):
            stateName = "THROWN_CHAR1_NORMAL_ANIM";
        break;

        case ((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD):
            stateName = "THROWN_CHAR1_NORMAL_HOLD";
        break;

        case ((int)CHAR1_STATE::AIR_THROW_TECH_CHAR1):
            stateName = "AIR_THROW_TECH_CHAR1";
        break;

        case ((int)CHAR1_STATE::AIR_THROW_TECH_OWN):
            stateName = "AIR_THROW_TECH_OWN";
        break;

        default:
            stateName = "OTHER_STATE";
    }

    res += "(" + std::to_string(framesInState) + ") ";
    res += stateName;

    if (m_inHitstop)
        res += "[##] ";
    else
        res += "[>>] ";
    
    if (m_airborne)
        res += "[^] ";
    else
        res += "[_] ";

    bool isActive = isInActiveFrames();
    if (isActive)
        res += "[!] ";
    else
        res += "[-] ";


    return res;
}

bool Char1::isInActiveFrames() const
{
    if (m_currentAction->m_isAttack)
    {
        auto atkAction = dynamic_cast<const Action_attack*>(m_currentAction);
        return atkAction->isActive(m_timer.getCurrentFrame() + 1);
    }

    return false;
}