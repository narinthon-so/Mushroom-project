#ifndef CONFIG_H
#define CONFIG_H
// Replace with your network credentials
const char* ssid = "TP-LINK_0AAF8C"; //Router_2.4GHz  ASUS  TP-LINK_0AAF8C  Mushroom Farm
const char* password = "094169265";  //593321Ab       123456789   094169265  123456789

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 100);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1); //0, 1

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

//NTP variables
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;     //Thailand time zone
const int   daylightOffset_sec = 3600;

// variable for get ngrok public url
const char* serverName_url = "http://192.168.0.105/public-url.php";

// url for save data in database
const String serverName_db = "http://192.168.0.105/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";

const char* http_username = "admin";
const char* http_password = "admin";

//Line token
#define TokenLine "e8nUB7z2I40DrOUHtkmvyk8ttLgoESPJRajb2skHpmP"

#endif // CONFIG_H
