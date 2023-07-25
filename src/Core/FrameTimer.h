#ifndef FRAME_TIMER_H_
#define FRAME_TIMER_H_
#include <cstdint>

class FrameTimer
{
public:
	FrameTimer(uint32_t framesToWait_ = 0);
	void begin(uint32_t framesToWait_);
    bool update();
	bool isOver() const;
	uint32_t getCurrentFrame() const;
	float getProgressNormalized() const;

private:
	uint32_t m_framesToWait;
    uint32_t m_currentFrame;
};

#endif