#include <M5Atom.h>

#define TRIG 23
#define DLED 33

#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_task_wdt.h>
#include <WebServer.h>


const char *ssid = "PLDTHOMEFIBRacNDs";
const char *password = "PLDTWIFIz8Anm";



String codeString = "37000138754"; // "";

String my_Api_Key = "oe5nw4c7y0ytif2tsnqee0zz4g5yxc";

String server_https = "https://api.barcodelookup.com/v3/products?barcode=" + codeString + "&formatted=y&key=" + my_Api_Key;

//WebServer server(80);

String httpGetString(String URL) {
  String payload = "";
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (http.begin(client, URL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
    } else {
      Serial.printf("fetch failed - error: %s\n",
                    http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  return payload;
}
String input = "";
void setup() {
  M5.begin(false, false, true);
  delay(220);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println();
  Serial.println("Configuring access point...");

  Serial2.begin(
    9600, SERIAL_8N1, 22,
    19);  // Set the baud rate of serial port 2 to 9600,8 data bits, no
  // parity bits, and 1 stop bit, and set RX to 22 and TX to 19.
  // 设置串口二的波特率为9600,8位数据位,没有校验位,1位停止位,并设置RX为22,TX为19
  M5.dis.fillpix(0xfff000);  // YELLOW 黄色
  pinMode(TRIG, OUTPUT);
  pinMode(DLED, INPUT);
  digitalWrite(TRIG, HIGH);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");



}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  if (M5.Btn.isPressed()) {
    digitalWrite(TRIG, LOW);


  } else {
    digitalWrite(TRIG, HIGH);
  }

  String ch = "\0";
  char a;
  char b[64]="0";
  //b[0]='w';
  int nm = 0;
  String s;
  int lenght_bar = 0;
  int arrayLength = 0;
  if (digitalRead(DLED) == HIGH) {  // If read the QR code.  如果读取到二维码
    M5.dis.fillpix(0x00ff00);  // GREEN  绿色

    while (Serial2.available() > 0) {
      a = Serial2.read();
      Serial.print(a);
      ch += a;
      b[nm] = a;
      nm++;
     //b[nm] = '\0';

    }
    s = String(b);
    lenght_bar = ch.length();
    arrayLength = sizeof(b) / sizeof(b[0]);

  } else {
    M5.dis.fillpix(0xfff000);
  }


  if (ch != "\0")
  {
    Serial.println();
    Serial.println("barcode-----------------------");
    Serial.println("length");
    Serial.println(lenght_bar);
    Serial.println(ch);
    Serial.println(s);
    Serial.println(arrayLength);
  }
  if (ch != "\0")
  {

    Serial.println("barcode+++++++++++++++++++++++++++++++");
    Serial.println("length");
    Serial.println(ch.length());
    Serial.println(ch);
    String server_https1 = "https://api.barcodelookup.com/v3/products?barcode=" + String(s) + "&formatted=y&key=" + my_Api_Key;
    
    String server_https2 = String(server_https1);
    Serial.println("Api barcode");
    Serial.println(s);
    Serial.println(s.length());
    server_https2.replace("\0", "");
    server_https2.replace("\n", "");
    server_https2.replace("\r", "");
    Serial.println(server_https2);
    Serial.println(server_https2.length());
    Serial.println(server_https1);
    Serial.println(server_https1.length());
    Serial.println(server_https);
    Serial.println(server_https.length());

    if(server_https==server_https2)
    {
      Serial.println("YUPP");
    }
    if (lenght_bar != 1 && lenght_bar != 0 && lenght_bar != 2)
    {
      Serial.println("RUNN ioT$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
      input = httpGetString(server_https2);
      Serial.println(input);
    }
  }

}
