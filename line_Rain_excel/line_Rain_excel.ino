#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>
char ssid[] = "********";//ชื่อ WIFI
char pass[] = "********";//รหัส WIFI
String GAS_ID = "JKfycXxvP-oHHsZdX6ZCRY4GG4dHIfOZLR9PpL0V64o99BU2fPm8KJTOJkGh3q74xoOVB2hOK"; //--> spreadsheet script ID
//Your Domain name with URL path or IP address with path
const char* host = "script.google.com"; // only google.com not https://google.com

// ----------------------------------------------------------------------------------------------
#define analogPin 2            //D4
#define UPDATE_INTERVAL_HOUR  (0)
#define UPDATE_INTERVAL_MIN   (0)
#define UPDATE_INTERVAL_SEC   (30)
#define LINE_TOKEN  "fsaNLL2kAriGug2bsCnHLOXH0Sju1RcOXty1E9WrQg8"  
#define UPDATE_INTERVAL_MS    ( ((UPDATE_INTERVAL_HOUR*60*60) + (UPDATE_INTERVAL_MIN * 60) + UPDATE_INTERVAL_SEC ) * 1000 )

// ----------------------------------------------------------------------------------------------
int val = 0;
bool notificationSent = false;
// ----------------------------------------------------------------------------------------------
void update_google_sheet()
{
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClientSecure client;
    const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
    
    client.setInsecure(); // this is the magical line that makes everything work
    
    if (!client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return;
    }
       
    //----------------------------------------Processing data and sending data
    String url = "/macros/s/" + GAS_ID + "/exec?warn=";

    url += "Rain_detected";
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
  
    Serial.println();
    Serial.println("closing connection");  
}

// ----------------------------------------------------------------------------------------------
void setup()
{
  // Debug console
  Serial.begin(115200); Serial.println();
  Serial.println(LINE.getVersion());

  // Digital output pin
  

//----------------------------------------Wait for connection
  WiFi.begin(ssid, pass);
  Serial.printf("WiFi connecting to %s\n",  ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

}

// ----------------------------------------------------------------------------------------------

void loop()
{
  val = analogRead(analogPin);  //อ่านค่าสัญญาณ analog ขา5
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(val); // พิมพ์ค่าของตัวแปร val
  // Read and print serial data every 1 sec
  if ( val < 500 && !notificationSent ) // 1sec 
  {
    //
    LINE.notify("ตรวจพบน้ำฝน");//คำที่จะส่งเข้าไลน์(สติกเกอร์ก็สามารถส่งได้)
    update_google_sheet();
    notificationSent = true;
  }
  else{
    delay(100);
    if(val > 900 && notificationSent){
      notificationSent = false;
    }
  }
 
}