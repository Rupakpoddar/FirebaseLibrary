#include <ArduinoJson.h>            // https://github.com/bblanchon/ArduinoJson 
#include <Firebase.h>

#define _SSID "ENTER HERE"          // Your WiFi SSID 
#define _PASSWORD "ENTER HERE"      // Your WiFi Password 
#define REFERENCE_URL "ENTER HERE"  // Your Firebase project reference url 

Firebase firebase(REFERENCE_URL);

void setup() {
  Serial.begin(115200);
  #if !defined(ARDUINO_UNOWIFIR4)
    WiFi.mode(WIFI_STA);
  #else
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
  #endif
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
  #if defined(ARDUINO_UNOWIFIR4)
    digitalWrite(LED_BUILTIN, HIGH);
  #endif

//================================================================//
//================================================================//

  // Write some data to the realtime database.
  firebase.setString("Example/setString", "It's Working");
  firebase.setInt("Example/setInt", 123);
  firebase.setFloat("Example/setFloat", 45.32);
  
  String data = firebase.getString("Example");  // Get data from the database.

  // Deserialize the data.
  // Consider using Arduino Json Assistant- https://arduinojson.org/v6/assistant/
  const size_t capacity = JSON_OBJECT_SIZE(3) + 50;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, data);

  // Store the deserialized data.
  const char* received_String = doc["setString"]; // "It's Working"
  int received_int = doc["setInt"];               // 123
  float received_float = doc["setFloat"];         // 45.32

  // Print data
  Serial.print("Received String:\t");
  Serial.println(received_String);

  Serial.print("Received Int:\t\t");
  Serial.println(received_int);

  Serial.print("Received Float:\t\t");
  Serial.println(received_float);

  // Remove data from the realtime database.
  firebase.remove("Example");
}

void loop() {
  // Nothing
}
