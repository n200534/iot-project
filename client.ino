#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


#include <SoftwareSerial.h>
const char* ssid = "BEARS";
const char* password = "BEARS123";
const char* server_url = "http://127.0.0.1:5000/update";
String status;
const char* url1="http://127.0.0.1:5000/display";
 
SoftwareSerial s1(D1,D2);
 
void setup() {
  //serial communication
  Serial.begin(115200);
  s1.begin(9600);


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("-");
  }

  Serial.println("WiFi connected!");
}

void loop() {
  //serial
  String status=s1.readStringUntil('\r');
  Serial.println(status);  
  // Send update request
  HTTPClient http;
  WiFiClient client;
  http.begin(client,server_url);
  http.addHeader("Content-Type", "application/json");
  // Construct request body
  String requestBody = "{";
  requestBody += "\"EXAM CENTER ID\": 1,";
  requestBody += "\"status\": \"1111111111\",";
  requestBody += "\"TIME\": \"" + getTime() + "\",";
  requestBody += "\"PAPER\": \"MATHS\",";
  requestBody += "\"EXAM CENTER INCHARGE_ID\": \"ABCD1234\"";
  requestBody += "}";
  
  // Send request
  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0) {
    Serial.printf("HTTP POST request sent to %s/update, response code: %d\n", server_url, httpResponseCode);
  } else {
    Serial.printf("HTTP POST request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  // Wait for 5 seconds before sending the next request
  delay(5000);

  // Send display request
  //url1="htpp://127.0.0.1:5000/display";
  http.begin(client,url1);

  // Send request
  httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String responseBody = http.getString();
    Serial.printf("HTTP GET request sent to %s/display, response code: %d\n", server_url, httpResponseCode);
    Serial.println("Response body:");
    Serial.println(responseBody);
  } else {
    Serial.printf("HTTP GET request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  // Wait for 5 seconds before sending the next request
  delay(5000);
}

String getTime() {
  String timeStr = ""; 

  // Get the current time in Unix timestamp
  time_t now = time(nullptr);

  if (now != -1) {
    // Convert Unix timestamp to struct tm
    struct tm* timeinfo = localtime(&now);

    // Format the time string as "YYYY-MM-DD HH:MM:SS"
    char timeBuf[20];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", timeinfo);
    timeStr = timeBuf;
  } else {
    Serial.println("Error getting time");
  }

  return timeStr;
}