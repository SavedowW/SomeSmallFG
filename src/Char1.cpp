#include "Application.h"
#include "Char1.h"
#include <memory>
#include "Actions.h"
#include <stdexcept>

void Char1::provideActions()
{
    m_actionResolver.addAction(std::make_unique<Action_char1_back_air_throw_startup>());
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw_startup>());
    m_actionResolver.addAction(std::make_unique<Action_char1_back_throw_startup>());
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw_startup>());
    m_actionResolver.addAction(std::make_unique<Action_char1_air_throw_tech>());
    m_actionResolver.addAction(std::make_unique<Action_char1_throw_tech>());
    m_actionResolver.addAction(std::make_unique<Action_char1_step>());
    m_actionResolver.addAction(std::make_unique<Action_char1_air_dash>());
    m_actionResolver.addAction(std::make_unique<Action_char1_air_backdash>());
    m_actionResolver.addAction(std::make_unique<Action_char1_backward_doublejump>());
    m_actionResolver.addAction(std::make_unique<Action_char1_forward_doublejump>());
    m_actionResolver.addAction(std::make_unique<Action_char1_neutral_doublejump>());
    m_actionResolver.addAction(std::make_unique<Action_char1_move_JC>());

    // j.A
    m_actionResolver.addAction(std::unique_ptr<Action_char1_air_attack>(new Action_char1_air_attack((int)CHAR1_STATE::MOVE_JA, ANIMATIONS::CHAR1_MOVE_JA, TimelineProperty(true), std::make_unique<InputComparatorAPress>(), 17,
    {
        hitgeneration::generate_char1_JA()
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
    },
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::AIR_DASH_EXTENTION, (int)CHAR1_STATE::JUMP}))));

    m_actionResolver.addAction(std::make_unique<Action_char1_move_214C>());
    m_actionResolver.addAction(std::make_unique<Action_char1_move_projectile>());

    //4A
    m_actionResolver.addAction(std::unique_ptr<Action_char1_ground_attack>(new Action_char1_ground_attack((int)CHAR1_STATE::MOVE_4A, ANIMATIONS::CHAR1_MOVE_4A, TimelineProperty(true), std::make_unique<InputComparator4APress>(), 24,
    {
        hitgeneration::generate_char1_move4A()
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
    },
    {},
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}), false, false)));

    // 2B
    m_actionResolver.addAction(std::unique_ptr<Action_char1_ground_attack>(new Action_char1_ground_attack((int)CHAR1_STATE::MOVE_2B, ANIMATIONS::CHAR1_MOVE_2B, TimelineProperty(true), std::make_unique<InputComparator2BPress>(), 29,
    {
        hitgeneration::generate_char1_move2B()
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
    }, TimelineProperty<Vector2<float>>(
        {
            {1, {23.0f, 0.0f}},
            {5, {7.0f, 0.0f}},
            {9, {0.0f, 0.0f}},
            {22, {-7.0f, 0.0f}},
            {26, {-23.0f, 0.0f}}
        }),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    true, false)));

    // 236C
    m_actionResolver.addAction(std::unique_ptr<Action_char1_ground_attack>(new Action_char1_ground_attack((int)CHAR1_STATE::MOVE_236C, ANIMATIONS::CHAR1_MOVE_236C, TimelineProperty(true), std::make_unique<InputComparator236CPress>(), 40,
    {
        hitgeneration::generate_char1_236C()
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
    }, TimelineProperty<Vector2<float>>(
        {
            {1, {20.0f, 0.0f}},
            {6, {15.0f, 0.0f}},
            {9, {10.0f, 0.0f}},
            {12, {6.0f, 0.0f}},
            {13, {0, 0}},
            {30, {-1.5f, 0.0f}},
        }),
        StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false)));

    // s.5C
    m_actionResolver.addAction(std::unique_ptr<Action_char1_ground_attack>(new Action_char1_ground_attack((int)CHAR1_STATE::MOVE_STEP_C, ANIMATIONS::CHAR1_MOVE_STEP_C, TimelineProperty(true), std::make_unique<InputComparatorCPress>(), 53,
    {
        hitgeneration::generate_char1_moveStepC()
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
    }, TimelineProperty<Vector2<float>>({{1, {30.0f, 0.0f}}, {4, {0, 0}}}),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, true)));

    // 5C
    m_actionResolver.addAction(std::unique_ptr<Action_char1_ground_attack>(new Action_char1_ground_attack((int)CHAR1_STATE::MOVE_C, ANIMATIONS::CHAR1_MOVE_C, TimelineProperty(true), std::make_unique<InputComparatorCPress>(), 26,
    {
        hitgeneration::generate_char1_moveC()
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
    }, TimelineProperty<Vector2<float>>({{3, {20.0f, 0.0f}}, {6, {0, 0}}, {22, {20.0f, 0.0f}}, {27, {0, 0}}}),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false)));

    // 5B
    m_actionResolver.addAction(std::unique_ptr<Action_char1_ground_attack>(new Action_char1_ground_attack((int)CHAR1_STATE::MOVE_B, ANIMATIONS::CHAR1_MOVE_B, TimelineProperty(true), std::make_unique<InputComparatorBPress>(), 22,
    {
        hitgeneration::generate_char1_moveB()
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
    }, TimelineProperty<Vector2<float>>({{1, {3.5f, 0.0f}}, {7, {0, 0}}, {20, {-2.0f, 0.0f}}, {23, {0, 0}}}),
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false)));

    // 5A
    m_actionResolver.addAction(std::unique_ptr<Action_char1_ground_attack>(new Action_char1_ground_attack((int)CHAR1_STATE::MOVE_A, ANIMATIONS::CHAR1_MOVE_A, TimelineProperty(true), std::make_unique<InputComparatorAPress>(), 16,
    {
        hitgeneration::generate_char1_jab()
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
    },
    {},
    StateMarker(gamedata::characters::totalStateCount, {(int)CHAR1_STATE::SOFT_LANDING_RECOVERY, (int)CHAR1_STATE::GROUND_DASH, (int)CHAR1_STATE::GROUND_DASH_RECOVERY, (int)CHAR1_STATE::WALK_BWD,
    (int)CHAR1_STATE::WALK_FWD, (int)CHAR1_STATE::CROUCH, (int)CHAR1_STATE::STEP_RECOVERY, (int)CHAR1_STATE::IDLE}),
    false, false)));

    m_actionResolver.addAction(std::make_unique<Action_char1_ground_backdash>());
    m_actionResolver.addAction(std::make_unique<Action_char1_ground_dash>());
    m_actionResolver.addAction(std::make_unique<Action_char1_backward_jump>());
    m_actionResolver.addAction(std::make_unique<Action_char1_forward_jump>());
    m_actionResolver.addAction(std::make_unique<Action_char1_neutral_jump>());
    m_actionResolver.addAction(std::make_unique<Action_char1_walk_bwd>());
    m_actionResolver.addAction(std::make_unique<Action_char1_walk_fwd>());
    m_actionResolver.addAction(std::make_unique<Action_char1_ground_dash_recovery>());
    m_actionResolver.addAction(std::make_unique<Action_char1_crouch>());
    m_actionResolver.addAction(std::make_unique<Action_char1_idle>());
    m_actionResolver.addAction(std::make_unique<Action_char1_soft_landing_recovery>());
    m_actionResolver.addAction(std::make_unique<Action_char1_vulnerable_landing_recovery>());
    m_actionResolver.addAction(std::make_unique<Action_char1_hard_landing_recovery>());
    m_actionResolver.addAction(std::make_unique<Action_char1_jc_landing_recovery>());
    m_actionResolver.addAction(std::make_unique<Action_char1_hard_knockdown>());
    m_actionResolver.addAction(std::make_unique<Action_char1_soft_knockdown>());
    m_actionResolver.addAction(std::make_unique<Action_char1_knockdown_recovery>());
    m_actionResolver.addAction(std::make_unique<Action_char1_float>());
    m_actionResolver.addAction(std::make_unique<Action_char1_air_dash_extention>());
    m_actionResolver.addAction(std::make_unique<Action_char1_step_recovery>());

    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw_hold>());
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw_whiff>());
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_throw>());

    m_actionResolver.addAction(std::make_unique<Action_char1_back_throw_hold>());

    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal_hold>());
    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal>());

    m_actionResolver.addAction(std::make_unique<Action_char1_throw_tech_char1>());


    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw_hold>());
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw_whiff>());
    m_actionResolver.addAction(std::make_unique<Action_char1_normal_air_throw>());

    m_actionResolver.addAction(std::make_unique<Action_char1_back_air_throw_hold>());

    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal_air_hold>());
    m_actionResolver.addAction(std::make_unique<Action_char1_thrown_char1_normal_air>());

    m_actionResolver.addAction(std::make_unique<Action_char1_air_throw_tech_char1>());
}

