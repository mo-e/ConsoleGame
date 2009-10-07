// Really basic console game engine :)


// Notes:
//   - You can ignore ALL the files except for this one.


// #includes and globals.
#include "gameMain.h"

bool gameRunning = true ;


void update()
{
  // If the person presses down the Escape key,
  if( IS_DOWN( VK_ESCAPE ) )
  {
    // Then quit the game.  This has an effect
    // WAAAY down below at the __MAIN GAME LOOP__
    gameRunning = false ;
  }
}


void draw()
{
  #pragma region clear
  //timer.update();  // Frame counter.
  timer.lock( 60 ) ; // lock at 60fps.
  static long long frameCount = 0 ;
  consoleRaster->setPos( 0, 1 );
  printf( "fps %f, fc %lld", timer.fps, frameCount++ ) ;
  
  // Clear out tris from last frame.
  consoleRaster->rasterizer->tris.clear();

  // Clear old transformation matrices
  consoleRaster->rasterizer->tMats.clear();
  #pragma endregion

  setStageBounds( -1, 1, -1, 1 ) ;

  #pragma region makes the rotating cube - YOU CAN ERASE ME
  static float r = 0 ;
  r++;
  consoleRaster->rotate( r, 0, 1, 0 ) ;
  consoleRaster->rotate( r, 1, 0, 0 ) ;
  consoleRaster->rasterizer->createCube();
  #pragma endregion

  // Rasterize.
  consoleRaster->draw() ;  // Do not touch me please.
}


void init()
{
  #pragma region init high performance counter
  QueryPerformanceFrequency( &timer.freq ) ;
  timer.fFreq = (double)timer.freq.QuadPart ;

  printf( "perf counter %lld ticks/second\n", timer.freq ) ;
  
  QueryPerformanceCounter( &timer.thisTime ) ;
  QueryPerformanceCounter( &timer.lastTime ) ;
  
  printf( "Program start @ time=%lld\n", timer.thisTime.QuadPart ) ;
  #pragma endregion

  consoleRaster = new ConsoleRaster() ;

  // Open log file
  logFile = fopen( "log.txt", "w" ) ;

  // Make the random numbers different every time
  // the program runs.
  srand( time(0) );

}


int main()
{
  init();

  // __MAIN GAME LOOP__
  while( gameRunning )
  {

    update();
    draw();

  }

  // Put cursor down at bottom.
  consoleRaster->setPos( 0, 24 ) ;

  // cleanup
  fclose( logFile ) ;

  delete consoleRaster ;
  
}