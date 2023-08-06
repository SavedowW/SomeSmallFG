#include "BlockHandler.h"

BlockHandler::BlockHandler()
{

}


void BlockHandler::update(const Vector2<int> &inputDir_, INPUT_BUTTON_STATE backState_, bool airborne_, const Vector2<float> &vecToEnemy_, bool inBlockStun_, bool canBlock_)
{
    m_lastVecToEnemy = vecToEnemy_;
    auto simplifiedDir = inputDir_;
    simplifiedDir.x *= vecToEnemy_.x;

    if (backState_ == INPUT_BUTTON_STATE::PRESSED)
        std::cout << "";

    if (m_frameTimer.update())
    {
        if (m_currentBlockFrame == BLOCK_FRAME::INSTANT)
        {
            enterIBAttemptDelay();
        }
        else
        {
            resetBlockFrame();
        }
    }

    if (!m_frameTimer.isActive() && simplifiedDir.x == -1 && backState_ == INPUT_BUTTON_STATE::PRESSED)
    {
        enterInstantBlock();
    }
    else if (simplifiedDir.x != -1 && m_currentBlockFrame == BLOCK_FRAME::INSTANT)
    {
        resetBlockFrame();
    }

    m_currentBlockState = BLOCK_STATE::NONE;

    if (!canBlock_)
    {
        m_blockScalingTimer.begin(0);
        return;
    }

    if (inBlockStun_)
    {
        if (airborne_)
        {
            m_currentBlockState = BLOCK_STATE::AIR;
        }
        else
        {
            m_currentBlockState = BLOCK_STATE::AUTO;
        }
        
    }

    if (!airborne_)
    {
        if (simplifiedDir == Vector2{-1, 0})
            m_currentBlockState = BLOCK_STATE::HIGH;
        else if (simplifiedDir == Vector2{-1, 1})
            m_currentBlockState = BLOCK_STATE::LOW;
    }
    else
    {
        if (simplifiedDir.x == -1)
        {
            m_currentBlockState = BLOCK_STATE::AIR;
        }
    }

    if (m_currentBlockState != BLOCK_STATE::NONE)
    {
        if (!m_blockScalingTimer.isOver() && !m_blockScalingTimer.isActive())
            m_blockScalingTimer.begin(20);
        else
            m_blockScalingTimer.update();
    }
    else
    {
        m_blockScalingTimer.begin(0);
    }

}

BLOCK_STATE BlockHandler::getBlockState() const
{
    return m_currentBlockState;
}

BLOCK_FRAME BlockHandler::getBlockFrame() const
{
    if (m_currentBlockFrame != BLOCK_FRAME::NONE && m_frameTimer.isActive())
        return m_currentBlockFrame;
    
    return BLOCK_FRAME::NONE;
}

void BlockHandler::resetBlockFrame()
{
    std::cout << "IB possible\n";
    m_frameTimer.begin(0);
    m_currentBlockFrame = BLOCK_FRAME::NONE;
}

void BlockHandler::enterInstantBlock()
{
    std::cout << "IB state\n";
    m_currentBlockFrame = BLOCK_FRAME::INSTANT;
    m_frameTimer.begin(gamedata::characters::instantBlockWindow);
}

void BlockHandler::enterIBAttemptDelay()
{
    std::cout << "IB attempt delay\n";
    m_currentBlockFrame = BLOCK_FRAME::NONE;
    m_frameTimer.begin(gamedata::characters::instantBlockAttemptDelay);
}

int BlockHandler::getBlockstunDuration(int baseDuration_) const
{
    if (m_currentBlockFrame == BLOCK_FRAME::INSTANT)
        return gamedata::characters::finalBlockstunScaling * baseDuration_;

    auto alpha = gamedata::characters::initialBlockstunScaling - m_blockScalingTimer.getProgressNormalized() * (gamedata::characters::blockstunScalingDiff);
    return alpha * baseDuration_;
}