Char1::Char1(Application &application_, Vector2<float> pos_, Camera *cam_, ParticleManager *particleManager_) :
    Character(application_, pos_, 400.0f, gamedata::characters::char1::gravity, cam_, particleManager_, 1, 1, 6, 5)
{
    provideActions();
}

void Char1::loadAnimations(Application &application_)
{
    m_animations[ANIMATIONS::CHAR1_IDLE] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_IDLE, LOOPMETHOD::SWITCH_DIR_LOOP);
    m_animations[ANIMATIONS::CHAR1_CROUCH_IDLE] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_CROUCH_IDLE, LOOPMETHOD::JUMP_LOOP);
    m_animations[ANIMATIONS::CHAR1_WALK_FWD] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_WALK_FWD, LOOPMETHOD::JUMP_LOOP);
    m_animations[ANIMATIONS::CHAR1_WALK_BWD] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_WALK_BWD, LOOPMETHOD::JUMP_LOOP, 65, -1);
    m_animations[ANIMATIONS::CHAR1_PREJUMP] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_PREJUMP);
    m_animations[ANIMATIONS::CHAR1_JUMP] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_JUMP);
    m_animations[ANIMATIONS::CHAR1_LANDING_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_LANDING_RECOVERY, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_GROUND_DASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_GROUND_DASH, LOOPMETHOD::JUMP_LOOP);
    m_animations[ANIMATIONS::CHAR1_GROUND_DASH_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_GROUND_DASH_RECOVERY);
    m_animations[ANIMATIONS::CHAR1_BACKDASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_BACKDASH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_AIRDASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_AIRDASH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_AIR_BACKDASH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_AIR_BACKDASH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_A] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_A, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_B] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_B, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_C] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_C, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_STEP_C] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_STEP_C, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_2B] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_2B, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_4A] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_4A, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_JA] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_JA, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_JC] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_JC, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_214C] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_214C, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_MOVE_236C] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_MOVE_236C, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_JC_LANDING_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_JC_LANDING_RECOVERY, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_LOW] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_LOW, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_MID] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_MID, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_HIGH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_HIGH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_AIR] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_AIR, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_HITSTUN_CROUCH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_HITSTUN_CROUCH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_STANDING] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_BLOCKSTUN_STANDING, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_CROUCHING] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_BLOCKSTUN_CROUCHING, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_KNOCKDOWN] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_KNOCKDOWN, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_SOFT_KNOCKDOWN] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_SOFT_KNOCKDOWN, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_KNOCKDOWN_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_KNOCKDOWN_RECOVERY, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_STEP] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_STEP, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_STEP_RECOVERY] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_STEP_RECOVERY, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_NORMAL_THROW_STARTUP, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_NORMAL_THROW] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_NORMAL_THROW, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL_HOLD, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_THROWN_CHAR1_NORMAL, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_THROW_TECH] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_THROW_TECH, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_NORMAL_THROW_WHIFF] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_NORMAL_THROW_WHIFF, LOOPMETHOD::NOLOOP);
    m_animations[ANIMATIONS::CHAR1_PROJECTILE_CHAR_ANIM] = std::make_unique<Animation>(*application_.getAnimationManager(), ANIMATIONS::CHAR1_PROJECTILE_CHAR_ANIM, LOOPMETHOD::NOLOOP);

    /*
    CHAR1_NORMAL_THROW_STARTUP,
	CHAR1_NORMAL_THROW,
	CHAR1_THROWN_CHAR1_NORMAL_HOLD,
	CHAR1_THROWN_CHAR1_NORMAL,
	CHAR1_THROW_TECH,
	CHAR1_NORMAL_THROW_WHIFF
    */

    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_IDLE].get();
    m_currentAnimation->reset();
}

