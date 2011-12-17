#ifndef EQUATION_H__
#define EQUATION_H__

#include "polymath.h"
#include <vector>
#include <iostream>
#include "fractal_types.h"

//Base class representing a generic equation which has some zeros
class equation
{

public:

  typedef fractal_types::base_type base_type;
  typedef fractal_types::C C;
  
  //localize the zeros of this function
  virtual void localize_zeros(base_type offset) = 0;

  // assign the answer to a zero
  virtual void assign_to_root(const C& cur,
                              int& zind,
                              base_type& mindist);

  // evaluate the function
  virtual void f_eval(const C& cur,
                      C& retval) = 0;
  
  // evaluate derivative
  virtual void deriv_eval(const C& cur,
                          C& retval) = 0;

  virtual void f_and_deriv_eval(const C& cur,
                                C& value,
                                C& deriv_value)
  {
    f_eval(cur,value);
    deriv_eval(cur,deriv_value);
  }

  int zero_size() { return this->zeros.size(); }
protected:
  std::vector<C> zeros;
  std::vector<int> zero_inds;
};

class radial_zeros_equation: public equation
{
private:
  // the state of the fractal is encoded by the zeros
  // as well as the view of it

  std::vector<base_type> powers;
  
  // the underying polynomial
  std::vector<C> poly_;
  
  // the underlying derivative polynomial
  std::vector<C> deriv_;

  int NZEROS;

public:

  radial_zeros_equation(int n): NZEROS(n) {}
  
  //generate zeros based on theta splits
  virtual void localize_zeros(base_type offset);


  virtual void f_eval(const C& cur,
                      C& retval)
  {
    polymath::polyeval(poly_,cur,retval);
  }
  
  virtual void deriv_eval(const C& cur,
                          C& retval)
  {
    polymath::polyeval(deriv_,cur,retval);
  }
};

class arbitrary_equation: public equation {
public:
  base_type bias;

  // do a quick newton solver
  void newton_solve(C& cur, int niter=20);

  // add this zero to list of zeros if it is different
  void add_if_new(const C& cur, int& zerocounter);

  virtual void localize_zeros(base_type offset);
  
  virtual void f_eval(const C& z,
                      C& retval)
  {
    // really cool
    //retval = z*(pow(z,-3.0)-C(1,0))-C(cos(bias),sin(bias));

    //retval = z*(pow(z,-3.0)-C(1,0))-C(cos(bias),sin(bias));

    // brown flower
    //retval = (z*(pow(z,-4.0)-C(.5+2*cos(bias),.5+2*sin(bias))) - (1.0+cos(bias))*C(.2,.3))*C(cos(bias)*sin(bias));
   
    //last thing
    //retval = z*(pow(z,-4.0)-C(cos(1.4*bias)*2,sin(2.3*bias)+cos(bias)))-C(cos(bias),.1+sin(2.3*bias));
    
    //retval = pow(z,3)*C(cos(10*bias),sin(10*bias))-C(1,0);
    retval = (z-C(1,0))*(z-C(-1,0))*(z-C(2*cos(bias),2*sin(bias)));
    //retval = pow(C(cos(bias),0),z)*cos(z)*sin(z)-C(1,1);
    //retval = z*cos(z+3*bias)*sin(z) + pow(C(1,1),z);
    //retval = bias*pow(z,3)-C(1,0);
  }

  // evaluate the function's derivative
  virtual void deriv_eval(const C& x,
                          C& retval)
  {
    const base_type h = .00001;
    C fx;
    C fxh;
    f_eval(x+h,fxh);
    f_eval(x,fx); 
    retval = (fxh-fx)/h;
  }

  // evaluate the function's derivative
  virtual void f_and_deriv_eval(const C& x,
                                C& value,
                                C& deriv_value)
  {
    const base_type h = .00001;
    C fxh;
    f_eval(x+h,fxh);
    f_eval(x,value);
    deriv_value = (fxh-value)/h;
  }

private:

};

#endif
