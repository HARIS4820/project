#include <TM1638.h>
#include <TM1638QYF.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <SPI.h>
#include <MFRC522.h>
// #include <WebServer.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <WebSerial.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#define MACHINE_ID "102"

#define STB 25
#define DIO 32
#define CLK 33

#define RST_PIN 22 // Configurable, see typical pin layout above
#define SS_PIN 21  // SCK 18, MISO 19 , SDA 21, MOSI 23

#define SENSOR 26
#define M_RELAY 13

#define BUZZER 27

TM1638QYF module(DIO, CLK, STB);
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

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
int max_loop = 15;
bool flag = false;
word mode;

const char *ssid = "sivalingammilks";
const char *password = "sivalingammilks";
AsyncWebServer server(80);
String endpoint = "https://sivalingammilks.com/api/";
String token = "";
boolean qtyFlag = false;
String customerId = "";
String saleMsg = "";
String role = "";
const char *ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n"
    "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
    "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n"
    "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n"
    "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"
    "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n"
    "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n"
    "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n"
    "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n"
    "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n"
    "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n"
    "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n"
    "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n"
    "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n"
    "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n"
    "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n"
    "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n"
    "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n"
    "-----END CERTIFICATE-----\n";

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Sivalingam Milks</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #2E2FF5}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Sivalingam Milks</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/change?output="+element.id+"&state=0", true); }
  else { xhr.open("GET", "/change?output="+element.id+"&state=1", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Motor On/Off</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"13\" " + outputState(13) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

void setup()
{

  Serial.begin(115200);
  module.setupDisplay(true, 7);
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(M_RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(M_RELAY, HIGH);
  digitalWrite(BUZZER, LOW);
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

  mode = 0; // initial button zero
  module.setDisplayToString("Ready");
  SPI.begin(); // Init SPI bus
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to Wifi");
    module.setDisplayToString("Connect");
  }
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (!MDNS.begin("code"))
  { // http://code.local
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
   server.on("/change", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/change?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  AsyncElegantOTA.begin(&server);
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  // digitalWrite(M_RELAY, HIGH);
  // delay(100);
  lastRead = 0;
  if ((WiFi.status() == WL_CONNECTED))
  { // Check the current connection status
    if (token.length() > 0)
    {
      module.setDisplayToString(saleMsg + '-');
      customerId = "";
      int price = 0;
      int qty = 0;
      int userInput = readInputFromKeyPad(1, 4);
      WebSerial.println("Smart Card Options: \n1.Write mobile \n2.Write Password \n3.Read Mobile \n4.Read Password");
      WebSerial.println("Enter option number and data seperated by comma");

      if (qtyFlag)
      {
        price = ceil((userInput * milkPrice) / 1000);
        qty = userInput;
        qtyFlag = false;
      }
      else
      {
        price = userInput;
        qty = ceil((userInput * 1000) / milkPrice);
      }
      Serial.println(price);
      Serial.println(qty);
      if (price > 0 && price < 400)
      {
        Serial.println(customerId);
        Serial.println(milkPrice);
        if (validateCustomer(customerId, price))
        {
          addSale(qty, price, customerId);
        }
      }
    }
    else
    {
      module.setDisplayToString("LOGIN");
      login();
    }
  }
  else
  {
    module.setDisplayToString("Connect");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);                          // new
      Serial.println("Connecting to Wifi"); // new
      module.setDisplayToString("Connect"); // new
    }                                       // new
    Serial.println("Connected");            // new
    Serial.print("IP address: ");           // new
  }                                         // new
}

boolean validateCustomer(String customerId, int price)
{
  if (customerId.length() == 10)
  {
    String payload = "{}";
    String message = "";
    Serial.println("Insdie validate");
    JSONVar requestBody = JSON.parse(payload);
    requestBody["mobile"] = customerId;
    requestBody["price"] = price;
    String response = httpRequest("POST", "saleverify", requestBody);
    JSONVar result = JSON.parse(response);
    message = result["message"];
    if (message == "Ok")
    {
      return true;
    }
    alarmBuzzer(3, 200);
    return false;
  }
  else
  {
    return true;
  }
}
void login()
{
  String payload = "{}";
  Serial.println("Insdie login");
  JSONVar requestBody = JSON.parse(payload);
  String mobile = "";
  //   do
  //   {
  //     Serial.println("reading card");
  //     requestBody = ReadRFIDData(1000);
  //     mobile = requestBody["mobile"];
  //     delay(1000);
  //   } while (mobile.length() < 10);
  requestBody["mobile"] = "9843485201";
  requestBody["password"] = "nikhisha";
  alarmBuzzer(1, 200);
  String response = httpRequest("POST", "login", requestBody);
  JSONVar result = JSON.parse(response);
  Serial.println(result["token"]);
  token = result["token"];
  requestBody["machineId"] = MACHINE_ID;
  requestBody["localIP"] = WiFi.localIP().toString();
  response = httpRequest("POST", "iotmachine", requestBody);
  result = JSON.parse(response);
  hundred = result["100"];
  twohundred = result["200"];
  threehundred = result["300"];
  fourhundred = result["400"];
  fivehundred = result["500"];
  thousand = result["1000"];
  thousandPlus = result["1000plus"];
  milkPrice = result["milkPrice"];
}

void addSale(int quantity, int price, String customerId)
{
  module.setDisplayToString(String(totalMilliLitres));
  String payload = "{}";
  JSONVar requestBody = JSON.parse(payload);
  requestBody["price"] = price;
  requestBody["quantity"] = quantity;
  requestBody["mobile"] = customerId;
  saleMsg = "0-0";
  flag = false;

  if (quantity <= 100)
  {
    calibrationFactor = hundred;
  }
  if (quantity <= 200)
  {
    calibrationFactor = twohundred;
  }
  else if (quantity <= 300)
  {
    calibrationFactor = threehundred;
  }
  else if (quantity <= 400)
  {
    calibrationFactor = fourhundred;
  }
  else if (quantity <= 500)
  {
    calibrationFactor = fivehundred;
  }
  else if (quantity <= 1000)
  {
    calibrationFactor = thousand;
  }
  else
  {
    calibrationFactor = thousandPlus;
  }
  WebSerial.println(String(calibrationFactor));

  digitalWrite(M_RELAY, LOW);
  delay(200);

  while (totalMilliLitres < quantity)
  {
    Serial.println("Sale inprogress");
    currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {
      if (loop_check > max_loop)
      {
        digitalWrite(M_RELAY, HIGH);
        delay(200);
        flag = true;
        break;
      }
      pulse1Sec = pulseCount;
      pulseCount = 0;

      flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
      previousMillis = millis();

      flowMilliLitres = (flowRate / 60) * interval;

      // Add the millilitres passed in this second to the cumulative total
      totalMilliLitres += flowMilliLitres;
      module.setDisplayToString(String(totalMilliLitres));
      if (lastRead != totalMilliLitres)
      {
        loop_check = 0;
        lastRead = totalMilliLitres;
      }
      else
      {
        loop_check++;
      }
    }
    delay(interval);
  }
  digitalWrite(M_RELAY, HIGH);
  if (totalMilliLitres > 25)
  {

    if (flag)
    {
      price = ceil((totalMilliLitres * milkPrice) / 1000);
      quantity = totalMilliLitres;
      requestBody["price"] = price;
      requestBody["quantity"] = totalMilliLitres;
    }
    saleMsg = String(quantity) + '-' + String(price);
    module.setDisplayToString(saleMsg);
    if ((WiFi.status() == WL_CONNECTED))               // new
    {                                                  // new
      httpRequest("POST", "transaction", requestBody); // new
    }                                                  // new
    else                                               // new
    {                                                  // new
      module.setDisplayToString("Connect");            // new
      WiFi.begin(ssid, password);                      // new
      while (WiFi.status() != WL_CONNECTED)            // new
      {                                                // new
        delay(1000);                                   // new
        Serial.println("Connecting to Wifi");          // new
        module.setDisplayToString("Connect");          // new
      }                                                // new
      Serial.println("Connected");                     // new
      Serial.print("IP address: ");                    // new
      httpRequest("POST", "transaction", requestBody); // new
    }                                                  // new
  }
  flag = false;
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  loop_check = 0;
  lastRead = 0;
}

String httpRequest(String reqType, String path, JSONVar params)
{

  HTTPClient http;

  http.begin(endpoint + path, ca_cert); // Specify the URL and certificate
  http.addHeader("Authorization", "Bearer " + token);

  int httpCode = 0;
  if (reqType == "GET")
  {
    httpCode = http.GET();
  }
  else
  {
    if (reqType == "POST")
    {
      http.addHeader("Content-Type", "application/json");
      httpCode = http.POST(JSON.stringify(params));
    }
  }

  String response = "{}";
  if (httpCode > 0)
  { // Check for the returning code
    response = http.getString();
    Serial.println(httpCode);
    Serial.println(response);
  }
  else if (httpCode == 401 || httpCode == 403)
  {
    token = "";
    alarmBuzzer(3, 200);
  }
  else
  {
    Serial.println("Error on HTTP request");
  }
  http.end(); // Free the resources
  return response;
}

JSONVar ReadRFIDData(int readInterval)
{
  mfrc522.PCD_Init();
  String payload = "{}";
  boolean readFlag = false;
  JSONVar requestBody = JSON.parse(payload);
  startTime = millis();
  currentTime = startTime;

  while (!mfrc522.PICC_IsNewCardPresent())
  {
    if ((currentTime - startTime) <= readInterval)
    {
      Serial.print(".");
      currentTime = millis();
    }
    else
    {
      readFlag = true;
      break;
    }
  };
  while (!mfrc522.PICC_ReadCardSerial())
  {
    if ((currentTime - startTime) <= readInterval)
    {
      Serial.print(".");
      currentTime = millis();
    }
    else
    {
      readFlag = true;
      break;
    }
  };
  if (readFlag)
  {
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

String ReadBlockData(byte block)
{
  byte blockBuffer[18];
  byte len = 18;
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;
  MFRC522::StatusCode status;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }

  status = mfrc522.MIFARE_Read(block, blockBuffer, &len);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }
  String blockData = "";
  for (uint8_t i = 0; i < 16; i++)
  {
    if (blockBuffer[i] == 32)
    {
      break;
    }
    blockData += char(blockBuffer[i]);
  }
  blockData.trim();
  return blockData;
}

int readInputFromKeyPad(int mn, int mx)
{
  String text = "";
  String temp = "";
  while (text.length() <= mx)
  {
    temp = handleInput(&module, &mode, temp, mx);
    if (temp == "ok")
    {
      if (text.length() >= mn)
      {
        break;
      }
      temp = text;
    }
    else if (temp == "qty")
    {
      qtyFlag = true;
      temp = "";
      text = "";
      module.setDisplayToString("qty");
    }
    else if (temp == "card")
    {
      module.setDisplayToString("cardread");
      String payload = "{}";
      Serial.println("Insdie card read");
      JSONVar requestBody = JSON.parse(payload);
      requestBody = ReadRFIDData(3000);
      customerId = requestBody["mobile"];

      if (customerId.length() < 10)
      {
        module.setDisplayToString("err");
        alarmBuzzer(2, 200);
      }
      else
      {
        module.setDisplayToString("cus");
        alarmBuzzer(1, 200);
      }

      temp = "";
      text = "";
    }
    else if (temp == "code")
    {
      module.setDisplayToString(MACHINE_ID);
      temp = "";
      text = "";
    }
    else
    {
      text = temp;
    }
  }
  return text.toInt();
}

String handleInput(TM1638QYF *module, word *mode, String tempText, int l)
{

  word buttons = module->getButtons();

  // button pressed - change mode
  if (buttons != 0)
  {
    *mode = buttons >> 1;

    if (*mode < 0)
    {
      module->clearDisplay();
      delay(100);
    }
    switch (*mode)
    {
    case 0: // S1
      tempText = "card";
      break;
    case 1: // S2
      tempText += "1";
      break;
    case 2: // S3
      tempText += "2";
      break;
    case 4: // S4
      tempText += "3";
      break;
    case 8: // S5
      tempText = "code";
      break;
    case 16: // S6
      tempText += "4";
      break;
    case 32: // S7
      tempText += "5";
      break;
    case 64: // S8
      tempText += "6";
      break;
    case 128: // S9
      break;
    case 256: // S10
      tempText += "7";
      break;
    case 512: // S11
      tempText += "8";
      break;
    case 1024: // S12
      tempText += "9";
      break;
    case 2048: // S13
      tempText = "qty";
      break;
    case 4096: // S14
      tempText = "clear";
      break;
    case 8192: // S15
      tempText += "0";
      break;
    case 16384: // S16
      tempText = "ok";
      break;
    default: // unknown busson
      break;
    }
    delay(300);
  }
  String displayInput = tempText;
  if (displayInput.length() > l)
  {
    displayInput = "";
    module->setDisplayToString("----");
  }
  else if (displayInput.length() > 0)
  {
    module->setDisplayToString(displayInput);
  }

  return displayInput;
}

void alarmBuzzer(int count, int buzDelay)
{
  int i = 0;
  while (i < count)
  {
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
