#ifndef RASTERIZER_H
#define RASTERIZER_H



inline int at( int row, int col, int NUMCOLS )
{
  return (col + row*NUMCOLS) ;
}

struct Colors
{
  // init colors by name for easy triangle and line creation
  Pixel black ;
  Pixel gray ;
  Pixel red ;
  Pixel green;
  Pixel blue;

  Pixel magenta;
  Pixel cyan;
  Pixel yellow;
  Pixel white;

  Colors()
  {
    black = Pixel( 0, 0, 0, 255 );
    gray = Pixel( 180, 180, 180, 255 ) ;
    red = Pixel( 255, 0, 0, 255 );
    green = Pixel( 0, 255, 0, 255 ) ;

    blue = Pixel( 0, 0, 255, 255 ) ;
    magenta = Pixel( 255, 0, 255, 255 ) ;
    cyan = Pixel( 0, 255, 255, 255 ) ;
    yellow = Pixel( 255, 255, 0, 255 );
    white = Pixel( 255, 255, 255, 255 );
  }
} colors ;

class Rasterizer
{
private:
	int cols ;  // width of pixel array, x,
	int rows ;  // height of pixel array, y,

public:
	Pixel * pixels ; // The 2d output buffer of the rasterizer.
  // ends up with color values after each frame.
  float * zBuffer ;// the 2d z-buffer for visible surface determination.
  // We use linear arrays for memset().

  vector<Tri>	tris;        // actual object coordinates in real space
  vector<Tri>	Xtris ;      // triangles after full transformation - in 'normalized device space'

  Matrix4 projection ;
  Matrix4 view ;
  vector<Matrix4> tMats ;  // other transformation matrices
  // that should be postmultiplied to the projection/view mats.
  // Might include additional rotations or whatev's.

  

  #pragma region // cube init - around the origin using 12 tris
  void createCube()
  {

    float size = 0.68f;
    Vector4f A( -size, -size,  size );
    Vector4f B( -size,  size,  size );
    Vector4f C(  size,  size,  size );
    Vector4f D(  size, -size,  size );
    Vector4f E( -size, -size, -size );
    Vector4f F( -size,  size, -size );
    Vector4f G(  size,  size, -size );
    Vector4f H(  size, -size, -size );

    // front face
    tris.push_back( Tri( A, B, C, colors.blue, colors.cyan, colors.white ) ) ;
    tris.push_back( Tri( A, C, D, colors.blue, colors.white, colors.magenta ) ) ;

    // back face
    tris.push_back( Tri( E, F, G, colors.black, colors.green, colors.yellow )  );
    tris.push_back( Tri( E, G, H, colors.black, colors.yellow, colors.red ) );

    // top side
    tris.push_back( Tri( B, F, G, colors.cyan, colors.green, colors.yellow ) );
    tris.push_back( Tri( B, G, C, colors.cyan, colors.yellow, colors.white ) );

    // bottom side
    tris.push_back( Tri( A, E, H, colors.blue, colors.gray, colors.red ) );
    tris.push_back( Tri( A, H, D, colors.blue, colors.red, colors.magenta ) );

    // left side
    tris.push_back( Tri( A, B, F, colors.blue, colors.cyan, colors.green ) );
    tris.push_back( Tri( A, F, E, colors.blue, colors.green, colors.gray ) );

    // right side
    tris.push_back( Tri( D, C, G, colors.magenta, colors.white, colors.yellow ) );
    tris.push_back( Tri( D, G, H, colors.magenta, colors.yellow, colors.red ) );
    
  }
  #pragma endregion
  

  /// whole lot of init'ing the data, verts for the cube,
  /// colors for the cube verts
	Rasterizer( int rowSize, int colSize )
	{
    pixels = 0 ;
    zBuffer = 0 ;

    // First we need to allocate mems for the color buffer (pixels) and zBuffer
    sizeRasterAndZBuff( rowSize, colSize );

    #pragma region // init transformation matrices
    // create orthographic projection matrix
    float r = 5.0f, l = -5.0f, t = 5.0f, b = -5.0f, n = -5.0f, f = 5.0f;

    // init with an orthographic projection matrix
    //projectionMatrix->ortho( l, r, b, t, n, f ) ;
    projection.persp( 45.0f, 1.0f, 1.0f, 100.0f ) ;

    view.lookAt( 0, 0, 5, 0, 0, 0, 0, 1, 0 ) ;
    #pragma endregion
  }



