addpath('./sphere/');

%% Elaboration
data_reader;
[~, output] = sphere_delaunay(length(input), input');

% Index adjustment
output = output' - 1;

%% Output
ftemp = fopen('hrtf_data.hpp', 'A');
format = '\t{ %d, %d, %d },\n';

fprintf(ftemp, '#define TRIPLET_SIZE %d\n\n', length(output));

fprintf(ftemp, 'extern "C" int hrtf_triplets[][3] =\n{\n');
for i = 1 : length(output)
  fprintf(ftemp, format, output(i, 1), output(i, 2), output(i, 3));
end
fprintf(ftemp, '};\n\n');

fclose(ftemp);
