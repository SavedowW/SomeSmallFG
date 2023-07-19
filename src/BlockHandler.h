#ifndef BLOCK_HANDLER_H_
#define BLOCK_HANDLER_H_

#include "Vector2.h"

/*
 *
 * HIGH - while holding back
 * LOW - while holding downback
 * AIR - while holding any back direction in the air
 * 
 * 
*/
enum class BLOCK_STATE {HIGH, LOW, AIR, AUTO, NONE};

class BlockHandler
{
public:
    BlockHandler();
    void update(const Vector2<int> &inputDir_, bool airborne_, const Vector2<float> &vecToEnemy_, bool inBlockStun_, bool canBlock_);

private:
    BLOCK_STATE m_currentBlockState;
    Vector2<float> m_lastVecToEnemy;
};

#endif