#include "ProjectileFactory.h"
#include "ActionResolver.h"
#include "Char1.h"
#include "Projectile.h"

ActionResolver::ActionResolver(InputSystem *input_) :
    InputReactor(input_)
{

}

void ActionResolver::subscribe_p1()
{
    //std::cout << "Subscribed to p1\n";
    subscribe(EVENTS::UP_P1);
    subscribe(EVENTS::RIGHT_P1);
    subscribe(EVENTS::DOWN_P1);
    subscribe(EVENTS::LEFT_P1);
    subscribe(EVENTS::START_P1);
    subscribe(EVENTS::SELECT_P1);
    subscribe(EVENTS::A_P1);
    subscribe(EVENTS::B_P1);
    subscribe(EVENTS::C_P1);
    subscribe(EVENTS::S_P1);
}

void ActionResolver::subscribe_p2()
{
    //std::cout << "Subscribed to p2\n";
    subscribe(EVENTS::UP_P2);
    subscribe(EVENTS::RIGHT_P2);
    subscribe(EVENTS::DOWN_P2);
    subscribe(EVENTS::LEFT_P2);
    subscribe(EVENTS::START_P2);
    subscribe(EVENTS::SELECT_P2);
    subscribe(EVENTS::A_P2);
    subscribe(EVENTS::B_P2);
    subscribe(EVENTS::C_P2);
    subscribe(EVENTS::S_P2);
}

void ActionResolver::unsubscribe_all()
{
    unsubscribeFromAll();
}

void ActionResolver::receiveInput(EVENTS event_, const float scale_)
{
    switch(event_)
    {
        case (EVENTS::A_P1):
            [[fallthrough]];
        case (EVENTS::A_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::A] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            break;

        case (EVENTS::B_P1):
            [[fallthrough]];
        case (EVENTS::B_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::B] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            break;

        case (EVENTS::C_P1):
            [[fallthrough]];
        case (EVENTS::C_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::C] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            break;

        case (EVENTS::S_P1):
            [[fallthrough]];
        case (EVENTS::S_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::S] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            break;

        case (EVENTS::UP_P1):
            [[fallthrough]];
        case (EVENTS::UP_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::UP] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            std::cout << "UP RECEIVED!" << std::endl;
            break;

        case (EVENTS::DOWN_P1):
            [[fallthrough]];
        case (EVENTS::DOWN_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::DOWN] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            break;

        case (EVENTS::LEFT_P1):
            [[fallthrough]];
        case (EVENTS::LEFT_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::LEFT] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            break;

        case (EVENTS::RIGHT_P1):
            [[fallthrough]];
        case (EVENTS::RIGHT_P2):
            m_currentInput.m_inputs[INPUT_BUTTON::RIGHT] = (scale_ == 1.0f ? INPUT_BUTTON_STATE::PRESSED : INPUT_BUTTON_STATE::RELEASED);
            break;
    }
}

Action *ActionResolver::update(InteractableStateMachine *char_, int extendBuffer_)
{
   //std::cout << "=== UPDATE RESOLVER ===\n";

    m_currentInput.setDirFromButtons();

    m_inputQueue.push(m_currentInput);
    Action *availableAction = nullptr;
    for (auto &el : m_actions)
    {
        auto res = el->isPossible(m_inputQueue, char_, extendBuffer_);

        // If this action is already active
        if (res == -1)
            break; // Dont look for another action

        // It will not lock player in idle since it has lowest priority (last in m_actions vector)

        // If this action is possible
        if (res == 1)
        {
            availableAction = el.get();
            break;
        }
    }

    m_currentInput = m_currentInput.getNextFrameState();

    return availableAction;
}

Vector2<int> ActionResolver::getCurrentInputDir() const
{
    return m_currentInput.m_dir;
}

Action *ActionResolver::getAction(int state_) const
{
    for (auto &el : m_actions)
        if (el->actionState == state_)
            return el.get();

    return nullptr;
}

InputQueue &ActionResolver::getInputQueue()
{
    return m_inputQueue;
}

INPUT_BUTTON_STATE ActionResolver::getPostFrameButtonState(INPUT_BUTTON button_) const
{
    //return m_currentInput.inputs.at(button_);
    if (m_inputQueue.getFilled() >= 1)
    {
        return m_inputQueue[0].m_inputs.at(button_);
    }

    return INPUT_BUTTON_STATE::OFF;
}

void ActionResolver::addAction(std::unique_ptr<Action> &&action_)
{
    m_actions.push_back(std::move(action_));
}