  // Change the size of the raster grid.
  void sizeRasterAndZBuff( int i_rows, int i_cols )
  {
    rows = i_rows ;
    cols = i_cols ;

    printf( "Resizing raster to rows=%d, cols=%d\n", rows, cols ) ;

    if( pixels )
      free( pixels ) ;
    if( zBuffer )
      free( zBuffer ) ;

    pixels = (Pixel*)malloc( rows*cols * sizeof(Pixel) ) ;
    zBuffer = (float*)malloc( rows*cols * sizeof(float) ) ;

  }

  /// returns to you the concatenated transformation matrix
  Matrix4 getConcatenatedTransform()
  {
    // Projection * View * ( rest of transfomations in tMats )
    //   redbook 108:  vertex => modelview => (( eye coordinates )) => projection =>
    //                 (( clip coordinates )) => perspective division => (( normalized device coordinates )) =>
    //                 viewport => (( window coordinates ))
    
    Matrix4 m = projection ;
    m.postMultiplyMatrix4By( view ) ;

    for(int i = 0; i < (int)tMats.size(); i++)
      m.postMultiplyMatrix4By( tMats[i] );    // multiply each matrix
      // to get concatenated transformation matrix
    
    return m;
  }

  /// function updates state of pixels in array
  void draw()
  {
    // let's update by first clearing the entire pixel mem to 0's
    memset( pixels, 0, rows*cols*sizeof(Pixel) );
    memset( zBuffer, 127, rows*cols*sizeof(float) );   // clear the depth buffer
    // This gives each 4 byte float in the z-buffer HUGE values
    // of 339615136492207130000000000000000000000.000000 exactly :)
    // If you're having portability issues, a blank screen, or
    // some other kind of weirdness that has to do with the z-buffer,
    // perhaps you should debugging here.
    
    ///////////////////////////////////
    transform();  // this applies some spatial transformation and projection
    // matrix transforms to the triangles being drawn

    ///////////////////////////////////
    // DRAWING
    // finally, RASTERIZE/DRAW each triangle
    for(int i = 0 ; i < (int)Xtris.size(); i++)        
      drawFillTri( Xtris[i] );

  }



  #pragma region // drawing funcs
  /// Fills triangle abc.  xa, ya, xb, yb... must all
  /// be pixel positions specified on the raster grid.  assumes ccw winding.
  /// helper func returns min and max of a, b and c (by ref, as args)
  void get3MinMax( float a, float b, float c, float & min, float & max )
  {
    if( a > b )   // b eliminated as max, but possibly min
    {
      if(a > c) // a > b and a > c, but we don't know if b > c or not
      {
        max = a;
        if(b < c)     // a > c > b
          min = b;
        else
          min = c;  // a > b > c
      }
      else                // here c > a > b
      {
        max = c;
        min = b;
      }
    }
    else	// b > a
    {
      if( b > c )   // b > a and b > c, can't tell if a > c at this point, though
      {
        max = b;
        if( a > c )   // b > a > c
          min = c;
        else
          min = a;  // b > c > a
      }
      else	// here we know right away that bx > ax (first comp) and b < c
        // this tells us then, that c > b > a
      {
        max = c;
        min = a;
      }
    }
  }

  /// a simple function that gives you x and y pixel coordinates (raster pos) for
  /// "canonical" floating point coords that are on ([-1, 1], [-1, 1], [-1, 1])
  inline float getXRasterPos( Vector4f & v )
  {
    // assuming v.x and v.y are between -1 and +1, we
    // can get their xy by:
    // 1.  TAKE COORDS from being in box [(-1, -1), (1,1)] to
    // being in box [(0,0),(2,2)] (x++ and y++)
    // 2.  MULTIPLY though by WIDTH/2
    // WIDTH / 2         *  ( 1 + xCANONICAL )
    return ( (0.5f*cols)  *  (1+v.x) );
  }

  /// gives you y raster pos, given a y value on [-1, 1]
  inline float getYRasterPos( Vector4f & v )
  {
    return ( 0.5f*rows*(1+v.y) );
  }

