#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    int ID; // ע���
    string name; // ����
    double math; // ��ѧ�ɼ�
    double english; // Ӣ��ɼ�
    double computer; // ������ɼ�

public:
    // ����ѧ��������Ϣ
    void setStuInf(int ID, const string& name, double math, double english, double computer) {
        this->ID = ID;
        this->name = name;
        this->math = math;
        this->english = english;
        this->computer = computer;
    }
    
    // �����ſ��ܳɼ�
    double sum() {
        return math + english + computer;
    }
    
    // �����ſ�ƽ���ɼ�
    double average() {
        return sum() / 3.0;
    }
    
    // ��ʾѧ��������Ϣ
    void print() {
        cout << "ע��ţ�" << ID << endl;
        cout << "������" << name << endl;
        cout << "��ѧ�ɼ���" << math << endl;
        cout << "Ӣ��ɼ���" << english << endl;
        cout << "������ɼ���" << computer << endl;
    }
    
    // ��ȡѧ��ע���
    int getID() {
        return ID;
    }
};

int main() {
    Student students[5]; 
    
    // ���벢����ѧ��������Ϣ
    for (int i = 0; i < 5; i++) {
        int ID;
        string name;
        double math, english, computer;
        
        cout << "������ѧ����ע��ţ�" << endl;
        cin >> ID;
        cout << "������ѧ����������" << endl;
        cin >> name;
        cout << "������ѧ������ѧ�ɼ���" << endl;
        cin >> math;
        cout << "������ѧ����Ӣ��ɼ���" << endl;
        cin >> english;
        cout << "������ѧ���ļ�����ɼ���" << endl;
        cin >> computer;
        
        students[i].setStuInf(ID, name, math, english, computer);
    }
    
    double sumScores = 0.00; 
    double maxScore = 0.00; 
    
    // ����ÿһ��ѧ�����ܳɼ�����������Ϣ
    for (int i = 0; i < 5; i++) {
        double totalScore = students[i].sum();
        double averageScore = students[i].average();
        
        cout << "��" << i+1 << "��ѧ�����ܳɼ���" << totalScore << endl;
        cout << "��" << i+1 << "��ѧ����ƽ���ɼ���" << averageScore << endl;
        
        sumScores += totalScore;
        
        if (totalScore > maxScore) {
            maxScore = totalScore;
        }
    }
    
    // ����ȫ��ѧ����ƽ���ɼ�
    double averageScoreOfClass = sumScores / 5;
    
    cout << "ȫ��ѧ���ܳɼ���߷֣�" << maxScore << endl;
    cout << "ȫ��ѧ��ƽ���ɼ���" << averageScoreOfClass << endl;
    
    // ���ָ��ע���ѧ����ȫ��������Ϣ
    int searchID;
    cout << "������Ҫ��ѯ��ѧ����ע��ţ�" << endl;
    cin >> searchID;
    
    for (int i = 0; i < 5; i++) {
        if (students[i].getID() == searchID) {
            students[i].print();
            break;
        }
    }
    
    return 0;
}