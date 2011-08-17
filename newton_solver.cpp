/*
  [x,y,niter,zeroind] = mine(x,y,zeros1,zeros2,ps,MITER);
*/
#include "math.h"
#include "mex.h"   
#include <complex>
#include <iostream>
#include <vector>

using namespace std;

void runit(int MITER,const complex<double>& start, complex<double>& cur, double& niter);
void runit2(int MITER, const complex<double>& start, complex<double>& cur, double& niter, std::vector<complex<double> > const& zeros, std::vector<double> const& ps, double& zind);

complex<double> f(complex<double> const& x);
complex<double> fprime(complex<double> const& x);
complex<double> fdivfp(complex<double> const& x, std::vector<complex<double> > const& zeros, std::vector<double> const& pows);
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  const mxArray *xData, *yData, *zeroxData, *zeroyData, *pData, *mData;
  double *xValues, *yValues, *zeroxValues,*zeroyValues, *pValues, *mValues, *xOut, *yOut, *iOut, *zeroindOut;
  int i,j;
  int rowLen, colLen;
  
  int NUMZEROS = -1;

  

  xData = prhs[0];
  yData = prhs[1];
  zeroxData = prhs[2];
  zeroyData = prhs[3];
  pData = prhs[4];
  mData = prhs[5];
  
  xValues = mxGetPr(xData);
  yValues = mxGetPr(yData);
  zeroxValues = mxGetPr(zeroxData);
  zeroyValues = mxGetPr(zeroyData);
  pValues = mxGetPr(pData);
  mValues = mxGetPr(mData);
  
  int MITER = (int)mValues[0];


  rowLen = mxGetN(xData);
  colLen = mxGetM(xData);
  NUMZEROS = mxGetN(zeroxData);

  std::vector<complex<double> > zeros(NUMZEROS);
  std::vector<double> ps(NUMZEROS);
  
  for (int i = 0; i < NUMZEROS; i++)
  {
    zeros[i] = complex<double>(zeroxValues[i],zeroyValues[i]);
    ps[i]    = pValues[i];
  }


  plhs[0] = mxCreateDoubleMatrix(colLen, rowLen, mxREAL); 
  plhs[1] = mxCreateDoubleMatrix(colLen, rowLen, mxREAL);
  plhs[2] = mxCreateDoubleMatrix(colLen, rowLen, mxREAL);
  plhs[3] = mxCreateDoubleMatrix(colLen, rowLen, mxREAL);
  
  
  xOut = mxGetPr(plhs[0]);
  yOut = mxGetPr(plhs[1]);
  iOut = mxGetPr(plhs[2]);
  zeroindOut = mxGetPr(plhs[3]);
  
  complex<double> endz;
  double iter,zind;
 
  for(i=0;i<rowLen;i++)
  {

    for(j=0;j<colLen;j++)
    {
      
      complex<double> z(xValues[(i*colLen)+j],yValues[(i*colLen)+j]); 
      
      runit2(MITER,z,endz,iter,zeros,ps,zind);
      /*runit(MITER,z,endz,iter);*/
      xOut[(i*colLen)+j] = endz.real();
      yOut[(i*colLen)+j] = endz.imag();
      iOut[(i*colLen)+j] = iter;
      zeroindOut[(i*colLen)+j] = zind;
      
    }
  }
    
  return;
}
      

void runit(int MITER, const complex<double>& start, complex<double>& cur, double& niter)
{
  niter = 0;
  double diff;
  complex<double> old(start);
  cur = start;
  
  while (1)
  {
    niter = niter + 1;
    old = cur;
    cur = cur - f(cur)/fprime(cur);
    diff = abs(old-cur);
    if ( niter==MITER || diff < .000000001 )
    {
      return;
    }
    

  }
}

void runit2(int MITER, const complex<double>& start, complex<double>& cur, double& niter, std::vector<complex<double> > const& zeros, std::vector<double> const& ps, double& zind)
{
  niter = 0;
  double diff;
  complex<double> old(start);
  cur = start;
  
  while (1)
  {
    old = cur;
    cur = cur - fdivfp(cur,zeros,ps);//%f(cur)/fprime(cur);
    diff = abs(old-cur);
    niter = niter + 1;
    if (niter == MITER || diff < .000001)
    {
      break;
    }
    

  }

  //now find the nearest zero
  zind = 0;
  double mindist = abs(zeros[0]-cur);
  double curdist;
  for (int i = 1; i < zeros.size(); ++i)
  {
    curdist = abs(zeros[i]-cur);
    if (curdist < mindist)
    {
      zind = (double)i;
      mindist = curdist;
    }
  }


}

complex<double> f(complex<double> const& x)
{
  return pow(x,3.0) - complex<double>(1);
  //return x * cos(x)*sin(x) - complex<double>(1,1);
}

complex<double> fprime(complex<double> const& x)
{
  return 3.0*pow(x,2.0);
  //return cos(x)*sin(x) - x*sin(x)*sin(x) + x*cos(x)*cos(x);
}

complex<double> fdivfp(complex<double> const& x, std::vector<complex<double> > const& zeros, std::vector<double> const& pows)
{
  //return f(z)/f'(z)
  complex<double> total(0.0,0.0);
  for (int k = 0; k <zeros.size(); ++k)
  {
    total = total + pows[k]/(x - zeros[k]);
  }
  return 1.0/total;
}
