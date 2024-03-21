#ifndef ACTION_RESOLVER_H_
#define ACTION_RESOLVER_H_

#include "FixedQueue.h"
#include <map>
#include "Actions.h"
#include "InputState.h"
#include "InputSystem.h"
#include <vector>

template <typename CharState_t, typename Char_t>
class ActionResolver : public InputReactor
{
public:
    ActionResolver(InputSystem *input_);

    void subscribe_p1();
    void subscribe_p2();
    void unsubscribe_all();
    void receiveInput(EVENTS event_, const float scale_) override;
    Action<CharState_t, Char_t> *getAction(CharState_t state_) const;

    Vector2<int> getCurrentInputDir() const;
    INPUT_BUTTON_STATE getPostFrameButtonState(INPUT_BUTTON button_) const;

    virtual void addAction(std::unique_ptr<Action<CharState_t, Char_t>> &&action_);

    const Action<CharState_t, Char_t> *update(Char_t *char_, int extendBuffer_ = 0);

protected:
    std::vector<std::unique_ptr<Action<CharState_t, Char_t>>> m_actions;
    InputQueue m_inputQueue;
    InputState m_currentInput;
};

#endif