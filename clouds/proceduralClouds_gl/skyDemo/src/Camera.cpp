#include "Camera.h"

#include "Constants.h"
#include "GlobalTimer.h"

#include <cmath>

Camera::Camera(vector3 startPos)
{
	m_pos = startPos;
	m_rightVec.set(1, 0, 0);
	m_upVec.set(0, 1, 0);
	m_lookVec.set(0, 0, -1);

	m_viewMtx.identity();
}

Camera::~Camera()
{
}

void Camera::pitch(int val)
{
	matrix44 rotMtx;
	rotMtx.identity();

	// Rotate around rightVec.
	float rad = ((float)val * GlobalTimer::getSingleton().getTimeElapsed() * PI) / 180.0f;
	rotMtx = RotateRadMatrix44(m_rightVec, rad);

	m_lookVec = m_lookVec * rotMtx;
	m_upVec = m_upVec * rotMtx;
}

void Camera::yaw(int val)
{
	matrix44 rotMtx;
	rotMtx.identity();

	// Rotate around world up.
	vector3 worldUp(0, 1, 0);
	float rad = ((float)val * GlobalTimer::getSingleton().getTimeElapsed() * PI) / 180.0f;
	rotMtx = RotateRadMatrix44(worldUp, rad);

	m_lookVec = m_lookVec * rotMtx;
	m_upVec = m_upVec * rotMtx;
	m_rightVec = m_rightVec * rotMtx;
}