#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>
using namespace std;

void stringTest()
{
    // 常用构造函数
    string str = "hello";
    // string();                  // 创建一一个空的字符串例如: string str;
    // string(const string& str); // 使用一个string对象初始化月一个string对象
    // string(const char *s);     // 使用字符串s初始化
    // string(int n, char c);     // 使用n个字符C初始化v

    // 赋值操作
    // string &operator=(const char *s);                  // char*类型字符串 鼠值给当前的字符串
    // string &operator=(const string &s);                // 把字符串s赋给当前的字符串
    // string &operator=(char c);                         // 字符赋值给当前的字符串
    // string &assign(const char *s);                     // 把字符串 s赋给当前的字符串
    // string &assign(const char *s, int n);              // 把字符串s的前n个字符赋给当前的字符串
    // string &assign(const string &s);                   // 把字符串 s联给当前字符串
    // string &assign(int n, char c);                     // 用 n个字符C試给当前字符串
    // string &assign(const string &s, int start, int n); // 将s从start开始n个字符赋值给字符串

    // 存取字符操作
    // char& operator[](int n); // 通过[] 方式取字符
    // char& at(int n);         // 通过at方法获取字符

    // 拼接操作
    // string &operator+=(const string &str);           // 页载+=操作符
    // string &operator+=(const char *str);             // 重载+=操作符
    // string &operator+=(const char c);                // 重 载+=操作符
    // string &append(const char *s);                   // 把字符串s连接到当前字符串结尼
    // string &append(const char *s, int n);            // 把字符串s的前n个字符连接到当前字符串结尼
    // string &append(const string &s);                 // 同operator+=()
    // string &append(const string &s, int pos, int n); // 把字符串s中从pos开始的n个字符连接到当前字符串结尼
    // string &append(int n, char c);                   // 在 当前字符串结比添加n个字符C

    // 查找和替换操作
    // int find(const string &str, int pos = 0) const;     // 查找str第-次出现位置, 从pos开始查找
    // int find(const char *s, int pos = 0) const;         // 查找s第一次出现位置,从pos 开始查找
    // int find(const char *s, int pos, int n) const;      // 从pos位置查找s的前n个字符第-次位 置
    // int find(const char c, int pos = 0) const;          // 查找字符C第一次出现位置
    // int rfind(const string &str, int pos = npos) const; // 在找 str最后一次位置, 从pos开始查找
    // int rfind(const char *s, int pos = npos) const;     // 查找s最后- -次出现位置,从pos开始查找
    // int rfind(const char *s, int pos, int n) const;     // 从 pos查找s的前n个字符最后-次位置
    // int rfind(const char c, int pos = 0) const;         // 查找字符c最后-次出现位置
    // string &replace(int pos, int n, const string &str); // 替换从pos开始n个字符为字符串str
    // string &replace(int pos, int n, const char *s);     // 替换从pos开始的n个字符为字符串s

    // 比较操作
    // int compare(const string &s) const; // 与字符串s比较
    // int compare(const char *s) const;   // 与字符串 s比较

    // string substr(int pos = 0, int n = npos) const; // 返回由pos开始的n个字符组成的字符串

    // 插入和删除操作
    // string &insert(int pos, const char *s);     // 插入字符串.
    // string &insert(int pos, const string &str); // 插入字符串
    // string &insert(int pos, int n, char c);     // 在指定位置插入n个字符C
    // string &erase(int pos, int n = npos);       // 删除从Pos开始的n个字符

    // string和c-style字符串的转换
    // string str = "itcast"; // string转char*
    // const char *cstr = str.c_str();
    // char *s = "itcast"; // char*转string
    // string str(s);
}