void Char1::initiate()
{
    m_timer.begin(0);

    if (m_playerId == 1)
        m_actionResolver.subscribe_p1();
    else if (m_playerId == 2)
        m_actionResolver.subscribe_p2();
    else
        throw std::runtime_error("Trying to initiate character without player ID");

    m_actionResolver.setInputEnabled(true);

    m_inertiaDrag = gamedata::characters::char1::inertiaDrag;

    switchToIdle();
}

void Char1::proceedCurrentState()
{
    if (!m_inHitstop)
    {
        auto timerRes = m_timer.update();


        if (timerRes)
        {
            if (isInHitstun() || isInBlockstun())
            {
                setThrowInvul();
            }

            switch (m_currentState)
            {
                case ((int)CHAR1_STATE::PREJUMP):
                    jumpUsingAction();
                    break;
                
                case ((int)CHAR1_STATE::HITSTUN):
                    [[fallthrough]];
                case ((int)CHAR1_STATE::BLOCKSTUN_CROUCHING):
                    [[fallthrough]];
                case ((int)CHAR1_STATE::BLOCKSTUN_STANDING):                
                    switchToIdle();
                    m_currentTakenHit.m_hitId = -1;
                    break;

                default:
                    if (m_currentAction)
                        m_currentAction->outdated(*this);
            }
        }
    }
}

