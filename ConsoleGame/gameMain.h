#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <conio.h>

#include <assert.h>
#include <time.h>

#include "ConsoleRaster.h"
#include "Timer.h"

Timer timer ;  // for counting frame rate.
ConsoleRaster * consoleRaster ;
#define IS_DOWN(x) (GetAsyncKeyState(x)&0x8000)


FILE * logFile ;

/// creates a polygon with any number of points desired
/// centered at (xCenter, yCenter).  Higher z values means
/// CLOSER to the viewer.
void polygon( int numPoints, Pixel color, float xCenter, float yCenter, float radius, float rotationDegrees, float z=0 )
{
  if( numPoints < 3 )
  {
    puts( "You can't draw a polygon with fewer than 3 points!  Correcting..." ) ;
    numPoints = 3 ;
  }

  float rads = DEGTORAD( rotationDegrees ) ;
  float incr = 2*PI / numPoints ;

  Vector4f center( xCenter, yCenter, z ) ;
  vector<Vector4f> points ;
  
  for( float phi = rads ;
       phi < rads + 2*PI ;
       phi += incr )
  {
    // rotate the point.
    float ptX = radius * cos( phi ) ;
    float ptY = radius * sin( phi ) ;

    // Create a point
    points.push_back( center + Vector4f( ptX, ptY, 0 ) ) ;
  }


  // Now assemble tris
  for( UINT i = 0 ; i < points.size() - 1 ; i++ )
  {
    consoleRaster->rasterizer->tris.push_back( 

      Tri(
        center,
        points[ i ],
        points[ i + 1 ],
        color
      )

    ) ;
  }

  // The last triangle goes from the last point to the first.
  consoleRaster->rasterizer->tris.push_back( 

    Tri(
      center,
      points[ points.size()-1 ],
      points[ 0 ],
      color
    )

  ) ;

}


void rectangle( Pixel color, float left, float right, float bottom, float top, float z=0 )
{
  Vector4f topLeft( left, top, z ) ;
  Vector4f topRight( right, top, z ) ;
  Vector4f bottomLeft( left, bottom, z ) ;
  Vector4f bottomRight( right, bottom, z ) ;

  consoleRaster->rasterizer->tris.push_back( 

    Tri(
      bottomRight,
      topRight,
      bottomLeft,
      color
    )

  ) ;

  consoleRaster->rasterizer->tris.push_back( 

    Tri(
      topRight,
      topLeft,
      bottomLeft,
      color
    )

  ) ;
}



/// This sets the area of space you are "looking at".
void setStageBounds( float left, float right, float bottom, float top )
{
  //consoleRaster->rasterizer->projection.ortho(
  //  left, right, bottom, top, -1000, 1000 ) ;

  //!!.
  consoleRaster->rasterizer->projection.ortho(
    -right, -left, -top, -bottom, -1000, 1000 ) ;
}


void triangleDebugSpew()
{
  system("cls");
  for( int i = 0 ; i < consoleRaster->rasterizer->tris.size(); i++ )
  {
    consoleRaster->rasterizer->tris[i].print();
  }
  system("pause");
}




float randFloat( float min, float max )
{
  return min + ((float)rand() / RAND_MAX)*max ;
}

float randFloat()
{
  return randFloat( 0.0f, 1.0f ) ;
}