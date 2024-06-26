#ifndef ACTION_RESOLVER_H_
#define ACTION_RESOLVER_H_

#include "FixedQueue.h"
#include <map>
#include "Actions.h"
#include "InputState.h"
#include "InputSystem.h"
#include <vector>

class InteractableStateMachine;

class ActionResolver : public InputReactor
{
public:
    ActionResolver(InputSystem *input_);

    void subscribe_p1();
    void subscribe_p2();
    void unsubscribe_all();
    void receiveInput(EVENTS event_, const float scale_) override;
    Action *getAction(int state_) const;
    InputQueue &getInputQueue();

    Vector2<int> getCurrentInputDir() const;
    INPUT_BUTTON_STATE getPostFrameButtonState(INPUT_BUTTON button_) const;

    virtual void addAction(std::unique_ptr<Action> &&action_);

    Action *update(InteractableStateMachine *char_, int extendBuffer_ = 0);

protected:
    std::vector<std::unique_ptr<Action>> m_actions;
    InputQueue m_inputQueue;
    InputState m_currentInput;
};

#endif