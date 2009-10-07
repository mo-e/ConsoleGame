#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vector4f.h"
#include "Tri.h"
#include <assert.h>

/// class defines 4x4 matrix with matrix multiply operations defined
class Matrix4
{
public:
	float m[4][4];	// all matrix values

	/// def ctor will just fill identity matrix
  Matrix4()
	{
		memset( &m, 0, 16*sizeof(float) );  // clear out to 0
    m[0][0] = 1;
                m[1][1] = 1;
                            m[2][2] = 1;
                                        m[3][3] = 1;
	}

	/// ol ctor
	Matrix4(	float m00, float m01, float m02, float m03,
				    float m10, float m11, float m12, float m13,
				    float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33 )
	{
		m[0][0] = m00;  m[0][1] = m01;  m[0][2] = m02;  m[0][3] = m03;
		m[1][0] = m10;  m[1][1] = m11;  m[1][2] = m12;  m[1][3] = m13;
		m[2][0] = m20;  m[2][1] = m21;  m[2][2] = m22;  m[2][3] = m23;
    m[3][0] = m30;  m[3][1] = m31;  m[3][2] = m32;  m[3][3] = m33;
	}

  /// copy ctor
  Matrix4( const Matrix4 & mat )
  {
    // unrolled code runs faster than looping
    m[0][0] = mat.m[0][0];
    m[0][1] = mat.m[0][1];
    m[0][2] = mat.m[0][2];
    m[0][3] = mat.m[0][3];

    m[1][0] = mat.m[1][0];
    m[1][1] = mat.m[1][1];
    m[1][2] = mat.m[1][2];
    m[1][3] = mat.m[1][3];

    m[2][0] = mat.m[2][0];
    m[2][1] = mat.m[2][1];
    m[2][2] = mat.m[2][2];
    m[2][3] = mat.m[2][3];

    m[3][0] = mat.m[3][0];
    m[3][1] = mat.m[3][1];
    m[3][2] = mat.m[3][2];
    m[3][3] = mat.m[3][3];
  }

  bool operator==( const Matrix4 & other )
  {
    return this->Near( other ) ;
  }

  // There was a weird, weird problem with naming this 'near'
  bool Near( const Matrix4 & other, float eps=0.000001f ) const
  {
    return
      fabs( m[0][0] - other.m[0][0] ) < eps && 
      fabs( m[0][1] - other.m[0][1] ) < eps && 
      fabs( m[0][2] - other.m[0][2] ) < eps && 
      fabs( m[0][3] - other.m[0][3] ) < eps && 
                             
      fabs( m[1][0] - other.m[1][0] ) < eps && 
      fabs( m[1][1] - other.m[1][1] ) < eps && 
      fabs( m[1][2] - other.m[1][2] ) < eps && 
      fabs( m[1][3] - other.m[1][3] ) < eps && 
                             
      fabs( m[2][0] - other.m[2][0] ) < eps && 
      fabs( m[2][1] - other.m[2][1] ) < eps && 
      fabs( m[2][2] - other.m[2][2] ) < eps && 
      fabs( m[2][3] - other.m[2][3] ) < eps && 
                             
      fabs( m[3][0] - other.m[3][0] ) < eps && 
      fabs( m[3][1] - other.m[3][1] ) < eps && 
      fabs( m[3][2] - other.m[3][2] ) < eps && 
      fabs( m[3][3] - other.m[3][3] ) < eps ;
  }

	/// set elements
	void set(	float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33 )
	{
		m[0][0] = m00;  m[0][1] = m01;  m[0][2] = m02;  m[0][3] = m03;
		m[1][0] = m10;  m[1][1] = m11;  m[1][2] = m12;  m[1][3] = m13;
		m[2][0] = m20;  m[2][1] = m21;  m[2][2] = m22;  m[2][3] = m23;
    m[3][0] = m30;  m[3][1] = m31;  m[3][2] = m32;  m[3][3] = m33;
	}

