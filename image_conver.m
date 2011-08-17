files = dir('ims/*png');
for i = 1:length(files)
  fprintf(1,'.');
  system(['convert ims/' files(i).name ' ims/' files(i).name '.gif']);
end