void vectorTest()
{
    // 常用构造函数
    vector<int> v;
    // vector<int> v(v.begin(), v.end()); // 将区间中的元素拷贝给本身
    // vector<int> v(n, elem);            // 构造函数将 n 个 elem 拷贝给本身

    // 赋值操作
    // v.assign(v.begin(), v.end()); // 将区间中的数据拷贝赋值给本身。
    // v.assign(n, e1em);            // 将 n个elem拷贝赋值给本身。
    // vector<int> v1 = v;           // 重载等号操作符
    // v.swap(vec);                  // 将vec与本身的元素互换。

    // 大小操作
    // v.size();                // 返回容器中元素的个数
    // v.empty();               // 判断容器是否为空
    // v.resize(int num);       // 重新指定容器的长度为num, 若容器变长, 则以默认值填充新位置。如果容器变短, 则末尾超出容器长度的元素被删除。
    // v.resize(int num, elem); // 重新指定容器的长度为num,若容器变长, 则以elem值填充新位置。如果容器变短, 则末尾超出容器长>度的元素被删除。
    // v.capacity();            // 容器的容量
    // v.reserve(int len);      // 容器预留 len个元素长度, 预留位置不初始化, 元素不可访问。

    // 数据存取操作
    // v.at(int idx); // 返回索引idx 所指的数据, 如果idx越界, 抛出out_of_range异常。
    // v[1];          // 返回索引idx所指的数据, 越界时, 运行直接报错
    // v.front();     // 返回容器中第一个数据元索
    // v.back();      // 返回容器中最后一一个数据元素

    // 插入和删除操作
    // v.insert(const_iterator pos, int count, ele);      // 迭代器指向位置 pos插入count个元素ele.
    // v.push_back(ele);                                  // 尾部插入元素ele
    // v.pop_back();                                      // 删除最后-一个元素
    // v.erase(const_iterator start, const_iterator end); // 删除迭代器从start 到end之间的元素
    // v.erase(const_iterator pos);                       // 删除迭代器 指向的元素
    // v.clear();                                         // 删除容器中所有元素

    v.push_back(1);
    v.push_back(2);

    cout << "容量：" << v.capacity() << " 大小：" << v.size() << endl;

    // 遍历
    vector<int>::iterator it = v.begin();
    for (; it != v.end(); it++)
        cout << *it << " ";
}

void dequeTest()
{
    // 常用构造函数
    deque<int> deq; // 默认构造形式
    // deque(beg, end);         // 构造函数将[beg, end)区同中的元素拷贝给本身。
    // deque(n, elem);          // 构造函数将 n个elem 拷贝给本身。
    // deque(const deque &deq); // 拷贝构造函数。

    // 赋值操作
    // deq.assign(beg.end);               // 将[beg, end)区同中的数据拷贝赋值给本身。
    // deq.assign(n, elem);               // 将 n个elem 拷贝赋值给本身。
    // deque& operator=(const deque deq); // 重载等号操作符
    // deq.swap(deq);                     // 将deq与本身的元素互换

    // 大小操作
    // deq.size();            // 返回容器中元素的个数
    // deq.empty();           // 判断容器是否为空
    // deq.resize(num);       // 重新指定容器的长度为num, 若容器变长, 则以默认值填充新位置。如果容器变复。则末尼超出容器长度的元素被剧除。
    // deq.resize(num, elem); // 重新 指定容器的长度为num,若容器变长,  则以elem 值填充断位置, 如果容器变知, 则术民超出容器长度的元素被剧除。

    // deque双端插入和删除操作
    // deq.push_back(elem);     // 在容器尾即法加一个数据
    // deq.push_front(elem);    // 在容器头部插入一个数据
    // deq.pop_back();          // 删除容器最后一个数据
    // deq.pop_front();         // 删除容器第一个数据

    // 数据存取操作
    // deq.at(idx); // 运网索引idx所指的数据。如果idx 越界。抛出out, _of, range.
    // operator[];  // 返同索引idx 所指的数据。如果idx 越界。不能出异常, 真接出错。
    // deq.front(); // 返问第一个数据。
    // deq.back();  // 近回最后一个数据

    // 插入和删除操作
    // deq.insert(pos, elem);     // 在pos位置插入一个elem元素的拷员, 返回新数据的位置。
    // deq.insert(pos, n, elem);  // 在pos位置插入n个elem数据, 无返问值。
    // deq.insert(pos, beg, end); // 在pos位置插入[beg,end)区间的数据, 无返回值。
    // deq.clear();               // 删除容器的所有数据
    // deq.erase(beg, end);       // 翻除[beg.end)区间的数据, 返回下一个数据的位置。
    // deq.erase(pos);            // 删除pos位置的数据。返日下一个数据的位置。

    deq.push_back(1);
    deq.push_back(2);
    deq.push_front(3);

    cout << "大小：" << deq.size() << endl;

    // 遍历
    deque<int>::iterator it = deq.begin();
    for (; it != deq.end(); it++)
        cout << *it << " ";
}

