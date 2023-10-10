#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    int ID; // 注册号
    string name; // 姓名
    double math; // 数学成绩
    double english; // 英语成绩
    double computer; // 计算机成绩

public:
    // 设置学生数据信息
    void setStuInf(int ID, const string& name, double math, double english, double computer) {
        this->ID = ID;
        this->name = name;
        this->math = math;
        this->english = english;
        this->computer = computer;
    }
    
    // 求三门课总成绩
    double sum() {
        return math + english + computer;
    }
    
    // 求三门课平均成绩
    double average() {
        return sum() / 3.0;
    }
    
    // 显示学生数据信息
    void print() {
        cout << "注册号：" << ID << endl;
        cout << "姓名：" << name << endl;
        cout << "数学成绩：" << math << endl;
        cout << "英语成绩：" << english << endl;
        cout << "计算机成绩：" << computer << endl;
    }
    
    // 获取学生注册号
    int getID() {
        return ID;
    }
};

int main() {
    Student students[5]; 
    
    // 输入并设置学生数据信息
    for (int i = 0; i < 5; i++) {
        int ID;
        string name;
        double math, english, computer;
        
        cout << "请输入学生的注册号：" << endl;
        cin >> ID;
        cout << "请输入学生的姓名：" << endl;
        cin >> name;
        cout << "请输入学生的数学成绩：" << endl;
        cin >> math;
        cout << "请输入学生的英语成绩：" << endl;
        cin >> english;
        cout << "请输入学生的计算机成绩：" << endl;
        cin >> computer;
        
        students[i].setStuInf(ID, name, math, english, computer);
    }
    
    double sumScores = 0.00; 
    double maxScore = 0.00; 
    
    // 计算每一个学生的总成绩并输出相关信息
    for (int i = 0; i < 5; i++) {
        double totalScore = students[i].sum();
        double averageScore = students[i].average();
        
        cout << "第" << i+1 << "个学生的总成绩：" << totalScore << endl;
        cout << "第" << i+1 << "个学生的平均成绩：" << averageScore << endl;
        
        sumScores += totalScore;
        
        if (totalScore > maxScore) {
            maxScore = totalScore;
        }
    }
    
    // 计算全班学生的平均成绩
    double averageScoreOfClass = sumScores / 5;
    
    cout << "全班学生总成绩最高分：" << maxScore << endl;
    cout << "全班学生平均成绩：" << averageScoreOfClass << endl;
    
    // 输出指定注册号学生的全部数据信息
    int searchID;
    cout << "请输入要查询的学生的注册号：" << endl;
    cin >> searchID;
    
    for (int i = 0; i < 5; i++) {
        if (students[i].getID() == searchID) {
            students[i].print();
            break;
        }
    }
    
    return 0;
}