#ifndef _Timer_h_
#define _Timer_h_

#include "Common.h"

class Timer
{
public:
	Timer();
	~Timer() {}

	void start();
	void stop();

	// Call this once per-frame.
	void update();

	// Returns 'seconds' per frame.
	float getTimeElapsed() const;

	uint getFrameRate() const;

private:
	uint m_lastTime;
	float m_timeElapsed;
	bool m_running;

	uint m_frameRate;
};

#endif // _Timer_h_