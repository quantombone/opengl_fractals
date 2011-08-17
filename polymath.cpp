#include "polymath.h"
#include <iostream>


void polymath::polymult(const std::vector<C >& coeff1,
              const std::vector<C >& coeff2,
              std::vector<C >& result )
{
  // computes the multiplication of all elements
  int maxpow = (coeff1.size()-1) + (coeff2.size()-1);
  result.resize(maxpow+1,C(0,0));

  for (int i = 0; i < coeff1.size(); ++i)
  {
    for (int j =0; j < coeff2.size(); ++j)
    {
      int curpow = i+j;
      result[curpow]+=(coeff1[i]*coeff2[j]);
    }

  }
}

std::vector<polymath::C> polymath::polyrep(const std::vector<C>& zeros)
{
  std::vector<C > poly(2);
  poly[0] = -zeros[0];
  poly[1] = 1.0;
  for (int i = 1; i < zeros.size(); ++i)
  {
    std::vector<C > cur(2);
    cur[0] = -zeros[i];
    cur[1] = 1.0;
    std::vector<C > newpoly;
    polymult(poly,cur,newpoly);
    poly = newpoly;
  }

  return poly;
}

std::vector<polymath::C> polymath::polyderiv(const std::vector<C >& poly)
{
  std::vector<C > deriv(poly.size()-1);
  for (int i = 1; i < poly.size(); ++i)
  {
    deriv[i-1] = (base_type)i*poly[i];
  }
  return deriv;
}



