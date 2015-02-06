#ifndef _Camera_h_
#define _Camera_h_

#include "MtxLib.h"

using namespace _mtxCM;

class Camera
{
public:
	Camera(vector3 startPos);
	virtual ~Camera();

	vector3 getPosition() { return m_pos; }
	vector3 getLookAt() { return m_lookVec; }
	vector3 getUp() { return m_upVec; }

	void pitch(int val);
	void yaw(int val);

private:
	vector3 m_rightVec, m_upVec, m_lookVec;
	vector3 m_pos;
	matrix44 m_viewMtx;
};

#endif // _Camera_h_