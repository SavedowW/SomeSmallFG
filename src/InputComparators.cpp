#include "InputComparators.h"

bool InputComparatorIdle::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    return true;
}

bool InputComparatorForward::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    switch (faceDirection_)
    {
        case (ORIENTATION::RIGHT):
            return (inputQueue_[0].dir == Vector2{1.0f, 0.0f});
            break;

        case (ORIENTATION::LEFT):
            return (inputQueue_[0].dir == Vector2{-1.0f, 0.0f});
            break;
    }

    return false;
}

bool InputComparatorBackward::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    switch (faceDirection_)
    {
        case (ORIENTATION::RIGHT):
            return (inputQueue_[0].dir == Vector2{-1.0f, 0.0f});
            break;

        case (ORIENTATION::LEFT):
            return (inputQueue_[0].dir == Vector2{1.0f, 0.0f});
            break;
    }

    return false;
}

bool InputComparatorUpPress::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    auto lastInput = inputQueue_[0];

    return (lastInput.dir == Vector2{0.0f, -1.0f} && lastInput.inputs[INPUT_BUTTON::UP] == INPUT_BUTTON_STATE::PRESSED);
}

bool InputComparatorUpHold::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    return (inputQueue_[0].dir == Vector2{0.0f, -1.0f});
}

bool InputComparatorUpForwardPress::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    auto lastInput = inputQueue_[0];

    switch (faceDirection_)
    {
        case (ORIENTATION::RIGHT):
            return (lastInput.dir == Vector2{1.0f, -1.0f} && lastInput.inputs[INPUT_BUTTON::UP] == INPUT_BUTTON_STATE::PRESSED);
            break;

        case (ORIENTATION::LEFT):
            return (lastInput.dir == Vector2{-1.0f, -1.0f} && lastInput.inputs[INPUT_BUTTON::UP] == INPUT_BUTTON_STATE::PRESSED);
            break;
    }

    return false;
}

bool InputComparatorUpForwardHold::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    auto lastInput = inputQueue_[0];

    switch (faceDirection_)
    {
        case (ORIENTATION::RIGHT):
            return (lastInput.dir == Vector2{1.0f, -1.0f});
            break;

        case (ORIENTATION::LEFT):
            return (lastInput.dir == Vector2{-1.0f, -1.0f});
            break;
    }

    return false;
}

bool InputComparatorUpBackwardPress::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    auto lastInput = inputQueue_[0];

    switch (faceDirection_)
    {
        case (ORIENTATION::RIGHT):
            return (lastInput.dir == Vector2{-1.0f, -1.0f} && lastInput.inputs[INPUT_BUTTON::UP] == INPUT_BUTTON_STATE::PRESSED);
            break;

        case (ORIENTATION::LEFT):
            return (lastInput.dir == Vector2{1.0f, -1.0f} && lastInput.inputs[INPUT_BUTTON::UP] == INPUT_BUTTON_STATE::PRESSED);
            break;
    }

    return false;
}

bool InputComparatorUpBackwardHold::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    auto lastInput = inputQueue_[0];

    switch (faceDirection_)
    {
        case (ORIENTATION::RIGHT):
            return (lastInput.dir == Vector2{-1.0f, -1.0f});
            break;

        case (ORIENTATION::LEFT):
            return (lastInput.dir == Vector2{1.0f, -1.0f});
            break;
    }

    return false;
}

bool InputComparator66::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() <= 1)
        return false;

    auto lastInput = inputQueue_[0];
    int inputsToParse = std::min(10, inputQueue_.getFilled());

    Vector2<float> forwardVec;
    forwardVec.x = (faceDirection_ == ORIENTATION::RIGHT ? 1.0f : -1.0f);
    Vector2<float> upForwardVec = {forwardVec.x, -1.0f};
    INPUT_BUTTON forwardButton = (faceDirection_ == ORIENTATION::RIGHT ? INPUT_BUTTON::RIGHT : INPUT_BUTTON::LEFT);

    if (!(lastInput.dir == forwardVec) || !(lastInput.inputs[forwardButton] == INPUT_BUTTON_STATE::PRESSED))
        return false;
    

    for (int i = 1; i < inputsToParse; ++i)
    {
        const auto &in = inputQueue_[i];
        if ((in.dir == forwardVec || in.dir == upForwardVec) && in.inputs.at(forwardButton) == INPUT_BUTTON_STATE::PRESSED)
            return true;
    }

    return false;
}

bool InputComparatorAPress::operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const
{
    if (inputQueue_.getFilled() == 0)
        return false;

    auto lastInput = inputQueue_[0];

    return lastInput.inputs.at(INPUT_BUTTON::A) == INPUT_BUTTON_STATE::PRESSED;
}