void stackTest()
{
    // 常用构造函数
    stack<int> st; // 默认构造形式
    // stack(const stack &st); // 拷贝构造函数。

    // 赋值操作
    // stack& operator=(const stack &st); // 重载等号操作符

    // 大小操作
    // st.size();  // 返回容器中元素的个数
    // st.empty(); // 判断容器是否为空

    // 数据存取操作
    // st.push(elem); // 向栈顶添如元素
    // st.pop();      // 从栈顶移除第一个元素
    // st.top();      // 返回栈顶元素

    st.push(1);
    st.push(2);
    st.push(3);
    st.pop();

    cout << "大小：" << st.size() << endl;

    cout << st.top() << " ";
}

void queueTest()
{
    // 常用构造函数
    queue<int> que; // 默认构造形式
    // queue(const queue &que); // 拷贝构造函数。

    // 赋值操作
    // queue& operator=(const queue &que); // 重载等号操作符

    // 大小操作
    // que.size();  // 返回容器中元素的个数
    // que.empty(); // 判断容器是否为空

    // 数据存取操作
    // que.push(elem); // 向队尾添如元素
    // que.pop();      // 从队头移除第一个元素
    // que.back();     // 返回队尾元素
    // que.front();    // 返回队头元素

    que.push(1);
    que.push(2);
    que.push(3);
    que.pop();

    cout << "大小：" << que.size() << endl;

    cout << "队头：" << que.front() << " 队尾：" << que.back() << " ";
}

void listTest()
{
    // 常用构造函数
    list<int> lst; // list 采用采用模板类实现, 对象的默认构造形式。
    // list(beg, end);         // 构造函数将[beg, end)区同中的元素拷员给本身。
    // list(n, elem);          // 构造函数将n个elem 拷贝给本身。
    // list(const list &lst);  // 拷贝构造图数。

    // 赋值操作
    // lst.assign(beg.end);               // 将[beg, end)区同中的数据拷贝赋值给本身。
    // lst.assign(n, elem);               // 将 n个elem 拷贝赋值给本身。
    // list& operator=(const list &lst); // 重载等号操作符
    // lst.swap(lst);                     // 将lst与本身的元素互换

    // 大小操作
    // lst.size();            // 返回容器中元素的个数
    // lst.empty();           // 判断容器是否为空
    // lst.resize(num);       // 重新指定容器的长度为num, 若容器变长, 则以默认值填充新位置。如果容器变复。则末尼超出容器长度的元素被剧除。
    // lst.resize(num, elem); // 重新指定容器的长度为num,若容器变长,  则以elem 值填充断位置, 如果容器变知, 则术民超出容器长度的元素被剧除。

    // 数据元素插入和删除操作
    // lst.push_back(elem);       // 在容器尾部加入一个元素
    // lst.pop_back();            // 剔除容器中最后一个元素
    // lst.push_front(elem);      // 在容器开头插入一个元东
    // lst.pop_front();           // 从容器开失移除第一个元素
    // lst.insert(pos, elem);     // 在pos位置插elem元素的拷见, 返问新数据的位置,
    // lst.insert(pos, n, elem);  // 在pos位置插入n个elem 数据, 无返问值。
    // lst.insert(pos, beg, end); // 在pos位置损入[beg,end)区间的数据, 无返回值。
    // lst.clear();               // 删除容器的所有数据
    // lst.erase(beg, end);       // 删除[beg,end)区间的数据。返回下一个数据的位置。
    // lst.erase(pos);            // 副除pos位置的数据, 返同下一个数据的位置。
    // lst.remove(elem);          // 删除容器中所有与elem相匹配的元素。

    // 数据存取操作
    // lst.front();    // 返回第一个元素
    // lst.back();     // 返回最后一个元素

    // 反转和排序
    // lst.reverse(); // 反转链表
    // lst.sort();    // 排序

    lst.push_back(1);
    lst.push_back(5);
    lst.push_front(3);

    cout << "大小：" << lst.size() << endl;

    lst.sort(); // algorithm提供的sort方法不支持链表

    list<int>::iterator it = lst.begin();
    for (; it != lst.end(); it++)
        cout << *it << " ";
}

// 仿函数, 实现更改 set 排序规则
template <class T>
class MyGreater
{
public:
    bool operator()(T v1, T v2)
    {
        return v1 > v2;
    }
};

