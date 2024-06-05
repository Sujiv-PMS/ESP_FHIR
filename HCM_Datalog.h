void HCM_data(String &return_data, String &in_data, String &d_id, String &p_id, String &c_id, String &o_id, String &time_stamp) {
  String dlog_arr[50];
  split_str(in_data, dlog_arr, ",");

  DynamicJsonDocument HCM_log(4096);

  HCM_log["id"] = o_id;
  HCM_log["resourceType"] = "Observation";
  HCM_log["status"] = "final";
  HCM_log["identifier"][0]["value"] = "PMS-HCM";
  HCM_log["subject"]["reference"] = "Patient/" + p_id;
  HCM_log["device"]["reference"] = "Device/" + d_id;
  HCM_log["effectiveDateTime"] = time_stamp;
  JsonObject code = HCM_log.createNestedObject("code");

  JsonObject code_coding_0 = code["coding"].createNestedObject();
  code_coding_0["system"] = "http://loinc.org";
  code_coding_0["code"] = "85353-1";
  code_coding_0["display"] = "Vital signs, weight, height, head circumference, oximetry, BMI, and BSA panel";
  code["text"] = "Vital signs panel";

  JsonObject category_0_coding_0 = HCM_log["category"][0]["coding"].createNestedObject();
  category_0_coding_0["system"] = "http://terminology.hl7.org/CodeSystem/observation-category";
  category_0_coding_0["code"] = "data-log";
  category_0_coding_0["display"] = "Data Log";

  JsonArray component = HCM_log.createNestedArray("component");

  if (dlog_arr[0] == "M") {

    JsonObject component_0 = component.createNestedObject();

    JsonObject component_0_code = component_0.createNestedObject("code");

    JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
    component_0_code_coding_0["system"] = "http://loinc.org";
    component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
    component_0_code_coding_0["display"] = "MODE";
    component_0_code["text"] = "MODE";

    JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
    component_0_valueQuantity["value"] = 1;
    component_0_valueQuantity["unit"] = "MATTRESS";
    component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_0_valueQuantity["code"] = "MATTRESS";

  } else if (dlog_arr[0] == "S") {

    JsonObject component_0 = component.createNestedObject();

    JsonObject component_0_code = component_0.createNestedObject("code");

    JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
    component_0_code_coding_0["system"] = "http://loinc.org";
    component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
    component_0_code_coding_0["display"] = "MODE";
    component_0_code["text"] = "MODE";

    JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
    component_0_valueQuantity["value"] = 2;
    component_0_valueQuantity["unit"] = "SERVO";
    component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_0_valueQuantity["code"] = "SERVO";
  } else if (dlog_arr[0] == "W") {
    JsonObject component_0 = component.createNestedObject();

    JsonObject component_0_code = component_0.createNestedObject("code");

    JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
    component_0_code_coding_0["system"] = "http://loinc.org";
    component_0_code_coding_0["code"] = "MMMMMM";
    component_0_code_coding_0["display"] = "MODE";
    component_0_code["text"] = "MODE";

    JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
    component_0_valueQuantity["value"] = 3;
    component_0_valueQuantity["unit"] = "WARMING";
    component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_0_valueQuantity["code"] = "WARMING";
  }
  JsonObject component_1 = component.createNestedObject();

  JsonObject component_1_code = component_1.createNestedObject("code");

  JsonObject component_1_code_coding_0 = component_1_code["coding"].createNestedObject();
  component_1_code_coding_0["system"] = "http://loinc.org";
  component_1_code_coding_0["code"] = "75951-4";
  component_1_code_coding_0["display"] = "Set Time";
  component_1_code["text"] = "Set Time";

  JsonObject component_1_valueQuantity = component_1.createNestedObject("valueQuantity");
  component_1_valueQuantity["value"] = dlog_arr[1].toInt();
  component_1_valueQuantity["unit"] = "Min";
  component_1_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_1_valueQuantity["code"] = "Min";


  JsonObject component_2 = component.createNestedObject();

  JsonObject component_2_code = component_2.createNestedObject("code");

  JsonObject component_2_code_coding_0 = component_2_code["coding"].createNestedObject();
  component_2_code_coding_0["system"] = "http://loinc.org";
  component_2_code_coding_0["code"] = "60839-8";
  component_2_code_coding_0["display"] = "Run Time";
  component_2_code["text"] = "Run Time";

  JsonObject component_2_valueQuantity = component_2.createNestedObject("valueQuantity");
  component_2_valueQuantity["value"] = dlog_arr[2].toFloat()/60;
  component_2_valueQuantity["unit"] = "Hr";
  component_2_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_2_valueQuantity["code"] = "Hr";

  JsonObject component_3 = component.createNestedObject();

  JsonObject component_3_code = component_3.createNestedObject("code");

  JsonObject component_3_code_coding_0 = component_3_code["coding"].createNestedObject();
  component_3_code_coding_0["system"] = "http://loinc.org";
  component_3_code_coding_0["code"] = "75927-4";
  component_3_code_coding_0["display"] = "Set Temp";
  component_3_code["text"] = "Set Temp";

  JsonObject component_3_valueQuantity = component_3.createNestedObject("valueQuantity");
  component_3_valueQuantity["value"] = dlog_arr[3].toFloat();
  component_3_valueQuantity["unit"] = "C°";
  component_3_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_3_valueQuantity["code"] = "C°";


  JsonObject component_4 = component.createNestedObject();

  JsonObject component_4_code = component_4.createNestedObject("code");

  JsonObject component_4_code_coding_0 = component_4_code["coding"].createNestedObject();
  component_4_code_coding_0["system"] = "http://loinc.org";
  component_4_code_coding_0["code"] = "20564-1";
  component_4_code_coding_0["display"] = "Rectal Measure Temp";
  component_4_code["text"] = "Rectal Measure Temp";

  JsonObject component_4_valueQuantity = component_4.createNestedObject("valueQuantity");
  component_4_valueQuantity["value"] = dlog_arr[4].toFloat();
  component_4_valueQuantity["unit"] = "C°";
  component_4_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_4_valueQuantity["code"] = "C°";

  JsonObject component_5 = component.createNestedObject();

  JsonObject component_5_code = component_5.createNestedObject("code");

  JsonObject component_5_code_coding_0 = component_5_code["coding"].createNestedObject();
  component_5_code_coding_0["system"] = "http://loinc.org";
  component_5_code_coding_0["code"] = "8867-4";
  component_5_code_coding_0["display"] = "Skin Measure Temp";
  component_5_code["text"] = "Skin Measure Temp";

  JsonObject component_5_valueQuantity = component_5.createNestedObject("valueQuantity");
  component_5_valueQuantity["value"] = dlog_arr[5].toFloat();
  component_5_valueQuantity["unit"] = "C°";
  component_5_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_5_valueQuantity["code"] = "C°";

  JsonObject component_6 = component.createNestedObject();

  JsonObject component_6_code = component_6.createNestedObject("code");

  JsonObject component_6_code_coding_0 = component_6_code["coding"].createNestedObject();
  component_6_code_coding_0["system"] = "http://loinc.org";
  component_6_code_coding_0["code"] = "73794-0";
  component_6_code_coding_0["display"] = "Mattress Measure Temp";
  component_6_code["text"] = "Mattress Measure Temp";

  JsonObject component_6_valueQuantity = component_6.createNestedObject("valueQuantity");
  component_6_valueQuantity["value"] = dlog_arr[6].toFloat();
  component_6_valueQuantity["unit"] = "C°";
  component_6_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_6_valueQuantity["code"] = "C°";

  JsonObject component_7 = component.createNestedObject();

  JsonObject component_7_code = component_7.createNestedObject("code");

  JsonObject component_7_code_coding_0 = component_7_code["coding"].createNestedObject();
  component_7_code_coding_0["system"] = "http://loinc.org";
  component_7_code_coding_0["code"] = "73793-2";
  component_7_code_coding_0["display"] = "Water flow";
  component_7_code["text"] = "Water flow";

  JsonObject component_7_valueQuantity = component_7.createNestedObject("valueQuantity");
  component_7_valueQuantity["value"] = dlog_arr[7].toInt();
  component_7_valueQuantity["unit"] = "NONE";  ////
  component_7_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_7_valueQuantity["code"] = "NONE";  ////

  JsonObject component_8 = component.createNestedObject();

  JsonObject component_8_code = component_8.createNestedObject("code");

  JsonObject component_8_code_coding_0 = component_8_code["coding"].createNestedObject();
  component_8_code_coding_0["system"] = "http://loinc.org";
  component_8_code_coding_0["code"] = "XXXXXXX";
  component_8_code_coding_0["display"] = "Water Level";
  component_8_code["text"] = "Water Level";

  JsonObject component_8_valueQuantity = component_8.createNestedObject("valueQuantity");
  component_8_valueQuantity["value"] = dlog_arr[8].toInt();
  component_8_valueQuantity["unit"] = "%";
  component_8_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_8_valueQuantity["code"] = "%";

  serializeJson(HCM_log, return_data);
}

