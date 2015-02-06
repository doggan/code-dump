#include "GlobalTimer.h"

// TODO: cross-platform issue !
#include <Windows.h>

#include <cmath>

GlobalTimer::GlobalTimer( float lockFPS )
{
	m_lockFPS = lockFPS;
}

GlobalTimer::~GlobalTimer()
{
}

bool GlobalTimer::start()
{
	// Query performance hardware and setup time scaling values.
	QueryPerformanceFrequency( (LARGE_INTEGER *)&m_perfFreq );
	QueryPerformanceCounter( (LARGE_INTEGER *)&m_lastTime );
	m_timeScale = 1.0f / m_perfFreq;

	// Clear any needed values.
	m_frameRate	= 0;
	m_FPSFrameCount	= 0;
	m_FPSTimeElapsed = 0.0;
	m_sampleCount = 0;
	m_timeElapsed = 0.0;

	return true;
}

void GlobalTimer::update()
{
	// Query high-resolution performance hardware.
	QueryPerformanceCounter((LARGE_INTEGER *)&m_currentTime);

	float timeElapsed;

	// Calculate elapsed time in seconds.
	timeElapsed = (m_currentTime - m_lastTime) * m_timeScale;

    // Should we lock the frame rate ?
    if ( m_lockFPS > 0.0f ) {
        while ( timeElapsed < (1.0f / m_lockFPS) ) {

			// Query high-resolution performance hardware
			QueryPerformanceCounter((LARGE_INTEGER *)&m_currentTime);

	        // Calculate elapsed time in seconds.
	        timeElapsed = (m_currentTime - m_lastTime) * m_timeScale;
        }
    } // End if lock frame rate.

	// Save current frame time.
	m_lastTime = m_currentTime;

    // Filter out values wildly different from current average.
    if ( fabsf( timeElapsed - m_timeElapsed ) < 1.0f  ) {
        // Wrap FIFO frame time buffer.
        memmove( &m_frameTime[1], m_frameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float) );
        m_frameTime[ 0 ] = timeElapsed;
        if ( m_sampleCount < MAX_SAMPLE_COUNT )
			m_sampleCount++;
    } // End if

	// Calculate Frame Rate
	m_FPSFrameCount++;
	m_FPSTimeElapsed += m_timeElapsed;

	if ( m_FPSTimeElapsed > 1.0f) {
		m_frameRate	= m_FPSFrameCount;
		m_FPSFrameCount	= 0;
		m_FPSTimeElapsed = 0.0f;
	} // End If Second Elapsed

	// Count up the new average elapsed time
    m_timeElapsed = 0.0f;
	for ( unsigned long i = 0; i < m_sampleCount; i++ ) {
		m_timeElapsed += m_frameTime[ i ];
	}

    if ( m_sampleCount > 0 )
		m_timeElapsed /= m_sampleCount;
}

void GlobalTimer::stop()
{

}

unsigned long GlobalTimer::getFrameRate() const
{
    return m_frameRate;
}

float GlobalTimer::getTimeElapsed() const
{
    return m_timeElapsed;

}