/************************************************
 * @file Name: main.cpp
 * @brief :
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-20
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "esp01s";          // AP的SSID（WiFi名字）
const char *password = "12345678"; // AP的密码
String inputString = "QRCode data";        // 定义一个字符串来接收串口传来的数据
boolean stringComplete = false;    // 字符串传输完成的标志位
int Station_NUM = 0;               // 连接的设备数目
IPAddress IP;                      // ip地址
ESP8266WebServer esp8266_server(80);
void handleRoot();
void handleNotFound();
void serialEvent();
void setup()
{
  Serial.begin(9600);
  Serial.println("serial ok");
  WiFi.mode(WIFI_AP);                 // 设置为AP模式
  WiFi.softAP(ssid, password);        // 配置AP信息，并开启AP
  IP = WiFi.softAPIP();               // 用变量IP接收AP当前的IP地址
  Serial.println(IP);                 // 打印输出IP的IP地址
  esp8266_server.begin();             //  开启web服务器
  esp8266_server.on("/", handleRoot); //  访问根路由即调用 handlerRoot 函数处理
  esp8266_server.onNotFound(handleNotFound);
  Serial.println("HTTP esp8266_server started"); //  告知用户ESP8266网络服务功能已经启动
  inputString.reserve(200);
}


void loop()
{
  int gotoAP = WiFi.softAPgetStationNum(); // 获取当前连接到AP的设备数量
  if (gotoAP != Station_NUM)
  {
    Station_NUM = gotoAP;
    Serial.println("ap");
    Serial.println(gotoAP);
  }
  esp8266_server.handleClient(); // 处理请求
  if (stringComplete)
  {
    Serial.println(inputString);
    // 清除字符串
    inputString = "";
    stringComplete = false;
  }
  serialEvent();
}

/************************************************
 * @brief : 处理网站根目录“/”的访问请求
 * @date : 2024-05-20
 *************************************************/
void handleRoot()
{
  esp8266_server.send(200, "text/plain", inputString); // 状态码 头报文 正文
}

/************************************************
 * @brief : 设置处理404情况的函数'handleNotFound'
 * @date : 2024-05-20
 *************************************************/
void handleNotFound()
{                                                           // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found"); // NodeMCU将调用此函数。
}

/************************************************
 * @brief : 串口事件函数，在该函数中实现串口接收字符串
 * @date : 2024-05-19
 *************************************************/
void serialEvent()
{
  while (Serial.available() > 0)
  {
    // get the new byte:
    char inChar = Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}