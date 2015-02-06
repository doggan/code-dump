#include "GestureSystem.h"

#include <cmath>
#include <cfloat>

#define ADD_PT(x, y)	s.push_back(Point2D((float)x, (float)y))

GestureSystem::GestureSystem()
: m_pointsPerStroke(32)
{
	Gesture g;
	Stroke s;

	// Left-to-Right horizontal line.
	ADD_PT(-33.500031, 0.000000);
	ADD_PT(-31.338745, 0.000000);
	ADD_PT(-29.177460, 0.000000);
	ADD_PT(-27.016174, 0.000000);
	ADD_PT(-24.854858, 0.000000);
	ADD_PT(-22.693573, 0.000000);
	ADD_PT(-20.532288, 0.000000);
	ADD_PT(-18.371002, 0.000000);
	ADD_PT(-16.209717, 0.000000);
	ADD_PT(-14.048431, 0.000000);
	ADD_PT(-11.887115, 0.000000);
	ADD_PT(-9.725830, 0.000000);
	ADD_PT(-7.564545, 0.000000);
	ADD_PT(-5.403259, 0.000000);
	ADD_PT(-3.241974, 0.000000);
	ADD_PT(-1.080658, 0.000000);
	ADD_PT(1.080627, 0.000000);
	ADD_PT(3.241913, 0.000000);
	ADD_PT(5.403198, 0.000000);
	ADD_PT(7.564484, 0.000000);
	ADD_PT(9.725769, 0.000000);
	ADD_PT(11.887054, 0.000000);
	ADD_PT(14.048370, 0.000000);
	ADD_PT(16.209656, 0.000000);
	ADD_PT(18.370941, 0.000000);
	ADD_PT(20.532227, 0.000000);
	ADD_PT(22.693512, 0.000000);
	ADD_PT(24.854797, 0.000000);
	ADD_PT(27.016083, 0.000000);
	ADD_PT(29.177368, 0.000000);
	ADD_PT(31.338654, 0.000000);
	ADD_PT(33.499969, 0.000000);

	g.push_back(s);
	m_gestureList.push_back(g);

	s.clear();
	g.clear();

	// Bow pattern (top left to top right).
	ADD_PT(-31.373352, -74.558090);
	ADD_PT(-25.035492, -64.699203);
	ADD_PT(-18.422485, -55.026871);
	ADD_PT(-11.553131, -45.532806);
	ADD_PT(-4.231506, -36.380768);
	ADD_PT(2.624756, -26.894363);
	ADD_PT(8.971283, -17.041153);
	ADD_PT(15.472534, -7.289261);
	ADD_PT(19.674988, 3.586945);
	ADD_PT(22.087433, 14.971390);
	ADD_PT(22.626648, 26.669281);
	ADD_PT(22.626648, 38.389633);
	ADD_PT(21.278320, 50.009445);
	ADD_PT(17.790131, 61.114914);
	ADD_PT(11.177490, 70.711411);
	ADD_PT(0.598633, 75.441910);
	ADD_PT(-10.496429, 72.392685);
	ADD_PT(-19.946716, 65.868546);
	ADD_PT(-26.798615, 56.399826);
	ADD_PT(-30.373352, 45.409836);
	ADD_PT(-29.984100, 33.717178);
	ADD_PT(-27.921783, 22.184006);
	ADD_PT(-23.382446, 11.460098);
	ADD_PT(-17.348541, 1.412964);
	ADD_PT(-9.721039, -7.485779);
	ADD_PT(-2.702301, -16.831757);
	ADD_PT(3.884827, -26.519547);
	ADD_PT(11.038696, -35.802139);
	ADD_PT(18.932739, -44.440720);
	ADD_PT(25.253387, -54.184830);
	ADD_PT(30.626648, -63.494736);
	ADD_PT(34.626648, -73.558090);

	g.push_back(s);
	m_gestureList.push_back(g);
}

void GestureSystem::normalizeSize(Gesture &gesture)
{
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;

	//Calculate extents of the gesture
	for (uint i = 0; i < gesture.size(); ++i)
	{
		Stroke stroke = gesture[i];
		for (uint j = 0; j < stroke.size(); ++j)
		{
			Point2D pt = stroke[j];
			if ( minX > pt.x ) minX = pt.x;
			if ( minY > pt.y ) minY = pt.y;
			if ( maxX < pt.x ) maxX = pt.x;
			if ( maxY < pt.y ) maxY = pt.y;
		}
	}

	//Calculate dimensions 
	float width = maxX - minX;
	float height = maxY - minY;

	//find out scale
	float scale = (width > height) ? width:height;
	if ( scale <= 0.0f ) return; //Empty or a single point stroke!
	scale = 1.0f/scale;

	//Do the actual scaling
	for (uint i = 0; i < gesture.size(); ++i)
	{
		Stroke &stroke = gesture[i];
		for (uint j = 0; j < stroke.size(); ++j)
		{
			Point2D &pt = stroke[j];
			pt.x *= scale;
			pt.y *= scale;
		}
	}
}

float GestureSystem::getStrokeLength(const Stroke &stroke)
{
	if ( stroke.size() <= 1 ) return 0;
	float len = 0.0f;

	Point2D startPt = stroke[0];

	for (uint i = 1; i < stroke.size(); ++i)
	{
		Point2D endPt = stroke[i];
		float dx = endPt.x - startPt.x;
		float dy = endPt.y - startPt.y;
		//Add the length of the current segment to the total
		len += sqrt(dx * dx + dy * dy);
		startPt = endPt;
	}

	return len;
}

