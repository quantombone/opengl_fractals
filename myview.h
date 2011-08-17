#ifndef MYVIEW_H__
#define MYVIEW_H__
// A simple datastructure representing the view of the fractal
struct myview
{
  // the center of the view
  double cx,cy;

  // the offset along x and y dimensions
  double offx, offy;
  
  // the width/height of the viewing area
  int width, height;
  
};
#endif
