#ifndef POLYMATH_H__
#define POLYMATH_H__
// Functions for polynomial math such as multiplication and taking derivatives

#include "fractal_types.h"
#include <vector>

class polymath
{
public:

  typedef fractal_types::base_type base_type;
  typedef fractal_types::C C;
  
  // Multiply two polynomials
  static void polymult(const std::vector<C >& coeff1,
                const std::vector<C >& coeff2,
                std::vector<C >& result );
  
  // Compute the repeated product over the 
  // terms \prod_{i=1}^{N_{zeros}}(z-zero_i)
  static std::vector<C > 
    polyrep(const std::vector<C >& zeros);
  
  // Compute the derivative of the polynomial
  static std::vector<C > 
    polyderiv(const std::vector<C >& poly);
  
  // evaluate the polynomial at this location
  //static void polyeval(const std::vector<C >& poly,
  //              const C& z,
  //              C& result);
			
  static void polyeval(const std::vector<C >&poly,
                                 const C& z,
                                 C& result)
{
/*  
  // not horner's
  result=poly[0];
  C curz = z;
  for (int i = 1; i < poly.size(); ++i)
  {
    result+=(poly[i]*curz);
    curz*=z;
  }
  //std::cout << "result is " << result << std::endl;
*/

  // horner's method
  result = poly[poly.size()-1];
  for (int i = poly.size()-2; i>=0; i--)
  {
	result*=z;
	result+=poly[i];
    //result = poly[i] + result*z;
  }

  //std::cout << "result is " << result << std::endl;
}
};
#endif
