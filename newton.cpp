#include "newton.h"
#include "polymath.h"
#include "equation.h"
#include <vector>

using namespace std;
newton::~newton()
{}

newton::newton(): MITER(20), DIFF_THRESH(.00001)
{  

  std::cout<<"Creating Fractal with MITER="<<MITER<<endl;

}


void newton::fill_view(myview& view)
{
  
  current_view.resize(view.height);
  for (int j = 0; j < view.height; ++j)
    current_view[j].resize(view.width);
  
  double minx = view.cx - view.offx;
  //double maxx = view.cx + view.offx;

  double miny = view.cy - view.offy;
  //double maxy = view.cy + view.offy;
    
  base_type x, y;
  for (int j = 0; j < view.height; ++j)
  {
    //current_view[j].resize(view.width);
    y = miny + double(j)/(view.height-1.0f)*(2*view.offy);
    for (int i = 0; i < view.width; ++i)
    {
      x = minx + double(i)/(view.width-1.0f)*(2*view.offx);
      C start(x,y);
      current_view[j][i] = start;
    }
  }

}

void newton::newtonfill(equation& eq,
                        std::vector<std::vector<int> >& zero_index,
                        std::vector<std::vector<float> >& strength)
{  
  C endz;
  int ziter;
  int zind;
  base_type mindist;
  base_type diff;
  
  base_type sumiter = 0;
  base_type target1;
  base_type target2;
  base_type target3;
  
  base_type zfactor;

  int dimi = current_view.size();
  int dimj = current_view[0].size();

  zero_index.resize(dimi);
  strength.resize(dimi);
  
  for (int i = 0; i < dimi; ++i)
  {
    zero_index[i].resize(dimj);
    strength[i].resize(dimj);
  }

  for (int j = 0; j < dimj; ++j)
  {
    for (int i = 0; i < dimi; ++i)
    {   
      //if (true)
      //{
        //  if ((i >= current_view.size()) || (j >= current_view[i].size()))
        //  std::cout<<"error imminent"<<std::endl;
        const C& start(current_view[i][j]);
        base_type old_diff(0.0);

        // run the newton iteration
        run_newton(start,eq,endz,ziter,diff,old_diff);
        
        eq.assign_to_root(endz,zind,mindist);
//        std::cout<<"zind is " << zind<<std::endl;
        
        //zfactor = (1.0f - ((float) ziter)/MITER);
        //std::cout<<"diff/ziter "<<diff<<" "<<ziter<<std::endl;
        //zfactor = (1.0f);// - ((float) ziter)/MITER);
        //zfactor -= diff/DIFF_THRESH;

        // gets rid of banding a little bit
        zfactor = (1.0f - ((float) ziter +  (diff/DIFF_THRESH))/MITER);

        //zfactor = (1.0f) - (diff-old_diff)/DIFF_THRESH;
        //zfactor = diff*10000000;
        //std::cout<<"zfactor is " << zfactor<<std::endl;
        //zfactor = -log( diff / old_diff); //DIFF_THRESH;

        zero_index[i][j] = zind;
        strength[i][j] = zfactor;
         
        
        sumiter = sumiter + ziter;
        //}
        //else
        //{
        //target1 = 0;
        //target2 = 0;
        //target3 = 0;
        // }

    }
  }
}

void newton::run_newton(const C & start,
                        equation& eq,
                        C& cur,
                        int & niter,
                        base_type& diff,
                        base_type& old_diff)
{

  cur = start;
  
  for (niter = 0; niter < MITER; ++niter)
  {
    old = cur;
    
    eq.f_and_deriv_eval(cur,numer,denom);
    
    //eq.f_eval(cur,numer);
    //eq.deriv_eval(cur,denom);
    
    total = numer/denom;

    cur -= total; 

    diff = abs(total);
    if (diff < DIFF_THRESH)
    {
    //  //std::cout<<"breaking"<<std::endl;
      return;
    }
    old_diff = diff;
  } 
}
