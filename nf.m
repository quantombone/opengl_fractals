function nf;
%newton's method fractals by Tomasz Malisiewicz
%mex-ed driver function to be super fast

center = [0 0];
off = [2 2];
NDIV = [400 400]; %[800 600];


colors = [1 0 0; ...
          0 1 0; ...
          0 0 1; ...
          1 1 0; ...
          0 1 1; ...
          [225 20 147] / 255; ...
          [255 69 0] / 255; ...
          [153 50 204] / 255; ...
          [205 193 197] / 255; ...
          [153 204 50]/ 255];

MITER = 50;


SKIP = 1000;
thetas = linspace(0,2*pi,SKIP);

iter = 1;
for ttt = 0:SKIP; %1:length(thetas)

  theta = thetas(mod(ttt,SKIP)+1);

  
  xs = linspace(center(1)-off(1),center(1)+off(1),NDIV(1));
  ys = linspace(center(2)-off(2),center(2)+off(2),NDIV(2));

  [x,y] = meshgrid(xs,ys);

  [curzeros,curps] = generate_zeros(theta);
  %curzeros = zeros * [cos(theta) sin(theta);-sin(theta) ...
  %                    cos(theta)];



  [xres,yres,niter,zeroinds] = mine(x,y,curzeros(:,1)',curzeros(:, ...
                                                  2)',curps,MITER);

  colorim = colorit(niter,zeroinds,colors,MITER);
  

  %[heights] = getheights(curzeros,curps,xs,ys);
  %surf(xs,ys,heights,colorim)
  %shading flat
  %light
  imwrite(colorim,sprintf('ims/%.5d.png',ttt));
  imshow(colorim)
  drawnow;
  %off = off*.95;
  
  iter = iter+1;
  
end

function colorim = colorit(niter,zeroinds,colors,MITER)

colorim = colors(zeroinds(:)+1,:) .* repmat(1-(niter(:)./MITER),1,3);
colorim = reshape(colorim,size(niter,1),size(niter,2),3);

function [curzeros,curps] = generate_zeros(theta)
%generate some zeros from a zero-generating function
NZEROS = 4;
thetacuts = linspace(0,2*pi,NZEROS+1);
thetacuts = thetacuts(1:(end-1))';

curps = ones(1,NZEROS);
curzeros = [cos((thetacuts)) sin((thetacuts))];
curps = [cos(2*theta) 1+sin(3*theta) sin(4*theta) .2+sin(5*theta)];

%for i = 1:NZEROS
%  curps(i) = 1+.1*cos(i*20*theta);
%end

%curzeros = [1 0; 0 1; -1 0; 0 -1];
%curps = [1 1 1 1];

function [heights] = getheights(curzeros,curps,xs,ys)

heights = xs;
for ii = 1:length(xs)
  for jj = 1:length(ys)
    val = 1;
    for kk = 1:length(curzeros)
%      val = val * curps(k)*exp(-3*norm([curzeros(k,:) - [xs(i)
%      ys(j)]])^2);
      val = val * (-curzeros(kk,1)-curzeros(kk,2)*i + xs(ii) + ys(jj)*i)^curps(kk);
    end
    heights(ii,jj) = -abs(val);
  end
end
