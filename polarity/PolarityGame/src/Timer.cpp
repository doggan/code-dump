#include "Timer.h"

#include "SDL.h"

Timer::Timer()
{
	m_lastTime = 0;
	m_timeElapsed = 0;

	m_running = false;
}

void Timer::start()
{
	m_lastTime = SDL_GetTicks();

	m_running = true;
}

void Timer::stop()
{
	m_running = false;
}

void Timer::update()
{
	if (m_running)
	{
		uint currentTime = SDL_GetTicks();

		// Convert from ms to seconds.
		m_timeElapsed = (float)(currentTime - m_lastTime) / 1000.0f;

		// Save current frame time.
		m_lastTime = currentTime;

		// FPS calculation.
		static float fpsTimeElapsed = 0;
		static uint fpsFrameCount = 0;

		fpsTimeElapsed += m_timeElapsed;
		++fpsFrameCount;

		if (fpsTimeElapsed > 1.0f)
		{
			m_frameRate = fpsFrameCount;
			fpsFrameCount = 0;
			fpsTimeElapsed = 0;
		}
	}
}

float Timer::getTimeElapsed() const
{
    return m_timeElapsed;
}

uint Timer::getFrameRate() const
{
	return m_frameRate;
}