void setTest()
{

    // 常用构造函数
    set<int> s;                  // set 默认构造函数, 默认升序排列
    set<int, MyGreater<int>> s2; // 降序排列
    // multiset<int> mst;           // multiset 默认构造函数, 它是允许元素重复的 set
    // set(const set &st);          // 拷贝构造函数

    // 赋值操作
    // set& operator=(const set &s); // 重载等号操作符
    // s.swap(s);                     // 将lst与本身的元素互换

    // 大小操作
    // s.size();            // 返回容器中元素的个数
    // s.empty();           // 判断容器是否为空

    // 插入和删除操作
    // s.insert(elem);          // 插入elem元素
    // s.clear();               // 删除容器的所有数据
    // s.erase(beg, end);       // 删除[beg,end)区间的数据。返回下一个数据的位置。
    // s.erase(pos);            // 副除pos位置的数据, 返同下一个数据的位置。

    // 查找操作
    // s.find(key);            // 查找健key是否存在, 若存在, 返回该键的元素的迭代器; 若不存在, 返回set.end();
    // s.count(key);           // 查找键key的元素个数
    // s.lower_bound(keyElem); // 返回第一个key >= keyElem 元素的迭代器。
    // s.upper_bound(keyElem); // 返回第一个key > keyElem 元素的迭代器。
    // s.equal_range(keyElem); // 返回容器中key与keyElem 相等的上下限的两个迭代器（以 pair 的方式）

    // 自动排序, 元素不可重复
    s.insert(1);
    s.insert(5);
    s.insert(5);
    s.insert(3);
    s.insert(2);
    // 降序排列
    s2.insert(1);
    s2.insert(5);
    s2.insert(5);
    s2.insert(3);
    s2.insert(2);

    cout << "大小：" << s.size() << endl;

    set<int>::const_iterator it = s.begin(); // 只读迭代器, 插入后不可更改
    for (; it != s.end(); it++)
        cout << *it << " ";
    cout << endl;
    set<int>::const_iterator it2 = s2.begin(); // 只读迭代器, 插入后不可更改
    for (; it2 != s2.end(); it2++)
        cout << *it2 << " ";
}

void pairTest()
{
    // 常用构造函数
    pair<string, int> p("test", 20);
    // pair<string, int> p2 = make_pair("name", 20);

    // 赋值操作
    // pair& operator=(const pair &p); // 重载等号操作符
    // p.swap(p);                      // 将p与本身的元素互换

    // 数据读取操作
    // p.first;    // 第一个元素
    // p.second;   // 第二个元素

    cout << p.first << " " << p.second << endl;
}

void mapTest()
{

    // 常用构造函数
    map<string, int> m; // map 默认构造函数, 默认升序排列
    // multimap<string, int> map; // multimap 默认构造函数, 它是允许元素重复的 map
    // map(const map &m);         // 拷贝构造函数

    // 赋值操作
    // map& operator=(const map &m); // 重载等号操作符
    // m.swap(m);                     // 将m与本身的元素互换

    // 大小操作
    // m.size();            // 返回容器中元素的个数
    // m.empty();           // 判断容器是否为空

    // 插入操作
    // m.insert(pair<int, string>(3, "小张"));            // 第一种, 通过pair的方式插入对象
    // m.insert(make_pair(-1, "校长"));                   // 第二种, 通过pair的方式插入对象
    // m.insert(map<int, string>::value_type(1, "小李")); // 第三种, 通过value_type的方式插入对象
    // m[3] = "小刘";                                     // 第四种, 通过数组的方式插入值

    // 删除操作
    // m.clear();               // 删除容器的所有数据
    // m.erase(beg, end);       // 删除[beg,end)区间的数据。返回下一个数据的迭代器。
    // m.erase(pos);            // 删除pos位置的数据, 返同下一个数据的迭代器。
    // m.erase(key);            // 制除容器中键值为key的对组。

    // 查找操作
    // m.find(key);            // 查找健key是否存在, 若存在, 返回该键的元素的迭代器; 若不存在, 返回set.end();
    // m.count(key);           // 查找键key的元素个数
    // m.lower_bound(keyElem); // 返回第一个key >= keyElem 元素的迭代器。
    // m.upper_bound(keyElem); // 返回第一个key > keyElem 元素的迭代器。
    // m.equal_range(keyElem); // 返回容器中key与keyElem 相等的上下限的两个迭代器（以 pair 的方式）

    // 自动排序, 元素不可重复
    m.insert(make_pair("e", 5));
    m.insert(make_pair("a", 1));
    m.insert(make_pair("d", 4));
    m.insert(make_pair("c", 3));
    m.insert(make_pair("b", 2));

    cout << "大小：" << m.size() << endl;

    map<string, int>::const_iterator it = m.begin(); // 只读迭代器, 插入后不可更改
    for (; it != m.end(); it++)
        cout << "(" << (*it).first << ", " << (*it).second << ") ";
}

int main()
{
    mapTest();
    return 0;
}