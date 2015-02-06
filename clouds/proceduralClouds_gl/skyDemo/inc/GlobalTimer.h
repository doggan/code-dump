//--------------------------------------------------------------------
// Name: Shyam M Guthikonda
// Date: 12.11.04
// File: GlobalTimer.h
// Desc: Main timer. Used to synchronize AI/Rendering/etc on all platforms.
//--------------------------------------------------------------------

#ifndef _CGLOBALTIMER_H_
#define _CGLOBALTIMER_H_

#include "Singleton.h"

const unsigned long MAX_SAMPLE_COUNT = 50;

class GlobalTimer : public Singleton< GlobalTimer >
{
public:
	GlobalTimer( float lockFPS );
	virtual ~GlobalTimer();

	bool start();
	void update();
	void stop();

	float getTimeElapsed() const;
	unsigned long getFrameRate() const;

private:
	float m_timeScale;		// Amount to scale counter.
	float m_timeElapsed;	// Time elapsed since previous frame.
	__int64 m_currentTime;	// Current Performance Counter.
	__int64 m_lastTime;		// Performance Counter last frame.
	__int64 m_perfFreq;		// Performance Frequency.

	unsigned long m_frameRate;			// Stores current framerate
	unsigned long m_FPSFrameCount;		// Elapsed frames in any given second
	float m_FPSTimeElapsed;				// How much time has passed during FPS sample
	float m_lockFPS;

	float m_frameTime[MAX_SAMPLE_COUNT];
    unsigned long m_sampleCount;
};

#endif // _CGLOBALTIMER_H_