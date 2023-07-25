#include "Actions.h"
#include "Char1.h"
#include <stdexcept>

/* ============================
 *
 *       UNIVERSAL ABSTRACT ACTIONS
 *
 *========================== */

template <typename CharState_t, typename CharData, typename Char_t>
Action<CharState_t, CharData, Char_t>::Action(CharState_t actionState_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_, bool isAttack_) :
    actionState(actionState_),
    m_hurtboxes(hurtboxes_),
    m_anim(anim_),
    m_isAttack(isAttack_)
{
    incmp = std::move(incmp_);
}

template <typename CharState_t, typename CharData, typename Char_t>
bool Action<CharState_t, CharData, Char_t>::isInputPossible(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const
{
    if (incmp)
        return (*incmp)(inputQueue_, ownDirection_);
    else
        return false;
}

template <typename CharState_t, typename CharData, typename Char_t>
const HurtboxVec Action<CharState_t, CharData, Char_t>::getCurrentHurtboxes(int currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const
{
    HurtboxVec vec;
    for (const auto &el : m_hurtboxes)
    {
        if (el.first.first <= currentFrame_ && el.first.second >= currentFrame_)
        {
            auto hbox = el.second;
            if (ownOrientation_ == ORIENTATION::RIGHT)
            {
                hbox.x += offset_.x;
                hbox.y += offset_.y;
            }
            else
            {
                hbox.x = offset_.x - hbox.x - hbox.w;
                hbox.y += offset_.y;
            }
            vec.push_back(hbox);
        }
    }

    return vec;
}

template <typename CharState_t, typename CharData, typename Char_t>
void Action<CharState_t, CharData, Char_t>::switchTo(Char_t &character_) const
{
    character_.m_currentState = actionState;
    character_.m_currentAnimation = character_.m_animations[m_anim].get();
    character_.m_currentAnimation->reset();
    character_.m_currentAction = this;
    character_.m_currentCancelWindow = {};
    character_.m_cancelTimer.begin(0);
    character_.framesInState = 0;
    character_.m_appliedHits.clear();
}

// ABSTRACT PROLONGED ACTION
template <typename CharState_t, typename CharData, typename Char_t>
Action_prolonged<CharState_t, CharData, Char_t>::Action_prolonged(CharState_t actionState_, InputComparator_ptr incmp_, InputComparator_ptr incmp_prolonged_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_) :
    Action<CharState_t, CharData, Char_t>(actionState_, std::move(incmp_), hurtboxes_, anim_)
{
    incmp_prolonged = std::move(incmp_prolonged_);
}

template <typename CharState_t, typename CharData, typename Char_t>
int Action_prolonged<CharState_t, CharData, Char_t>::isPossibleToProlong(const InputQueue &inputQueue_, ORIENTATION ownDirection_) const
{
    return (*incmp_prolonged)(inputQueue_, ownDirection_);
}



// ABSTRACT JUMP ACTION
template <typename CharState_t, typename CharData, typename Char_t>
Action_jump<CharState_t, CharData, Char_t>::Action_jump(CharState_t actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_, ANIMATIONS anim_) :
    Action<CharState_t, CharData, Char_t>(actionState_, std::move(incmp_), hurtboxes_, anim_),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
}




// ABSTRACT ATTACK ACTION
template <typename CharState_t, typename CharData, typename Char_t>
Action_attack<CharState_t, CharData, Char_t>::Action_attack(CharState_t actionState_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec hurtboxes_, std::vector<std::pair<std::pair<int, int>, Vector2<float>>> velocity_, ANIMATIONS anim_) :
    Action<CharState_t, CharData, Char_t>(actionState_, std::move(incmp_), hurtboxes_, anim_, true),
    m_fullDuration(fullDuration_),
    m_hits(hits_),
    m_velocity(velocity_)
{
}

template <typename CharState_t, typename CharData, typename Char_t>
const HitsVec Action_attack<CharState_t, CharData, Char_t>::getCurrentHits(int currentFrame_, const Vector2<float>& offset_, ORIENTATION ownOrientation_) const
{
    HitsVec vec;
    for (const auto &el : m_hits)
    {
        if (el.first.first <= currentFrame_ && el.first.second >= currentFrame_)
        {
            auto hit = el.second;
            for (auto &hbox : hit.m_hitboxes)
            {
                hbox = hbox.getHitboxAtOffset(offset_, ownOrientation_);
            }
            vec.push_back(hit);
        }
    }

    return vec;
}

template <typename CharState_t, typename CharData, typename Char_t>
const Vector2<float> &Action_attack<CharState_t, CharData, Char_t>::getCurrentVelocity(int currentFrame_) const
{
    for (const auto &el : m_velocity)
    {
        if (el.first.first <= currentFrame_ && el.first.second >= currentFrame_)
            return el.second;
    }

    return {};
}

template <typename CharState_t, typename CharData, typename Char_t>
void Action_attack<CharState_t, CharData, Char_t>::switchTo(Char_t &character_) const
{
    Action<CharState_t, CharData, Char_t>::switchTo(character_);
    character_.m_timer.begin(m_fullDuration);
    character_.turnVelocityToInertia();
}

/* ============================
 *
 *       CHAR 1 ACTIONS
 *
 *========================== */

// IDLE ACTION
Action_char1_idle::Action_char1_idle() :
    Action(CHAR1_STATE::IDLE, std::make_unique<InputComparatorIdle>(), {}, ANIMATIONS::CHAR1_IDLE)
{
}

int Action_char1_idle::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    switch (charData_.state)
    {
        case (CHAR1_STATE::IDLE):
            return -1;
            break;

        case (CHAR1_STATE::CROUCH):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_BWD):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_idle::switchTo(Char1 &character_) const
{
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
    character_.turnVelocityToInertia();
    character_.updateOwnOrientation();
}

// CROUCH ACTION
Action_char1_crouch::Action_char1_crouch() :
    Action(CHAR1_STATE::CROUCH, std::make_unique<InputComparatorDownHold>(), {}, ANIMATIONS::CHAR1_CROUCH_IDLE)
{
}

int Action_char1_crouch::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    switch (charData_.state)
    {
        case (CHAR1_STATE::CROUCH):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? -1 : 0);
            break;

        case (CHAR1_STATE::IDLE):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_BWD):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_crouch::switchTo(Char1 &character_) const
{
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
}


