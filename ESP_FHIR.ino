#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "FS.h"
#include "EEPROM.h"
#include "HTML.h"
#include <WebServer.h>
WebServer server(80);
#include "Additions.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "time.h"
#include <ESP32Time.h>
#include <base64.h>
#include "CIC_Datalog.h"
#include "INC_Datalog.h"
#include "SVAAS_Datalog.h"
#include "HCM_Datalog.h"

ESP32Time rtc;
HTTPClient http;
HTTPClient https;
WiFiClientSecure client;
File root;

#define DebugPort Serial
// #ifndef DebugPort
//   HardwareSerial DebugPort(2);
// #endif
#define LED 2
#define FORMAT_SPIFFS_IF_FAILED true

int NUC_flag = 0;
int count = 0;
int packet_loss = 0;
String fhir_creds = "fhiruser:change-password";
String macd = WiFi.macAddress();
char ssid[30];
char pass[30];
String p_time, in_data, postData, s, p, d, doc_id, pid, time_stamp;
unsigned long previousMillis = 0;
unsigned long interval = 3000;
const char* ntpServer = "time.windows.com";  //pool.ntp.org
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;
String split_arr[60];
String p_deets[10];
//String base_url = "http://pmsind.co.in:9444/fhir-server/api/v4";
String base_url = "https://192.168.0.126/fhir";
String graph_url = "http://192.168.0.126:9043/data";
int httpCode;
int ownerflag = 1;
int locationflag = 1;
String output;
String device_resource_id;
String patient_resource_id;
String observation_resource_id;
String communication_resource_id;
String mother_name;
String owner_id;
String location_id;
int patientid;
#define httpPOST 1
#define httpPost 2
#define httpGET 3
#define httpPUT 4