void Char1::updateState()
{
    framesInState++;

    // Update cancel window
    if (!m_inHitstop)
    {
        auto timerres = m_cancelTimer.update();
        if (timerres)
        {
            if (!m_cancelAvailable)
            {
                m_cancelTimer.begin(m_currentCancelWindow.first.second - m_currentCancelWindow.first.first + 1);
                std::cout << "Cancel window opened\n";
                m_cancelAvailable = true;
            }
            else
            {
                std::cout << "Cancel window outdated\n";
                m_cancelTimer.begin(0);
                m_cancelAvailable = false;
            }
        }
    }

    auto resolverRes = m_actionResolver.update(this, m_extendedBuffer);

    if (resolverRes)
    {
        resolverRes->switchTo(*this);
    }

    if (m_currentAction)
        m_currentAction->update(*this);
}

void Char1::switchToIdle()
{
    m_actionResolver.getAction((int)CHAR1_STATE::IDLE)->switchTo(*this);
}

void Char1::jumpUsingAction()
{
    turnVelocityToInertia();

    auto ownOrientationVector = getOwnHorDir();
    ownOrientationVector.y = 1;
    auto jumpAction = dynamic_cast<const Action_jump*>(m_currentAction);
    m_velocity = (jumpAction)->m_impulse.mulComponents(ownOrientationVector);
    if (m_inertia.y > 0)
        m_inertia.y = 0;

    auto currentXInertia = m_inertia.x;
    currentXInertia *= ownOrientationVector.x;
    if (currentXInertia > jumpAction->m_maxHorInertia)
    {
        std::cout << "LIMIT INERTIA\n";
        m_inertia.x = jumpAction->m_maxHorInertia * ownOrientationVector.x;
    }

    m_currentState = (int)CHAR1_STATE::JUMP;
    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_JUMP].get();
    m_currentAnimation->reset();

    m_airborne = true;
}

void Char1::switchToSoftLandingRecovery()
{
    m_actionResolver.getAction((int)CHAR1_STATE::SOFT_LANDING_RECOVERY)->switchTo(*this);
}

void Char1::land()
{
    m_blockstunType = BLOCK_FRAME::NONE;
    bool vulnerable = m_usedAirAttack;
    m_jumpsAvailable.free();
    m_airdashesAvailable.free();
    m_airjumpTimer.begin(0);
    m_airdashTimer.begin(0);
    m_usedAirAttack = false;

    if (m_lockedInAnimation)
        return;

    switch (m_currentState)
    {

        case ((int)CHAR1_STATE::GROUND_BACKDASH):
            m_velocity = {0.0f, 0.0f};
            m_inertia = {0.0f, 0.0f};
            break;

        case ((int)CHAR1_STATE::AIR_THROW_TECH_CHAR1):
            [[fallthrough]];
        case ((int)CHAR1_STATE::BLOCKSTUN_AIR):
            m_actionResolver.getAction((int)CHAR1_STATE::HARD_LANDING_RECOVERY)->switchTo(*this);
            break;
            
        case ((int)CHAR1_STATE::MOVE_JC):
            m_actionResolver.getAction((int)CHAR1_STATE::MOVE_JC_LANDING_RECOVERY)->switchTo(*this);
            break;

        case ((int)CHAR1_STATE::HITSTUN_AIR):
            if (m_hitProps.groundBounce)
            {
                std::cout << "pos: " << m_pos << std::endl;
                m_inertia.y = 0;
                m_velocity.y = -m_comboPhysHandler.getGroundBounceForce(m_hitProps.groundBounceStrength);
                m_hitProps.groundBounce = false;
                m_hitstunAnimation = HITSTUN_ANIMATION::FLOAT;
                m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_AIR].get();
                //m_pos.y = gamedata::stages::levelOfGround - 1;
            }
            else if (m_hitProps.hardKnd)
                m_actionResolver.getAction((int)CHAR1_STATE::HARD_KNOCKDOWN)->switchTo(*this);
            else
                m_actionResolver.getAction((int)CHAR1_STATE::SOFT_KNOCKDOWN)->switchTo(*this);
            m_currentTakenHit.m_hitId = -1;
            break;

        default:
            if (vulnerable)
                m_actionResolver.getAction((int)CHAR1_STATE::VULNERABLE_LANDING_RECOVERY)->switchTo(*this);
            else
                switchToSoftLandingRecovery();
    }
}

