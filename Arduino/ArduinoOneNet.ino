#include<Servo.h>
//引入ESP8266.h头文件，建议使用教程中修改后的文件
#include "ESP8266.h"
#include "dht11.h"
#include "SoftwareSerial.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <IRremote.h>
#include <IRremote.h>//引入红外解码库
int RECV_PIN = 10;   //红外线接收器OUTPUT端接在pin 10
IRrecv irrecv(RECV_PIN);  //定义IRrecv对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里
//配置ESP8266WIFI设置
#define SSID "Redmi"    //填写2.4GHz的WIFI名称，不要使用校园网
#define PASSWORD "qawsedrf"//填写自己的WIFI密码
#define HOST_NAME "api.heclouds.com"  //API主机名称，连接到OneNET平台，无需修改
#define DEVICE_ID "645459120"       //填写自己的OneNet设备ID
#define HOST_PORT (80)                //API端口，连接到OneNET平台，无需修改
String APIKey = "OvtuYOhNenw5o0afM=TqYx3=DRI="; //与设备绑定的APIKey
#define INTERVAL_SENSOR 5000 //定义传感器采样及发送时间间隔 1000表示一秒钟
dht11 DHT11;
const int ledyellowPin = 8;
const int ledgreenPin = 9;
int buttonPushCounter = 0;
// 记录当前按键的状态
int buttonState = 0;
// 记录按键之前的状态
int lastButtonState = 0;
//定义DHT11接入Arduino的管脚
#define DHT11PIN 4
//定义ESP8266所连接的软串口
/*********************
 * 该实验需要使用软串口
 * Arduino上的软串口RX定义为D3,
 * 接ESP8266上的TX口,
 * Arduino上的软串口TX定义为D2,
 * 接ESP8266上的RX口.
 * D3和D2可以自定义,
 * 但接ESP8266时必须恰好相反
 *********************/
SoftwareSerial mySerial(3, 2);
ESP8266 wifi(mySerial);
Servo duoji;
void setup()
{
  mySerial.begin(115200); //初始化软串口
  Serial.begin(9600);     //初始化串口
  Serial.print("setup begin\r\n");

  //以下为ESP8266初始化的代码
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }

  //ESP8266接入WIFI
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  Serial.println("");
  Serial.print("DHT11 LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);

  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  Serial.println("setup end\r\n");
  Serial.begin(9600); //初始化串口，9600baud
  irrecv.enableIRIn(); // 启动红外解码
  pinMode(ledyellowPin, OUTPUT);
  pinMode(ledgreenPin, OUTPUT);

  irrecv.enableIRIn(); // 启动红外解码
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);//蜂鸣器接11端口
 duoji.attach(6);//舵机在6号引脚
}//此处为初始化设置，

unsigned long net_time1 = millis(); //数据上传服务器时间
void loop(){
  
String getString="GET/devices/";
getString+=DEVICE_ID;
getString+="/datastreams?datastream_ids=status,light HTTP/1.1";
getString+="\r\n";
getString+="api-key";
getString+=APIKey;
getString+="\r\n";
getString+="Host:api.heclouds.com\r\n";
getString+="Connection:close\r\n";
getString+="\r\n";
const char*getArray=getString.c_str();//将str转化
Serial.println("used   ");
Serial.println(getArray);
Serial.println("used   ");
wifi.send((const uint8_t*)getArray,strlen(getArray));
Serial.println("get success");
uint8_t buffer[10]={9};
uint32_t len=wifi.recv(buffer,sizeof(buffer),20000);
Serial.println("recv success");
for(int i=0;i<30;i++){
  Serial.println(buffer[i]);
}

Serial.println(len);
int math=12345;
Serial.println(math);///////////////////////////////////////////////////////////////
if (irrecv.decode(&results)) {      //解码成功，收到一组红外线信号
    Serial.println(results.value, HEX);//// 输出红外线解码结果（十六进制）
    irrecv.resume(); //  接收下一个值
  }
  delay(1000); // 延时1秒
 digitalWrite(ledyellowPin, HIGH); // 点亮LED
digitalWrite(11, HIGH);
Serial.print("还没吃药 ");
 if (irrecv.decode(&results))  
 { Serial.println(results.value, HEX);
  digitalWrite(ledyellowPin, LOW); // 熄灭LED
 digitalWrite(11, LOW);
  digitalWrite(ledgreenPin, HIGH);
 digitalWrite(11, HIGH);
  digitalWrite(11, HIGH);
  delay(2000);
 digitalWrite(11, LOW);
//两秒插上去
 duoji.write(0);
 delay(12000); // 延时1秒
 
 digitalWrite(ledgreenPin, LOW);
 delay(2000);
  duoji.write(90);

  digitalWrite(11, HIGH);
  delay(2000);
 digitalWrite(11, LOW);
 
 Serial.print("吃药了 ");
  buttonPushCounter++;
      Serial.println("按钮加一");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
      
//接下来进行数据的上传    

      if (net_time1 > millis())
    net_time1 = millis();

  if (millis() - net_time1 > INTERVAL_SENSOR) //发送数据时间间隔
  {

    int chk = DHT11.read(DHT11PIN);

    Serial.print("Read sensor: ");
    switch (chk) {
      case DHTLIB_OK:
        Serial.println("OK");
        break;
      case DHTLIB_ERROR_CHECKSUM:
        Serial.println("Checksum error");
        break;
      case DHTLIB_ERROR_TIMEOUT:
        Serial.println("Time out error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }

    int sensor_hum = (int)DHT11.humidity;
    int sensor_tem = (int)DHT11.temperature;
    Serial.print("老人吃药了 ");
    Serial.println(sensor_hum, 2);

    Serial.print("按钮计数器的值为 ");
   Serial.println(buttonPushCounter);//************************此处为串口监视器中打印
    Serial.println("");

    if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
      Serial.print("create tcp ok\r\n");
      char buf[10];
      //拼接发送data字段字符串
      String jsonToSend = "{\"Temperature\":";//*******************
      dtostrf(1, 1, 2, buf);
      jsonToSend += "\"" + String(buf) + "\"";
      jsonToSend += ",\"Humidity\":";
      dtostrf(buttonPushCounter, 1, 0, buf);
      jsonToSend += "\"" + String(buf) + "\"";
      jsonToSend += "}";

      //拼接POST请求字符串
      String postString = "POST /devices/";
      postString += DEVICE_ID;
      postString += "/datapoints?type=3 HTTP/1.1";
      postString += "\r\n";
      postString += "api-key:";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host:api.heclouds.com\r\n";
      postString += "Connection:close\r\n";
      postString += "Content-Length:";
      postString += jsonToSend.length();
      postString += "\r\n";
      postString += "\r\n";
      postString += jsonToSend;
      postString += "\r\n";
      postString += "\r\n";
      postString += "\r\n";

      const char *postArray = postString.c_str(); //将str转化为char数组

      Serial.println(postArray);
      wifi.send((const uint8_t *)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
      Serial.println("send success");
      if (wifi.releaseTCP()) { //释放TCP连接
        Serial.print("release tcp ok\r\n");
      } else {
        Serial.print("release tcp err\r\n");
      }
      postArray = NULL; //清空数组，等待下次传输数据
    } else {
      Serial.print("create tcp err\r\n");
    }

    Serial.println("");

    net_time1 = millis();
    }
   delay(300); 
  //此处的括号扩到时间net_time的if，应该不用改
 }
 }//此处是loop函数的下括号
