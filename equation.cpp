#include "equation.h"
#include <iostream>

void equation::assign_to_root(const C& cur,
                              int& zind,
                              base_type& mindist)
{

  base_type curdist;
  zind = zero_inds[0];
  mindist = abs(zeros[0]-cur);
  for (int i = 1; i < zeros.size(); ++i)
  {
    curdist = abs(zeros[i]-cur);
    if (curdist < mindist)
    {
      zind = zero_inds[i];
      mindist = curdist;
    }
  }

}

void radial_zeros_equation::localize_zeros(base_type offset) 
{
  zeros.clear();
  zero_inds.clear();
  powers.clear();
  
  int counter = 0;
  //base_type thetadiff = (2*M_PI)/NZEROS * .5;
  //int SPINTARGET = 0;
  for (int i = 0; i < NZEROS; ++i)
  {
    base_type thetanow = (2*M_PI)/NZEROS*i;
    for (int j = 0; j < 1; ++j)
    {
      base_type radius = (j+1)*1.0f;
      //zeros.push_back( (radius + 2*(j==1)*sin(4*offset))* C(cos(thetanow+j*thetadiff + (j==SPINTARGET)*offset),sin(thetanow+j*thetadiff + (j==SPINTARGET)*offset)));
      //zeros.push_back( radius* C(cos(thetanow)*sin(j*thetadiff)+cos(offset),sin(thetanow)*cos(j*thetadiff)+sin(offset)));
      zeros.push_back(radius*C(cos(thetanow)*sin(offset*10),cos(offset)+sin(thetanow)));
      zero_inds.push_back(counter++);
      powers.push_back( 1 );
    }
  }

  poly_ = polymath::polyrep(zeros);
  deriv_ = polymath::polyderiv(poly_);
}

void arbitrary_equation::newton_solve(C& cur, int niter)
{
  C f;
  C fprime;
  
  for (int i = 0; i < niter; ++i)
  {
    f_eval(cur,f);
    deriv_eval(cur,fprime);
    cur = cur - f/fprime;
  }
}

void arbitrary_equation::add_if_new(const C& cur, int& zerocounter)
{
  if (zeros.size()==0)
  {
    zeros.push_back(cur);
    zero_inds.push_back(zerocounter++);
    return;
  }
  
  base_type mindist = abs(cur-zeros[0]);
  
  for (int i = 1; i < zeros.size(); ++i)
  {
    base_type curdist = abs(cur-zeros[i]);
    if (curdist < mindist)
    {
      mindist = curdist;
    }
  }
  
  if (mindist > .01)
  {
    zeros.push_back(cur);
    zero_inds.push_back(zerocounter++);
  }
  // else we have already found this one  
}

void arbitrary_equation::localize_zeros(base_type offset) {
  bias = offset;
  
  // now we must fill the zeros so let's just go crazy here with
  // newton iterations

  std::vector<C> oldzeros = zeros;
  std::vector<int> oldinds = zero_inds;

  zeros.clear();
  zero_inds.clear();

  int zerocounter = 0;
 
  for (base_type x = -2; x<=2; x+=.1)
    for (base_type y = -2; y<=2; y+=.1)
    {
      C start(x,y);
      newton_solve(start,100);
      add_if_new(start,zerocounter);
    }

  if (oldzeros.size() > 0) 
  {
    
    // remapping is not going to work out nicely
    //remap_zeros(zeros,zero_inds,oldzeros,oldinds);
  }
  
  std::cout<<"Done localizing zeros: found "<<zeros.size()<<" zeros"<<std::endl;
  //std::cout<<"Done localizing zeros: found "<<zero_inds.size()<<" zeros"<<std::endl;
  //for (int i = 0; i < zero_inds.size(); ++i)
  //  std::cout<<"ind is " << zero_inds[i]<<std::endl;
}