bool Char1::canApplyDrag() const
{
    switch (m_currentState)
    {
        case ((int)CHAR1_STATE::PREJUMP):
            return false;

        default:
            return true;
    }
}

bool Char1::canBeDraggedByInertia() const
{
    switch (m_currentState)
    {
        case ((int)CHAR1_STATE::PREJUMP):
            return false;

        default:
            return true;
    }
}

HitsVec Char1::getHits(bool allHits_)
{
    if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto hits = dynamic_cast<const Action_attack*>(m_currentAction)->getCurrentHits(m_timer.getCurrentFrame() + 1, m_pos, m_ownOrientation);
        int i = 0;
        while (i < hits.size())
        {
            if (!allHits_ && m_appliedHits.contains(hits[i].m_hitId))
            {
                hits.erase(hits.begin() + i);
            }
            i++;
        }

        return hits;
    }
    return {};
}

HurtboxVec Char1::getHurtboxes()
{
    if (m_currentAction)
    {
        auto currentFrame = m_timer.getCurrentFrame() + 1;
        return m_currentAction->getCurrentHurtboxes(currentFrame, m_pos, m_ownOrientation);
    }
    else if (m_currentState == (int)CHAR1_STATE::HITSTUN ||
    m_currentState == (int)CHAR1_STATE::HITSTUN_AIR ||
    m_currentState == (int)CHAR1_STATE::BLOCKSTUN_CROUCHING ||
    m_currentState == (int)CHAR1_STATE::BLOCKSTUN_STANDING ||
    m_currentState == (int)CHAR1_STATE::BLOCKSTUN_AIR)
    {
        auto currentHBox = gamedata::characters::char1::standingHurtbox;
        if (m_hitstunAnimation == HITSTUN_ANIMATION::FLOAT)
            currentHBox = gamedata::characters::char1::airHitstunHurtbox;
        else if (m_currentState == (int)CHAR1_STATE::CROUCH || m_currentState == (int)CHAR1_STATE::BLOCKSTUN_CROUCHING || m_hitstunAnimation == HITSTUN_ANIMATION::CROUCH)
            currentHBox = gamedata::characters::char1::crouchingHurtbox;

        currentHBox.y += m_pos.y;
        if (m_ownOrientation == ORIENTATION::RIGHT)
            currentHBox.x += m_pos.x;
        else
            currentHBox.x = m_pos.x - currentHBox.x - currentHBox.w;

        return {currentHBox};
    }

    return {};

}

