struct Timer
{
  LARGE_INTEGER freq, thisTime, lastTime ;
  double fFreq ;
  double fps;
  double timeElapsed ;

  void update()
  {
    lastTime = thisTime ;
    QueryPerformanceCounter( &thisTime ) ;
    timeElapsed = ( thisTime.QuadPart - lastTime.QuadPart ) / fFreq ;
    fps = 1 / timeElapsed ;
  }

  void lock( float frames )
  {
    // Locks framerate at (frames) fps.
    lastTime = thisTime ;
    
    timeElapsed = 0;
    do
    {
      QueryPerformanceCounter( &thisTime ) ;
      timeElapsed = ( thisTime.QuadPart - lastTime.QuadPart ) / fFreq ;
      fps = 1/timeElapsed;
    } while( fps > frames ) ;

  }
} ;
