#ifndef ACTION_RESOLVER_H_
#define ACTION_RESOLVER_H_

#include "FixedQueue.h"
#include <map>
#include "Actions.h"
#include "InputState.h"
#include "InputSystem.h"
#include <vector>

template <typename CharState_t, typename CharData>
class ActionResolver : public InputReactor
{
public:
    ActionResolver(InputSystem *input_);

    void subscribe_p1();
    void subscribe_p2();
    void unsubscribe_all();
    void receiveInput(EVENTS event_, const float scale_) override;

    virtual void createActions() = 0;

    const Action<CharState_t, CharData> *update(Char1Data charData);

protected:
    std::vector<std::unique_ptr<Action<CharState_t, CharData>>> m_actions;
    InputQueue m_inputQueue;
    InputState m_currentInput;
};

#endif