HIT_RESULT Char1::applyHit(HitEvent &hitEvent)
{
    float range = (m_pos - m_otherCharacter->getPos()).getLen();

    // Attacker's side
    if (hitEvent.m_hittingPlayerId == m_playerId)
    {
        callForPriority();
        if (hitEvent.m_hitRes == HIT_RESULT::COUNTER)
        {
            m_notifyWidget->addNotification("COUNTER!");
        }
        m_notifyWidget->addNotification(std::to_string(hitEvent.realDamage));

        applyHitstop(hitEvent.m_hitData.hitProps.hitstop);

        if (!isInHitstun())
        {
            m_appliedHits.insert(hitEvent.m_hitData.m_hitId);

            if (hitEvent.m_hitRes == HIT_RESULT::HIT || hitEvent.m_hitRes == HIT_RESULT::COUNTER)
                applyCancelWindow(hitEvent.m_hitData.cancelsOnHit);
            else
                applyCancelWindow(hitEvent.m_hitData.cancelsOnBlock);

        }

        return HIT_RESULT::NONE;
    }
    // Defender's side
    else
    {
        auto blockState = m_blockHandler.getBlockState();
        m_currentTakenHit = hitEvent.m_hitData;
        bool blocked = hitEvent.m_hitData.canBeBlockedAs.contains(blockState);
        // Took hit
        if (!blocked)
        {
            turnVelocityToInertia();
            m_inertia.x *= gamedata::characters::takenHitInertiaCarry;
            std::cout << "Took hit!\n";

            auto hitres = HIT_RESULT::HIT;
            bool isCounter = false;

            if (m_currentAction && m_currentAction->isInCounterState(m_timer.getCurrentFrame() + 1))
            {
                isCounter = true;
            }

            if (isCounter)
            {
                m_hitProps = hitEvent.m_hitData.chProps;
                hitres = HIT_RESULT::COUNTER;
                startShine({150, 0, 0, 225}, std::max(hitEvent.m_hitData.chProps.hitstop - 3, 5), 3);
            }
            else
            {
                m_hitProps = hitEvent.m_hitData.hitProps;
            }

            hitEvent.m_hitRes = hitres;
            hitEvent.realDamage = m_healthHandler.takeDamage(hitEvent);
            m_comboPhysHandler.takeHit(hitEvent);

            if (!m_lockedInAnimation)
            {
                Vector2<float> newImpulse;
                auto hordir = getHorDirToEnemy();
                
                if (m_airborne)
                {
                    newImpulse = m_hitProps.opponentImpulseOnAirHit.mulComponents(Vector2{hordir.x * -1.0f, 1.0f});
                    newImpulse = m_comboPhysHandler.getImpulseScaling(newImpulse, hordir);
                    newImpulse += m_inertia * gamedata::characters::airborneImpulseIntoPushbackCarry;
                    m_inertia = newImpulse;

                    newImpulse.x *= gamedata::characters::airbornePushbackMultiplier;
                }
                else
                {
                    if (m_hitProps.opponentImpulseOnHit.y == 0)
                    {
                    newImpulse = m_hitProps.opponentImpulseOnHit.mulComponents(Vector2{hordir.x * -1.0f, 1.0f});
                    newImpulse = m_comboPhysHandler.getImpulseScaling(newImpulse, hordir);
                    }
                    else
                    {
                        m_airborne = true;
    
                        newImpulse = m_hitProps.opponentImpulseOnHit.mulComponents(Vector2{hordir.x * -1.0f, 1.0f});
                        newImpulse = m_comboPhysHandler.getImpulseScaling(newImpulse, hordir);
                        newImpulse += m_inertia * gamedata::characters::airborneImpulseIntoPushbackCarry;

                        m_inertia += newImpulse;

                        newImpulse.x *= gamedata::characters::airbornePushbackMultiplier;
                        
                    }
                }

                newImpulse.y = 0;
                takePushback(newImpulse);
            }


            enterHitstunAnimation(m_hitProps);

            applyCancelWindow({{0, 0}, {}});
            applyHitstop(m_hitProps.hitstop);

            return hitres;
        }
        // Blocked
        else
        {
            bool inBlockstun = isInBlockstun();

            bool isInstant = false;
            if ((!inBlockstun || m_blockstunType == BLOCK_FRAME::INSTANT) && m_blockHandler.getBlockFrame() == BLOCK_FRAME::INSTANT)
            {
                isInstant = true;
                m_blockstunType = BLOCK_FRAME::INSTANT;
                startShine({255, 255, 255, 255}, 4, 8);
                m_notifyWidget->addNotification("INSTANT");
            }

            applyHitstop(hitEvent.m_hitData.hitProps.hitstop);
            if (!isInstant)
            {
                turnVelocityToInertia();
                m_inertia.x *= gamedata::characters::pushblockInertiaCarry;
                takePushback(getHorDirToEnemy() * -1.0f * hitEvent.m_hitData.opponentPushbackOnBlock);
            }
            else if (m_airborne)
            {
                turnVelocityToInertia();
                if (!isInstant)
                    takePushback(getHorDirToEnemy() * -1.0f * hitEvent.m_hitData.opponentPushbackOnBlock);
            }
            else
            {
                m_velocity = {0, 0};
                m_inertia = {0, 0};
            }
            auto blockstunDuration = m_blockHandler.getBlockstunDuration(hitEvent.m_hitData.blockstun);
            m_notifyWidget->addNotification(std::to_string(blockstunDuration));
            m_timer.begin(blockstunDuration);
            HIT_RESULT res = HIT_RESULT::BLOCK_HIGH;

            switch (blockState)
            {
                case (BLOCK_STATE::AUTO):
                    [[fallthrough]];
                case (BLOCK_STATE::HIGH):
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_STANDING].get();
                    m_currentState = (int)CHAR1_STATE::BLOCKSTUN_STANDING;
                    res = HIT_RESULT::BLOCK_HIGH;
                    break;

                case (BLOCK_STATE::LOW):
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_CROUCHING].get();
                    m_currentState = (int)CHAR1_STATE::BLOCKSTUN_CROUCHING;
                    res = HIT_RESULT::BLOCK_LOW;
                    break;

                case (BLOCK_STATE::AIR):
                    m_currentAnimation = m_animations[ANIMATIONS::CHAR1_BLOCKSTUN_STANDING].get();
                    m_currentState = (int)CHAR1_STATE::BLOCKSTUN_AIR;
                    res = HIT_RESULT::BLOCK_AIR;
                    break;
            }

            m_currentAnimation->reset(0);
            m_currentAction = nullptr;

            hitEvent.m_hitRes = res;
            if (!isInstant)
            {
                hitEvent.realDamage = m_healthHandler.takeDamage(hitEvent);
            }

            return res;
        }
    }
}