void GestureSystem::normalizeSpacing(Stroke &newStroke, const Stroke &oldStroke)
{
	//Clear the new stroke
	newStroke.clear();
	float newSegmentLen = getStrokeLength(oldStroke);
	if ( oldStroke.size() <= 1 || newSegmentLen <= 0.0f ) return;
	newSegmentLen /= (m_pointsPerStroke-1);

	//Add the first point to the new stroke
	uint i = 0;
	newStroke.push_back(oldStroke[i]);
	Point2D startPt = oldStroke[i];  //Ends of the current segment
	Point2D endPt = oldStroke[i];    //(begin with the empty segment)
	++i;

	//Distance along old stroke at the end of the current segment
	float endOldDist     = 0.0f;
	//Distance along the old stroke at the beginning of the current segment
	float startOldDist   = 0.0f;
	//Distance along new stroke
	float newDist        = 0.0f;
	//Length of the current segment (on the old stroke)
	float currSegmentLen = 0.0f;

	while (true)
	{
		float excess = endOldDist - newDist;
		//we have accumulated enough length, add a point
		if ( excess >= newSegmentLen )
		{
			newDist+=newSegmentLen;
			float ratio = (newDist - startOldDist)/currSegmentLen;
			Point2D newPt;
			newPt.x = ( endPt.x - startPt.x ) * ratio + startPt.x;
			newPt.y = ( endPt.y - startPt.y ) * ratio + startPt.y;
			newStroke.push_back(newPt);
		} else {
			if ( i == oldStroke.size()) break; //No more data

			//Store the start of the current segment
			startPt = endPt;
			endPt = oldStroke[i]; //Get next point
			++i;
			float dx = endPt.x - startPt.x;
			float dy = endPt.y - startPt.y;

			//Start accumulated distance (along the old stroke)
			//at the beginning of the segment
			startOldDist = endOldDist;
			//Add the length of the current segment to the
			//total accumulated length
			currSegmentLen = sqrtf(dx*dx+dy*dy);
			endOldDist+=currSegmentLen;
		}
	}
	//Due to floating point errors we may miss the last
	//point of the stroke
	if ( newStroke.size() < m_pointsPerStroke )
	{
		newStroke.push_back(endPt);
	}
}

void GestureSystem::normalizeCenter(Gesture &gesture)
{
	float centerX = 0.0f;
	float centerY = 0.0f;
	int pointCount = 0;

	//Calculate the centroid of the gesture
	for (uint i = 0; i < gesture.size(); ++i)
	{
		Stroke stroke = gesture[i];
		//size should always be == kPointsPerStroke
		pointCount += (int)stroke.size();
		for (uint j = 0; j < stroke.size(); ++j)
		{
			Point2D pt = stroke[j];
			centerX += pt.x;
			centerY += pt.y;
		}
	}
	//Calculate centroid
	if ( pointCount <= 0 ) return; //empty gesture
	centerX /= pointCount;
	centerY /= pointCount;

	//To move the gesture into the origin, subtract centroid coordinates
	//from point coordinates

	for (uint i = 0; i < gesture.size(); ++i)
	{
		Stroke &stroke = gesture[i];
		for (uint j = 0; j < stroke.size(); ++j)
		{
			Point2D &pt = stroke[j];
			pt.x -= centerX;
			pt.y -= centerY;
		}
	}
}

float GestureSystem::gestureDotProduct(const Gesture &gesture1, const Gesture &gesture2)
{
	if (gesture1.size() != gesture2.size()) return -1;

	float dotProduct = 0.0f;

	for (uint i = 0; i < gesture1.size(); ++i)
	{
		const Stroke stroke1 = gesture1[i];
		const Stroke stroke2 = gesture2[i];

		if (stroke1.size() != stroke2.size()) return -1;

		for (uint j = 0; j < stroke1.size(); ++j)
		{
			const Point2D pt1 = stroke1[j];
			const Point2D pt2 = stroke2[j];
			dotProduct += pt1.x * pt2.x + pt1.y * pt2.y;
		}
	}

	return dotProduct;
}

float GestureSystem::match(const Gesture &gesture1, const Gesture &gesture2 )
{
	float score = gestureDotProduct(gesture1,gesture2);
	if ( score <= 0.0f ) return 0.0f; //No match for sure
	//at this point our gesture-vectors are not quite normalized
//	yet - their dot product with themselves is not 1.

		//we normalize the score itself

	// TODO: gesture1 * gesture1 can be pre-computed (store the dot products of all
					//  of our 'master gestures').

		//this is basically a version of a famous formula for a cosine of the 
		//angle between 2 vectors:
		//cos a = (u.v) / (sqrt(u.u) * sqrt(v.v)) = (u.v) / sqrt((u.u) * (v.v))
		score /= sqrtf(gestureDotProduct(gesture1, gesture1) *
		gestureDotProduct(gesture2, gesture2));
	return score;
}

uint GestureSystem::getBestMatch(const std::vector< Point2D > &points, float *bestVal)
{
	Gesture gesture;
	Stroke stroke = points;
	gesture.push_back(stroke);

	normalizeSize(gesture);

	Stroke newStroke;
	normalizeSpacing(newStroke, stroke);
	gesture.pop_back();
	gesture.push_back(newStroke);

	normalizeCenter(gesture);

//	for (int i = 0; i < 32; ++i)
//	{
//		printf("ADD_PT(%f, %f);\n", gesture[0][i].x, gesture[0][i].y);
//	}

	uint bestIndex = 0;
	float bestValue = 0.0f;

	// Find the best matching gesture.
	for (uint i = 0; i < m_gestureList.size(); ++i)
	{
		float val = match(m_gestureList[i], gesture);

		if (val > bestValue)
		{
			bestValue = val;
			bestIndex = i;
		}
	}

//	printf("Best: %i, %f\n", bestIndex, bestValue);

	if (bestVal != NULL)
		*bestVal = bestValue;

	return bestIndex;
}