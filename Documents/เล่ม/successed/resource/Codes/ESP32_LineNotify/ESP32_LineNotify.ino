#include <WiFi.h>
#include <WiFiClientSecure.h>

//########## CONFIGURATION : MODIFY HERE ##########
char ssid[] = "ASUS"; // เครือข่าย Wi-Fi ที่ต้องการเชื่อมต่อ
char pass[] = "123456789"; // รหัสผ่านเครือข่าย

//รหัสเชื่อมต่อ Line token
#define TokenLine "e8nUB7z2I40DrOUHtkmvyk8ttLgoESPJRajb2skHpmP"

// ข้อความ ที่จะแสดงใน Line
String txt1 = "มีผู้บุกรุก!!! ที่อาคาร 5";



void setup() {

  //ตั้งค่าคอนโซล
  Serial.begin(115200);

  //เริ่มการเชื่อมต่อกับวายฟายแม่ข่าย
  WiFi.begin(ssid, pass);

  //แสดง "WiFi Connecting" ในคอนโซล
  Serial.print("WiFi Connecting");

  //ตรวจเช็คสถานะการเขื่อมต่อวายฟาย
  //ถ้าไม่สำเร็จให้แสดง "." ในคอนโซลจนกว่าจะเขื่อมต่อได้
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //แสดงสถานะการวายฟายเชื่อมต่อแล้ว
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  NotifyLine("https://www.google.com/");
  delay(10000);

}

void NotifyLine(String t) {
  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("Connection failed");

    return;
  }
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(TokenLine) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP32\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + t).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + t;
  Serial.println(req);
  client.print(req);
  delay(20);
  Serial.println("-------------");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  } 
} 
