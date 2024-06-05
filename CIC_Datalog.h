int cic_alarm(String &return_data, String &in_data, String &d_id, String &p_id, String &c_id, String &o_id, String &time_stamp) {
  const int total_alarms = 60;
  const int additional_element_size = 60;
  const char *Alarm_Data[total_alarms][2] = {
    { "Skin1 Temperature High", "Low Priority" },    // 0
    { "Skin1 Temperature Low", "Low Priority" },    // 1
    { "Peri. Temperature High", "Low Priority" },    // 2
    { "Peri. Temperature Low", "Low Priority" },    // 3
    { "Air Temperature High", "Low Priority" },      // 4
    { "Air Temperature Low", "Low Priority" },      // 5
    { "Oxygen High", "Low Priority" },               // 6
    { "Oxygen Low", "Low Priority" },               // 7
    { "Humidity High", "Low Priority" },             // 8
    { "Humidity Low", "Low Priority" },             // 9
    { "Heater Fail", "High Priority" },             // 10     <>
    { "Water Heater Fail", "Low Priority" },        // 11
    { "Fan Fail", "High Priority" },                 // 12
    { "Power Fail", "High Priority" },              // 13
    { "Battery Low", "High Priority" },              // 14
    { "Sensor Board Failed", "Low Priority" },      // 15
    { "Left Door Open", "Low Priority" },           // 16
    { "Right Door Open", "Low Priority" },          // 17
    { "Oxygen Valve Fail", "Low Priority" },        // 18
    { "Water Probe Fail", "Low Priority" },         // 19
    { "Tilt Actuator Fail", "Low Priority" },       // 20
    { "Air Filter Fail", "Low Priority" },          // 21
    { "Water Reservoir Empty", "Low Priority" },    // 22
    { "Check Display Cable", "Low Priority" },      // 23
    { "Air Over Temperature", "Low Priority" },     // 24
    { "Air Probe Fail", "Low Priority" },           // 25
    { "Air Probe Fail", "Low Priority" },           // 26
    { "Skin Probe Fail", "High Priority" },         // 27
    { "Peri. Probe Fail", "Low Priority" },        // 28
    { "Humidity Sensor Fail", "Low Priority" },     // 29
    { "Oxygen Sensor1 Fail", "Low Priority" },      // 30
    { "Oxygen Sensor2 Fail", "Low Priority" },      // 31
    { "Sensor Box1 Displaced", "Low Priority" },    // 32
    { "Sensor Box2 Displaced", "Low Priority" },    // 33
    { "BWS OverLoad", "Low Priority" },             // 34
    { "BWS Connection Fail", "Low Priority" },      // 35
    { "BWS Sensor1 Fail", "Low Priority" },         // 36
    { "BWS Sensor2 Fail", "Low Priority" },         // 37
    { "BWS Sensor3 Fail", "Low Priority" },         // 38
    { "BWS Sensor4 Fail", "Low Priority" },         // 39
    { "Baby Over Temperature", "High Priority" },   // 40
    { "Sensor Board Failed", "Low Priority" },      // 41
    { "Air Probe Fail", "Low Priority" },           // 42
    { "Blower speed Low", "Low Priority" },         // 43
    { "Blower speed Low", "Low Priority" },         // 44
    { "Sensor Memory Fail", "High Priority" },       // 45
    { "Control Memory Fail", "High Priority" },      // 46
    { "Sensor Fan Fail", "Low Priority" },          // 47
    { "Main heater TRIAC fail", "Low Priority" },   // 48
    { "Water heater TRIAC fail", "Low Priority" },  // 49 rtd alm  //Water heater hot to changed into water heater dry 28062023
    { "Temp Probe 4 Fail", "Low Priority" },        // 50 //This message will be displayed from a separate group of flags
    { "Temp Probe 5 Fail", "Low Priority" },
    { "Temp Probe 6 Fail", "Low Priority" },
    { "Skin Temp. Difference", "Low Priority" },
    { "Heater Box Misplaced", "Low Priority" },
    { "Probe Dislocated", "Low Priority" },
    { "Control Board Removed", "High Priority" },
    { "Manual Mode Alert", "Low Priority" },
    { "Power Down Alert", "Low Priority" },
  };


  int index;
  String Index;
  String priority;
  String display;
  String name;
  String dlog_arr[50];
  int count = split_str(in_data, dlog_arr, ":");
  Serial.println(count);
  if (count == 0) {
    return 1;
  }
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


  for (int i =0; i < count; i++) {
    String temp = dlog_arr[i];
    index = temp.toInt();
    Index = String(index);
    name = Alarm_Data[index][0];

    JsonObject extension_0_coding_obj = extension_0_valueCodeableConcept_coding.createNestedObject();
    JsonArray extension_0_coding_arr = alarm["extension_0_coding"];

    extension_0_coding_obj["system"] = "http://example.com/fhir/alarm-codes";
    extension_0_coding_obj["code"] = Index;
    extension_0_coding_obj["display"] = name;

    extension_0_coding_arr.add(extension_0_coding_obj);

    JsonObject extension_1_coding_obj = extension_1_valueCodeableConcept_coding.createNestedObject();
    JsonArray extension_1_coding_arr = alarm["extension_1_coding"];

    extension_1_coding_obj["system"] = "http://example.com/fhir/alarm-prioritiess";
    extension_1_coding_obj["code"] = Alarm_Data[index][1];
    extension_1_coding_obj["display"] = Alarm_Data[index][1];

    extension_1_coding_arr.add(extension_1_coding_obj);
  }
  serializeJson(alarm, return_data);
  return 100;
}

