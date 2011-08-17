#include <fstream>
#include <vector>
#include <complex>
using namespace std;

void generate_zeros(std::vector<std::complex<double> > &zeros,std::vector<double>& powers, int NZEROS) 
{
  zeros.clear();
  powers.clear();

  zeros.resize(NZEROS);
  powers.resize(NZEROS);

  for (int i = 0; i < NZEROS; ++i)
  {
    double thetanow = (2*M_PI)/NZEROS*i;
    zeros[i] = std::complex<double>(cos(thetanow),sin(thetanow));
    powers[i] = 1;
  }
}

int main()
{

  // generating out.cpp which is a run_newton function
  ofstream outfile("out.cpp");
  std::vector<std::complex<double> > zeros;
  std::vector<double> powers;

  generate_zeros(zeros,powers,5);

  outfile<<"void newton::run_newton(const complex<double> & start, complex<double>& cur, int & niter){"<<endl;
  outfile<<"cur = start;\n"<<endl;

  //for (int i = 0; i < 50; ++i)
  //{
    outfile<<"for (niter = 0; niter < MITER; ++niter)"<<endl;

    //outfile<<"niter = "<<niter<<";"<<endl;
    outfile<<"{\nold = cur;\ncomplex<double> total(0.0,0.0);\n"<<endl;
    outfile<<"total = total ";
    for (int k = 0; k < zeros.size(); ++k)
    {
      outfile<<"+ "<<powers[k]<<".0/(cur - std::complex<double>("<<zeros[k].real()<<","<<zeros[k].imag()<<"))";
    }
    outfile<<";"<<endl;
    outfile<<"cur = cur - 1.0/total; diff = abs(old-cur); if (diff < .00001) { return;}\n} "<<endl;
    //}
  outfile<<"}"<<endl;
  
  outfile.close();
  return 0;

/*

  // generating out.cpp which is a run_newton function
  ofstream outfile("out.cpp");
  std::vector<std::complex<double> > zeros;
  std::vector<double> powers;

  generate_zeros(zeros,powers,5);

  outfile<<"void newton::run_newton(const complex<double> & start, complex<double>& cur, int & niter){"<<endl;
  outfile<<"double diff;\ncomplex<double> old;\ncur = start;\n"<<endl;
  
  outfile<<"for (niter = 0; niter < MITER; ++niter)"<<endl;

  outfile<<"{\nold = cur;\ncomplex<double> total(0.0,0.0);\n"<<endl;
  outfile<<"total = total ";
  for (int k = 0; k < zeros.size(); ++k)
  {
    outfile<<"+ "<<powers[k]<<".0/(cur - std::complex<double>("<<zeros[k].real()<<","<<zeros[k].imag()<<"))";
  }
  outfile<<";"<<endl;
  outfile<<"cur = cur - 1.0/total; diff = abs(old-cur); if (diff < .00001) { return;}} "<<endl;
  outfile<<"}"<<endl;

  outfile.close();
  return 0;

 */
}
