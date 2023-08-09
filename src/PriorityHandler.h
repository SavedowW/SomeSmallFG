#ifndef PRIORITY_HANDLER_H_
#define PRIORITY_HANDLER_H_

#include <array>
#include <iostream>

class PriorityHandler
{
public:
    void callForPriority(int playerID_);
    const std::array<int, 2> &getUpdatedPriority();
    const std::array<int, 2> &getCurrentPriority() const;

private:
    std::array<int, 2> m_order {0, 1};
    std::array<bool, 2> m_calledForPriority {false, false};

};

#endif