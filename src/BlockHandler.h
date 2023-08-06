#ifndef BLOCK_HANDLER_H_
#define BLOCK_HANDLER_H_

#include "Vector2.h"
#include "InputState.h"
#include "FrameTimer.h"
#include "GameData.h"

/*
 *
 * HIGH - while holding back
 * LOW - while holding downback
 * AIR - while holding any back direction in the air
 * 
 * 
*/
enum class BLOCK_STATE {HIGH, LOW, AIR, AUTO, NONE};
enum class BLOCK_FRAME {INSTANT, NONE};

class BlockHandler
{
public:
    BlockHandler();
    void update(const Vector2<int> &inputDir_, INPUT_BUTTON_STATE backState_, bool airborne_, const Vector2<float> &vecToEnemy_, bool inBlockStun_, bool canBlock_);
    BLOCK_STATE getBlockState() const;
    BLOCK_FRAME getBlockFrame() const;
    void resetBlockFrame();
    int getBlockstunDuration(int baseDuration_) const;
    

private:
    void enterInstantBlock();
    void enterIBAttemptDelay();

    BLOCK_STATE m_currentBlockState;
    Vector2<float> m_lastVecToEnemy;

    FrameTimer m_frameTimer;
    BLOCK_FRAME m_currentBlockFrame = BLOCK_FRAME::NONE;

    FrameTimer m_blockScalingTimer;

};

#endif