void Char1::updateBlockState()
{
    bool inBlockstun = isInBlockstun();

    bool canBlock = (inBlockstun || m_currentAction && m_currentAction->canBlock(m_timer.getCurrentFrame() + 1));

    auto backButton = (m_dirToEnemy == ORIENTATION::RIGHT ? INPUT_BUTTON::LEFT : INPUT_BUTTON::RIGHT);

    m_blockHandler.update(m_actionResolver.getCurrentInputDir(), m_actionResolver.getPostFrameButtonState(backButton), m_airborne, getHorDirToEnemy(), inBlockstun, canBlock, m_inHitstop);
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

        case ((int)CHAR1_STATE::HITSTUN):
            stateName = "HITSTUN";
            break;

        case ((int)CHAR1_STATE::HITSTUN_AIR):
            stateName = "HITSTUN_AIR";
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
    if (m_currentAction && m_currentAction->m_isAttack)
    {
        auto atkAction = dynamic_cast<const Action_attack*>(m_currentAction);
        return atkAction->getCurrentHits(m_timer.getCurrentFrame() + 1, m_pos, m_ownOrientation).size();
    }

    return false;
}

bool Char1::isInHitstun() const
{
    return (m_currentState == (int)CHAR1_STATE::HITSTUN || m_currentState == (int)CHAR1_STATE::HITSTUN_AIR);
}

bool Char1::isInBlockstun() const
{
    return (m_currentState == (int)CHAR1_STATE::BLOCKSTUN_STANDING ||
                        m_currentState == (int)CHAR1_STATE::BLOCKSTUN_CROUCHING ||
                        m_currentState == (int)CHAR1_STATE::BLOCKSTUN_AIR ||
                        m_currentState == (int)CHAR1_STATE::HARD_LANDING_RECOVERY);
}

bool Char1::isInInstantBlockstun() const
{
    return isInBlockstun() && m_blockstunType == BLOCK_FRAME::INSTANT;
}

bool Char1::isKnockedDown() const
{
    return (m_currentState == (int)CHAR1_STATE::KNOCKDOWN_RECOVERY ||
            m_currentState == (int)CHAR1_STATE::HARD_KNOCKDOWN ||
            m_currentState == (int)CHAR1_STATE::SOFT_KNOCKDOWN);
}

void Char1::enterKndRecovery()
{
    m_actionResolver.getAction((int)CHAR1_STATE::KNOCKDOWN_RECOVERY)->switchTo(*this);
}

void Char1::switchToFloat()
{
    m_actionResolver.getAction((int)CHAR1_STATE::FLOAT)->switchTo(*this);
}

bool Char1::canApplyGravity() const
{
    if (m_currentAction)
        return m_currentAction->applyGravity(m_timer.getCurrentFrame() + 1);

    return Character::canApplyGravity();
}

void Char1::enterHitstunAnimation(const PostHitProperties &props_)
{
    m_blockstunType = BLOCK_FRAME::NONE;
    if (m_lockedInAnimation)
        return;

    if (m_airborne)
    {
        m_currentState = (int)CHAR1_STATE::HITSTUN_AIR;
        m_hitstunAnimation = props_.airHitstunAnimation;

        m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_AIR].get();
    }
    else
    {
        m_currentState = (int)CHAR1_STATE::HITSTUN;
        m_timer.begin(m_hitProps.hitstun);

        if (m_currentAction && m_currentAction->m_isCrouchState || m_hitstunAnimation == HITSTUN_ANIMATION::CROUCH || props_.forceCrouch)
        {
            m_hitstunAnimation = HITSTUN_ANIMATION::CROUCH;
            m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_CROUCH].get();
        }
        else if (props_.groundHitstunAnimation == HITSTUN_ANIMATION::HIGH)
        {
            m_hitstunAnimation = props_.groundHitstunAnimation;
            m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_HIGH].get();
        }
        else if (props_.groundHitstunAnimation == HITSTUN_ANIMATION::MID)
        {
            m_hitstunAnimation = props_.groundHitstunAnimation;
            m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_MID].get();
        }
        else
        {
            m_hitstunAnimation = props_.groundHitstunAnimation;
            m_currentAnimation = m_animations[ANIMATIONS::CHAR1_HITSTUN_LOW].get();
        }
    }

    m_currentAnimation->reset(0);
    m_currentAction = nullptr;
}

