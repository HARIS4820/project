// rotate clockwise for step down voltage
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <ESPmDNS.h>
#include <ElegantOTA.h>
#include <WebSerial.h>

#include <MFRC522.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "I2CKeyPad.h"
#include <SPI.h>


#define BUZZER 34
#define SENSOR 27
#define M_RELAY 13

// #define CLK 32
// #define DIO 33

#define RST_PIN 21  // Configurable, see typical pin layout above
#define SS_PIN 5   // SCK 18, MISO 19 , SDA 21, MOSI 23

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 16, 2);

const uint8_t KEYPAD_ADDRESS = 0x20;

I2CKeyPad keyPad(KEYPAD_ADDRESS, &Wire1);

// uint32_t start, stop;
// uint32_t lastKeyPressed = 0;
// uint32_t value = 0;

const char *ssid = "sivalingammilks";  // AP SSID of the first ESP32
const char *password = "sivalingammilks";


AsyncWebServer server(80);


void recvMsg(uint8_t *data, size_t len)
{
  mfrc522.PCD_Init(); // Init MFRC522 card
  String msg = "";
  for (int i = 0; i < len; i++)
  {
    msg += char(data[i]);
  }
  WebSerial.println("Received Data...");
  WebSerial.println(msg);
  int option = msg.substring(0, 1).toInt();
  String value = msg.substring(2);
  if (checkCardDetails())
  {
    switch (option)
    {
    case 1:
      WebSerial.println("Writing data in mobile blocks");
      writeDataintoCard(value, 1, 2);
      break;
    case 2:
      WebSerial.println("Writing data in password blocks");
      writeDataintoCard(value, 4, 5);
      break;
    case 3:
      WebSerial.println("Mobile :");
      WebSerial.println(ReadBlockData(1));

      break;
    case 4:
      WebSerial.println("Password:");
      WebSerial.println(ReadBlockData(4));
      break;
    default:
      break;
    }
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  WebSerial.println("Smart Card Options: \n1.Write mobile \n2.Write Password \n3.Read Mobile \n4.Read Password");
  WebSerial.println("Wifi Ip");
  WebSerial.println(WiFi.localIP().toString());
}



#define MACHINE_ID "10001"
JSONVar userDetails;
long startTime = 0;
long currentTime = 0;
int milkPrice = 0;
long currentMillis = 0;
long previousMillis = 0;
int interval = 100;
float calibrationFactor = 0;
double hundred;
double twohundred;
double threehundred;
double fourhundred;
double fivehundred;
double thousand;
double thousandPlus;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long lastRead;
int loop_check = 0;
int max_loop = 50;
bool flag = false;
boolean debugFlag = false;
int mSpeed = 0;
word mode;
// int RelayInput = 0;


String endpoint = "https://sivalingammilks.com/api/";
String token = "";
// boolean qtyFlag = false;
String customerId = "";
String saleMsg = "";
String role = "";
const char *rootCACertificate = R"string_literal(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
    )string_literal";


unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
    delay(100);
    ESP.restart();
  } else {
    Serial.println("There was an error during OTA update!");
  }

  // <Add your own code here>
}
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to AP...");
  }
  Wire.begin(26, 25);
  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight

  // turn on LCD backlight
  lcd.backlight();
  mfrc522.PCD_Init();  // Init MFRC522

  pinMode(M_RELAY, OUTPUT);
  analogWrite(M_RELAY, mSpeed);
  // digitalWrite(M_RELAY, LOW);


  // display.setBrightness(0x0f);
  // display.showNumberDec(0, true);
  pinMode(SENSOR, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  Serial.println("Connected to AP");

  if (!MDNS.begin("client")) {
    Serial.println("Error setting up MDNS responder!");
    return;
  }
  Serial.println("mDNS responder started");

  MDNS.addService("http", "tcp", 80);


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    const char html[] =
      "<html><body>"
      "<h1>Client</h1>"
      "<button onclick=\"window.location.href='/webserial'\">WebSerial</button>"
      "<button onclick=\"window.location.href='/update'\">upload code</button>"
      "</body></html>";
    request->send(200, "text/html", html);
  });



  // Initialize WebSerial
  WebSerial.begin(&server);
  WebSerial.onMessage(recvMsg);


  // Initialize Elegant OTA
  ElegantOTA.begin(&server);


  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);


  server.begin();

  Wire1.begin(33, 32);  //  adjust pins if needed
  // Wire1.setClock(400000);
  // while (keyPad.begin() != true) {
  //   delay(1000);
  //   Serial.println("Attempting to communicate with keypad");
  //   WebSerial.println("Attempting to communicate with keypad");

  lcd.clear();
  lcd.print("Setup Complete");
  delay(1000);
  WebSerial.println("setup complete");
  lcd.clear();
  SPI.begin(); 
}
void loop() {
   ElegantOTA.loop();
  delay(1000);
  WebSerial.println("loop");
  int milkPrice = 50;
  int price = 0;
  int qty = 0;
  static uint32_t value = 0;

  // Handle keypad value
  price = handleKeyPadValue(value);
  value = 0; // Reset the value after handling

  if (price > 0) {
    WebSerial.println("call add sale");
    qty = (price * 1000) / milkPrice;
    addSale(qty, price, customerId);
  }
}

