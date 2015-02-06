#ifndef _GestureSystem_H_
#define _GestureSystem_H_

#include "Common.h"

#include <list>
#include <vector>

#include "vector_math.h"

class GestureSystem
{
public:
	GestureSystem();
	~GestureSystem() {}

	typedef vec2<float> Point2D;

	uint getBestMatch(const std::vector< Point2D > &points, float *bestVal = NULL);

private:
	typedef std::vector< Point2D > Stroke;
	typedef std::vector< Stroke > Gesture;

	std::vector< Gesture > m_gestureList;

	void normalizeSize(Gesture &gesture);
	void normalizeSpacing(Stroke &newStroke, const Stroke &oldStroke);
	void normalizeCenter(Gesture &gesture);

	float getStrokeLength(const Stroke &stroke);
	float gestureDotProduct(const Gesture &gesture1, const Gesture &gesture2);
	float match(const Gesture &gesture1, const Gesture &gesture2 );

	const uint m_pointsPerStroke;
};

#endif // _GestureSystem_H_