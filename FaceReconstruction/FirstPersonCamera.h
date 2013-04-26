#pragma once

#include "Vector3.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


/// <summary>
/// Represents a first person camera that can be moved, rotated,
/// and passed to opengl (via Tellgl()). In addition, it 
/// provides the user with an access to the direction vectors
/// of the axes of the camera (R, U, N).
/// </summary>
class FirstPersonCamera
{
public:
	/// <summary>
	/// The position of the eye.
	/// </summary>
	Vector3 P;

	/// <summary>
	/// The right direction. x
	/// </summary>
	Vector3 R;

	/// <summary>
	/// The up direction. y
	/// </summary>
	Vector3 U;

	/// <summary>
	/// The -ve of the look direction. z
	/// </summary>
	Vector3 N;

	/// <summary>
	/// Returns the direction in which the camera is looking (-N).
	/// </summary>
	Vector3 GetLookDirection()
	{
		return -N;

	}

	FirstPersonCamera(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
	{
		Reset(eye, center, up);
	}

	/// <summary>
	/// Eye = (0, 0, 0), Center = (0, 0, -1), Up = (0, 1, 0)
	/// </summary>
	FirstPersonCamera()
	{
		Reset(0, 0, 0, 
			0, 0, -1, 
			0, 1, 0);
	}

	FirstPersonCamera(
		float eyeX,
		float eyeY,
		float eyeZ,
		float centerX,
		float centerY,
		float centerZ,
		float upX,
		float upY,
		float upZ)
	{
		Reset(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	}

	void Reset(
		float eyeX,
		float eyeY,
		float eyeZ,
		float centerX,
		float centerY,
		float centerZ,
		float upX,
		float upY,
		float upZ)
	{
		Vector3 eyePt(eyeX, eyeY, eyeZ);
		Vector3 centerPt(centerX, centerY, centerZ);
		Vector3 upVec(upX, upY, upZ);
		Reset(eyePt, centerPt, upVec);
	}

	void Reset(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
	{
		P = eye;
		//still needs normalization
		N = eye - center;
		//i = j x k
		R = Vector3::CrossProduct(up, N);
		//j = k x i
		U = Vector3::CrossProduct(N, R);
		//normalize all
		R.Normalize();
		U.Normalize();
		N.Normalize();
	}

	/// <summary>
	/// Multiplies the current matrix with the camera view matrix.
	/// </summary>
	/// <remarks>
	/// Generally, you will use this function to tell opengl the
	/// camera matrix as follows:
	/// Gl.glMatrixMode(Gl.GL_MODELVIEW);
	/// Gl.glLoadIdentity(); //MV = I
	/// camera.Tellgl(); //MV = I * V = V
	/// </remarks>
	void Tellgl()
	{
		//take any point in the look direction of the camera
	//	Vector3 subjectPt = P + GetLookDirection();
		gluLookAt(
			P.x, P.y, P.z,
			N.x, N.y, N.z,
			U.x, U.y, U.z);
	}

	#pragma region Rotations
	/// <summary>
	/// Rotates the 3 axes about the world y-axis with the specified angle. This is equivalent to
	/// a left-turn.
	/// </summary>
	void Yaw(float angleDegrees)
	{
		Vector3 jDir(0, 1, 0);

		R.RotateAround(angleDegrees, jDir);
		U.RotateAround(angleDegrees, jDir);
		N.RotateAround(angleDegrees, jDir);

	}

	/// <summary>
	/// Rotates the axes (U, N) about the R-axis with the specified angle. This is equivalent
	/// to a look-up (up turn).
	/// </summary>//x--->y--->z
	void Pitch(float angleDegrees)
	{
		U.RotateAround(angleDegrees, R);
		N.RotateAround(angleDegrees, R);
	}

	/// <summary>
	/// Rotates the axes (R, U) about the N-axis with the specified angle.
	/// </summary>//z--->x--->y
	/// Roll is rarely used except with flying objects.
	void Roll(float angleDegrees)
	{
		R.RotateAround(angleDegrees, N);
		U.RotateAround(angleDegrees, N);
	}
	#pragma endregion

	#pragma region Translations

	/// <summary>
	/// Moves the eye point a distance dist forward == -dist * N
     /// Walk
	/// </summary>
	void Walk(float dist)
	{
		P -= dist * N;
	}

     /// <summary>
     /// Moves the eye point a distance dist to the right == +dist * R
     /// Strafe
     /// </summary>
     void Strafe(float dist)
     {
         P += dist * R;
     }

     /// <summary>
     /// Moves the eye point a distance dist upward == +dist * U
     /// Fly
     /// </summary>
     void Fly(float dist)
     {
         P += dist * U;
		}

	#pragma endregion

		/// <summary>
	  /// Moves the eye position a distance stepR along the vector R,
	  /// a distance stepU along the vector U, and a distance stepD
	  /// along the vector N.
	  /// </summary>
	  void Slide(float stepR, float stepU, float stepD)
	  {
		  P += stepR * R;
		  P += stepU * U;
		  P += stepD * N;
	  }

	  /// <summary>
	  /// Multiplies the current matrix with the camera view matrix.
	  /// </summary>
	  /// <remarks>
	  /// Generally, you will use this function to tell opengl the
	  /// camera matrix as follows:
	  /// Gl.glMatrixMode(Gl.GL_MODELVIEW);
	  /// Gl.glLoadIdentity(); //MV = I
	  /// camera.Tellgl(); //MV = I * V = V
	  /// </remarks>
	  void TellglManually()
	  {
		  /*
			* Rx Ry Rz -eye.R
			* Ux Uy Uz -eye.U
			* Dx Dy Dz -eye.N
			* 0  0  0   1
			*/
		  float m[16];

		  m[0] = R.x; m[4] = R.y; m[8] = R.z; m[12] = -Vector3::DotProduct(P, R);
		  m[1] = U.x; m[5] = U.y; m[9] = U.z; m[13] = -Vector3::DotProduct(P, U);
		  m[2] = N.x; m[6] = N.y; m[10] = N.z; m[14] = -Vector3::DotProduct(P, N);
		  m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1;
		  //post-multiply the current matrix by m: ct = ct * M
		  glMultMatrixf(m);
	  }
};