int handleKeyPadValue(uint32_t &value) {
  Wire1.setClock(400000);
  while (keyPad.begin() != true) {
    delay(1000);
    Serial.println("Attempting to communicate with keypad");
    WebSerial.println("Attempting to communicate with keypad");
  }

  char v[19] = "D#0*C987B654A321NF";
  static uint8_t lastKey = 0;
  static unsigned long lastPressTime = 0;
  unsigned long debounceDelay = 200; // 200 ms debounce delay
  uint8_t idx;
  char c;
  unsigned long currentTime;

  while (true) {
    idx = keyPad.getKey();
    c = v[idx];
    currentTime = millis();

    if (c != lastKey && (currentTime - lastPressTime > debounceDelay)) {
      lastKey = c;
      lastPressTime = currentTime;

      switch (c) {
        case '0' ... '9':
          if (value < 2000) {
            value *= 10;
            value += c - '0';
             lcd.clear();
      lcd.print(value);
      WebSerial.println(value);
          }
          break;
        case '*':
          if (value > 0) value /= 10;
         
          break;
        case '#':
          value = 0;
          break;
        case 'D':
          lcd.clear();
          return value;
        case 'A':
          functionA();
          break;
        case 'B':
          functionB();
          break;
        case 'C':
          functionC();
          break;
        case 'F':
          Serial.println("FAIL");
          break;
        case 'N':
          Serial.println("NOKEY");
          break;
        default:
          break;
      }

    
      // Reset lastKey to avoid repeated processing of the same key
      lastKey = 0;
    }
  }

  return -1; // This will not be reached
}

void functionA() {
  Serial.println("Function A called");
}

void functionB() {
  Serial.println("Function B called");
}

void functionC() {
  Serial.println("Function C called");

  // Clear the LCD and prompt the user to place the card
  lcd.clear();
  lcd.print("Place card");

  // Wait for a card to be present
  while (!mfrc522.PICC_IsNewCardPresent()) {
    delay(100);
  }

  // Read the card
  if (mfrc522.PICC_ReadCardSerial()) {
    // Card detected, read the UID
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    // Display the UID on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card UID:");
    lcd.setCursor(0, 1);
    lcd.print(uid);

    // Display the UID on WebSerial
    WebSerial.println("RFID Reading Results:");
    WebSerial.print("Card UID: ");
    WebSerial.println(uid);
  } else {
    // No card detected
    lcd.clear();
    lcd.print("No card detected");
    WebSerial.println("No card detected");
  }

  // Halt PICC
  mfrc522.PICC_HaltA();
}

void functionD() {
  Serial.println("Function D called");
}