float Char1::touchedWall(int sideDir_)
{
    auto fw = getFullVelocity();

    if (sideDir_ * fw.x > 0 && isInHitstun() && m_airborne && m_hitProps.wallBounce)
    {
        m_velocity.x *= -1 * m_hitProps.wallbounceInertiaMultiplierX;
        m_inertia.x *= -1 * m_hitProps.wallbounceInertiaMultiplierX;
        
        if (m_inertia.y < 0)
            m_inertia.y *= m_hitProps.wallbounceInertiaMultiplierY;

        if (m_velocity.y < 0)
            m_velocity.y *= m_hitProps.wallbounceInertiaMultiplierY;

        m_hitProps.wallBounce = false;
    }

    // With code below, if the character reaches max range and continues to walk back and another character moves in,
    // character that walks back would be stuck in 1 point
    /*if (utils::sameSign<float>(m_velocity.x, sideDir_))
        m_velocity.x = 0;
    if (utils::sameSign<float>(m_inertia.x, sideDir_))
        m_inertia.x = 0;*/

    if ((isInHitstun() || isInBlockstun()) && utils::sameSign<float>(m_pushback.x, sideDir_))
    {
        auto pbx = m_pushback.x;
        m_pushback.x = 0;
        return abs(pbx + fw.x * gamedata::characters::inertiaIntoCornerPushbackCarry);
    }

    return 0;
}

Collider Char1::getUntiedPushbox() const
{
    Collider pb;
    if (m_airborne && m_hitstunAnimation != HITSTUN_ANIMATION::NONE)
        pb = gamedata::characters::char1::airborneHitstunPushbox;
    else if (m_currentAction && m_currentAction->m_isCrouchState || m_hitstunAnimation == HITSTUN_ANIMATION::CROUCH)
        pb = gamedata::characters::char1::crouchingPushbox;
    else if (m_airborne)
        pb = gamedata::characters::char1::airbornePushbox;
    else
        pb = gamedata::characters::char1::standingPushbox;

    pb.x += m_pos.x;
    pb.y += m_pos.y;
    return pb;
}

void Char1::enterThrown(THROW_LIST throw_)
{
    // Enter proper state depending on throw_
    lockInAnimation();

    switch (throw_)
    {
        case (THROW_LIST::CHAR1_NORMAL_THROW):
            m_actionResolver.getAction((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_HOLD)->switchTo(*this);
            break;

        case (THROW_LIST::CHAR1_AIR_THROW):
            m_actionResolver.getAction((int)CHAR1_STATE::THROWN_CHAR1_NORMAL_AIR_HOLD)->switchTo(*this);
            break;
    }
}

void Char1::throwTeched(THROW_TECHS_LIST tech_)
{
    std::cout << "Called tech\n";

    if (tech_ == THROW_TECHS_LIST::CHAR1_GROUND)
        m_actionResolver.getAction((int)CHAR1_STATE::THROW_TECH_CHAR1)->switchTo(*this);

    else if (tech_ == THROW_TECHS_LIST::CHAR1_AIR)
        m_actionResolver.getAction((int)CHAR1_STATE::AIR_THROW_TECH_CHAR1)->switchTo(*this);
}

void Char1::attemptThrow()
{
    if (m_currentAction && m_currentAction->m_isThrowStartup)
    {
        auto action = dynamic_cast<const Action_throw_startup*>(m_currentAction);
        action->attemptThrow(*this);
    }
}

void Char1::applyClash(const Hit &clashedHit_)
{
    Character::applyClash(clashedHit_);
    
    m_appliedHits.insert(clashedHit_.m_hitId);

    CancelWindow tempwindow;

    tempwindow.first = {1, 20};
    tempwindow.second = {
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

    applyCancelWindow(tempwindow);
    
}

void Char1::turnVelocityToInertia(float horMultiplier_)
{
    if (m_currentState == (int)CHAR1_STATE::WALK_BWD || m_currentState == (int)CHAR1_STATE::WALK_FWD)
        m_velocity = {0, 0};

    Character::turnVelocityToInertia(horMultiplier_);
}

ORIENTATION Char1::getInputDir() const
{
    auto res = m_ownOrientation;
    if (m_currentState == (int)CHAR1_STATE::SOFT_LANDING_RECOVERY || m_currentState == (int)CHAR1_STATE::GROUND_DASH_RECOVERY)
        res = m_dirToEnemy;
    return res;
}

bool Char1::isInAttackState() const
{
    return (m_currentAction && m_currentAction->m_isAttack);
}