String HOUT = "";
String Ind;
void intTo8BitArr(int inputInt, int *outputArray) {
  for (int i = 7; i >= 0; i--) {
    outputArray[i] = inputInt & 1;  // Get the least significant bit
    inputInt >>= 1;                 // Right shift inputInt by 1 to move to the next bit
  }
}

void HCM_alarm(String &return_data, String &in_data, String &d_id, String &p_id, String &c_id, String &o_id, String &time_stamp) {
  const int total_alarms = 60;
  const int additional_element_size = 50;

  const char *Alarm_Data[total_alarms][2] = {
    { "Rectal Probe Fail", "High Priority" }, //0
    { "Mattress Too Cold", "Medium Priority" }, //1
    { "Mattress Probe Fail", "Medium Priority" }, //2
    { "Baby Skin - Over Temp", "Medium Priority" },//3  ------------
    { "Water Flow Fail", "High Priority" },//4
    { "System Error(P)", "High Priority" },//5
    { "System Error(F)", "High Priority" },//6
    { "System Error(M)", "High Priority" }, //7     // 8th bit
    { "Power Fail", "Medium Priority" },//8
    { "Baby Core Temp Low ", "Medium Priority" },//9
    { "Baby Core Temp High", "High Priority" },//10
    { "Skin Probe Fail", "Medium Priority" },//11     ---------------
    { "Mattress Over  Temp", "Medium Priority" },
    { "Baby Core Over Temp", "Medium Priority" },
    { "Baby Core Temp Too Low", "Medium Priority" },
    { "Rectal Probe Dislocated", "Medium Priority" },  //16th bit
    { "Battery Low", "Medium Priority" },
    { "UNEXPECTED SYSTEM RESET", "Medium Priority" },
    { "CONTROL CARD ERROR", "High Priority" },
    { "Therapy Completed", "Medium Priority" },
    { "Skin Probe Dislocated", "Medium Priority" },
    { "Mattress Temp High", "Low Priority" },
    { "Mattress Temp Low", "Medium Priority" },
    { "Connect To Mains ", "High Priority" },  //24th bit
    { "NO ALARM", "Medium Priority" },
    { "NEW PATIENT ADDED", "Medium Priority" },
    { "WARM MODE", "Medium Priority" },
    { "COOL MODE", "Medium Priority" },
    { "MATTRESS MODE", "Medium Priority" },
    { "SERVO MODE", "Medium Priority" },
    { "Water Level Sensor Fai", "Medium Priority" },
    { "Water Level Low", "Medium Priority" },  //32nd bit
  };


  DynamicJsonDocument alarm(4000);
  alarm["status"] = "completed";
  alarm["resourceType"] = "Communication";
  alarm["id"] = c_id;

  JsonObject category_0 = alarm["category"].createNestedObject();

  JsonObject category_0_coding_0 = category_0["coding"].createNestedObject();
  category_0_coding_0["system"] = "http://acme.org/messagetypes";
  category_0_coding_0["code"] = "Alert";
  category_0["text"] = "Alert";
  alarm["subject"]["reference"] = "Patient/" + p_id;
  alarm["sent"] = time_stamp;
  alarm["sender"]["reference"] = "Device/" + d_id;
  alarm["payload"][0]["contentReference"]["display"] = "Alarm details";

  JsonArray extension = alarm.createNestedArray("extension");

  JsonObject extension_0 = extension.createNestedObject();
  extension_0["url"] = "http://example.com/fhir/extensions#alarmType";
  JsonArray extension_0_valueCodeableConcept_coding = extension_0["valueCodeableConcept"].createNestedArray("coding");

  JsonObject extension_1 = extension.createNestedObject();
  extension_1["url"] = "http://example.com/fhir/extensions#alarmPriority";
  JsonArray extension_1_valueCodeableConcept_coding = extension_1["valueCodeableConcept"].createNestedArray("coding");

  String dlog_arr[5];
  split_str(in_data, dlog_arr, ",");

  String binaryString = "";
  for (int i = 0; i < 4; i++) {
    String binaryRep = String(dlog_arr[i].toInt(), BIN);
    while (binaryRep.length() < 8) {
      binaryRep = "0" + binaryRep;
    }
    binaryString += binaryRep;
  }

  // Iterate through each bit of the binary string
  for (int i = 0; i < binaryString.length(); i++) {
    if (binaryString[i] == '1') {
      int index = i;
      String Alarm_name = Alarm_Data[index][0];
      String Alarm_priority = Alarm_Data[index][1];

      // Create JSON objects for alarm type and priority
      JsonObject extension_0_coding_obj = extension_0_valueCodeableConcept_coding.createNestedObject();
      JsonArray extension_0_coding_arr = alarm["extension_0_coding"];

      extension_0_coding_obj["system"] = "http://example.com/fhir/alarm-codes";
      extension_0_coding_obj["code"] = String(index);
      extension_0_coding_obj["display"] = Alarm_name;

      extension_0_coding_arr.add(extension_0_coding_obj);

      JsonObject extension_1_coding_obj = extension_1_valueCodeableConcept_coding.createNestedObject();
      JsonArray extension_1_coding_arr = alarm["extension_1_coding"];

      extension_1_coding_obj["system"] = "http://example.com/fhir/alarm-prioritiess";
      extension_1_coding_obj["code"] = Alarm_priority;
      extension_1_coding_obj["display"] = Alarm_priority;

      extension_1_coding_arr.add(extension_1_coding_obj);
    }
  }

  serializeJson(alarm, return_data);
}