void addSale(int quantity, int price, String customerId) {
  WebSerial.println("inside add sale");
  String payload = "{}";
  JSONVar requestBody = JSON.parse(payload);
  requestBody["price"] = price;
  requestBody["quantity"] = quantity;
  requestBody["mobile"] = customerId;
  saleMsg = "0-0";

  if (quantity <= 100) {
    calibrationFactor = 2;
  } else if (quantity <= 200) {
    calibrationFactor = 6;
  } else if (quantity <= 300) {
    calibrationFactor = 8;
  } else if (quantity <= 400) {
    calibrationFactor = 9;
  } else if (quantity <= 500) {
    calibrationFactor = 9;
  } else if (quantity <= 1000) {
    calibrationFactor = 10;
  } else {
    calibrationFactor = 4.8;
  }
  WebSerial.println(String(calibrationFactor));
  mSpeed = 60;
  analogWrite(M_RELAY, mSpeed);

  while (totalMilliLitres < quantity) {
    Serial.println("Sale in progress");
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      if (loop_check > max_loop) {
        mSpeed = 0;
        analogWrite(M_RELAY, mSpeed);
        flag = true;
        break;
      }
      pulse1Sec = pulseCount;
      pulseCount = 0;
      flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
      previousMillis = millis();
      flowMilliLitres = (flowRate / 60) * interval;
      totalMilliLitres += flowMilliLitres;
      lcd.clear();
      lcd.print(totalMilliLitres);
      WebSerial.println(String(totalMilliLitres));
      if (lastRead != totalMilliLitres) {
        loop_check = 0;
        lastRead = totalMilliLitres;
      } else {
        loop_check++;
      }
    }
    delay(interval);
  }
  mSpeed = 0;
  analogWrite(M_RELAY, mSpeed);
  WebSerial.println(String(calibrationFactor));
  WebSerial.println(totalMilliLitres);
  lcd.clear();
  lcd.print(totalMilliLitres);
  if (totalMilliLitres > 25) {

    if (flag) {
      price = ceil((totalMilliLitres * milkPrice) / 1000);
      quantity = totalMilliLitres;
      requestBody["price"] = price;
      requestBody["quantity"] = totalMilliLitres;
    }
    saleMsg = String(quantity) + '-' + String(price);
    // module.setDisplayToString(saleMsg);
    if ((WiFi.status() == WL_CONNECTED))                // new
    {                                                   // new
      httpRequest("POST", "transaction", requestBody);  // new
    }                                                   // new
    else                                                // new
    {                                                   // new
      //module.setDisplayToString("Connect");             // new
      WiFi.begin(ssid, password);              // new
      while (WiFi.status() != WL_CONNECTED)    // new
      {                                        // new
        delay(1000);                           // new
        Serial.println("Connecting to Wifi");  // new
        //  module.setDisplayToString("Connect");           // new
      }                                                 // new
      Serial.println("Connected");                      // new
      Serial.print("IP address: ");                     // new
      httpRequest("POST", "transaction", requestBody);  // new
    }                                                   // new
  }
  WebSerial.println("Total Quantity :");
  WebSerial.print(totalMilliLitres);
  lcd.setCursor(0, 1);
  lcd.print(totalMilliLitres);
  flag = false;
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  loop_check = 0;
  lastRead = 0;
  price = 0;
  WebSerial.println("sale end");
 // value = 0; // Reset the value after sale ends
}

String httpRequest(String reqType, String path, JSONVar params) {

  HTTPClient http;

  http.begin(endpoint + path, rootCACertificate);  // Specify the URL and certificate
  http.addHeader("Authorization", "Bearer " + token);

  int httpCode = 0;
  if (reqType == "GET") {
    httpCode = http.GET();
  } else {
    if (reqType == "POST") {
      http.addHeader("Content-Type", "application/json");
      httpCode = http.POST(JSON.stringify(params));
    }
  }

  String response = "{}";
  if (httpCode > 0) {  // Check for the returning code
    response = http.getString();
    Serial.println(httpCode);
    Serial.println(response);
  } else if (httpCode == 401 || httpCode == 403) {
    token = "";
    // alarmBuzzer(3, 200);
  } else {
    Serial.println("Error on HTTP request");
    Serial.println(http.errorToString(httpCode));
  }
  http.end();  // Free the resources
  return response;
}

