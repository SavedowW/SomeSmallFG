#include "PriorityHandler.h"

void PriorityHandler::callForPriority(int playerID_)
{
    if (playerID_ < 1 || playerID_ > 2)
    {
        std::cout << "Wrong player id for priority: " << playerID_ << std::endl;
        return;
    }
        
    m_calledForPriority[playerID_ - 1] = true;
}

const std::array<int, 2> &PriorityHandler::getUpdatedPriority()
{
    if (m_calledForPriority[0] && !m_calledForPriority[1])
        m_order = {0, 1};
    else if (m_calledForPriority[1] && !m_calledForPriority[0])
        m_order = {1, 0};

    m_calledForPriority = {false, false};

    return m_order;
}

const std::array<int, 2> &PriorityHandler::getCurrentPriority() const
{
    return m_order;
}