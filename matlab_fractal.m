function [niters,solutions] = matlab_fractal
%Create Newton's Method Fractal Image
%Tomasz Malisiewcz (tomasz@cmu.edu)
%http://quantombone.blogspot.com/
NITER = 40;
threshold = .001;

[xs,ys] = meshgrid(linspace(-1,1,800), linspace(-1,1,800));
solutions = xs(:) + i*ys(:);
select = 1:numel(xs);
niters = NITER*ones(numel(xs), 1);

for iteration = 1:NITER
  oldi = solutions(select);
  
  %in newton's method we have z_{i+1} = z_i - f(z_i) / f'(z_i)
  solutions(select) = oldi - f(oldi) ./ fprime(oldi);
  
  %check for convergence or NaN (division by zero)
  differ = (oldi - solutions(select));
  converged = abs(differ) < threshold;
  problematic = isnan(differ);
  
  niters(select(converged)) = iteration;
  niters(select(problematic)) = NITER+1;
  select(converged | problematic) = [];
end

niters = reshape(niters,size(xs));
solutions = reshape(solutions,size(xs));

function res = f(x)
res = (x.^2).*x - 1;

function res = fprime(x)
res = 3*x.^2;
