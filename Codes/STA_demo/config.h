#ifndef CONFIG_H
#define CONFIG_H
// Replace with your network credentials
const char* ssid = "ASUS"; //Router_2.4GHz  ASUS
const char* password = "123456789";  //593321Ab       123456789

// Set your Static IP address
IPAddress local_IP(192, 168, 43, 100);
// Set your Gateway IP address
IPAddress gateway(192, 168, 43, 194);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

//NTP variables
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;     //Thailand time zone
const int   daylightOffset_sec = 3600;

// variable for get ngrok public url
const char* serverName = "http://192.168.43.181/public-url.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";

const char* http_username = "admin";
const char* http_password = "admin";

//Line token
#define TokenLine "e8nUB7z2I40DrOUHtkmvyk8ttLgoESPJRajb2skHpmP"

#endif // CONFIG_H
