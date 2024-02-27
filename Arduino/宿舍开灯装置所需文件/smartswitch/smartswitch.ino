#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>
#include <Servo.h>

Servo myservo;//定义舵机

#define rotation_angle 90  //旋转角度，复位角度为90时最大为90，复位角度为0时最大180,3V电压不够
#define default_angle 90   //舵机复位角度
#define delay_time 1000   //归位延迟时间

char auth[] = "ec0742e5bfe8";  //点灯Key
char ssid[] = "SPRIC LAPTOP";      //wifi名称
char pswd[] = "20031130";     //wifi密码

// 新建组件对象
BlinkerButton Button1("test");


int counter = 0;

void miotPowerState(const String & state)//电源类操作
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
      
        myservo.write(default_angle + rotation_angle);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();//反馈状态
        delay(delay_time);//延时1秒
        myservo.write(default_angle);//舵机归零，回到垂直状态
         
        
    }
    else if (state == BLINKER_CMD_OFF) {
      
         myservo.write(default_angle - rotation_angle);
         BlinkerMIOT.powerState("off");
         BlinkerMIOT.print();
         delay(delay_time);
         myservo.write(default_angle);
         
      
    }
}

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    if (state=="on") 
    {
        myservo.write(default_angle + rotation_angle);
        delay(delay_time);//延时
        myservo.write(default_angle);//舵机归零垂直
         
        
    } else if(state=="press"||state=="tap")
    {
       myservo.write(default_angle - rotation_angle);
       delay(delay_time);//延时
       myservo.write(default_angle);//舵机归零垂直
    }
        
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
 
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    // 初始化舵机
    myservo.attach(2);//舵机的IO口，nodemcu的D4口
    myservo.write(default_angle);//上电时舵机归零垂直
  BlinkerMIOT.attachPowerState(miotPowerState);
  
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);

    Button1.attach(button1_callback);
}

void loop() {
    Blinker.run();
}
