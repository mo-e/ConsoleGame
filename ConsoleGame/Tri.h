/// class Tri just defines an object type with 3 vertices to define a triangle
#ifndef TRI_H
#define TRI_H

#include "Vector4f.h"

class Tri
{
public:
	Vector4f vertices[3];  // will contain 3 verts
  Pixel    color[3];     // each vertex has a color specified

  Tri()
  {

  }

  Tri( const Tri & t )
  {
    memcpy( vertices, t.vertices, 3*sizeof(Vector4f) ) ;
    memcpy( color, t.color, 3*sizeof(Pixel) ) ;
  }

  // ol ctor, creates triangle vertices out of vectors a, b, c
  Tri( float ax, float ay, float az,
       float bx, float by, float bz,
       float cx, float cy, float cz )
  {
    vertices[ 0 ] = Vector4f( ax, ay, az );
    vertices[ 1 ] = Vector4f( bx, by, bz );
    vertices[ 2 ] = Vector4f( cx, cy, cz );

    color[ 0 ] = Pixel( 255, 0,   0,   255 );
    color[ 1 ] = Pixel( 0,   255, 0,   255 );
    color[ 2 ] = Pixel( 0,   0,   255, 255 );
  }

  // ol ctor takes 3 vector objects and makes a tri with default coloring scheme
  // (vertex 1 = red, vertex 2 = green, vertex 3 = blue
  Tri( Vector4f a, Vector4f b, Vector4f c)
  {
    vertices[ 0 ] = a;
    vertices[ 1 ] = b;
    vertices[ 2 ] = c;

    color[ 0 ] = Pixel( 255, 0,   0,   255 );
    color[ 1 ] = Pixel( 0,   255, 0,   255 );
    color[ 2 ] = Pixel( 0,   0,   255, 255 );
  }

  // ol ctor set vectors and colors of each vector
  Tri( Vector4f a, Vector4f b, Vector4f c,
       Pixel ca, Pixel cb, Pixel cc)
  {
    vertices[ 0 ] = a;
    vertices[ 1 ] = b;
    vertices[ 2 ] = c;

    color[ 0 ] = ca;
    color[ 1 ] = cb;
    color[ 2 ] = cc;
  }

  Tri( Vector4f a, Vector4f b, Vector4f c,
       Pixel colorToUse )
  {
    vertices[ 0 ] = a;
    vertices[ 1 ] = b;
    vertices[ 2 ] = c;

    color[ 0 ] = color[ 1 ] = color[ 2 ] = colorToUse ;
  }

  void setColor( Pixel & theColor )
  {
    color[ 0 ] = color[ 1 ] = color[ 2 ] = theColor ;
  }

  void print()
  {
    puts("");
    for(int i = 0; i < 3 ; i++)
      vertices[i].print();
  }
};

#endif