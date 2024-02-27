% 清空当前工作区
clear
clc

% 定义圆柱体参数
cylinderHeight = 10; % 圆柱体高度
cylinderRadius = 4; % 圆柱体半径

% 定义圆锥体参数
coneHeight = 6; % 圆锥体高度
coneRadius = 2; % 圆锥体底面半径

% 定义球体参数
sphereRadius = 2; % 球体半径

% 定义长方体参数
boxLength = 8; % 长方体长度
boxWidth = 2; % 长方体宽度
boxHeight = 2; % 长方体高度

% 画底视图
subplot(2, 2, 1);
rectangle('Position', [-cylinderRadius, -cylinderRadius, cylinderRadius*2, cylinderRadius*2], 'Curvature', [1, 1], 'LineWidth', 1);
hold on;
rectangle('Position', [-coneRadius, -coneRadius, coneRadius*2, coneRadius*2], 'Curvature', [1, 1], 'LineWidth', 1);
hold on;
rectangle('Position', [-boxWidth/2, -cylinderRadius, boxWidth, cylinderRadius*2], 'LineWidth', 1);
hold on;
rectangle('Position', [-cylinderRadius-boxLength, -cylinderRadius, boxLength, cylinderRadius*2], 'LineWidth', 1);
hold off;
axis equal;
axis([-cylinderRadius-boxLength-2, cylinderRadius+2, -cylinderRadius-2, cylinderRadius+2]);

% 画左视图
subplot(2, 2, 2);
rectangle('Position', [-cylinderRadius, -cylinderRadius, cylinderHeight, cylinderRadius*2], 'LineWidth', 1);
hold on;
rectangle('Position', [cylinderHeight, -coneRadius, coneHeight, coneRadius*2], 'Curvature', [0, 1], 'LineWidth', 1);
hold on;
rectangle('Position', [cylinderHeight, -sphereRadius, sphereRadius*2, sphereRadius*2], 'Curvature', [1, 1], 'LineWidth', 1);
hold on;
rectangle('Position', [cylinderHeight, -cylinderRadius, boxHeight, cylinderRadius*2], 'LineWidth', 1);
hold off;
axis equal;
axis([-2, cylinderHeight+coneHeight+sphereRadius+2, -cylinderRadius-2, cylinderRadius+2]);



