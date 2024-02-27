% 假设有N个座位，教室的尺寸为MxM
N = 30; % 座位数量
M = 8;  % 教室尺寸

% 创建座位坐标
seats = zeros(N, 2);
for i = 1:N
    seats(i, 1) = randi(M); % 随机生成x坐标
    seats(i, 2) = randi(M); % 随机生成y坐标
end

% 计算座位之间的距离矩阵
distances = pdist2(seats, seats);

% 设置目标函数权重
weight_visibility = 0.5;
weight_interaction = 0.3;
weight_comfort = 0.2;

% 定义目标函数
objective = @(x) weight_visibility * visibility(x, distances) + ...
    weight_interaction * interaction(x, distances) + ...
    weight_comfort * comfort(x, seats);

% 定义约束函数
constraints = @(x) seat_constraints(x, M);

% 使用遗传算法进行优化
options = gaoptimset('Display', 'iter');
[x, fval] = ga(objective, N, [], [], [], [], [], [], constraints, options);

% 结果解释
disp("最优座位选择方案：");
disp(x);
disp("目标函数值：");
disp(fval);

% 辅助函数：计算可见度
function vis = visibility(x, distances)
    N = size(x, 2);
    vis = 0;
    for i = 1:N-1
        for j = i+1:N
            dist = distances(x(i), x(j));
            vis = vis + 1/dist;
        end
    end
end

% 辅助函数：计算互动性
function inter = interaction(x, distances)
    N = size(x, 2);
    inter = 0;
    for i = 1:N-1
        for j = i+1:N
            dist = distances(x(i), x(j));
            inter = inter + dist;
        end
    end
end

% 辅助函数：计算舒适度
function comf = comfort(x, seats)
    N = size(x, 2);
    comf = 0;
    for i = 1:N-1
        for j = i+1:N
            dist = norm(seats(x(i), :) - seats(x(j), :));
            comf = comf + 1/dist;
        end
    end
end

% 辅助函数：座位约束
function [c, ceq] = seat_constraints(x, M)
    N = size(x, 2);
    c = [];
    ceq = [];
    for i = 1:N-1
        for j = i+1:N
            if x(i) == x(j) % 座位重复约束
                c(end+1) = 0;
            end
        end
    end
end
