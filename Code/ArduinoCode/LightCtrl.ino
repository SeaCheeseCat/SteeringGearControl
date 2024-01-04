/* * nodemcu引脚对应数字：      * 16=D0;  *  4=D2; *  0=D3; *  2=D4; * 14=D5; * 12=D6; * 13=D7; * 15=D8; *  3=D9; *  1=D10; * ESP-01引脚对应数字：    * GPIO2 --2 舵机 -> 模拟量输出 * GPIO16--16 继电器 -> 开关量输出*/ 
/* * 
开发板选择:Generic Esp8266 Module
开发板管理网址： http://arduino.esp8266.com/stable/package_esp8266com_index.json
* */
#include <ESP8266WiFi.h>
#include <Servo.h>

#ifndef STASSID
#define STASSID "TP-LINK_E86B"    //WiFi名称
#define STAPSK  "PassWord1703"  //WiFi密码
//#define STASSID "12X"    //WiFi名称
//#define STAPSK  "123456789"  //WiFi密码
#define ServoPin 4
#endif

#define ANGLE_ON  360    //开灯时的舵机角度
#define ANGLE_OFF 250   //关灯时的舵机角度
#define ANGLE_NONE 110  //置空时的舵机角度

const char* ssid = STASSID;
const char* password = STAPSK;
int angle=ANGLE_NONE;//舵机角度
Servo myservo;  //创造一个servo结构体
 
//创建服务器实例，指定要监听的端口为80端口
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  myservo.attach(4); //舵机信号线，接analogIn脚
  myservo.write(angle);// 0~180°
  //连接WiFi
  Serial.println();
  Serial.println();
  Serial.print(F("WiFi start connect "));//用F函数将会把字符串放入RAM中，输出至串口
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);//设置为接收WiFi模式
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi已连接！"));
 
  //开启模块的服务器
  server.begin();
  Serial.println(F("模块服务端已开启！"));
 
  //打印模块的IP地址
  Serial.println(WiFi.localIP());

//  Serial.begin(115200);
pinMode(ServoPin, OUTPUT);
digitalWrite(ServoPin, LOW);//先保证拉低
}
 
void loop() {
  //监测客户端是否连接
  WiFiClient client = server.available();
  if (!client) {
    return;//如果未连接就重新循环监测
  }
//  Serial.println(F("new client"));
 
  client.setTimeout(1000); // default is 1000
 
  //读取请求内容的第一行
  String req = client.readStringUntil('\r');
  Serial.println(F("请求: "));
  Serial.println(req);
 
  //匹配 请求-->信号变量
  if (req.indexOf(F("/gpio/0")) != -1) {
//    angle = ANGLE_ON;//如果/gpio/0在请求内容中，发出开灯的信号
//    myservo.write(angle);// 0~180°
//    delay(1000);
//    myservo.write(ANGLE_NONE);//将舵机重新置空，以免一直受力损坏

  ServoControl(120);
  } 
  else if (req.indexOf(F("/gpio/1")) != -1) {
//    angle = ANGLE_OFF;
//    myservo.write(angle);// 0~180°
//    delay(1000);
//    myservo.write(ANGLE_NONE);//将舵机重新置空，以免一直受力损坏
ServoControl(190);
  } 
  else {
    Serial.println(F("无效请求"));
  }
  //读取或忽略余下的请求内容
  while (client.available()) {
    client.read();
  }
  
  //向客户端发送回答，HTML网页代码内容
  //nagle算法会将它们分组为一个数据包
  client.print(F("<!DOCTYPE html>\r\n<html xmlns='http://www.w3.org/1999/xhtml'>\r\n"));
  client.print(F("<head>\r\n<meta charset='utf-8'/>\r\n<title>智能卧室灯</title>\r\n"));
  client.print(F("<style type='text/css'>.state{text-align:center;font-size:80px;}"));
  client.print(F(".on{text-align:center;font-size:200px;alink='#1a9e5f'}.off{text-align:center;font-size:200px;alink='#da4f43'}"));
  client.print(F("</style><body><div class='state'>灯状态："));
  client.print((angle>ANGLE_NONE) ? F("关</div>") : F("开</div>"));
  client.print(F("<br><br><div class='on'><a href='http://"));//<br>换行
  client.print(WiFi.localIP());
  client.print(F("/gpio/0'>开</a></div><br><br><div class='off'><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>关</a></div></body></html>"));
 
  // 客户端实际上将被“刷新”,然后断开连接
  // 当函数返回并且“客户端”对象被销毁时
  // 刷新=确保书面数据被服务端接收
  Serial.println(F("客户端已挂起！"));

}


void ServoControl(int servoAngle)
{
  double thisAngle = map(servoAngle, 0, 180, 500, 2500);//等比例角度值范围转换高电平持续时间范围
  unsigned char i = 50;//50Hz 每秒的周期次数(周期/秒) 即1S 50 个周期 每个周期20ms
  while (i--)
  {
    digitalWrite(ServoPin, HIGH); 
    delayMicroseconds(thisAngle); //高电平时间
    digitalWrite(ServoPin, LOW); 
    delayMicroseconds(20000 - thisAngle);//每个周期20ms减去高电平持续时间
  }
}


