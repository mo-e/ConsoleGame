#ifndef CONSOLE_RASTER_H
#define CONSOLE_RASTER_H

#include <vector>
using namespace std;


#include "Pixel.h"
#include "Tri.h"
#include "Vector4f.h"
#include "Matrix4.h"
#include "Rasterizer.h"

struct ConsoleRaster
{
private:
  int gridX ;
  int gridY ;

  COORD gridSize ;
  COORD startingPos ;

  CHAR_INFO* grid ;
  HANDLE h ;

  SMALL_RECT rectWriteRegion ;
  COORD pos ;

  
public:
  // Rasterizing subsystem.
  Rasterizer * rasterizer ;



  // Stuff that's necessary for this object.
  ConsoleRaster()
  {
    // Get the size of hte console window.
    h = GetStdHandle( STD_OUTPUT_HANDLE ) ;
    
    system( "mode 80, 37" ); // change size of console.

    // Another way to change size of the console.
    //HWND hwnd = GetConsoleWindow();
    //MoveWindow( hwnd, 0, 0, 800, 800, true ) ;

    CONSOLE_CURSOR_INFO cci ;
    cci.bVisible = false ; // hide blinking console cursor.
    cci.dwSize = 10 ;    // between 1 and 100.  100 means "full size"
    SetConsoleCursorInfo( h, &cci ) ;

    CONSOLE_SCREEN_BUFFER_INFO csbi ;
    GetConsoleScreenBufferInfo( h, &csbi ) ;

    printf( "Maxwinsize %d %d\n",csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y );
    printf( "current size %d %d\n", csbi.dwSize.X, csbi.dwSize.Y );
    printf( "window measurements %d %d %d %d\n", csbi.srWindow.Left, csbi.srWindow.Top, csbi.srWindow.Right, csbi.srWindow.Bottom ) ;
    printf( "attributes: %d\n", csbi.wAttributes ) ;
    //system( "pause" ) ; // annoying.
    system( "cls" ) ;
    // Determine raster grid size.
    gridX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    gridY = csbi.srWindow.Bottom - csbi.srWindow.Top - 6 ;

    // init array
    grid = (CHAR_INFO*)malloc( gridX * gridY * sizeof( CHAR_INFO ) ) ;

    gridSize.X = gridX ;
    gridSize.Y = gridY ;

    startingPos.X = 0 ;
    startingPos.Y = 0 ;

    // Init console with some values.
    for( int i = 0 ; i < gridSize.X * gridSize.Y; i++ )
    {
      grid[ i ].Attributes = rand()%16 ;  // 0==black, 15==white
      grid[ i ].Char.AsciiChar = (char)(rand()%255);

      // 7 is beep, so prevent from outputting it.
      if( grid[i].Char.AsciiChar == 7 ) grid[ i ].Char.AsciiChar = 6 ;
    }


    // This controls the VIEWPORT.  moving this around
    // moves around your display of your graphics
    // WITHIN the console window.
    rectWriteRegion.Left = 0;
    rectWriteRegion.Top = 2;   // leave 2 lines at the top
    // for the fps counter as well as other text messages.
    rectWriteRegion.Bottom = rectWriteRegion.Top + gridSize.Y ;
    rectWriteRegion.Right = rectWriteRegion.Left + gridSize.X ;




    // Now initialize the rasterizing system underneath
    rasterizer = new Rasterizer( gridY, gridX ) ;
  }

  void setPos( int x, int y )
  {
    pos.X = x ;
    pos.Y = y ;
    SetConsoleCursorPosition( h, pos ) ;
  }

  void rotate( float angleDegrees, float x, float y, float z )
  {
    Matrix4 mat ;
    mat.rotate( angleDegrees, x, y, z ) ;
    rasterizer->tMats.push_back( mat ) ;
  }

private:
  unsigned char getChar( Pixel & pixel )
  {
    unsigned char c = ' ' ;

    // Compute the intensity
    float brightness = 0.2126f*pixel.r + 0.7152f*pixel.g + 0.0722f*pixel.b ;
    // http://en.wikipedia.org/wiki/Luma_(video)

    // max value is 0.2126*255 + 0.7152*255 + 0.0722*255 = 255 ;
    // select character based on brightness grade

    if( brightness < 2 )
      c = ' ' ;
    else if( brightness < 5 )
      c = '\'' ;
    else if( brightness < 15 )
      c = '`' ;
    else if( brightness < 25 )
      c = '"' ;
    else if( brightness < 35 )
      c = ',' ;
    else if( brightness < 45 )
      c = ':' ;
    else if( brightness < 55 )
      c = ';' ;
    else if( brightness < 65 )
      c = 176 ; // 176 = lightly shaded
    else if( brightness < 75 )
      c = '-' ;
    else if( brightness < 85 )
      c = '+' ;
    else if( brightness < 95 )
      c = '<' ;
    else if( brightness < 115 )
      c = 'k' ;
    else if( brightness < 135 )
      c = 'm' ;
    else if( brightness < 155 )
      c = '8' ;
    else if( brightness < 185 )
      c = 157 ; // 157 = Yen sign
    else if( brightness < 205 )
      c = 177 ; // 177 = mid shaded block
    else if( brightness < 225 )
      c = 178 ; // 178 = heavily shaded block
    else
      c = 219 ; // 219 = completely solid block

    return c ;
  }

  unsigned short getColor( Pixel & pixel )
  {
    // mix it down to 1 bit for each.
    unsigned short color = 0 ;

    if( pixel.r < 25 )  // red isn't on.
      ;
    else if( pixel.r < 150 ) // red dim
      color |= FOREGROUND_RED ;
    else  // red bright on
      color |= FOREGROUND_RED | FOREGROUND_INTENSITY ;

    if( pixel.g < 25 ) // not on.
      ;
    else if( pixel.g < 150 ) // dim
      color |= FOREGROUND_GREEN ;
    else  // bright on
      color |= FOREGROUND_GREEN | FOREGROUND_INTENSITY ;

    if( pixel.b < 25 ) // not on.
      ;
    else if( pixel.b < 150 ) // dim
      color |= FOREGROUND_BLUE ;
    else  // bright on
      color |= FOREGROUND_BLUE | FOREGROUND_INTENSITY ;

    return color ;
  }
  
public:
  /// draws out pixels in 2D array to window
  // Copy pixels to the CONSOLE'S RASTER.
  // Transform rgb to simple colors and characters.
  // High intensity use a character like X, low intensity, l or , or '
  void draw()
  {
    
    // Clear the screen
    //static int count; // (test with random)
    //memset( raster.grid, count++, byteSize ) ;
    
    int byteSize = gridX * gridY * sizeof( CHAR_INFO ) ;
    memset( grid, 19, byteSize ) ;

    rasterizer->draw() ;
    for( int row = 0 ; row < gridY ; row++ )
    {
      for( int col = 0 ; col < gridX ; col++ )
      {
        
        int rasterIndex = at( row, col, gridX ) ;

        int invertedRow = gridY - 1 - row ; // the console is upside down.
        int consoleIndex = at( invertedRow, col, gridX ) ;

        grid[ consoleIndex ].Attributes = getColor( rasterizer->pixels[ rasterIndex ] ) ;
        grid[ consoleIndex ].Char.AsciiChar = getChar( rasterizer->pixels[ rasterIndex ] ) ;

      }
    }

    // draw the raster grid
    WriteConsoleOutputA( h, grid, gridSize, startingPos, &rectWriteRegion ) ;



  }


  ~ConsoleRaster()
  {
    free( grid ) ;

    delete rasterizer ;
  }
}  ;




#endif