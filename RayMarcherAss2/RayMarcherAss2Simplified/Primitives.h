WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
/*  The following code is a VERY heavily modified from code originally sourced from:
	Ray tracing tutorial of http://www.codermind.com/articles/Raytracer-in-C++-Introduction-What-is-ray-tracing.html
	It is free to use for educational purpose and cannot be redistributed outside of the tutorial pages. */

#ifndef __PRIMITIVES_H
#define __PRIMITIVES_H

#include <cmath>
#include "Constants.h"


// points consist of three coordinates and represent a point in 3d space
typedef struct Point 
{
	float x, y, z;

	inline Point() : x(0), y(0), z(0) { }
	inline Point(float x, float y, float z) : x(x), y(y), z(z) { }

	inline float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
} Point;


// vectors consist of three coordinates and represent a direction from (an implied) origin
typedef struct Vector 
{
	float x, y, z;

	inline Vector() : x(0), y(0), z(0) { }
	inline Vector(float x, float y, float z) : x(x), y(y), z(z) { }

    Vector& operator += (const Vector& v2) 
	{
	    this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
	    return *this;
    }

	inline float dot() const 
	{
		return x * x + y * y + z * z;
	}

	inline float length()
	{
		return sqrtf(x * x + y * y + z * z);
	}
} Vector;


// rays are cast from a starting point in a direction
typedef struct Ray
{
	Point start;
	Vector dir;
} Ray;


// ---- Point specific functions/operators ----

// point + vector (produces a point)
inline Point operator + (const Point& p, const Vector& v) 
{
	Point p2 = { p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

// point - vector (produces a point)
inline Point operator - (const Point& p, const Vector& v)
{
	Point p2 = { p.x - v.x, p.y - v.y, p.z - v.z };
	return p2;
}

// point * float (each component of the point is multiplied by the float)
inline Point operator * (const Point& p, float c)
{
	Point p2 = { p.x * c, p.y * c, p.z * c };
	return p2;
}

// point / float (each component of the point is divided by the float)
inline Point operator / (const Point& p, float c)
{
	Point p2 = { p.x / c, p.y / c, p.z / c };
	return p2;
}


// ---- Vector specific functions/operators ----

// vector + vector
inline Vector operator + (const Vector& v1, const Vector& v2)
{
	Vector v = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return v;
}

// vector / vector
inline Vector operator / (const Vector& v1, const Vector& v2)
{
	Vector v = { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
	return v;
}

// point + point (produces a vector)
inline Vector operator + (const Point& p1, const Point& p2)
{
	Vector v = { p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
	return v;
}

// point - point (produces a vector)
inline Vector operator - (const Point& p1, const Point& p2)
{
	Vector v = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

// vector * float (each component of the vector is multipled by the float)
inline Vector operator * (const Vector& v, float c)
{
	Vector v2 = { v.x * c, v.y * c, v.z * c };
	return v2;
}

// vector / float (each component of the vector is divided by the float)
inline Vector operator / (const Vector& v, float c)
{
	Vector v2 = { v.x / c, v.y / c, v.z / c };
	return v2;
}

// vector - vector
inline Vector operator - (const Vector& v1, const Vector& v2)
{
	Vector v = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}

// vector * vector (dot-product, produces a single float)
inline float operator * (const Vector& v1, const Vector& v2 ) 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// helper function to calculate reciprocal square root
inline float invsqrtf(const float& x) 
{ 
	return 1.0f / sqrtf(x); 
}

// helper function to calculate sign (i.e. just whether the number is positive, negative, or zero)
inline float sign(const float& x)
{
	return (x < 0.0f ? -1.0f : (x > 0.0f ? 1.0f : 0.0f));
}

// normalise the vector (ie. make the vector's magnitude equal to 1)
// not to be confused with the normal vector of a surface/intersection
inline Vector normalise(const Vector& x)
{
	return x * invsqrtf(x.dot());
}

// vector cross product
inline Vector cross(const Vector& v1, const Vector& v2)
{
	Vector v = { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
	return v;
}

// calculate the maximum value of each component of two vectors
inline Vector max(const Vector& v1, const Vector& v2)
{
	Vector v = { fmaxf(v1.x, v2.x), fmaxf(v1.y, v2.y), fmaxf(v1.z, v2.z) };
	return v;
}

// calculate the minimum value of each component of two vectors
inline Vector min(const Vector& v1, const Vector& v2)
{
	Vector v = { fminf(v1.x, v2.x), fminf(v1.y, v2.y), fminf(v1.z, v2.z) };
	return v;
}

// calculate the maximum value across all components of a single vector
inline float hmax(const Vector& v1)
{
	return fmaxf(v1.x, fmaxf(v1.y, v1.z));
}

// calculate the absolute value of each component of a vector
inline Vector abs(const Vector& v1)
{
	Vector v = { fabsf(v1.x), fabsf(v1.y), fabsf(v1.z) };
	return v;
}

// ---- general math functions/operators ----

// clamp a value between 0 and 1 (inclusive)
inline float clamp01(float x)
{
	return fminf(1.0, fmaxf(0.0, x));
}

// linearly interpolate between two values
inline float lerp(float x, float y, float t)
{
	return x * (1 - t) + y * t;
}

#endif //__PRIMITIVES_H
