% 读取Excel文件
data = readtable('通信课表.xlsx');

% 获取各列数据
course_number = data{:, 1};
course_name = data{:, 2};
credit = data{:, 3};
is_required = data{:, 4};
prerequisite = cellfun(@(x) str2num(x), split(data{:, 5}, ','), 'UniformOutput', false);
course_type = cellfun(@(x) str2num(x), split(data{:, 6}, ','), 'UniformOutput', false);

% 创建线性规划问题
prob1 = optimproblem('ObjectiveSense', 'maximize');
prob2 = optimproblem('ObjectiveSense', 'minimize');
prob3 = optimproblem('ObjectiveSense', 'minimize');
prob4 = optimproblem('ObjectiveSense', 'maximize');

% 创建决策变量
x1 = optimvar('x1', numel(data), 'Type', 'integer', 'LowerBound', 0, 'UpperBound', 1);
x2 = optimvar('x2', numel(data), 'Type', 'integer', 'LowerBound', 0, 'UpperBound', 1);
x3 = optimvar('x3', numel(data), 'Type', 'integer', 'LowerBound', 0, 'UpperBound', 1);
x4 = optimvar('x4', numel(data), 'Type', 'integer', 'LowerBound', 0, 'UpperBound', 1);

% 添加目标函数
prob1.Objective = sum(x1);
prob2.Objective = sum(x2);
prob3.Objective = 0.3 * sum(credit .* x3) + 0.7 * sum(x3);
prob4.Objective = 0.7 * sum(credit .* x4) + 0.3 * sum(x4);

% 添加约束条件
for i = 1:numel(data)
    % 必修课程必须选择
    if is_required(i) == 1
        prob1.Constraints.cons1(i) = x1(i) == 1;
        prob2.Constraints.cons1(i) = x2(i) == 1;
        prob3.Constraints.cons1(i) = x3(i) == 1;
        prob4.Constraints.cons1(i) = x4(i) == 1;
    end
    
    % 先修课程要求
    for pre = prerequisite{i}
        prob1.Constraints.cons2(i) = x1(i) <= x1(pre);
        prob2.Constraints.cons2(i) = x2(i) <= x2(pre);
        prob3.Constraints.cons2(i) = x3(i) <= x3(pre);
        prob4.Constraints.cons2(i) = x4(i) <= x4(pre);
    end
    
    % 属于类型的约束
    for t = course_type{i}
        prob1.Constraints.cons3(i) = x1(i) <= sum(x1(course_type{i}));
        prob2.Constraints.cons3(i) = x2(i) <= sum(x2(course_type{i}));
        prob3.Constraints.cons3(i) = x3(i) <= sum(x3(course_type{i}));
        prob4.Constraints.cons3(i) = x4(i) <= sum(x4(course_type{i}));
    end
end

% 解决四个模型
sol1 = solve(prob1);
sol2 = solve(prob2);
sol3 = solve(prob3);
sol4 = solve(prob4);

% 获取最优解和学分总和
x1_opt = sol1.x1;
x2_opt = sol2.x2;
x3_opt = sol3.x3;
x4_opt = sol4.x4;
total_credits = [sum(credit(x1_opt == 1)), sum(credit(x2_opt == 1)), sum(credit(x3_opt == 1)), sum(credit(x4_opt == 1))];

% 打印结果
disp("目标一模型 - 只考虑尽可能多的学分");
disp("学分总和: " + total_credits(1));
disp("选修课程: ");
disp(course_name(x1_opt == 1)');

disp("目标二模型 - 选修课程最少");
disp("学分总和: " + total_credits(2));
disp("选修课程: ");
disp(course_name(x2_opt == 1)');

disp("目标三模型 - 同时考虑学分最多和选修课最少");
disp("学分总和: " + total_credits(3));
disp("选修课程: ");
disp(course_name(x3_opt == 1)');

disp("目标四模型 - 同时考虑课程最少和所获得的学分最多");
disp("学分总和: " + total_credits(4));
disp("选修课程: ");
disp(course_name(x4_opt == 1)');
