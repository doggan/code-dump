#ifndef _MTXLIB_H_
#define _MTXLIB_H_

#include <cstdio>
#include <cmath>
#include <cassert>

static const float PI = 3.141592653f;

static inline float DegToRad(float a) { return a*0.01745329252f;};
static inline float RadToDeg(float a) { return a*57.29577951f;};

////////////////////////////////////////////////////////////
// vec2 class
//

class vec2 
{
public:
	// Members
	float x, y;

public:
	// Constructors
	vec2() { x = 0.0; y = 0.0; };
	// Constructor with initializing float values
	vec2(float inX, float inY): x(inX), y(inY) {}
	// Constructor with initializing vec2
	vec2(const vec2 &v): x(v.x), y(v.y) {}

public:
	// Array indexing
	float                 &operator [] (unsigned int i) {
		assert (i<2);
		return *(&x+i);
	}
	// Array indexing
	const float           &operator [] (unsigned int i) const {
		assert (i<2);
		return *(&x+i);
	}
	// Add a vec2 to this one
	vec2               &operator += (const vec2 &v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	// Subtract a vec2 from this one
	vec2               &operator -= (const vec2 &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	// Multiply the vec2 by a float
	vec2               &operator *= (float f) {
		x *= f;
		y *= f;
		return *this;
	}
	// Divide the vec2 by a float
	vec2               &operator /= (float f) {
		x /= f;
		y /= f;
		return *this;
	}
	// Are these two vec2's equal?
	friend bool           operator == (const vec2 &a, const vec2 &b) {
		return((a.x == b.x) && (a.y == b.y));
	}
	// Are these two vec2's not equal?
	friend bool           operator != (const vec2 &a, const vec2 &b) {
		return((a.x != b.x) || (a.y != b.y));
	}
	// Negate this vector
	friend vec2        operator - (const vec2 &a) {
		return vec2(-a.x, -a.y);
	}
	// Add two vec2's
	friend vec2        operator + (const vec2 &a, const vec2 &b) {
		vec2 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one vec2 from another
	friend vec2        operator - (const vec2 &a, const vec2 &b) {
		vec2 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply vec2 by a float
	friend vec2        operator * (const vec2 &v, float f) {
		return vec2(f * v.x, f * v.y);
	}
	// Multiply vec2 by a float
	friend vec2        operator * (float f, const vec2 &v) {
		return vec2(f * v.x, f * v.y);
	}
	// Divide vec2 by a float
	friend vec2        operator / (const vec2 &v, float f) {
		return vec2(v.x / f, v.y / f);
	}

public:
	// Methods
	// Set Values
	void                  set(float xIn, float yIn) {
		x = xIn;
		y = yIn;
	}
	// Get length of a vec2
	float                 length() const {
		return(float) sqrt(x*x + y*y);
	}
	// Get squared length of a vec2
	float                 lengthSqr() const {
		return(x*x + y*y);
	}
	// Does vec2 equal (0, 0)?
	bool                  isZero() const {
		return((x == 0.0F) && (y == 0.0F));
	}
	// Normalize a vec2
	vec2               &normalize() {
		float m = length();

		if (m > 0.0F)
			m = 1.0F / m;
		else
			m = 0.0F;
		x *= m;
		y *= m;

		return *this;
	}

	// Debug
	void                  fprint(FILE* file, char* str) const;
};

////////////////////////////////////////////////////////////
// vec3 class
//

class vec3 
{
public:
	// Members
	float x, y, z;

public:
	// Constructors
	vec3() { x = 0.0; y = 0.0; z = 0.0; };
	// Constructor with initializing float values
	vec3(float inX, float inY, float inZ): x(inX), y(inY), z(inZ) {}
	// Constructor with initializing vec3
	vec3(const vec3 &v): x(v.x), y(v.y), z(v.z) {}
	// Constructor with initializing vec2
	explicit vec3(const vec2 &v): x(v.x), y(v.y), z(0.0F) {}

public:
	// Operators
	// Array indexing
	float                 &operator [] (unsigned int i) {
		assert (i<3);
		return *(&x+i);
	}
	// Array indexing
	const float           &operator [] (unsigned int i) const {
		assert (i<3);
		return *(&x+i);
	}
	// Assign from a vec2
	vec3               &operator =  (const vec2 &v) {
		x = v.x;
		y = v.y;
		z = 0.0F;
		return *this;
	}
	// Add a vec3 to this one
	vec3               &operator += (const vec3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	// Subtract a vec3 from this one
	vec3               &operator -= (const vec3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	// Multiply the vec3 by a float
	vec3               &operator *= (float f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	// Divide the vec3 by a float
	vec3               &operator /= (float f) {
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
	// Are these two vec3's equal?
	friend bool           operator == (const vec3 &a, const vec3 &b) {
		return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
	}
	// Are these two vec3's not equal?
	friend bool           operator != (const vec3 &a, const vec3 &b) {
		return((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
	}
	// Negate a vec3
	friend vec3        operator - (const vec3 &a) {
		return vec3(-a.x, -a.y, -a.z);
	}
	// Add two vec3's
	friend vec3        operator + (const vec3 &a, const vec3 &b) {
		vec3 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one vec3 from another
	friend vec3        operator - (const vec3 &a, const vec3 &b) {
		vec3 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply vec3 by a float
	friend vec3        operator * (const vec3 &v, float f) {
		return vec3(f * v.x, f * v.y, f * v.z);
	}
	// Multiply vec3 by a float
	friend vec3        operator * (float f, const vec3 &v) {
		return vec3(f * v.x, f * v.y, f * v.z);
	}
	// Divide vec3 by a float
	friend vec3        operator / (const vec3 &v, float f) {
		return vec3(v.x / f, v.y / f, v.z / f);
	}

public:
	// Methods
	// Set Values
	void                  set(float xIn, float yIn, float zIn) {
		x = xIn;
		y = yIn;
		z = zIn;
	}
	// Get length of a vec3
	float                 length() const {
		return(float) sqrt(x*x + y*y + z*z);
	}
	// Get squared length of a vec3
	float                 lengthSqr() const {
		return(x*x + y*y + z*z);
	}
	// Does vec3 equal (0, 0, 0)?
	bool                  isZero() const {
		return((x == 0.0F) && (y == 0.0F) && (z == 0.0F));
	}
	// Normalize a vec3
	vec3               &normalize() {
		float m = length();
		if (m > 0.0F)
			m = 1.0F / m;
		else
			m = 0.0F;
		x *= m;
		y *= m;
		z *= m;
		return *this;
	}

	// Debug
	void                  fprint(FILE* file, char* str) const;
};

vec2 Normalized(const vec2 &a);
vec3 Normalized(const vec3 &a);
float DotProduct(const vec2 &a, const vec2 &b);
float DotProduct(const vec3 &a, const vec3 &b);
void SwapVec(vec2 &a, vec2 &b);
void SwapVec(vec3 &a, vec3 &b);
vec3 CrossProduct(const vec3 &a, const vec3 &b);
bool NearlyEquals(const vec2 &a, const vec2 &b, float r);
bool NearlyEquals(const vec3 &a, const vec3 &b, float r);

////////////////////////////////////////////////////////////
// mtx33 class
//

class mtx33 
{
public:
	// Members
	vec3       col[3];

public:
	// Constructors
	mtx33() {};
	// Constructor with initializing value
	mtx33(float v) {
		col[0].set(v, v, v);
		col[1].set(v, v, v);
		col[2].set(v, v, v);
	}
	// Constructor with initializing mtx33
	mtx33(const mtx33 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
	}
	// Constructor with initializing vec3's
	mtx33(const vec3 &v0, const vec3 &v1, const vec3 &v2) {
		col[0] = v0;
		col[1] = v1;
		col[2] = v2;
	}

public:
	// Operators
	// Array indexing
	vec3       &operator [] (unsigned int i) {
		assert (i<3);
		return(vec3&)col[i];
	}
	// Array indexing
	const vec3     &operator [] (unsigned int i) const {
		assert (i<3);
		return(vec3&)col[i];
	}
	// Assign
	mtx33      &operator =  (const mtx33 &m) {
		col[0] = m[0];
		col[1] = m[1];
		col[2] = m[2];
		return *this;
	}
	// Add a mtx33 to this one
	mtx33      &operator += (const mtx33 &m) {
		col[0] += m[0];
		col[1] += m[1];
		col[2] += m[2];
		return *this;
	}
	// Subtract a mtx33 from this one
	mtx33      &operator -= (const mtx33 &m) {
		col[0] -= m[0];
		col[1] -= m[1];
		col[2] -= m[2];
		return *this;
	}
	// Multiply the mtx33 by another mtx33
	mtx33      &operator *= (const mtx33 &m);
	// Multiply the mtx33 by a float
	mtx33      &operator *= (float f) {
		col[0] *= f;
		col[1] *= f;
		col[2] *= f;
		return *this;
	}
	// Are these two mtx33's equal?
	friend bool       operator == (const mtx33 &a, const mtx33 &b) {
		return((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
	}
	// Are these two mtx33's not equal?
	friend bool       operator != (const mtx33 &a, const mtx33 &b) {
		return((a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]));
	}
	// Add two mtx33's
	friend mtx33   operator + (const mtx33 &a, const mtx33 &b) {
		mtx33 ret(a);
		ret += b;
		return ret;
	}
	// Subtract one mtx33 from another
	friend mtx33   operator - (const mtx33 &a, const mtx33 &b) {
		mtx33 ret(a);
		ret -= b;
		return ret;
	}
	// Multiply mtx33 by another mtx33
	friend mtx33   operator * (const mtx33 &a, const mtx33 &b) {
		mtx33 ret(a);
		ret *= b;
		return ret;
	}
	// Multiply a vec3 by this mtx33
	friend vec3    operator * (const mtx33 &m, const vec3 &v) {
		vec3 ret;
		ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
		ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
		ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
		return ret;
	}
	// Multiply a vec3 by this mtx33
	friend vec3    operator * (const vec3 &v, const mtx33 &m) {
		vec3 ret;
		ret.x = DotProduct(m[0], v);
		ret.y = DotProduct(m[1], v);
		ret.z = DotProduct(m[2], v);
		return ret;
	}
	// Multiply mtx33 by a float
	friend mtx33   operator * (const mtx33 &m, float f) {
		mtx33 ret(m);
		ret *= f;
		return ret;
	}
	// Multiply mtx33 by a float
	friend mtx33   operator * (float f, const mtx33 &m) {
		mtx33 ret(m);
		ret *= f;
		return ret;
	}

public:
	// Methods
	// Set mtx33 to the identity matrix
	mtx33      &identity() {
		col[0].set(1.0, 0.0, 0.0);
		col[1].set(0.0, 1.0, 0.0);
		col[2].set(0.0, 0.0, 1.0);
		return *this;
	}
	// Transpose the mtx33
	mtx33      &transpose();
	// Invert the mtx33
	mtx33      &invert();

	// Debug
	void          fprint(FILE* file, char* str) const;

	vec2 getTranslate() { return vec2(col[2].x, col[2].y); }
	void setTranslate(vec2 trans) { col[2].x = trans.x; col[2].y = trans.y; }
};

mtx33    Identity();
mtx33    Transpose(const mtx33 &m);
mtx33    Invert(const mtx33 &m);
mtx33    RotateRad(float rad);
mtx33    Translate(float x, float y);
mtx33    Scale(float x, float y, float z = 1.0);

#endif  // _MTXLIB_H_