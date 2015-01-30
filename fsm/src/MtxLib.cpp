#include "MtxLib.h"

////////////////////////////////////////////////////////////
// Miscellaneous vector functions
//

// Return Normal of vector2's
vec2 Normalized(const vec2 &a)
{
	vec2 ret(a);
	return ret.normalize();
}

// Return Normal of vec3's
vec3 Normalized(const vec3 &a)
{
	vec3 ret(a);
	return ret.normalize();
}

// Dot product of two vec2's
float DotProduct(const vec2 &a, const vec2 &b) 
{
	return a.x*b.x + a.y*b.y;
}

// Dot product of two vec3's
float DotProduct(const vec3 &a, const vec3 &b) 
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Swap two vec2's
void SwapVec(vec2 &a, vec2 &b) 
{
	vec2 tmp(a);

	a = b;
	b = tmp;
}

// Swap two vec3's
void SwapVec(vec3 &a, vec3 &b) 
{
	vec3 tmp(a);

	a = b;
	b = tmp;
}

// Cross product of two vec3's
vec3 CrossProduct(const vec3 &a, const vec3 &b) 
{
	return vec3(a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x);
}

// Are these two vec2's nearly equal?
bool NearlyEquals( const vec2& a, const vec2& b, float r ) 
{
	vec2 diff = a - b;  // difference

	return(DotProduct(diff, diff) < r*r);  // radius
}

// Are these two vec3's nearly equal?
bool NearlyEquals( const vec3& a, const vec3& b, float r ) 
{
	vec3 diff = a - b;  // difference

	return(DotProduct(diff, diff) < r*r);  // radius
}

////////////////////////////////////////////////////////////
// mtx33 class
//

// Multiply the mtx33 by another mtx33
mtx33 &mtx33::operator *= (const mtx33 &m) 
{
	mtx33 t;

	for (unsigned int r = 0; r < 3; r++)
	{
		for (unsigned int c = 0; c < 3; c++)
		{
			float f = 0;

			f += col[0][r] * m[c][0];
			f += col[1][r] * m[c][1];
			f += col[2][r] * m[c][2];

			t[c][r] = f;
		}
	}

	*this = t;

	return *this;
}

// Transpose the mtx33
mtx33 &mtx33::transpose() 
{
	float t;

	for (unsigned int c = 0; c < 3; c++)
	{
		for (unsigned int r = c + 1; r < 3; r++)
		{
			t = col[c][r];
			col[c][r] = col[r][c];
			col[r][c] = t;
		} 
	}

	return *this;
}

// Invert the mtx33
mtx33 &mtx33::invert() 
{
	mtx33 a(*this);
	mtx33 b(Identity());

	unsigned int c, r;
	unsigned int cc;
	unsigned int rowMax; // Points to max abs value row in this column
	unsigned int row;
	float tmp;

	// Go through columns
	for (c=0; c<3; c++)
	{
		// Find the row with max value in this column
		rowMax = c;
		for (r=c+1; r<3; r++)
		{
			if (fabs(a[c][r]) > fabs(a[c][rowMax]))
			{
				rowMax = r;
			}
		}

		// If the max value here is 0, we can't invert.  Return identity.
		if (a[rowMax][c] == 0.0F)
			return(identity());

		// Swap row "rowMax" with row "c"
		for (cc=0; cc<3; cc++)
		{
			tmp = a[cc][c];
			a[cc][c] = a[cc][rowMax];
			a[cc][rowMax] = tmp;
			tmp = b[cc][c];
			b[cc][c] = b[cc][rowMax];
			b[cc][rowMax] = tmp;
		}

		// Now everything we do is on row "c".
		// Set the max cell to 1 by dividing the entire row by that value
		tmp = a[c][c];
		for (cc=0; cc<3; cc++)
		{
			a[cc][c] /= tmp;
			b[cc][c] /= tmp;
		}

		// Now do the other rows, so that this column only has a 1 and 0's
		for (row = 0; row < 3; row++)
		{
			if (row != c)
			{
				tmp = a[c][row];
				for (cc=0; cc<3; cc++)
				{
					a[cc][row] -= a[cc][c] * tmp;
					b[cc][row] -= b[cc][c] * tmp;
				}
			}
		}

	}

	*this = b;

	return *this;
}

// Return a mtx33 set to the identity matrix
mtx33 Identity() 
{
	mtx33 ret;

	return ret.identity();
}

// Return the transpose of the mtx33
mtx33 Transpose(const mtx33 &m) 
{
	mtx33 ret(m);

	return ret.transpose();
}

// Return the inverted mtx33
mtx33 Invert(const mtx33 &m) 
{
	mtx33 ret(m);

	return ret.invert();
}

// Return a 2D rotation mtx33
mtx33 RotateRad(float rad) 
{
	mtx33 ret;
	float sinA, cosA;

	sinA = (float)sin(rad);
	cosA = (float)cos(rad);

	ret[0][0] = cosA; ret[1][0] = -sinA; ret[2][0] = 0.0F;
	ret[0][1] = sinA; ret[1][1] =  cosA; ret[2][1] = 0.0F;
	ret[0][2] = 0.0F; ret[1][2] =  0.0F; ret[2][2] = 1.0F;

	return ret;
}

// Return a 2D translation mtx33
mtx33 Translate(float x, float y) 
{
	mtx33 ret;

	ret.identity();
	ret[2][0] = x;
	ret[2][1] = y;

	return ret;
}

// Return a 2D/3D scale mtx33
mtx33 Scale(float x, float y, float z) 
{
	mtx33 ret;

	ret.identity();
	ret[0][0] = x;
	ret[1][1] = y;
	ret[2][2] = z;

	return ret;
}

////////////////////////////////////////////////////////////
// Debug functions
//

// Print a vector2 to a file
void vec2::fprint(FILE* file, char* str) const 
{
	fprintf(file, "%svector2: <%f, %f>\n", str, x, y);
}

// Print a vec3 to a file
void vec3::fprint(FILE* file, char* str) const 
{
	fprintf(file, "%svec3: <%f, %f, %f>\n", str, x, y, z);
}

// Print a mtx33 to a file
void mtx33::fprint(FILE* file, char * str) const 
{
	fprintf(file, "%smtx33:\n", str);
	vec3 row0(col[0][0], col[1][0], col[2][0]);
	row0.fprint(file, "\t");
	vec3 row1(col[0][1], col[1][1], col[2][1]);
	row1.fprint(file, "\t");
	vec3 row2(col[0][2], col[1][2], col[2][2]);
	row2.fprint(file, "\t");
}