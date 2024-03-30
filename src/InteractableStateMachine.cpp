#include "InteractableStateMachine.h"
#include "Character.h"

bool InteractableStateMachine::isAirborne() const
{
    return m_airborne;
}

void InteractableStateMachine::turnVelocityToInertia(float horMultiplier_)
{
    m_inertia.x += m_velocity.x * horMultiplier_;
    m_inertia.y += m_velocity.y;
    m_velocity = {0.0f, 0.0f};
}

bool InteractableStateMachine::canApplyDrag() const
{
    return true;
}

bool InteractableStateMachine::canBeDraggedByInertia() const
{
    return true;
}

InteractableStateMachine::InteractableStateMachine(Application &application_, Vector2<float> pos_, int stateCnt_, Camera *cam_) :
    m_pos(pos_),
    m_actionResolver(application_.getInputSystem()),
    m_currentAction(nullptr),
    m_currentState(0),
    m_playerId(0),
    m_currentAnimation(nullptr),
    m_cam(cam_)
{
}

Vector2<float> InteractableStateMachine::getPos() const
{
    return m_pos;
}


void InteractableStateMachine::setPos(Vector2<float> pos_)
{
    //std::cout << "setPos to " << pos_ << std::endl;
    m_pos = pos_;
}

Vector2<float> InteractableStateMachine::getVelocity() const
{
    return m_velocity;
}

Vector2<float> InteractableStateMachine::getFullVelocity() const
{
    return m_velocity + m_inertia;
}

Vector2<float> InteractableStateMachine::getInertia() const
{
    return m_inertia;
}

void InteractableStateMachine::setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_)
{
    m_timer.begin(0);

    m_playerId = playerId_;
    if (m_playerId == 1)
    {
        m_dirToEnemy = ORIENTATION::RIGHT;
        m_ownOrientation = ORIENTATION::RIGHT;
    }
    else
    {
        m_dirToEnemy = ORIENTATION::LEFT;
        m_ownOrientation = ORIENTATION::LEFT;
    }

    m_otherCharacter = otherCharacter_;
    m_priorityHandler = priorityHandler_;

    loadAnimations(application_);

    initiate();
}

void InteractableStateMachine::updateOwnOrientation()
{
    m_ownOrientation = m_dirToEnemy;
}

void InteractableStateMachine::updateDirToEnemy()
{
    auto vecToEnemy = getHorDirToEnemy();
    if (vecToEnemy.x > 0)
        m_dirToEnemy = ORIENTATION::RIGHT;
    else if (vecToEnemy.x < 0)
        m_dirToEnemy = ORIENTATION::LEFT;
}

Vector2<float> InteractableStateMachine::getHorDirToEnemy() const
{
    return Vector2(m_otherCharacter->getPos().x - getPos().x, 0.0f).normalised();
}

Vector2<float> InteractableStateMachine::getOwnHorDir() const
{
    return Vector2<float>{(m_ownOrientation == ORIENTATION::RIGHT ? 1.0f : -1.0f), 0.0f};
}

ORIENTATION InteractableStateMachine::getOwnOrientation() const
{
    return m_ownOrientation;
}

ORIENTATION InteractableStateMachine::getOrientationToEnemy() const
{
    return m_dirToEnemy;
}

ORIENTATION InteractableStateMachine::getInputDir() const
{
    auto res = m_ownOrientation;
    return m_ownOrientation;
}

void InteractableStateMachine::switchTo(int state_)
{
    m_actionResolver.getAction(state_)->switchTo(*this);
}

void InteractableStateMachine::callForPriority()
{
    m_priorityHandler->callForPriority(m_playerId);
}

bool InteractableStateMachine::isInHitstop() const
{
    return m_inHitstop;
}

void InteractableStateMachine::applyCancelWindow(CancelWindow cw_)
{
    if (cw_.first.first == 0 && cw_.first.second == 0)
    {
        m_currentCancelWindow = cw_;
        m_cancelAvailable = false;
        m_cancelTimer.begin(0);
    }

    m_currentCancelWindow.first = std::move(cw_.first);
    m_currentCancelWindow.second = std::move(cw_.second);
    m_cancelAvailable = false;
    if (m_currentCancelWindow.second.size() > 0)
        m_cancelTimer.begin(m_currentCancelWindow.first.first);
    else
        m_cancelTimer.begin(0);
}

bool InteractableStateMachine::isCancelAllowed(int cancelTarget_)
{
    return m_cancelAvailable && m_currentCancelWindow.second.contains(cancelTarget_);
}

void InteractableStateMachine::proceedCurrentState()
{
    if (!m_inHitstop)
    {
        auto timerRes = m_timer.update();

        if (timerRes)
        {
            if (m_currentAction)
                    m_currentAction->outdated(*this);

        }
    }
}

void InteractableStateMachine::updateState()
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

HurtboxVec InteractableStateMachine::getHurtboxes()
{
    auto currentFrame = m_timer.getCurrentFrame() + 1;
    return m_currentAction->getCurrentHurtboxes(currentFrame, m_pos, m_ownOrientation);
}

void InteractableStateMachine::updatePosition()
{
    if (m_inHitstop)
        return;

    // Apply velocity and inertia
    auto posAddition = m_velocity;
    if (canBeDraggedByInertia())
        posAddition += m_inertia;
    setPos(m_pos + posAddition);

    // Define if character is airborne
    if (m_pos.y < gamedata::stages::levelOfGround)
    {
        m_airborne = true;
    }
    else if (m_airborne && m_pos.y >= gamedata::stages::levelOfGround)
    {
        m_inertia.y = 0;
        m_airborne = false;
        m_pos.y = gamedata::stages::levelOfGround;
    }

    // Apply drag for grounded character
    if (m_inertia.x != 0.0f && !m_airborne && canApplyDrag())
    {
        auto absInertia = abs(m_inertia.x);
        auto m_inertiaSign = m_inertia.x / abs(m_inertia.x);
        absInertia = std::max(absInertia - m_inertiaDrag, 0.0f);
        m_inertia.x = m_inertiaSign * absInertia;
    }
    
    m_currentAnimation->update();
}