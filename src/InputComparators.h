#ifndef INPUT_COMPARATORS_H_
#define INPUT_COMPARATORS_H_

#include "FixedQueue.h"
#include "Vector2.h"
#include "InputState.h"

using InputQueue = FixedQueue<InputState, 30>;
enum class ORIENTATION {RIGHT, LEFT};

class InputComparator
{
public:
    virtual bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const = 0;
};

class InputComparatorIdle : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorForward : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorBackward : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorUpPress : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorUpHold : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorUpForwardPress : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorUpForwardHold : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorUpBackwardPress : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorUpBackwardHold : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparator66 : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

class InputComparatorAPress : public InputComparator
{
public:
    bool operator()(const InputQueue &inputQueue_, ORIENTATION faceDirection_) const final;
};

using InputComparator_ptr = std::unique_ptr<InputComparator>;

#endif