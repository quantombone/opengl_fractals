void newton::run_newton(const complex<double> & start, complex<double>& cur, int & niter){
cur = start;

for (niter = 0; niter < MITER; ++niter)
{
old = cur;
complex<double> total(0.0,0.0);

total = total + 1.0/(cur - std::complex<double>(1,0))+ 1.0/(cur - std::complex<double>(0.309017,0.951057))+ 1.0/(cur - std::complex<double>(-0.809017,0.587785))+ 1.0/(cur - std::complex<double>(-0.809017,-0.587785))+ 1.0/(cur - std::complex<double>(0.309017,-0.951057));
cur = cur - 1.0/total; diff = abs(old-cur); if (diff < .00001) { return;}
} 
}
