%MATLAB WITHOUT LOOP

tic; 

M2 = ones(100, 100) * 2;
M3 = ones(100, 100) * 3;
M1 = M2 + M3;

toc;

disp('M2:')
disp(M2(1:5, 1:5))
disp('M3:')
disp(M3(1:5, 1:5))
disp('M1:')
disp(M1(1:5, 1:5))


%  MATLAB - ROW MAJOR LOOPS
tic;
M2 = ones(100, 100) * 2;
M3 = ones(100, 100) * 3;
M1 = zeros(100, 100);

for i = 1:100
    for j = 1:100
        M1(i, j) = M2(i, j) + M3(i, j);
    end
end

toc; 

disp('M2 sample elements:')
disp(M2(1:5, 1:5))
disp('M3 sample elements:')
disp(M3(1:5, 1:5))
disp('M1 sample elements:')
disp(M1(1:5, 1:5))

%  MATLAB - COLUM MAJOR LOOPS
tic;
M2 = ones(100, 100) * 2;
M3 = ones(100, 100) * 3;
M1 = zeros(100, 100);

for j = 1:100
    for i = 1:100
        M1(i, j) = M2(i, j) + M3(i, j);
    end
end

toc; 

disp('M2 sample elements:')
disp(M2(1:5, 1:5))
disp('M3 sample elements:')
disp(M3(1:5, 1:5))
disp('M1 sample elements:')
disp(M1(1:5, 1:5))