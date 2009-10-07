#ifndef PIXEL_H
#define PIXEL_H

/// struct for picture element
struct Pixel
{
	unsigned char r, g, b, a;	// 8 bits for each of rgba

	// initializer list in def ctor to zero out each new pix
	Pixel()
		: r(0), g(0), b(0), a(0)
	{
	}

	Pixel( unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia )
		: r(ir), g(ig), b(ib), a(ia) 
	{
	}

  Pixel( unsigned char ir, unsigned char ig, unsigned char ib )
		: r(ir), g(ig), b(ib)
	{
	}


};

#endif