	// performs matrix multiplication with other matrix
	void postMultiplyMatrix4By( const Matrix4 & a )
	{
		// e.g.
		/*
        [ m00 m01 m02 ]   [ a00 a01 a02 ]  
		    [ m10 m11 m12 ] * [ a10 a11 a12 ] 
		    [ m20 m21 m22 ]	  [ a20 a21 a22 ]

        [ m00*a00 + m01*a10 + m02*a20    m00*a01 + m01*a11 + m02*a21    m00*a02 + m01*a12 + m02*a22 ] 
      = [ m10*a00 + m11*a10 + m12*a20    m10*a01 + m11*a11 + m12*a21    m10*a02 + m11*a12 + m12*a22 ] 
        [ m20*a00 + m21*a10 + m22*a20    m20*a01 + m21*a11 + m22*a21    m20*a02 + m21*a12 + m22*a22 ] 
		*/

    

    Matrix4 result; // need new empty matrix to store result in
    // because values of m change as we compute the result.

    // "unrolled" code runs faster than loop
//      m[C][D] = mC0*a0D + mC1*a1D + mC2*a2D + mC3*a3D;
    result.m[0][0] = m[0][0]*a.m[0][0] + m[0][1]*a.m[1][0] + m[0][2]*a.m[2][0] + m[0][3]*a.m[3][0];
    result.m[0][1] = m[0][0]*a.m[0][1] + m[0][1]*a.m[1][1] + m[0][2]*a.m[2][1] + m[0][3]*a.m[3][1];
    result.m[0][2] = m[0][0]*a.m[0][2] + m[0][1]*a.m[1][2] + m[0][2]*a.m[2][2] + m[0][3]*a.m[3][2];
    result.m[0][3] = m[0][0]*a.m[0][3] + m[0][1]*a.m[1][3] + m[0][2]*a.m[2][3] + m[0][3]*a.m[3][3];
                                                                                         
    result.m[1][0] = m[1][0]*a.m[0][0] + m[1][1]*a.m[1][0] + m[1][2]*a.m[2][0] + m[1][3]*a.m[3][0];
    result.m[1][1] = m[1][0]*a.m[0][1] + m[1][1]*a.m[1][1] + m[1][2]*a.m[2][1] + m[1][3]*a.m[3][1];
    result.m[1][2] = m[1][0]*a.m[0][2] + m[1][1]*a.m[1][2] + m[1][2]*a.m[2][2] + m[1][3]*a.m[3][2];
    result.m[1][3] = m[1][0]*a.m[0][3] + m[1][1]*a.m[1][3] + m[1][2]*a.m[2][3] + m[1][3]*a.m[3][3];
                                                                                         
    result.m[2][0] = m[2][0]*a.m[0][0] + m[2][1]*a.m[1][0] + m[2][2]*a.m[2][0] + m[2][3]*a.m[3][0];
    result.m[2][1] = m[2][0]*a.m[0][1] + m[2][1]*a.m[1][1] + m[2][2]*a.m[2][1] + m[2][3]*a.m[3][1];
    result.m[2][2] = m[2][0]*a.m[0][2] + m[2][1]*a.m[1][2] + m[2][2]*a.m[2][2] + m[2][3]*a.m[3][2];
    result.m[2][3] = m[2][0]*a.m[0][3] + m[2][1]*a.m[1][3] + m[2][2]*a.m[2][3] + m[2][3]*a.m[3][3];
                                                                                                
    result.m[3][0] = m[3][0]*a.m[0][0] + m[3][1]*a.m[1][0] + m[3][2]*a.m[2][0] + m[3][3]*a.m[3][0];
    result.m[3][1] = m[3][0]*a.m[0][1] + m[3][1]*a.m[1][1] + m[3][2]*a.m[2][1] + m[3][3]*a.m[3][1];
    result.m[3][2] = m[3][0]*a.m[0][2] + m[3][1]*a.m[1][2] + m[3][2]*a.m[2][2] + m[3][3]*a.m[3][2];
    result.m[3][3] = m[3][0]*a.m[0][3] + m[3][1]*a.m[1][3] + m[3][2]*a.m[2][3] + m[3][3]*a.m[3][3];
    
    // copy results to THIS matrix.
    *this = result;
	}

  // performs matrix mult. with a vector.  modifies vector passed in.
  Vector4f & postmultiplyVector4By( Vector4f & v ) const
  {

    // Do not mangle v as we find the solution
    // Use temp variables x,y,z
    Vector4f result = getPostmultiplyVector4By( v ) ;

    v = result ;

    return v;
  }

  // performs matrix mult. with a vector.  diff between this and above func is
  // this func does NOT modify vector passed in, it returns a new vector object
  Vector4f getPostmultiplyVector4By( const Vector4f & v ) const
  {
    /*
    m00 m01 m02 m03   v.x   m00*v.x + m01*v.y + m02*v.z + m03*v.w
    m10 m11 m12 m13 * v.y = m10*v.x + m11*v.y + m12*v.z + m13*v.w
    m20 m21 m22 m23   v.z   m20*v.x + m21*v.y + m22*v.z + m23*v.w
    m30 m31 m32 m33   v.w   1.0 (remains 1.0)
    */

    Vector4f result ;

    result.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w;
    result.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w;
    result.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w;
    result.w = m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w;

    return result ;
  }

  Tri getTransformedTriangle( Tri * t ) const
  {
    // Copy the triangle over,
    Tri cpy = *t ;
    
    // apply xform to each vertex in THIS triangle
    this->postmultiplyVector4By( cpy.vertices[ 0 ] ) ;
    this->postmultiplyVector4By( cpy.vertices[ 1 ] ) ;
    this->postmultiplyVector4By( cpy.vertices[ 2 ] ) ;
    
    // perspective divide..
    cpy.vertices[0].perspectiveDivide() ;
    cpy.vertices[1].perspectiveDivide() ;
    cpy.vertices[2].perspectiveDivide() ;

    return cpy ;
  }

  //gl.
  void ortho( float l, float r, float b, float t, float n, float f )
  {
    *(this) = Matrix4(
      2/(r - l), 0,         0,         (r + l)/(r - l),
      0,         2/(t - b), 0,         (t + b)/(t - b),
      0,         0,         2/(n - f), (f + n)/(f - n),
      0,         0,         0,         1 );
  }