// WALK FORWARD ACTION
Action_char1_walk_fwd::Action_char1_walk_fwd() :
    Action(CHAR1_STATE::WALK_FWD, std::make_unique<InputComparatorForward>(),
    {
        {
            {1, 1},
            {-70, -375, 140, 375}
        }
    }, ANIMATIONS::CHAR1_WALK_FWD)
{
}

int Action_char1_walk_fwd::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    switch (charData_.state)
    {
        case (CHAR1_STATE::WALK_FWD):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? -1 : 0);
            break;

        case (CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case (CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_walk_fwd::switchTo(Char1 &character_) const
{
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
    character_.m_velocity = character_.getHorDirToEnemy().mulComponents(Vector2{6.0f, 0.0f});
}

// WALK BACKWARD ACTION
Action_char1_walk_bwd::Action_char1_walk_bwd() :
    Action(CHAR1_STATE::WALK_BWD, std::make_unique<InputComparatorBackward>(), {
        {
            {1, 1},
            {-70, -375, 140, 375}
        }
    }, ANIMATIONS::CHAR1_WALK_BWD)
{
}

int Action_char1_walk_bwd::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    switch (charData_.state)
    {
        case (CHAR1_STATE::WALK_BWD):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? -1 : 0);
            break;

        case (CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case (CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_walk_bwd::switchTo(Char1 &character_) const
{
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
    character_.m_currentAnimation->reset(65, -1);
    character_.m_velocity = character_.getHorDirToEnemy().mulComponents(Vector2{-6.0f, 0.0f});
}


// ABSTRACT CHAR1 JUMP ACTION
Action_char1_jump::Action_char1_jump(CHAR1_STATE actionState_, const Vector2<float> &impulse_, float prejumpLen_, float maxHorInertia_, InputComparator_ptr incmp_, HurtboxFramesVec hurtboxes_) :
    Action_jump<CHAR1_STATE, Char1Data, Char1>(actionState_, impulse_, prejumpLen_, maxHorInertia_, std::move(incmp_), hurtboxes_, ANIMATIONS::CHAR1_PREJUMP),
    m_impulse(impulse_),
    m_prejumpLen(prejumpLen_),
    m_maxHorInertia(maxHorInertia_)
{
}

int Action_char1_jump::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    if (charData_.cancelOptions)
    {
        if (charData_.cancelOptions->contains((int)actionState))
        {
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
        }
    }

    switch (charData_.state)
    {
        case (CHAR1_STATE::PREJUMP):
            return -1;
            break;

        case (CHAR1_STATE::GROUND_DASH):
            [[fallthrough]];
        case (CHAR1_STATE::GROUND_DASH_RECOVERY):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case (CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_jump::switchTo(Char1 &character_) const
{
    auto oldState = character_.m_currentState;
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
    character_.m_timer.begin(m_prejumpLen);

    if (oldState != CHAR1_STATE::WALK_BWD && oldState != CHAR1_STATE::WALK_FWD)
        character_.turnVelocityToInertia();
    else
        character_.m_velocity = {0.0f, 0.0f};
}

// NEUTRAL JUMP ACTION
Action_char1_neutral_jump::Action_char1_neutral_jump() :
    Action_char1_jump(CHAR1_STATE::PREJUMP,
    {0.0f, -gamedata::characters::char1::groundJumpVerticalImpulse},
    gamedata::characters::char1::prejumpFrames, gamedata::characters::char1::groundJumpMaxHorizontalInertia,
    std::make_unique<InputComparatorUpHold>(),
    {
        {
            {1, 5},
            {-70, -50, 140, 50}
        },
        {
            {0, 5},
            {-70, -350, 140, 300}
        }
    })
{
}

// FORWARD JUMP ACTION
Action_char1_forward_jump::Action_char1_forward_jump() :
    Action_char1_jump(CHAR1_STATE::PREJUMP,
    {gamedata::characters::char1::groundJumpHorizontalImpulse, -gamedata::characters::char1::groundJumpVerticalImpulse},
    gamedata::characters::char1::prejumpFrames, gamedata::characters::char1::groundJumpMaxHorizontalInertia,
    std::make_unique<InputComparatorUpForwardHold>(),
    {
        {
            {1, 5},
            {-70, -50, 140, 50}
        },
        {
            {0, 5},
            {-70, -350, 140, 300}
        }
    })
{
}

// BACKWARD JUMP ACTION
Action_char1_backward_jump::Action_char1_backward_jump() :
    Action_char1_jump(CHAR1_STATE::PREJUMP,
    {-gamedata::characters::char1::groundJumpHorizontalImpulse, -gamedata::characters::char1::groundJumpVerticalImpulse},
    gamedata::characters::char1::prejumpFrames, gamedata::characters::char1::groundJumpMaxHorizontalInertia,
    std::make_unique<InputComparatorUpBackwardHold>(),
    {
        {
            {1, 5},
            {-70, -50, 140, 50}
        },
        {
            {0, 5},
            {-70, -350, 140, 300}
        }
    })
{
}





// ABSTRACT AIR JUMP ACTION
Action_char1_airjump::Action_char1_airjump(const Vector2<float> &impulse_, InputComparator_ptr incmp_) :
    Action(CHAR1_STATE::JUMP, std::move(incmp_), {
        {
            {0, 1},
            {-70, -350, 140, 300}
        }
    }, ANIMATIONS::CHAR1_JUMP),
    m_impulse(impulse_)
{
}

int Action_char1_airjump::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    switch (charData_.state)
    {
        case (CHAR1_STATE::JUMP):
            return ((!charData_.usedDoubleJump && !charData_.usedAirDash && isInputPossible(inputQueue_, charData_.dirToEnemy)) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_airjump::switchTo(Char1 &character_) const
{
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);

    character_.updateOwnOrientation();
    auto ownOrientationVector = character_.getOwnHorDir();
    ownOrientationVector.y = 1;
    character_.m_velocity = m_impulse.mulComponents(ownOrientationVector);

    if (character_.m_inertia.y > 0)
        character_.m_inertia.y = 0;

    character_.m_usedDoubleJump = true;
}

// NEUTRAL DOUBLEJUMP ACTION
Action_char1_neutral_doublejump::Action_char1_neutral_doublejump() :
    Action_char1_airjump({0.0f, -gamedata::characters::char1::airJumpVerticalImpulse}, std::make_unique<InputComparatorUpPress>())
{
}

// FORWARD DOUBLEJUMP ACTION
Action_char1_forward_doublejump::Action_char1_forward_doublejump() :
    Action_char1_airjump({gamedata::characters::char1::airJumpHorizontalImpulse, -gamedata::characters::char1::airJumpVerticalImpulse}, std::make_unique<InputComparatorUpForwardPress>())
{
}

// BACKWARD DOUBLEJUMP ACTION
Action_char1_backward_doublejump::Action_char1_backward_doublejump() :
    Action_char1_airjump({-gamedata::characters::char1::airJumpHorizontalImpulse, -gamedata::characters::char1::airJumpVerticalImpulse}, std::make_unique<InputComparatorUpBackwardPress>())
{
}




// GROUND DASH ACTION
Action_char1_ground_dash::Action_char1_ground_dash() :
    Action_prolonged(CHAR1_STATE::GROUND_DASH, std::move(std::make_unique<InputComparator66>()), std::move(std::make_unique<InputComparatorForward>()), 
    {
        {
            {1, 1},
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_GROUND_DASH),
    m_accel(gamedata::characters::char1::dashAccel),
    m_maxspd(gamedata::characters::char1::dashMaxSpeed)
{
}

int Action_char1_ground_dash::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    if (charData_.cancelOptions)
    {
        if (charData_.cancelOptions->contains((int)actionState))
        {
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
        }
    }

    switch (charData_.state)
    {
        case (CHAR1_STATE::GROUND_DASH):
            return (isPossibleToProlong(inputQueue_, charData_.ownDirection) ? -1 : 0);
            break;

        case (CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case (CHAR1_STATE::IDLE):
            return ((!charData_.usedDoubleJump && !charData_.usedAirDash && isInputPossible(inputQueue_, charData_.ownDirection)) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

// GROUND DASH RECOVERY ACTION
Action_char1_ground_dash_recovery::Action_char1_ground_dash_recovery() :
    Action(CHAR1_STATE::GROUND_DASH_RECOVERY, std::move(std::make_unique<InputComparator66>()), {
        {
            {1, gamedata::characters::char1::dashRecovery},
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_GROUND_DASH_RECOVERY),
    m_recoveryLen(gamedata::characters::char1::dashRecovery)
{
}

int Action_char1_ground_dash_recovery::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    switch (charData_.state)
    {
        case (CHAR1_STATE::GROUND_DASH_RECOVERY):
            return -1;
            break;

        case (CHAR1_STATE::GROUND_DASH):
            return 1;
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_ground_dash_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_ground_dash_recovery::switchTo(Char1 &character_) const
{
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
    character_.m_timer.begin(m_recoveryLen);
    character_.turnVelocityToInertia();
}

// SOFT LANDING RECOVERY
Action_char1_soft_landing_recovery::Action_char1_soft_landing_recovery() :
    Action(CHAR1_STATE::SOFT_LANDING_RECOVERY, nullptr, {
        {
            {1, 6},
            {-100, -300, 200, 300}
        }
    }, ANIMATIONS::CHAR1_LANDING_RECOVERY),
    m_recoveryLen(gamedata::characters::char1::dashRecovery)
{
}

int Action_char1_soft_landing_recovery::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    return false;
}

void Action_char1_soft_landing_recovery::outdated(Char1 &character_) const
{
    character_.switchToIdle();
}

void Action_char1_soft_landing_recovery::switchTo(Char1 &character_) const
{
    Action<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
    character_.m_velocity = {0.0f, 0.0f};
    character_.m_inertia = {0.0f, 0.0f};
    character_.m_timer.begin(6);
}

// ABSTRACT CHAR1 ATTACK ACTION
Action_char1_attack::Action_char1_attack(CHAR1_STATE actionState_, ANIMATIONS anim_, InputComparator_ptr incmp_, int fullDuration_, const ActiveFramesVec &hits_, HurtboxFramesVec hurtboxes_, std::vector<std::pair<std::pair<int, int>, Vector2<float>>> velocity_) :
    Action_attack<CHAR1_STATE, Char1Data, Char1>(actionState_, std::move(incmp_), fullDuration_, hits_, hurtboxes_, velocity_, anim_)
{
}

int Action_char1_attack::isPossible(const InputQueue &inputQueue_, Char1Data charData_) const
{
    if (charData_.inHitstop)
        return 0;

    if (charData_.cancelOptions)
    {
        if (charData_.cancelOptions->contains((int)actionState))
        {
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
        }
    }

    switch (charData_.state)
    {

        case (CHAR1_STATE::SOFT_LANDING_RECOVERY):
            [[fallthrough]];
        case (CHAR1_STATE::GROUND_DASH):
            [[fallthrough]];
        case (CHAR1_STATE::GROUND_DASH_RECOVERY):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_BWD):
            [[fallthrough]];
        case (CHAR1_STATE::WALK_FWD):
            [[fallthrough]];
        case (CHAR1_STATE::CROUCH):
            [[fallthrough]];
        case (CHAR1_STATE::IDLE):
            return (isInputPossible(inputQueue_, charData_.ownDirection) ? 1 : 0);
            break;

        default:
            return 0;
            break;
    }

    throw std::runtime_error("Undefined state transition");
    return false;
}

void Action_char1_attack::outdated(Char1 &character_) const
{
    character_.m_velocity = {0.0f, 0.0f};
    character_.switchToIdle();

}

void Action_char1_attack::switchTo(Char1 &character_) const
{
    if (character_.m_currentState == CHAR1_STATE::SOFT_LANDING_RECOVERY)
                    character_.updateOwnOrientation();

    Action_attack<CHAR1_STATE, Char1Data, Char1>::switchTo(character_);
}

// JAB ACTION
Action_char1_jab::Action_char1_jab() :
    Action_char1_attack(CHAR1_STATE::MOVE_A, ANIMATIONS::CHAR1_MOVE_A, std::make_unique<InputComparatorAPress>(), 16,
    {
        {
            {5, 7},
            hitgeneration::generate_char1_jab()
        }
    },
    {
        {
            {1, 16},
            {-70, -375, 140, 375}
        },
        {
            {4, 12},
            {40.0f, -310.0f, 140.0f, 50.0f}
        }
    },
    {})
{
}

// MOVE B ACTION
Action_char1_move_B::Action_char1_move_B() :
    Action_char1_attack(CHAR1_STATE::MOVE_B, ANIMATIONS::CHAR1_MOVE_B, std::make_unique<InputComparatorBPress>(), 22,
    {
        {
            {7, 10},
            hitgeneration::generate_char1_moveB()
        }
    },
    {
        {
            {1, 22},
            {-70, -375, 140, 375}
        },
        {
            {7, 19},
            {50.0f, -275.0f, 175.0f, 80.0f}
        }
    },
    {
        {
            {1, 6},
            {3.5f, 0.0f}
        },
        {
            {20, 22},
            {-2.0f, 0.0f}
        }
    })
{
}

// MOVE C ACTION
Action_char1_move_C::Action_char1_move_C() :
    Action_char1_attack(CHAR1_STATE::MOVE_C, ANIMATIONS::CHAR1_MOVE_C, std::make_unique<InputComparatorCPress>(), 34,
    {
        {
            {13, 16},
            hitgeneration::generate_char1_moveC()
        }
    },
    {
        {
            {1, 34},
            {-70, -200, 140, 200}
        },
        {
            {1, 4},
            {-70.0f, -300.0f, 140.0f, 100.0f}
        },
        {
            {13, 34},
            {-70.0f, -300.0f, 140.0f, 100.0f}
        },
        {
            {17, 22},
            {60.0f, -350.0f, 40.0f, 150.0f}
        },
    },
    {
        {
            {1, 5},
            {20.0f, 0.0f}
        },
        {
            {6, 8},
            {15.0f, 0.0f}
        },
        {
            {9, 11},
            {10.0f, 0.0f},
        },
        {
            {12, 12},
            {6.0f, 0.0f}
        },
        {
            {23, 34},
            {-1.5f, 0.0f}
        }
    })
    //{ { 1.0f, {{0.0f, 0.0f, 10.0f, 10.0f}}} })
{
}

// MOVE 2B ACTION
Action_char1_move_2B::Action_char1_move_2B() :
    Action_char1_attack(CHAR1_STATE::MOVE_2B, ANIMATIONS::CHAR1_MOVE_2B, std::make_unique<InputComparator2BPress>(), 29,
    {
        {
            {10, 12},
            hitgeneration::generate_char1_move2B()
        }
    },
    {
        {
            {1, 29},
            {-70, -200, 140, 200}
        },
        {
            {9, 21},
            {10.0f, -110.0f, 200.0f, 110.0f}
        }
    },
    {
        {
            {1, 8},
            {15.0f, 0.0f}
        },
        {
            {22, 29},
            {-15.0f, 0.0f}
        }
    })
{
}

template Action<CHAR1_STATE, Char1Data, Char1>;
template Action_jump<CHAR1_STATE, Char1Data, Char1>;
template Action_prolonged<CHAR1_STATE, Char1Data, Char1>;