#include "BlockHandler.h"

BlockHandler::BlockHandler()
{

}


void BlockHandler::update(const Vector2<int> &inputDir_, bool airborne_, const Vector2<float> &vecToEnemy_, bool inBlockStun_, bool canBlock_)
{
    m_lastVecToEnemy = vecToEnemy_;

    m_currentBlockState = BLOCK_STATE::NONE;

    if (!canBlock_)
        return;

    if (inBlockStun_)
    {
        if (airborne_)
        {
            m_currentBlockState = BLOCK_STATE::AIR;
            return;
        }
        else
        {
            m_currentBlockState = BLOCK_STATE::AUTO;
        }
        
    }
        

    auto simplifiedDir = inputDir_;
    simplifiedDir.x *= vecToEnemy_.x;

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
}

BLOCK_STATE BlockHandler::getBlockState() const
{
    return m_currentBlockState;
}