  //kilgard
  void persp( float fieldOfViewDegrees,
              float aspectRatio,
              float zNear, float zFar )
  {
    float sine, cotangent, deltaZ;
    float radians = fieldOfViewDegrees / 2.0f * PI / 180.0f;
    
    deltaZ = zFar - zNear;
    sine = sin(radians);
    /* Should be non-zero to avoid division by zero. */
    assert(deltaZ);
    assert(sine);
    assert(aspectRatio);
    cotangent = cos(radians) / sine;

    m[0][0] = cotangent / aspectRatio;
    m[0][1] = 0.0;
    m[0][2] = 0.0;
    m[0][3] = 0.0;
    
    m[1][0] = 0.0;
    m[1][1] = cotangent;
    m[1][2] = 0.0;
    m[1][3] = 0.0;
    
    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -2 * zNear * zFar / deltaZ;
    
    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = -1;
    m[3][3] = 0;
  }

  //kilgard
  void rotate( float angleDegrees, float ax, float ay, float az )
  {
    float radians, sine, cosine, ab, bc, ca, tx, ty, tz;
    float axis[3];
    float mag;

    axis[0] = ax;
    axis[1] = ay;
    axis[2] = az;
    mag = sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
    if (mag) {
      axis[0] /= mag;
      axis[1] /= mag;
      axis[2] /= mag;
    }

    radians = angleDegrees * PI / 180.0f;
    sine = sin(radians);
    cosine = cos(radians);
    ab = axis[0] * axis[1] * (1 - cosine);
    bc = axis[1] * axis[2] * (1 - cosine);
    ca = axis[2] * axis[0] * (1 - cosine);
    tx = axis[0] * axis[0];
    ty = axis[1] * axis[1];
    tz = axis[2] * axis[2];

    m[0][0] = tx + cosine * (1 - tx);
    m[0][1] = ab + axis[2] * sine;
    m[0][2] = ca - axis[1] * sine;
    m[0][3] = 0.0f;
    
    m[1][0] = ab - axis[2] * sine;
    m[1][1] = ty + cosine * (1 - ty);
    m[1][2] = bc + axis[0] * sine;
    m[1][3] = 0.0f;
    
    m[2][0] = ca + axis[1] * sine;
    m[2][1] = bc - axis[0] * sine;
    m[2][2] = tz + cosine * (1 - tz);
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

    this->transpose();
  }

  //kilgard
  void translate( float x, float y, float z )
  {
    m[0][0] = 1;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = x;
    m[1][0] = 0;  m[1][1] = 1;  m[1][2] = 0;  m[1][3] = y;
    m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 1;  m[2][3] = z;
    m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
  }

  //kilgard
  void lookAt( float eyex, float eyey, float eyez,
               float centerx, float centery, float centerz,
               float upx, float upy, float upz )
  {
    float x[3], y[3], z[3], mag;

    /* Difference eye and center vectors to make Z vector. */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;

    /* Normalize Z. */
    mag = sqrt(z[0]*z[0] + z[1]*z[1] + z[2]*z[2]);
    if (mag) {
      z[0] /= mag;
      z[1] /= mag;
      z[2] /= mag;
    }

    /* Up vector makes Y vector. */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;

    /* X vector = Y cross Z. */
    x[0] =  y[1]*z[2] - y[2]*z[1];
    x[1] = -y[0]*z[2] + y[2]*z[0];
    x[2] =  y[0]*z[1] - y[1]*z[0];

    /* Recompute Y = Z cross X. */
    y[0] =  z[1]*x[2] - z[2]*x[1];
    y[1] = -z[0]*x[2] + z[2]*x[0];
    y[2] =  z[0]*x[1] - z[1]*x[0];

    /* Normalize X. */
    mag = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    if (mag) {
      x[0] /= mag;
      x[1] /= mag;
      x[2] /= mag;
    }

    /* Normalize Y. */
    mag = sqrt(y[0]*y[0] + y[1]*y[1] + y[2]*y[2]);
    if (mag) {
      y[0] /= mag;
      y[1] /= mag;
      y[2] /= mag;
    }

    /* Build resulting view matrix. */
    m[0][0] = x[0];
    m[0][1] = x[1];
    m[0][2] = x[2];
    m[0][3] = -x[0]*eyex + -x[1]*eyey + -x[2]*eyez;

    m[1][0] = y[0];
    m[1][1] = y[1];
    m[1][2] = y[2];
    m[1][3] = -y[0]*eyex + -y[1]*eyey + -y[2]*eyez;

    m[2][0] = z[0];
    m[2][1] = z[1];
    m[2][2] = z[2];
    m[2][3] = -z[0]*eyex + -z[1]*eyey + -z[2]*eyez;

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;

  }

  void transpose()
  {
    for( int i = 0 ; i < 4 ; i++ )
    {
      for( int j = 0 ; j < i ; j++ )
      {
        float g = m[ i ][ j ] ;
        m[ i ][ j ] = m[ j ][ i ] ;
        m[ j ][ i ] = g ;
      }
    }
  }
};

#endif