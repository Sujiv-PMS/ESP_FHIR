/*
 * Function to handle unknown URLs
 */
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

/*
 * Function for writing WiFi creds to EEPROM
 * Returns: true if save successful, false if unsuccessful
 */


bool writeToMemory(String ssid, String pass, String hos, String device, String location) {
  char buff1[30], buff2[30], buff3[30], buff4[10], buff5[30];
  ssid.toCharArray(buff1, 30);
  pass.toCharArray(buff2, 30);
  hos.toCharArray(buff3, 30);
  location.toCharArray(buff5, 30);
  //  device.toCharArray(buff5,30);
  EEPROM.writeString(100, buff1);
  EEPROM.writeString(150, buff2);
  EEPROM.writeString(200, buff3);
  //  EEPROM.writeString(250,buff4);
  EEPROM.writeString(250, buff5);
  delay(100);
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(150);
  //#if DEBUG
  Serial.print("Stored SSID, password, are: ");
  Serial.print(s);
  Serial.print(" / ");
  Serial.print(p);
  //#endif
  if (ssid == s && pass == p) {
    return true;
  } else {
    return false;
  }
}


/*
 * Function for handling form
 */
void handleSubmit() {
  String response_success = "<h1>Attempting to Connect</h1>";
  response_success += "<h2>Device will restart in 3 seconds. In case of faliure, hotspot will turn on again</h2>";

  String response_error = "<h1>Error</h1>";
  response_error += "<h2><a href='/'>Go back</a>to try again";

  if (writeToMemory(String(server.arg("ssid")), String(server.arg("password")), String(server.arg("hos")), String(server.arg("device")), String(server.arg("loc")))) {
    server.send(200, "text/html", response_success);
    EEPROM.commit();
    delay(3000);
    ESP.restart();
  } else {
    server.send(200, "text/html", response_error);
  }
}

/*
 * Function for home page
 */
void handleRoot() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    handleSubmit();
  } else {
    server.send(200, "text/html", INDEX_HTML);
  }
}

/*
 * Function for loading form
 * Returns: false if no WiFi creds in EEPROM
 */
bool loadWIFICredsForm() {
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(150);

  const char* ssid = "Phoenix_WiFi_Manager";
  const char* password = "opencare100";

  Serial.println("Setting Access Point...");

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println("HTTP server started");

  while (s.length() <= 0 && p.length() <= 0) {
    server.handleClient();
    delay(100);
  }

  return false;
}

/*
 * Function checking WiFi creds in memory 
 * Returns: true if not empty, false if empty
 */
bool CheckWIFICreds() {
  Serial.println("Checking WIFI credentials");
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(150);
  //#if DEBUG
  Serial.print("Found credentials: ");
  Serial.print(s);
  Serial.print("/");
  Serial.print(p);
  delay(500);
  //#endif
  if (s.length() > 0 && p.length() >= 0) {
    return true;
  } else {
    return false;
  }
}


void writeFile(fs::FS& fs, String path_s, String message_s) {
  Serial.println("Writing file: " + path_s);

  const char* path = path_s.c_str();
  const char* message = message_s.c_str();
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("File does not Exist");
    return;
  }
  if (file.print(message)) {
    Serial.println("Data Stored in Flash");
  } else {
    Serial.println("Flash Write Failed");
  }
  file.close();
}

String readFile(fs::FS& fs, String path_s) {
  Serial.println("Reading file: " + path_s);
  const char* path = path_s.c_str();
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("Failed to open file for reading");
    return "error";
  }

  Serial.println("Data read from file:");
  String op_str;
  char buffer[100];
  while (file.available()) {
    // Serial.write(file.read());
    // op_str.concat(file.read());
    op_str = file.readString();
  }
  file.close();
//  Serial.println(op_str);
  return op_str;
}

void deleteFile(fs::FS &fs, String path_s){
  const char * path = path_s.c_str();
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}


int split_str(String s, String split_arr[], String delimeter) {
  int i = 0;
  int a = s.indexOf(delimeter);
  while (a != -1) {
    split_arr[i] = s.substring(0, a);
    if (a + 1 > s.length() - 1) {
      break;
    }
    s = s.substring(a + 1);
    a = s.indexOf(delimeter);
    i++;
    if (a == -1) {
      split_arr[i] = s.substring(0, s.length());
    }
  }
  return i+1;
}

void wipeEEPROM() {
  for (int i = 0; i < 300; i++) {
    EEPROM.writeByte(i, 0);
  }
  EEPROM.commit();
  deleteFile(SPIFFS, "/config.json");
}

