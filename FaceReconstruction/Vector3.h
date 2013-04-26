/*
 *  Vector3.h
 *  Lab04
 *
 *  Created by Amr Elsehemy on 3/21/10.
 *  Copyright 2010 Elsehemy Dev. All rights reserved.
 *
 */
#pragma once
#include <math.h>

template <class T> T Sqr(const T& val) { return val * val; }

struct Vector3
{
public:
	float x;
	float y;
	float z;
	
	Vector3()
	{
		x = y = z = 0.0f;
	}
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	void operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    Vector3 operator+(const Vector3& v) const
    {
        return Vector3(x+v.x, y+v.y, z+v.z);
    }
	
	Vector3 operator+(float value) const
    {
        return Vector3(x+value, y+value, z+value);
    }
	
    void operator-=(const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    
	Vector3 operator-(const Vector3& v) const
    {
        return Vector3(x-v.x, y-v.y, z-v.z);
    }

	Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }
	
	Vector3 operator-(float value) const
    {
        return Vector3(x-value, y-value, z-value);
    }
	
	/** Multiplies this vector by the given scalar. */
    void operator*=(const float value)
    {
        x *= value;
        y *= value;
        z *= value;
    }
	
    /** Returns a copy of this vector scaled the given value. */
    Vector3 operator*(const float value) const
    {
        return Vector3(x*value, y*value, z*value);
    }

	 /** Returns a copy of this vector scaled the given value. */
    friend Vector3 operator*(float value, Vector3 v)
    {
        return Vector3(v.x*value, v.y*value, v.z*value);
    }
	
    /** Returns a copy of this vector scaled the given value. */
    Vector3 operator/(const float value) const
    {
        return Vector3(x/value, y/value, z/value);
    }
	
	
	/** Checks if the two vectors have identical components. */
    bool operator==(const Vector3& other) const
    {
        return x == other.x &&
		y == other.y &&
		z == other.z;
    }
	
    /** Checks if the two vectors have non-identical components. */
    bool operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }
	
    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @note This does not behave like a single-value comparison:
     * !(a < b) does not imply (b >= a).
     */
    bool operator<(const Vector3& other) const
    {
        return x < other.x && y < other.y && z < other.z;
    }
	
    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @note This does not behave like a single-value comparison:
     * !(a < b) does not imply (b >= a).
     */
    bool operator>(const Vector3& other) const
    {
        return x > other.x && y > other.y && z > other.z;
    }
	
    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @note This does not behave like a single-value comparison:
     * !(a <= b) does not imply (b > a).
     */
    bool operator<=(const Vector3& other) const
    {
        return x <= other.x && y <= other.y && z <= other.z;
    }
	
    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @note This does not behave like a single-value comparison:
     * !(a <= b) does not imply (b > a).
     */
    bool operator>=(const Vector3& other) const
    {
        return x >= other.x && y >= other.y && z >= other.z;
    }
	
    /** Zero all the components of the vector. */
    void Clear()
    {
        x = y = z = 0;
    }
	
    /** Flips all the components of the vector. */
    void Invert()
    {
        x = -x;
        y = -y;
        z = -z;
    }
	
	void Scale(float s)
	{
		x = s * x;
		y = s * y;
		z = s * z;
	}
	
	//public static float AngleBetween(ref Vector3 v1, ref Vector3 v2)
//	{
//		double cosZeta = DotProduct(ref v1, ref v2) / v1.Magnitude;
//		cosZeta = cosZeta / v2.Magnitude;
//		return (float)System.Math.Acos( cosZeta );
//	}
//	
//	static float DotProduct(ref Vector3 v1, ref Vector3 v2)
//	{
//		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
//	}
	 static float DotProduct(Vector3 v1, Vector3 v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	float MagnitudeSquare()
	{
		return x*x+y*y+z*z;
	}
	float Magnitude()
	{
		 return sqrtf(x*x+y*y+z*z);
	}
	 void Normalize()
	{
				float l = Magnitude();
		        if (l > 0)
		        {
		            (*this) *= ((float)1)/l;
		        }
	}
	
	void Set(float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}
	
	void Set(Vector3 v)
	{
		Set(v.x, v.y, v.z);
	}
	
	static Vector3 CrossProduct(const Vector3 &v1,  const Vector3 &v2) 
	{
		return Vector3(
						v1.y*v2.z - v1.z*v2.y,
						-(v1.x*v2.z - v1.z*v2.x),
						v1.x*v2.y - v1.y*v2.x);
	}
	
	/// <summary>
	/// Rotates the vector v about the axis axisVec the specified angle (in degrees),
	/// in a CCW, and returns the resulting vector.
	/// </summary>
	static Vector3 RotateVector(const Vector3 &v, float angle, Vector3 axisVec)
	{
	const double PI = 3.1415926535897932384626433832795;
		angle *= -(float)PI / 180;
		axisVec.Normalize();
		float c = (float)cos(angle);
		float s = (float)sin(angle);
		float t = 1 - c;
		Vector3 retval;
		retval.x = (t * Sqr(axisVec.x) + c) * v.x;
		retval.x += (t * axisVec.x * axisVec.y + s * axisVec.z) * v.y;
		retval.x += (t * axisVec.x * axisVec.z - s * axisVec.y) * v.z;

		retval.y = (t * axisVec.x * axisVec.y - s * axisVec.z) * v.x;
		retval.y += (t * Sqr(axisVec.y) + c) * v.y;
		retval.y += (t * axisVec.y * axisVec.z + s * axisVec.x) * v.z;

		retval.z = (t * axisVec.x * axisVec.y + s * axisVec.y) * v.x;
		retval.z += (t * axisVec.y * axisVec.z - s * axisVec.x) * v.y;
		retval.z += (t * Sqr(axisVec.z) + c) * v.z;

		return retval;
	}

	
	void RotateAround(float angleDegrees, Vector3 axis)
	{
		Vector3 meRotated = RotateVector(*this, angleDegrees, axis);
		Set(meRotated);
	}	
};