  /// Fills triangle specified by verts of Tri object passed to this func.
  void drawFillTri( Tri & tri )
  {
    // set up some aliases for easy handling of vertices
    Vector4f & a = tri.vertices[0];    // a is first vertex
    Vector4f & b = tri.vertices[1];    // b is second..
    Vector4f & c = tri.vertices[2];

    // get the positions of vertices a, b and c on the raster grid
    float xa = getXRasterPos( a );
    float ya = getYRasterPos( a );
    float xb = getXRasterPos( b );
    float yb = getYRasterPos( b );
    float xc = getXRasterPos( c );
    float yc = getYRasterPos( c );

    // alright, how do we rasterize a triangle given 3 points on the raster grid?
    /*
    ''''''''''''''
    '''''''▒▒▒c'''
    '''a▒▒▒▓▓▒''''
    ''''▒▒▓▓▓▒''''
    ''''''▒▓▒'''''
    '''''''▒▒'''''
    ''''''''b'''''
    ''''''''''''''
    */

    // basically what we have to do is to scan the entire space that bounds the triangle
    // from (min( ax, bx, cx )) to max(ax, bx, cx).
    // if a pixel is inside the triangle, then we color it.  else, we don't color it.

    // find max, mins:
    float minx, miny, maxx, maxy;
    get3MinMax( xa, xb, xc, minx, maxx );
    get3MinMax( ya, yb, yc, miny, maxy );

    // now, iterate in the square that surrounds the entire triangle
    /*
    8 ''''''''''''''
    7 '''X------c'''
    6 '''a▒▒▒▓▓▒|'''
    5 '''|▒▒▓▓▓▒|'''
    4 '''|''▒▓▒'|'''
    3 '''|'''▒▒'|'''
    2 '''X----b-X'''
    1 ''''''''''''''
      12345678901234
               11111
    */
    // that's the set of pixels we need to check to see if they are inside the triangle

    // must get barycentric coordinates
    float alpha, beta, gamma;
    float betaDen;

    // at each pixel in the larger rectangle surrounding the triangle...
    for( int xp = (int)minx; xp <= maxx ; xp++ )
    {
      for( int yp = (int)miny; yp <= maxy ; yp++ )
      {
        // we need to test to see IF each PIXEL falls inside the triangle.....
        // finding barycentric coordinates, alpha, beta, gamma.

        betaDen = (xa - xb)*(yc - yb) + (xc - xb)*(yb - ya);
        
        beta = (xp - xa)*(ya - yc) + (xa - xc)*(ya - yp) ;
        beta /= betaDen;
        
        gamma = (xp - xa)*(ya - yb) + (xa - xb)*(ya - yp) ;
        gamma /= -betaDen;
        alpha = 1 - beta - gamma;

        // now I have alpha, beta and gamma, the barycentric coordinates

        // CONDITIONS to be inside tri:
        // 1.  alpha + beta + gamma = 1 (guaranteed, since
        //     found alpha as 1 - beta - gamma)
        // 2.  0 <= alpha <= 1 && 0 <= beta <= 1 && 0 <= gamma <= 1

        // Now let me get the z coords only at this pixel
        float zValue = ( a.z * alpha + b.z * beta + c.z * gamma ) ;

        // if we use conditions 0 < alpha < 1, drawing excludes triangles edges
        // if we use conditions 0 <= alpha <= 1, drawing includes triangles edges
        if( 0 <= alpha && alpha <= 1 &&
            0 <= beta && beta <= 1 &&
            0 <= gamma && gamma <= 1 )
        {
          // at this point in the code, we're inside a triangle
          // first, a bounds check:
          if( xp >= 0 && xp < cols && yp >= 0 && yp < rows )
          {
            int index = at( yp, xp, cols ) ;
            // next, check to see if the z-value of this pixel that we
            // are about to draw should appear in front of any pixels
            // that are already drawn
            if( zValue < zBuffer[ index ] ) // then this fragment closer than existing fragment.
            {
              zBuffer[ index ] = zValue; // update current zValue of fragment,
              // so that this pixel will not be overwritten
              // by pixels from another triangle that is
              // actually further away

              // color of this fragment is going to be weighted average of
              // colors of each vertex, alpha as "weight" of color contribution
              // from first vertex, beta as weight of color contribution from
              // second vertex, and gamma as weight of color from third vertex.
              pixels[ index ].r = (UCHAR)(
                tri.color[0].r * alpha +
                tri.color[1].r * beta +
                tri.color[2].r * gamma );

              pixels[ index ].g = (UCHAR)(
                tri.color[0].g * alpha +
                tri.color[1].g * beta +
                tri.color[2].g * gamma );

              pixels[ index ].b = (UCHAR)(
                tri.color[0].b * alpha +
                tri.color[1].b * beta +
                tri.color[2].b * gamma );
            }
          }
        }
      }
    }
  }

  /// applies the set of transformation matrices defined in 'matrices' to all
  /// the vertices in the scene
  void transform()
  {
    Matrix4 xform = getConcatenatedTransform();
    Xtris.clear() ;

    // iterate through all polygons and apply the concatenated
    // transformation matrix to each vertex.

    // the concatenated transformation matrix takes vertices from
    // real space into the unit cube space defined around the origin
    for(int i = 0; i < (int)tris.size(); i++)
    {
      Xtris.push_back( xform.getTransformedTriangle( &tris[i] ) ) ;
    }
  }

  #pragma endregion


  ~Rasterizer()
  {
    free( pixels ) ;
    free( zBuffer ) ;
  }

};

#endif