void cic_data(String &return_data, String &in_data, String &d_id, String &p_id, String &c_id, String &o_id, String &time_stamp) {
  String dlog_arr[50];
  split_str(in_data, dlog_arr, ",");

  DynamicJsonDocument cic_log(4096);

  cic_log["id"] = o_id;
  cic_log["resourceType"] = "Observation";
  cic_log["status"] = "final";
  cic_log["identifier"][0]["value"] = "PMS-CIC";
  cic_log["subject"]["reference"] = "Patient/" + p_id;
  cic_log["device"]["reference"] = "Device/" + d_id;
  JsonObject code = cic_log.createNestedObject("code");

  JsonObject code_coding_0 = code["coding"].createNestedObject();
  code_coding_0["system"] = "http://loinc.org";
  code_coding_0["code"] = "85353-1";
  code_coding_0["display"] = "Vital signs, weight, height, head circumference, oximetry, BMI, and BSA panel";
  code["text"] = "Vital signs panel";

  JsonObject category_0_coding_0 = cic_log["category"][0]["coding"].createNestedObject();
  category_0_coding_0["system"] = "http://terminology.hl7.org/CodeSystem/observation-category";
  category_0_coding_0["code"] = "data-log";
  category_0_coding_0["display"] = "Data Log";

  cic_log["effectiveDateTime"] = time_stamp;


  JsonArray component = cic_log.createNestedArray("component");

  if (dlog_arr[0] == "BABY") {

    JsonObject component_0 = component.createNestedObject();

    JsonObject component_0_code = component_0.createNestedObject("code");

    JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
    component_0_code_coding_0["system"] = "http://loinc.org";
    component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
    component_0_code_coding_0["display"] = "MODE";
    component_0_code["text"] = "MODE";

    JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
    component_0_valueQuantity["value"] = 1;
    component_0_valueQuantity["unit"] = "BABY";
    component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_0_valueQuantity["code"] = "BABY";

    JsonObject component_1 = component.createNestedObject();

    JsonObject component_1_code = component_1.createNestedObject("code");

    JsonObject component_1_code_coding_0 = component_1_code["coding"].createNestedObject();
    component_1_code_coding_0["system"] = "http://loinc.org";
    component_1_code_coding_0["code"] = "75951-4";
    component_1_code_coding_0["display"] = "Set Skin Temp ";
    component_1_code["text"] = "Set Skin Temp";

    JsonObject component_1_valueQuantity = component_1.createNestedObject("valueQuantity");
    component_1_valueQuantity["value"] = dlog_arr[1].toInt();
    component_1_valueQuantity["unit"] = "C°";
    component_1_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_1_valueQuantity["code"] = "C°";

  }

  else if (dlog_arr[0] == "PREWARM") {

    JsonObject component_0 = component.createNestedObject();

    JsonObject component_0_code = component_0.createNestedObject("code");

    JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
    component_0_code_coding_0["system"] = "http://loinc.org";
    component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
    component_0_code_coding_0["display"] = "MODE";
    component_0_code["text"] = "MODE";

    JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
    component_0_valueQuantity["value"] = 2;
    component_0_valueQuantity["unit"] = "PREWARM";
    component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_0_valueQuantity["code"] = "PREWARM";

    JsonObject component_1 = component.createNestedObject();

    JsonObject component_1_code = component_1.createNestedObject("code");

    JsonObject component_1_code_coding_0 = component_1_code["coding"].createNestedObject();
    component_1_code_coding_0["system"] = "http://loinc.org";
    component_1_code_coding_0["code"] = "75951-4";
    component_1_code_coding_0["display"] = "Set Heater";
    component_1_code["text"] = "Set Heater";

    JsonObject component_1_valueQuantity = component_1.createNestedObject("valueQuantity");
    component_1_valueQuantity["value"] = dlog_arr[1].toInt();
    component_1_valueQuantity["unit"] = "%";
    component_1_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_1_valueQuantity["code"] = "%";
  } else if (dlog_arr[0] == "MANUAL") {
    JsonObject component_0 = component.createNestedObject();

    JsonObject component_0_code = component_0.createNestedObject("code");

    JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
    component_0_code_coding_0["system"] = "http://loinc.org";
    component_0_code_coding_0["code"] = "MMMMMM";
    component_0_code_coding_0["display"] = "MODE";
    component_0_code["text"] = "MODE";

    JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
    component_0_valueQuantity["value"] = 3;
    component_0_valueQuantity["unit"] = "MANUAL";
    component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_0_valueQuantity["code"] = "MANUAL";

    JsonObject component_1 = component.createNestedObject();

    JsonObject component_1_code = component_1.createNestedObject("code");

    JsonObject component_1_code_coding_0 = component_1_code["coding"].createNestedObject();
    component_1_code_coding_0["system"] = "http://loinc.org";
    component_1_code_coding_0["code"] = "75951-4";
    component_1_code_coding_0["display"] = "Set Heater";
    component_1_code["text"] = "Set Heater";

    JsonObject component_1_valueQuantity = component_1.createNestedObject("valueQuantity");
    component_1_valueQuantity["value"] = dlog_arr[1].toInt();
    component_1_valueQuantity["unit"] = "%";
    component_1_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_1_valueQuantity["code"] = "%";
  }

  JsonObject component_2 = component.createNestedObject();

  JsonObject component_2_code = component_2.createNestedObject("code");

  JsonObject component_2_code_coding_0 = component_2_code["coding"].createNestedObject();
  component_2_code_coding_0["system"] = "http://loinc.org";
  component_2_code_coding_0["code"] = "60839-8";
  component_2_code_coding_0["display"] = "Measured Skin Temp 1";
  component_2_code["text"] = "Measured Skin Temp 1";

  JsonObject component_2_valueQuantity = component_2.createNestedObject("valueQuantity");
  component_2_valueQuantity["value"] = dlog_arr[2].toFloat();
  component_2_valueQuantity["unit"] = "C°";
  component_2_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_2_valueQuantity["code"] = "C°";

  JsonObject component_3 = component.createNestedObject();

  JsonObject component_3_code = component_3.createNestedObject("code");

  JsonObject component_3_code_coding_0 = component_3_code["coding"].createNestedObject();
  component_3_code_coding_0["system"] = "http://loinc.org";
  component_3_code_coding_0["code"] = "75927-4";
  component_3_code_coding_0["display"] = "Heater Level";
  component_3_code["text"] = "Heater Level";

  JsonObject component_3_valueQuantity = component_3.createNestedObject("valueQuantity");
  component_3_valueQuantity["value"] = dlog_arr[5].toInt();
  component_3_valueQuantity["unit"] = "%";
  component_3_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_3_valueQuantity["code"] = "%";


  if ((dlog_arr[6] != "0" && dlog_arr[7] != "0" && dlog_arr[8] != "0" && dlog_arr[10] != "0") || (dlog_arr[6] != "000" && dlog_arr[7] != "000" && dlog_arr[8] != "000" && dlog_arr[10] != "000") || (dlog_arr[6].toInt() != 0 && dlog_arr[7].toInt() != 0 && dlog_arr[8].toInt() != 0 && dlog_arr[10].toInt() != 0)) {  // Changed stuff
    JsonObject component_4 = component.createNestedObject();

    JsonObject component_4_code = component_4.createNestedObject("code");

    JsonObject component_4_code_coding_0 = component_4_code["coding"].createNestedObject();
    component_4_code_coding_0["system"] = "http://loinc.org";
    component_4_code_coding_0["code"] = "20564-1";
    component_4_code_coding_0["display"] = "SpO2";
    component_4_code["text"] = "SpO2";

    JsonObject component_4_valueQuantity = component_4.createNestedObject("valueQuantity");
    component_4_valueQuantity["value"] = dlog_arr[6].toInt();
    component_4_valueQuantity["unit"] = "%";
    component_4_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_4_valueQuantity["code"] = "%";

    JsonObject component_5 = component.createNestedObject();

    JsonObject component_5_code = component_5.createNestedObject("code");

    JsonObject component_5_code_coding_0 = component_5_code["coding"].createNestedObject();
    component_5_code_coding_0["system"] = "http://loinc.org";
    component_5_code_coding_0["code"] = "8867-4";
    component_5_code_coding_0["display"] = "Pulse Rate";
    component_5_code["text"] = "Pulse Rate";

    JsonObject component_5_valueQuantity = component_5.createNestedObject("valueQuantity");
    component_5_valueQuantity["value"] = dlog_arr[7].toInt();
    component_5_valueQuantity["unit"] = "BPM";
    component_5_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_5_valueQuantity["code"] = "BPM";

    JsonObject component_6 = component.createNestedObject();

    JsonObject component_6_code = component_6.createNestedObject("code");

    JsonObject component_6_code_coding_0 = component_6_code["coding"].createNestedObject();
    component_6_code_coding_0["system"] = "http://loinc.org";
    component_6_code_coding_0["code"] = "73794-0";
    component_6_code_coding_0["display"] = "PI";
    component_6_code["text"] = "PI";

    JsonObject component_6_valueQuantity = component_6.createNestedObject("valueQuantity");
    component_6_valueQuantity["value"] = dlog_arr[8].toInt();
    component_6_valueQuantity["unit"] = "%";
    component_6_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_6_valueQuantity["code"] = "%";

    JsonObject component_7 = component.createNestedObject();

    JsonObject component_7_code = component_7.createNestedObject("code");

    JsonObject component_7_code_coding_0 = component_7_code["coding"].createNestedObject();
    component_7_code_coding_0["system"] = "http://loinc.org";
    component_7_code_coding_0["code"] = "73793-2";
    component_7_code_coding_0["display"] = "SIQ";
    component_7_code["text"] = "SIQ";

    JsonObject component_7_valueQuantity = component_7.createNestedObject("valueQuantity");
    component_7_valueQuantity["value"] = dlog_arr[10].toInt();
    component_7_valueQuantity["unit"] = "NONE";
    component_7_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_7_valueQuantity["code"] = "NONE";

    JsonObject component_8 = component.createNestedObject();

    JsonObject component_8_code = component_8.createNestedObject("code");

    JsonObject component_8_code_coding_0 = component_8_code["coding"].createNestedObject();
    component_8_code_coding_0["system"] = "http://loinc.org";
    component_8_code_coding_0["code"] = "XXXXXXX";
    component_8_code_coding_0["display"] = "PVI";
    component_8_code["text"] = "PVI";

    JsonObject component_8_valueQuantity = component_8.createNestedObject("valueQuantity");
    component_8_valueQuantity["value"] = dlog_arr[9].toInt();
    component_8_valueQuantity["unit"] = "%";
    component_8_valueQuantity["system"] = "http://unitsofmeasure.org";
    component_8_valueQuantity["code"] = "%";
  }
  JsonObject component_9 = component.createNestedObject();

  JsonObject component_9_code = component_9.createNestedObject("code");

  JsonObject component_9_code_coding_0 = component_9_code["coding"].createNestedObject();
  component_9_code_coding_0["system"] = "http://loinc.org";
  component_9_code_coding_0["code"] = "60839-8";
  component_9_code_coding_0["display"] = "Measured Skin Temp 2";
  component_9_code["text"] = "Measured Skin Temp 2";

  JsonObject component_9_valueQuantity = component_9.createNestedObject("valueQuantity");
  component_9_valueQuantity["value"] = dlog_arr[3].toFloat();
  component_9_valueQuantity["unit"] = "C°";
  component_9_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_9_valueQuantity["code"] = "C°";

  JsonObject component_10 = component.createNestedObject();

  JsonObject component_10_code = component_10.createNestedObject("code");

  JsonObject component_10_code_coding_0 = component_10_code["coding"].createNestedObject();
  component_10_code_coding_0["system"] = "http://loinc.org";
  component_10_code_coding_0["code"] = "29463-7";
  component_10_code_coding_0["display"] = "Measure Weigh";
  component_10_code["text"] = "Measure Weigh";

  JsonObject component_10_valueQuantity = component_10.createNestedObject("valueQuantity");
  component_10_valueQuantity["value"] = dlog_arr[4].toInt();
  component_10_valueQuantity["unit"] = "g";
  component_10_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_10_valueQuantity["code"] = "[g]";

  serializeJson(cic_log, return_data);
}
