#ifndef VECTOR4F_H
#define VECTOR4F_H

#include <stdio.h>
#include <math.h>

// This file contains our Vector4f class definition.
#define PI 3.1415926535897932384626433832795f
#define RADTODEGCONST 57.295779513082320876798154814105f
#define DEGTORADCONST 0.0174532925199433f
#define RADTODEG(X) (X*RADTODEGCONST)
#define DEGTORAD(X) (X*DEGTORADCONST)
class Vector4f
{
public:
	float x, y, z, w;

	#pragma region ctors
	// Default ctor
	Vector4f()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
    w = 1.0f;
	}

	// Overloaded ctor
	Vector4f(float ix, float iy, float iz, float iw)
	{
		x = ix;
		y = iy;
		z = iz;
    w = iw;
	}
    
  // Overloaded ctor to allow user to specify 3d vertex, w set to 1.0 automatically
	Vector4f(float ix, float iy, float iz)
	{
		x = ix;
		y = iy;
	  z = iz;
    w = 1.0f;
	}

	Vector4f(float ix, float iy)
	{
		x = ix;
		y = iy;
	  z = 0.0f;
    w = 1.0f;
	}
	#pragma endregion

	#pragma region const member functions
	// These functions perform vector operations
	// keyword CONST at the end of the function name (after the brackets.. as in
	// Vector4f operator+(Vector4f other) CONST <--- a promise that calling object won't be modified

	// Vector addition.  Does NOT modify
	// the Vector object that calls this member
	// function!  Returns a NEW vector object.
	Vector4f operator+(const Vector4f& other) const
	{
		// Left this way so you can see exactly what is happening.
		// This is done in a more compact way in the operator- function below.
		Vector4f result;

		result.x = this->x + other.x;
		result.y = this->y + other.y;
		result.z = this->z + other.z;
        // w component ignored in addition

		return result;
	}

	// Vector subtraction
	Vector4f operator-(const Vector4f& other) const
	{
		return Vector4f( x - other.x,
			y - other.y,
			z - other.z );
	}

	// Finds the dot product
	float DotProduct(const Vector4f& other) const
	{
		return(x*other.x + y*other.y + z*other.z);
	}

	// Calculates the cross product
	Vector4f XProd(const Vector4f& other) const
	{
		return Vector4f( y*other.z - z*other.y,
			z*other.x - x*other.z,
			x*other.y - y*other.x);
	}

	// Scalar multiply, but does not affect the vector object
	// that calls this member function (returns a new vector obj instead)
	Vector4f MultiplyByScalarNOMODIFY( const float s ) const
	{
		return Vector4f(x * s,
			y * s,
			z * s);
	}

	// Gets magnitude of vector
	float mag() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	// Finds angle between 'this' vector and another vector object
	float angleBetween(const Vector4f& other) const
	{
		// we use the formula:
		// cos(theta) = (A) DOT (B) / (|A||B|)
		float angleRadians = acos( this->DotProduct(other) / (this->mag()*other.mag()) );
		return (180.0f/PI)*angleRadians;
	}

	
	/// elevation - angle between +y axis and vector
	float getSphericalPHI() const
	{
		return (float)RADTODEG( acos( y/getSphericalR() ) );
	}

	/// azimuth - angle between +x axis and vector
	float getSphericalTHETA() const
	{
		if(x < 0)
		{
			// in 2nd or 3rd quadrant
			return 180.0f + (float)RADTODEG(atan(z/x));
		}
		else
			return (float)RADTODEG(atan(z/x));
		
	}

	/// radius of sphere would trace
	float getSphericalR() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	#pragma endregion

	#pragma region non-const functions
	// Adds another vector to 'this' vector.
	// This function does (and certainly should)
	// affect the vector object that calls it.
	Vector4f& operator+=(const Vector4f& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return (*this);
	}

	// Subtracts another vector from 'this' vector
	Vector4f& operator-=(const Vector4f& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return (*this);
	}

	// This member function normalizes the vector
	Vector4f& Normalize()
	{
		float magnitude = sqrt(x*x + y*y + z*z);
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;

		return (*this);
	}

	// Scalar multiply.. affects the vector object
	// calling this member function.
	Vector4f& MultiplyByScalar( const float s )
	{
		x *= s;
		y *= s;
		z *= s;

		return (*this);
	}

	/// Corrects a vector to a certain magnitude
	Vector4f& CorrectToMagnitude(float magnitude)
	{
		this->Normalize();
		this->MultiplyByScalar( magnitude );
		return (*this);
	}

	/// Rotates a 'point' about the UNIT VECTOR (a, b, c)
  /// From Anton & Rorres, ~pg 160 (I think)
	Vector4f& RotateAbout( float a, float b, float c, float theta )
	{
		float newx = (		
          x*( a*a*(1-cos(theta)) + cos(theta)   ) +
					y*( a*b*(1-cos(theta)) - c*sin(theta) ) +
					z*( a*c*(1-cos(theta)) + b*sin(theta)  ) );

		float newy = (		
          x*( a*b*(1-cos(theta)) + c*sin(theta) ) +
					y*( b*b*(1-cos(theta)) + cos(theta)   ) +
					z*( b*c*(1-cos(theta)) - a*sin(theta)   )  );

		float newz = (		
          x*( a*c*(1-cos(theta)) - b*sin(theta) ) +
					y*( b*c*(1-cos(theta)) + a*sin(theta) ) +
					z*( c*c*(1-cos(theta)) + cos(theta)   )  );

    x = newx;
    y = newy;
    z = newz;
		return (*this);
	}

  void perspectiveDivide()
  {
    x /= w ;
    y /= w ;
    z /= w ;

    w /= w ;
  }

	void print()
	{
		printf( "%.2f %.2f %.2f %.2f\n", x, y, z, w ) ;
	}
	#pragma endregion
};

#endif // VECTOR4F_H