void login() {
  String payload = "{}";
  Serial.println("Insdie login");
  JSONVar requestBody = JSON.parse(payload);
  String mobile = "";
  //salesmanId = "";
  //   do
  //   {
  //     Serial.println("reading card");
  //     requestBody = ReadRFIDData(1000);
  //     mobile = requestBody["mobile"];
  //     delay(1000);
  //   } while (mobile.length() < 10);
  requestBody["mobile"] = "7418489311";
  requestBody["password"] = "hari1234";
  // alarmBuzzer(1, 200);
  String response = httpRequest("POST", "login", requestBody);
  JSONVar result = JSON.parse(response);
  WebSerial.println(result["token"]);
  token = result["token"];
  userDetails = result["userInfo"];
  role = userDetails["role"];

  if (role == "branchmanager")

  {
    //  digitalWrite(M_RELAY, LOW);
    delay(1000);
  }

  requestBody["machineId"] = MACHINE_ID;
  requestBody["localIP"] = WiFi.localIP().toString();
  response = httpRequest("POST", "iotmachine", requestBody);
  WebSerial.println(response);
  // sendJsonData(response);
  result = JSON.parse(response);
  hundred = result["100"];
  twohundred = result["200"];
  threehundred = result["300"];
  fourhundred = result["400"];
  fivehundred = result["500"];
  thousand = result["1000"];
  thousandPlus = result["1000plus"];
  milkPrice = result["milkPrice"];
  //   myData.hundred = hundred ;
  //   myData.twohundred = twohundred ;
  //   myData.threehundred = threehundred ;
  //   myData.fourhundred = fourhundred ;
  //   myData.fivehundred = fivehundred ;
  //    myData.thousand = thousand ;
  //    myData.thousandPlus= thousandPlus ;
  //   myData.milkPrice = milkPrice ;
  //   myData.price = -1 ;
  // esp_err_t espresult = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}



JSONVar ReadRFIDData(int readInterval) {
  mfrc522.PCD_Init();
  String payload = "{}";
  boolean readFlag = false;
  JSONVar requestBody = JSON.parse(payload);
  startTime = millis();
  currentTime = startTime;

  while (!mfrc522.PICC_IsNewCardPresent()) {
    if ((currentTime - startTime) <= readInterval) {
      Serial.print(".");
      currentTime = millis();
    } else {
      readFlag = true;
      break;
    }
  };
  while (!mfrc522.PICC_ReadCardSerial()) {
    if ((currentTime - startTime) <= readInterval) {
      Serial.print(".");
      currentTime = millis();
    } else {
      readFlag = true;
      break;
    }
  };
  if (readFlag) {
    requestBody["mobile"] = "";
    requestBody["password"] = "";

    return requestBody;
  }

  requestBody["mobile"] = ReadBlockData(1);
  requestBody["password"] = ReadBlockData(4);
  Serial.println(requestBody["mobile"]);
  Serial.println(requestBody["password"]);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  return requestBody;
}

String ReadBlockData(byte block) {
  byte blockBuffer[18];
  byte len = 18;
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;
  MFRC522::StatusCode status;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));  // line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }

  status = mfrc522.MIFARE_Read(block, blockBuffer, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }
  String blockData = "";
  for (uint8_t i = 0; i < 16; i++) {
    if (blockBuffer[i] == 32) {
      break;
    }
    blockData += char(blockBuffer[i]);
  }
  blockData.trim();
  return blockData;
}

boolean validateCustomer(String customerId, int price) {
  if (customerId.length() == 10) {
    String payload = "{}";
    String message = "";
    Serial.println("Insdie validate");
    JSONVar requestBody = JSON.parse(payload);
    requestBody["mobile"] = customerId;
    requestBody["price"] = price;
    String response = httpRequest("POST", "saleverify", requestBody);
    JSONVar result = JSON.parse(response);
    message = result["message"];
    if (message == "Ok") {
      return true;
    }
    alarmBuzzer(3, 200);
    return false;
  } else {
    return true;
  }
}

void alarmBuzzer(int count, int buzDelay) {
  int i = 0;
  while (i < count) {
    digitalWrite(BUZZER, HIGH);
    delay(buzDelay);
    digitalWrite(BUZZER, LOW);
    delay(buzDelay);
    i++;
  }
}


boolean checkCardDetails()
{
  long startTime = millis();
  long currentTime = startTime;
  while (!mfrc522.PICC_IsNewCardPresent())
  {
    if ((currentTime - startTime) <= 3000)
    {
      Serial.print(".");
      currentTime = millis();
    }
    else
    {
      WebSerial.println("No new card found");
      return false;
      break;
    }
  };
  while (!mfrc522.PICC_ReadCardSerial())
  {
    if ((currentTime - startTime) <= 3000)
    {
      Serial.print(".");
      currentTime = millis();
    }
    else
    {
      WebSerial.println("No card found");
      return false;
      break;
    }
  };
  return true;
}

void writeDataintoCard(String value, byte block1, byte block2)
{

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;
  byte buffer[34];
  MFRC522::StatusCode status;
  byte len = byte(value.length());
  value.getBytes(buffer, 30);
  for (byte i = len; i < 30; i++)
    buffer[i] = ' ';
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block1, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    WebSerial.print(F("PCD_Authenticate()block 1 failed: "));
    WebSerial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
    WebSerial.println(F("PCD_Authenticate() success: "));

  // Write block
  status = mfrc522.MIFARE_Write(block1, buffer, 16);
  if (status != MFRC522::STATUS_OK)
  {
    WebSerial.print(F("MIFARE_Write() block 1 failed: "));
    WebSerial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
    WebSerial.println(F("MIFARE_Write() block 1 success: "));

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block2, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    WebSerial.print(F("PCD_Authenticate() block 2 failed: "));
    WebSerial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block2, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK)
  {
    WebSerial.print(F("MIFARE_Write() block 2 failed: "));
    WebSerial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
    WebSerial.println(F("MIFARE_Write() block 2 success: "));
}
