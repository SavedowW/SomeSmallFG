#include "InputState.h"

bool InputState::isInputActive(const INPUT_BUTTON &btn) const
{
    return (inputs.at(btn) == INPUT_BUTTON_STATE::PRESSED || inputs.at(btn) == INPUT_BUTTON_STATE::HOLD);
}

void InputState::setDirFromButtons()
{
    dir = Vector2{0.0f, 0.0f};
    dir.x += (isInputActive(INPUT_BUTTON::RIGHT) ? 1 : 0);
    dir.x -= (isInputActive(INPUT_BUTTON::LEFT) ? 1 : 0);

    dir.y += (isInputActive(INPUT_BUTTON::DOWN) ? 1 : 0);
    dir.y -= (isInputActive(INPUT_BUTTON::UP) ? 1 : 0);
}

InputState InputState::getNextFrameState() const
{
    InputState newState(*this);
    for (auto &el : newState.inputs)
    {
        if (el.second == INPUT_BUTTON_STATE::PRESSED)
            el.second = INPUT_BUTTON_STATE::HOLD;
        else if (el.second == INPUT_BUTTON_STATE::RELEASED)
            el.second = INPUT_BUTTON_STATE::OFF;
    }

    return newState;
}

InputState::InputState(const InputState &inputState_)
{
    dir = inputState_.dir;
    inputs = inputState_.inputs;
}

InputState &InputState::operator=(const InputState &rhs_)
{
    dir = rhs_.dir;
    inputs = rhs_.inputs;
    return *this;
}

InputState::InputState(InputState &&inputState_)
{
    dir = inputState_.dir;
    inputs = std::forward<std::map<INPUT_BUTTON, INPUT_BUTTON_STATE>>(inputState_.inputs);
}

InputState &InputState::operator=(InputState &&rhs_)
{
    dir = rhs_.dir;
    inputs = std::forward<std::map<INPUT_BUTTON, INPUT_BUTTON_STATE>>(rhs_.inputs);
    return *this;
}