int http_send(String url, uint8_t method, String& data) {
  int return_code;
  switch (method) {
    case httpPOST:
      {
        http.begin(url);
        http.addHeader("Authorization", String("Basic " + base64::encode(fhir_creds)));
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Accept", "*/*");
        http.addHeader("Accept-Encoding", "gzip, deflate, br");
        http.addHeader("Connection", "keep-alive");
        http.addHeader("Content-Length", String(data.length()));
        return_code = http.POST(data);
        http.end();
        break;
      }

    case httpPost:
      {
        https.begin(url);
        unsigned long sstart = millis();
        https.addHeader("Content-Type", "application/json");
        return_code = https.POST(data);
        https.end();
        unsigned long se_start = millis();
        se_start = se_start - sstart;
        Serial.print("MS:");
        Serial.println(se_start);
        break;
      }
    case httpGET:
      http.begin(url);
      http.addHeader("Authorization", String("Basic " + base64::encode(fhir_creds)));
      return_code = http.GET();
      data = http.getString();
      http.end();
      break;

    case httpPUT:
      http.begin(url);
      unsigned long start = millis();
      http.addHeader("Authorization", String("Basic " + base64::encode(fhir_creds)));
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Accept", "*/*");
      http.addHeader("Accept-Encoding", "gzip, deflate, br");
      http.addHeader("Connection", "keep-alive");
      http.addHeader("Content-Length", String(data.length()));
      return_code = http.PUT(data);
      unsigned long e_start = millis();
      e_start = e_start - start;
      Serial.print("DL:");
      Serial.println(e_start);
      http.end();
      break;

      // default: Serial.println("Invalid method");
  }

  return return_code;
}

void get_owner() {
  DynamicJsonDocument n(4000);
  output = "";
  Serial.println(device_resource_id);
  httpCode = http_send(base_url + "/Device/" + String(device_resource_id), httpGET, output);
  Serial.println(output);
  DeserializationError error = deserializeJson(n, output);
  if (error) {
    Serial.println("Device Reg:");
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  output = "";
  owner_id = "";
  location_id = "";
  if (n["owner"]["reference"]) {
    const char* tem = n["owner"]["reference"];
    owner_id = String(tem);
    Serial.println("#########" + owner_id);

  } else {
    ownerflag = 2;
    Serial.println("NO OWNER_ID IN DEVICE... TRY AGAIN");
  }
  if (n["location"]["reference"]) {
    const char* tem = n["location"]["reference"];
    location_id = String(tem);
    Serial.println("!!!!!!!!!!!!!" + location_id);
  } else {
    locationflag = 2;
    Serial.println("NO location_id IN DEVICE... TRY AGAIN");
  }
  n.clear();
}

// function to register new user
void new_user_c(String* patient_deets, String D_name) {
  get_owner();
  DynamicJsonDocument nuc(5000);
  nuc["resourceType"] = "Patient";

  JsonObject extension_0 = nuc["extension"].createNestedObject();
  extension_0["url"] = "http://hl7.org/fhir/StructureDefinition/patient-mothersMaidenName";
  extension_0["valueString"] = patient_deets[1];

  JsonArray identifier = nuc.createNestedArray("identifier");
  identifier[0]["system"] = "urn:ietf:rfc:3986";
  identifier[0]["value"] = patient_deets[0];

  Serial.println(ownerflag);
  if (ownerflag == 1) {
    JsonObject managingOrganization = nuc.createNestedObject("managingOrganization");
    managingOrganization["reference"] = owner_id;
  }

  serializeJson(nuc, output);
  Serial.println(output);
  httpCode = http_send(String(base_url) + "/Patient", httpPOST, output);
  if (httpCode == 201) {
    NUC_flag++;
  }

  String payload;
  patient_deets[1].replace(" ", "%20");
  httpCode = http_send(String(base_url) + "/Patient?identifier=" + patient_deets[0], httpGET, payload);
  nuc.clear();

  DeserializationError error = deserializeJson(nuc, payload);
  if (error) {
    Serial.println("New User Creation:");
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  patient_resource_id = "";
  const char* temp = nuc["entry"][0]["resource"]["id"];
  patient_resource_id = String(temp);
  Serial.println(patient_resource_id);
  nuc.clear();

  nuc["resourceType"] = "Device";
  nuc["status"] = "active";
  nuc["id"] = device_resource_id;
  nuc["manufacturer"] = "Phoenix Medical Systems Pvt. Ltd";

  JsonArray identifier_0 = nuc.createNestedArray("identifier");
  identifier_0[0]["system"] = "urn:ietf:rfc:3986";
  identifier_0[0]["value"] = macd;
  identifier_0[1]["system"] = "urn:ietf:rfc:3986";
  identifier_0[1]["value"] = D_name;
  nuc["patient"]["reference"] = "Patient/" + patient_resource_id;
  if (ownerflag == 1) {
    nuc["owner"]["reference"] = owner_id;
  }
  if (locationflag == 1) {
    nuc["location"]["reference"] = location_id;
  }
  output = "";
  serializeJson(nuc, output);
  Serial.println(output);

  httpCode = http_send(String(base_url) + "/Device/" + String(device_resource_id), httpPUT, output);
  Serial.println(httpCode);
  if (httpCode == 200) {
    NUC_flag++;
  }
  nuc.clear();
}


void SVAAS_new_user_c(String M_name, String D_name) {
  get_owner();
  DynamicJsonDocument nuc(5000);
  nuc["resourceType"] = "Patient";

  JsonObject extension_0 = nuc["extension"].createNestedObject();
  extension_0["url"] = "http://hl7.org/fhir/StructureDefinition/patient-mothersMaidenName";
  extension_0["valueString"] = M_name;

  JsonArray identifier = nuc.createNestedArray("identifier");
  identifier[0]["system"] = "urn:ietf:rfc:3986";
  String tempvv = String(random(10000));
  identifier[0]["value"] = tempvv;

  JsonObject managingOrganization = nuc.createNestedObject("managingOrganization");
  managingOrganization["reference"] = owner_id;

  serializeJson(nuc, output);
  Serial.println(output);
  httpCode = http_send(String(base_url) + "/Patient", httpPOST, output);
  Serial.println(httpCode);
  if (httpCode != 201) {
    return;
  }
  if (httpCode == 201) {
    NUC_flag++;
  }
  String payload;
  httpCode = http_send(String(base_url) + "/Patient?identifier=" + String(tempvv), httpGET, payload);  //change
  Serial.println(httpCode);
  nuc.clear();


  DeserializationError error = deserializeJson(nuc, payload);
  if (error) {
    Serial.println("New User Creation:");
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  patient_resource_id = "";
  const char* temp = nuc["entry"][0]["resource"]["id"];
  patient_resource_id = String(temp);
  Serial.println(patient_resource_id);
  nuc.clear();

  nuc["resourceType"] = "Device";
  nuc["status"] = "active";
  nuc["id"] = device_resource_id;
  nuc["manufacturer"] = "Phoenix Medical Systems Pvt. Ltd";

  JsonArray identifier_0 = nuc.createNestedArray("identifier");
  identifier_0[0]["system"] = "urn:ietf:rfc:3986";
  identifier_0[0]["value"] = macd;
  identifier_0[1]["system"] = "urn:ietf:rfc:3986";
  identifier_0[1]["value"] = D_name;
  nuc["patient"]["reference"] = "Patient/" + patient_resource_id;
  if (ownerflag == 1) {
    nuc["owner"]["reference"] = owner_id;
  }
  if (locationflag == 1) {
    nuc["location"]["reference"] = location_id;
  }
  output = "";
  serializeJson(nuc, output);

  httpCode = http_send(base_url + "/Device/" + String(device_resource_id), httpPUT, output);
  Serial.println(httpCode);
  if (httpCode != 201) {
    return;
  }
  if (httpCode == 200) {
    NUC_flag++;
  }
  nuc.clear();
}

void new_obs_c(String dev) {

  DynamicJsonDocument noc(4000);

  noc["resourceType"] = "Observation";
  noc["status"] = "final";

  JsonArray identifier = noc.createNestedArray("identifier");
  identifier[0]["value"] = dev;
  identifier[1]["value"] = "Data Log";
  noc["device"]["reference"] = "Device/" + device_resource_id;
  noc["subject"]["reference"] = "Patient/" + patient_resource_id;

  JsonObject code = noc.createNestedObject("code");

  JsonObject code_coding_0 = code["coding"].createNestedObject();
  code_coding_0["system"] = "http://loinc.org";
  code_coding_0["code"] = "85353-1";
  code_coding_0["display"] = "Vital signs, weight, height, head circumference, oximetry, BMI, and BSA panel";
  code["text"] = "Vital signs panel";

  JsonObject category_0_coding_0 = noc["category"][0]["coding"].createNestedObject();
  category_0_coding_0["system"] = "http://terminology.hl7.org/CodeSystem/observation-category";
  category_0_coding_0["code"] = "data-log";
  category_0_coding_0["display"] = "Data Log";
  output = "";
  serializeJson(noc, output);

  httpCode = http_send(base_url + "/Observation", httpPOST, output);
  if (httpCode == 201) {
    NUC_flag++;
  }
  if (httpCode != 201) {
    return;
  }

  noc.clear();
  output = "";
  httpCode = http_send(base_url + "/Observation?patient=" + String(patient_resource_id), httpGET, output);

  DeserializationError error = deserializeJson(noc, output);
  if (error) {
    Serial.println("New Observation Creating:");
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  observation_resource_id = "";
  const char* temp = noc["entry"][0]["resource"]["id"];
  observation_resource_id = String(temp);
  Serial.println(observation_resource_id);
  noc.clear();
}

void new_com_c(String dev) {
  DynamicJsonDocument doc(4000);

  doc["resourceType"] = "Communication";
  doc["status"] = "completed";
  doc["sender"]["reference"] = "Device/" + device_resource_id;
  doc["sent"] = "2023-06-12T15:09:10-08:00";
  doc["payload"][0]["contentReference"]["display"] = "Alarm details";
  output = "";
  serializeJson(doc, output);

  httpCode = http_send(String(base_url) + "/Communication", httpPOST, output);
  if (httpCode == 201) {
    NUC_flag++;
  }
  if (httpCode != 201) {
    return;
  }
  output = "";
  httpCode = http_send(base_url + "/Communication?_sort=-sent&_count=1&sender=" + String(device_resource_id), httpGET, output);
  doc.clear();

  DeserializationError error = deserializeJson(doc, output);
  if (error) {
    Serial.println("New Communication Creating:");
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  communication_resource_id = "";
  const char* temp = doc["entry"][0]["resource"]["id"];
  communication_resource_id = String(temp);
  http.end();
  doc.clear();
}

void device_reg() {
  Serial.println("starting dev_reg");
  DynamicJsonDocument device_reg(4000);
  device_reg["resourceType"] = "Device";
  device_reg["status"] = "active";
  device_reg["manufacturer"] = "Phoenix Medical Systems Pvt. Ltd";

  JsonArray identifier = device_reg.createNestedArray("identifier");

  JsonObject identifier_0 = identifier.createNestedObject();
  identifier_0["system"] = "urn:ietf:rfc:3986";
  identifier_0["value"] = macd;

  JsonObject identifier_1 = identifier.createNestedObject();
  identifier_1["system"] = "urn:ietf:rfc:3986";
  identifier_1["value"] = "Phoenix Devices";

  output = "";
  serializeJson(device_reg, output);
  int check = 0;
  while (check < 1) {
    httpCode = http_send(base_url + "/Device", httpPOST, output);
    if (httpCode == 201) {
      check = 1;
    } else {
      Serial.println("DEV_REG FAILED");
    }
  }
  check = 0;
  device_reg.clear();

  output = "";
  httpCode = http_send(base_url + "/Device?identifier=" + macd, httpGET, output);  //+ macd
  DeserializationError error = deserializeJson(device_reg, output);
  if (error) {
    Serial.println("Device Reg:");
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  output = "";
  device_resource_id = "";
  const char* temp = device_reg["entry"][0]["resource"]["id"];
  device_resource_id = String(temp);
  Serial.println(device_resource_id);

  writeFile(SPIFFS, "/config.json", "{\"device\":\"" + String(device_resource_id) + "\",\"patient\":\"" + String(patient_resource_id) + "\",\"communication\":\"" + String(communication_resource_id) + "\",\"observation\":\"" + String(observation_resource_id) + "\",\"mother_name\":\"" + String(mother_name) + "\"}");
  EEPROM.writeByte(0x01, 0x01);
  EEPROM.commit();
  device_reg.clear();
  Serial.println("ending dev_reg");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, pass);
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  // #ifndef DebugPort
  //   DebugPort.begin(15200, SERIAL_8N1, 16, 17);
  // #endif
  //http.setTimeout(400);
  pinMode(LED, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  digitalWrite(0, HIGH);
  EEPROM.begin(300);
  WiFi.disconnect(true);
  http.setReuse(true);
  https.setReuse(true);
  Serial.println(WiFi.macAddress());

  if (!CheckWIFICreds()) {
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    digitalWrite(2, HIGH);
    while (loadWIFICredsForm())
      ;
  } else {
    s = EEPROM.readString(100);
    p = EEPROM.readString(150);
    s.toCharArray(ssid, 30);
    p.toCharArray(pass, 30);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.print("\nConnecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
      count = count + 1;
      if (count > 30) {
        WiFi.disconnect();
        wipeEEPROM();
        Serial.println("Check Password and Try Again");
        loadWIFICredsForm();
        count = 0;
      }
    }
    Serial.println("Succesfully Connected");
  }
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  bool t_flag = true;
  while (t_flag) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      Serial.println("Time successfully retrieved.");
      Serial.print("Local time: ");
      Serial.println(&timeinfo, "%Y-%m-%dT%X+05:30");  // Print the local time with the desired format and timezone offset
      rtc.setTimeStruct(timeinfo);
      Serial.print("RTC time: ");
      Serial.println(rtc.getTime("%Y-%m-%dT%X+05:30"));  // Print the RTC time with the desired format and timezone offset
      t_flag = false;                                    // Set t_flag to false to exit the loop after successfully setting the time
    } else {
      Serial.println("Failed to retrieve time. Retrying...");
      delay(100);  // Delay before retrying
    }
  }
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  if (digitalRead(0) == LOW) {
    Serial.println("Wiping WiFi credentials from memory...");
    wipeEEPROM();
    digitalWrite(2, HIGH);
    while (loadWIFICredsForm())
      ;
    digitalWrite(2, LOW);
    return;  // Exit setup to prevent further execution
  }
  uint8_t device_reg_check = EEPROM.readByte(0x01);
  if (device_reg_check != 0x01) {
    device_reg();
  } else {
    Serial.println("Skipping Device Reg");
    DynamicJsonDocument dev_data(6000);
    File input = SPIFFS.open("/config.json");
    DeserializationError error = deserializeJson(dev_data, input);
    input.close();
    if (error) {
      Serial.println("Skipping Device Reg:");
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    const char* buffer;
    buffer = dev_data["device"];
    device_resource_id = String(buffer);
    buffer = dev_data["patient"];
    patient_resource_id = String(buffer);
    buffer = dev_data["communication"];
    communication_resource_id = String(buffer);
    buffer = dev_data["observation"];
    observation_resource_id = String(buffer);
    buffer = dev_data["mother_name"];
    mother_name = String(buffer);

    Serial.println(device_resource_id);
    Serial.println(patient_resource_id);
    Serial.println(communication_resource_id);
    Serial.println(observation_resource_id);
    Serial.println(mother_name);
    dev_data.clear();
    digitalWrite(0, LOW);
  }
}

void loop() {
  unsigned long startTime;
  if (digitalRead(0) == LOW) {
    Serial.println("Wiping WiFi credentials from memory...");
    wipeEEPROM();
    digitalWrite(2, HIGH);
    while (loadWIFICredsForm())
      ;
    digitalWrite(2, LOW);
  }
  if (Serial.available() > 0) {
    String in_data = Serial.readString();
    DebugPort.println(in_data);
    int cont = split_str(in_data, split_arr, "|");
    if (cont == 3 || cont == 4 || cont == 5) {
      if (split_arr[0] == "CIC") {
        if (split_arr[1] == "NUC") {
          split_str(split_arr[2], p_deets, ",");
          new_user_c(p_deets, "Comprehensive Infant Care Centre");
          new_obs_c(split_arr[0]);
          new_com_c(split_arr[0]);
          writeFile(SPIFFS, "/config.json", "{\"device\":\"" + String(device_resource_id) + "\",\"patient\":\"" + String(patient_resource_id) + "\",\"communication\":\"" + String(communication_resource_id) + "\",\"observation\":\"" + String(observation_resource_id) + "\",\"mother_name\":\"" + String(mother_name) + "\"}");
          if (NUC_flag = 4) {
            Serial.println("HANDSHAKE SENT");
            Serial.write(01);  // transmit to start datalog
          }
        } else if (split_arr[1] == "DATALOG") {
          output = "";
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
          cic_data(output, split_arr[2], device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
          httpCode = http_send(base_url + "/Observation/" + observation_resource_id, httpPUT, output);
          Serial.printf("Http_CIC_Datalog_Resp - %d\n", httpCode);
          output = "";
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
          int vytemp = cic_alarm(output, split_arr[3], device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
          if (vytemp != 1) {
            httpCode = http_send(base_url + "/Communication/" + String(communication_resource_id), httpPUT, output);
          }
        } else if (split_arr[1] == "MASIMO") {
          output = "";
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
          String sanitizedData = split_arr[2];
          sanitizedData = sanitizedData.substring(0, sanitizedData.length() - 1);
          String jsonData = "{\"device_id\":\"" + String(device_resource_id) + "\",\"patient_id\":\"" + String(patient_resource_id) + "\",\"timestamp\":\"" + time_stamp + "\",\"data\":[" + sanitizedData + "]}";
          httpCode = http_send(graph_url, httpPost, jsonData);
          Serial.printf("Http_CIC_MAS_Resp - %d\n", httpCode);
          if (httpCode == 200) {
            digitalWrite(2, HIGH);
          }
          jsonData.clear();
          sanitizedData.clear();
        } else {
          Serial.println("INVALID PACKET RECEIVED");
        }
      }
      //BRAMMI DEV
      else if (split_arr[0] == "HCM") {
        if (split_arr[1] == "NUC") {
          split_str(split_arr[2], p_deets, ",");
          if (p_deets[0] == "PATID001") { //PATID001
            p_deets[0] = String(random(10000));  //Random Device ID because Device ID Page not present
          }
          new_user_c(p_deets, "Heating Cooling Machine");
          new_obs_c(split_arr[0]);
          new_com_c(split_arr[0]);
          writeFile(SPIFFS, "/config.json", "{\"device\":\"" + String(device_resource_id) + "\",\"patient\":\"" + String(patient_resource_id) + "\",\"communication\":\"" + String(communication_resource_id) + "\",\"observation\":\"" + String(observation_resource_id) + "\",\"mother_name\":\"" + String(mother_name) + "\"}");
        } else if (split_arr[1] == "DATALOG") {
          String dlog_ar[50];
          split_str(split_arr[2], dlog_ar, ",");
          if (dlog_ar[0] == "I") {
            Serial.println("IDLE");
          } else {
            output = "";
            time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
            HCM_data(output, split_arr[2], device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
            httpCode = http_send(base_url + "/Observation/" + observation_resource_id, httpPUT, output);
            output = "";
            time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
            HCM_alarm(output, split_arr[3], device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
            httpCode = http_send(base_url + "/Communication/" + String(communication_resource_id), httpPUT, output);
          }
        }
      }
      //INC DEV
      else if (split_arr[0] == "INC") {
        if (split_arr[1] == "NUC") {
          split_str(split_arr[2], p_deets, ",");
          new_user_c(p_deets, "Intensive Neonatal Care Center");
          new_obs_c(split_arr[0]);
          new_com_c(split_arr[0]);

          writeFile(SPIFFS, "/config.json", "{\"device\":\"" + String(device_resource_id) + "\",\"patient\":\"" + String(patient_resource_id) + "\",\"communication\":\"" + String(communication_resource_id) + "\",\"observation\":\"" + String(observation_resource_id) + "\",\"mother_name\":\"" + String(mother_name) + "\"}");
          if (NUC_flag = 4) {
            Serial.write(01);
          }
        } else if (split_arr[1] == "DATALOG") {
          String dlog_ar[50];
          split_str(split_arr[2], dlog_ar, ",");
          output = "";
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));

          inc_data(output, split_arr[2], device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
          httpCode = http_send(base_url + "/Observation/" + String(observation_resource_id), httpPUT, output);
          Serial.printf("Http_INC_Ob_Resp - %d\n", httpCode);
          if (httpCode == 200) {
            digitalWrite(2, HIGH);
          }
          output = "";
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
          int vvtemp = inc_alarm(output, split_arr[3], device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
          if (vvtemp != 1) {
            httpCode = http_send(base_url + "/Communication/" + String(communication_resource_id), httpPUT, output);
            Serial.printf("Http_INC_Cm_Resp - %d\n", httpCode);
          }
        } else if (split_arr[1] == "MASIMO") {
          output = "";
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
          String sanitizedData = split_arr[2];
          sanitizedData = sanitizedData.substring(0, sanitizedData.length() - 1);  //To remove the end comma and /n
          String jsonData = "{\"device_id\":\"" + String(device_resource_id) + "\",\"patient_id\":\"" + String(patient_resource_id) + "\",\"timestamp\":\"" + time_stamp + "\",\"data\":[" + sanitizedData + "]}";
          httpCode = http_send(graph_url, httpPost, jsonData);
          Serial.printf("Http_INC_MAS_Resp - %d\n", httpCode);
          if (httpCode == 200) {
            digitalWrite(2, HIGH);
          }
          jsonData.clear();
          sanitizedData.clear();
        } else {
          Serial.println("WRONG PACKET");
        }
      }
    }
    // SVAAS
    else if (cont <= 1) {
      int connt = split_str(in_data, split_arr, ",");
      Serial.println(connt);
      if (connt == 45) {
        String m_name = split_arr[44];
        if (mother_name != m_name && m_name != "000") {
          writeFile(SPIFFS, "/config.json", "{\"device\":\"" + String(device_resource_id) + "\",\"patient\":\"" + " " + "\",\"communication\":\"" + " " + "\",\"observation\":\"" + " " + "\",\"mother_name\":\"" + " " + "\"}");
          patient_resource_id = " ";
          observation_resource_id = " ";
          communication_resource_id = " ";
          mother_name = m_name;
          SVAAS_new_user_c(mother_name, "PMS-SVAAS");
          new_obs_c("SVAAS");
          new_com_c("SVAAS");
          writeFile(SPIFFS, "/config.json", "{\"device\":\"" + String(device_resource_id) + "\",\"patient\":\"" + String(patient_resource_id) + "\",\"communication\":\"" + String(communication_resource_id) + "\",\"observation\":\"" + String(observation_resource_id) + "\",\"mother_name\":\"" + String(mother_name) + "\"}");
        } else {
          output = "";
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
          SVAAS_data(output, in_data, device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
          httpCode = http_send(base_url + "/Observation/" + String(observation_resource_id), httpPUT, output);
          Serial.println(httpCode);
          output = "";
          if (httpCode != 200) {
            return;
          }
          time_stamp = String(rtc.getTime("%Y-%m-%dT%X+05:30"));
          SVAAS_alarm(output, in_data, device_resource_id, patient_resource_id, communication_resource_id, observation_resource_id, time_stamp);
          httpCode = http_send(base_url + "/Communication/" + String(communication_resource_id), httpPUT, output);
          Serial.println(httpCode);
          if (httpCode != 200) {
            return;
          }
        }
      }
    } else {
      packet_loss++;
      DebugPort.println("WRONG DATA PACKET:");
      DebugPort.println(packet_loss);
    }
    digitalWrite(2, LOW);
    in_data.clear();
  }
}