#include "math.h"
#include "myview.h"
#include "equation.h"
#include "fractal_types.h"
#include <iostream>
#include <vector>

//using namespace std;

// A class representing a newton's method fractal
class newton
{

  typedef fractal_types::base_type base_type;
  typedef fractal_types::C C;

protected:
  
  // The maximum number of newton iterations
  int MITER;

  // The difference threshold that determines when newton iterations stop
  base_type DIFF_THRESH;
  
  // all of the pixels in the current view
  std::vector<std::vector<C > > current_view;

public:
  
  newton();
  ~newton();
  
  // run newton iterations that given a start complex number
  // iterates and returns the final state (cur)
  // the number of iterations (niter)
  // the index of the nearest root (zind)
  // the distance to the nearest root (mindist)
  void run_newton(const C & start,
                  equation& eq,
                  C& cur,
                  int & niter,
                  base_type& diff,
                  base_type& old_diff);
  
  // run newton for all pixels in the view
  void newtonfill(equation& eq,
                  std::vector<std::vector<int> >& zero_index,
                  std::vector<std::vector<float> >& strength); 
  
  // fill the locations of all pixels inside view
  void fill_view(myview& view);

private:

  // some temporary variables for speedup 
  C numer;
  C denom;
  C total;
  C old;
};
