import random as r

legal_x = [0,10]
legal_y = [0,10]

class Turtle:
    def __init__(self):
        # 初始体力
        self.power =100
        #初始位置随机
        self.x = r.randint(legal_x[0],legal_x[1])
        self.y = r.randint(legal_y[0],legal_y[1])

    def move(self):
        # 随机计算方向并移动到新的位置（x,y）
        new_x = self.x + r.choice([1,2,-1,-2])
        new_y = self.y + r.choice([1,2,-1,-2])
        # 检查移动后是否超出场景x轴边界
        if new_x < legal_x[0]:
            self.x = legal_x[0] - (new_x - legal_x[0])#移出数的绝对值
        elif new_x > legal_x[1]:
            self.x = legal_x[1] - (new_x - legal_x[1])#移出数的绝对值
        else:
            self.x = new_x
        # 检查移动后是否超出场景y轴边界
        if new_y < legal_y[0]:
            self.y = legal_y[0] - (new_y - legal_y[0])#移出数的绝对值
        elif new_y > legal_y[1]:
            self.y = legal_y[1] - (new_y - legal_y[1])#移出数的绝对值
        else:
            self.y = new_y
        #体力消耗
        self.power -= 1
        # 返回移动后的新位置
        return (self.x,self.y)

    def eat(self):
        self.power += 20
        if self.power > 100:
            self.power = 100

class Fish:
        def __init__(self):
            self.x = r.randint(legal_x[0],legal_x[1])
            self.y = r.randint(legal_y[0],legal_y[1])

        def move(self):
            # 随机计算方向并移动到新的位置（x,y）
            new_x = self.x + r.choice([1,-1])
            new_y = self.y + r.choice([1,-1])
            # 检查移动后是否超出场景x轴边界
            if new_x < legal_x[0]:
                self.x = legal_x[0] - (new_x - legal_x[0])#移出数的绝对值
            elif new_x > legal_x[1]:
                self.x = legal_x[1] - (new_x - legal_x[1])#移出数的绝对值
            else:
                self.x = new_x
            # 检查移动后是否超出场景y轴边界
            if new_y < legal_y[0]:
                self.y = legal_y[0] - (new_y - legal_y[0])#移出数的绝对值
            elif new_y > legal_y[1]:
                self.y = legal_y[1] - (new_y - legal_y[1])#移出数的绝对值
            else:
                self.y = new_y
            # 返回移动后的新位置
            return (self.x,self.y)

turtle = Turtle()
fish = []
for i in range(10):#运行十次 十条鱼
    new_fish = Fish()
    fish.append(new_fish)#在列表末尾添加新的对象。

while True:
    if not len(fish):#没鱼了
        print("鱼儿都吃完了，游戏结束！")
        break
    if not turtle.power:#返回乌龟体力
        print("乌龟体力耗尽，挂掉了！")
        break

    pos = turtle.move()
    # 在迭代器中删除列表元素是非常危险的，经常会出现意想不到的问题，因为迭代器是直接引用列表的数据进行引用
    # 这里我们把列表拷贝给迭代器，然后对原列表进行删除操作就不会有问题了^_^
    for each_fish in fish[:]:
        if each_fish.move() == pos:
        # 鱼儿被吃掉了
            turtle.eat()
            fish.remove(each_fish)
            print("有一条鱼儿被吃掉了...")


