String out = "";
String Index;
void intTo8BitArray(int inputInt, int *outputArray) {
  for (int i = 7; i >= 0; i--) {
    outputArray[i] = inputInt & 1;  // Get the least significant bit
    inputInt >>= 1;                 // Right shift inputInt by 1 to move to the next bit
  }
}

void SVAAS_alarm(String &return_data, String &in_data, String &d_id, String &p_id, String &c_id, String &o_id, String &time_stamp) {
  const int total_alarms = 60;
  const int additional_element_size = 50;

  const char *Alarm_Data[total_alarms][2] = {
    { "O2 Ip Pressure Low", "Medium Priority" },
    { "No O2 Supply", "Medium Priority" },
    { "Air Ip Pressure High", "Medium Priority" },
    { "Air Ip Pressure Low", "Medium Priority" },
    { "No Air Supply", "Medium Priority" },
    { "Power Fail", "Medium Priority" },
    { "Battery Low", "Medium Priority" },
    { "Connect Mains", "Medium Priority" },
    { "NO ALARM", "Medium Priority" },  // 8th bit
    { "Patient Disconnected", "Medium Priority" },
    { "Flow High ", "Medium Priority" },
    { "Flow Low ", "High Priority" },
    { "FiO2 High", "High Priority" },
    { "FiO2 Low", "High Priority" },
    { "CPAP High", "High Priority" },
    { "CPAP Low", "High Priority" },
    // { "New Patient Connected", "High Priority"},
    { "O2 Ip Pressure High", "Medium Priority" },  //16th bit
    { "Air Valve Fail", "High Priority" },
    { "Flow Path Blocked", "High Priority" },
    { "O2 Flow High", "Medium Priority" },
    { "O2 Flow Low", "Medium Priority" },
    { "Air Flow High", "High Priority" },
    { "Air Flow Low", "Low Priority" },
    { "High Pressure", "High Priority" },
    { "Apnea Detected", "High Priority" },
    // { "Patient Disconnected", "Medium Priority" },  //24th bit
    { "Sensor Fail", "Medium Priority" },
    { "Sensor Pass", "Medium Priority" },
    { "Suction Sensor Fail", "Medium Priority" },
    { "Flow Pressure Sensor Fail", "Medium Priority" },
    { "CPAP Sensor Fail", "Medium Priority" },
    { "O2 Flow Sensor Fail", "Medium Priority" },
    { "Air Flow Sensor Fail", "Medium Priority" },
    { "O2 Valve Fail", "High Priority" },
    // { "Air Valve Fail", "High Priority" },  //32nd bit
    { "Both Valve Ok", "High Priority" },
    { "FiO2 Test Fail", "High Priority" },
    { "FiO2 Test Pass", "Low Priority" },
    { "Battery Test Fail", "Low Priority" },
    { "Battery Test Pass", "Low Priority" },
    { "Ac Test Fail", "Low Priority" },
    { "Ac Test Pass", "Low Priority" },
    { "Self Test Done", "Low Priority" },
    // { "Sensor Fail", "Medium Priority" },  //40th bit
    { "SpO2 High", "High Priority" },
    { "Pulse Rate Low", "Medium Priority" },
    { "Pulse Rate High", "High Priority" },
    { "Flow Calibration Done", "Low Priority" },
    { "Calibration Done", "Low Priority" },
    { "Check O2 Supply", "High Priority" },
    { "Check Air Supply", "Medium Priority" },
    { "Valve Test Fail", "High Priority" },
    // { "Both Valve Ok", "High Priority" },  //48th bit
    { "Calibration Failed", "High Priority" },
    { "New Patient Connected", "High Priority"},
    { "SpO2 Low", "High Priority" },
    // { "SpO2 High", "High Priority" },  //51th bit
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

  String dlog_arr[50];
  split_str(in_data, dlog_arr, ",");
  int binaryArray[8];
  for (int i = 37; i <= 43; i++) {
    intTo8BitArray(dlog_arr[i].toInt(), binaryArray);
    for (int i = 0; i < 8; i++) {
      out.concat(binaryArray[i]);
    }
    for (int i = 0; i < 8; i++) {
      if (out[i] == '1') {
        int index = i;
        Index = String(index);
        String Alarm_name = Alarm_Data[index][0];
        String Alarm_priority = Alarm_Data[index][1];
        JsonObject extension_0_coding_obj = extension_0_valueCodeableConcept_coding.createNestedObject();
        JsonArray extension_0_coding_arr = alarm["extension_0_coding"];

        extension_0_coding_obj["system"] = "http://example.com/fhir/alarm-codes";
        extension_0_coding_obj["code"] = Index;
        extension_0_coding_obj["display"] = Alarm_name;

        extension_0_coding_arr.add(extension_0_coding_obj);

        JsonObject extension_1_coding_obj = extension_1_valueCodeableConcept_coding.createNestedObject();
        JsonArray extension_1_coding_arr = alarm["extension_1_coding"];

        extension_1_coding_obj["system"] = "http://example.com/fhir/alarm-prioritiess";
        extension_1_coding_obj["code"] = Alarm_priority;
        extension_1_coding_obj["display"] = Alarm_priority;

        extension_1_coding_arr.add(extension_1_coding_obj);
      } else if (out[i] == '0') {
      }
    }
    out = "";
  }
  serializeJson(alarm, return_data);
}

void SVAAS_data(String &return_data, String &in_data, String &d_id, String &p_id, String &c_id, String &o_id, String &time_stamp) {
  String dlog_arr[50];
  split_str(in_data, dlog_arr, ",");
  DynamicJsonDocument inc_log(4096);

  inc_log["id"] = o_id;
  inc_log["resourceType"] = "Observation";
  inc_log["status"] = "final";
  inc_log["identifier"][0]["value"] = "PMS-SVAAS";
  inc_log["subject"]["reference"] = "Patient/" + p_id;
  inc_log["effectiveDateTime"] = time_stamp;

  JsonObject code = inc_log.createNestedObject("code");

  JsonObject code_coding_0 = code["coding"].createNestedObject();
  code_coding_0["system"] = "http://loinc.org";
  code_coding_0["code"] = "85353-1";
  code_coding_0["display"] = "Vital signs, weight, height, head circumference, oximetry, BMI, and BSA panel";
  code["text"] = "Vital signs panel";

  JsonObject category_0_coding_0 = inc_log["category"][0]["coding"].createNestedObject();
  category_0_coding_0["system"] = "http://terminology.hl7.org/CodeSystem/observation-category";
  category_0_coding_0["code"] = "data-log";
  category_0_coding_0["display"] = "Data Log";

  JsonArray component = inc_log.createNestedArray("component");
  long modeValue = dlog_arr[0].toInt();
  String mode = String(modeValue);
  switch (modeValue) {
    case 1:
      {
        JsonObject component_0 = component.createNestedObject();

        JsonObject component_0_code = component_0.createNestedObject("code");

        JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
        component_0_code_coding_0["system"] = "http://loinc.org";
        component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
        component_0_code_coding_0["display"] = "MODE";
        component_0_code["text"] = "MODE";

        JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
        component_0_valueQuantity["value"] = 1;
        component_0_valueQuantity["unit"] = "BUBBLE CPAP";
        component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
        component_0_valueQuantity["code"] = "BUBBLE CPAP";
        break;
      }
    case 2:
      {
        JsonObject component_0 = component.createNestedObject();

        JsonObject component_0_code = component_0.createNestedObject("code");

        JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
        component_0_code_coding_0["system"] = "http://loinc.org";
        component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
        component_0_code_coding_0["display"] = "MODE";
        component_0_code["text"] = "MODE";

        JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
        component_0_valueQuantity["value"] = 2;
        component_0_valueQuantity["unit"] = "FLOW CPAP 1";
        component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
        component_0_valueQuantity["code"] = "FLOW CPAP 1";
        break;
      }
    case 3:
      {
        JsonObject component_0 = component.createNestedObject();

        JsonObject component_0_code = component_0.createNestedObject("code");

        JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
        component_0_code_coding_0["system"] = "http://loinc.org";
        component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
        component_0_code_coding_0["display"] = "MODE";
        component_0_code["text"] = "MODE";

        JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
        component_0_valueQuantity["value"] = 3;
        component_0_valueQuantity["unit"] = "FLOW CPAP 2";
        component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
        component_0_valueQuantity["code"] = "FLOW CPAP 2";
        break;
      }
    case 4:
      {
        JsonObject component_0 = component.createNestedObject();

        JsonObject component_0_code = component_0.createNestedObject("code");

        JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
        component_0_code_coding_0["system"] = "http://loinc.org";
        component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
        component_0_code_coding_0["display"] = "MODE";
        component_0_code["text"] = "MODE";

        JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
        component_0_valueQuantity["value"] = 4;
        component_0_valueQuantity["unit"] = "PULSE OXIMETER";
        component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
        component_0_valueQuantity["code"] = "PULSE OXIMETER";
        break;
      }
    case 5:
      {
        JsonObject component_0 = component.createNestedObject();

        JsonObject component_0_code = component_0.createNestedObject("code");

        JsonObject component_0_code_coding_0 = component_0_code["coding"].createNestedObject();
        component_0_code_coding_0["system"] = "http://loinc.org";
        component_0_code_coding_0["code"] = "urn:ietf:rfc:3986";
        component_0_code_coding_0["display"] = "MODE";
        component_0_code["text"] = "MODE";

        JsonObject component_0_valueQuantity = component_0.createNestedObject("valueQuantity");
        component_0_valueQuantity["value"] = 5;
        component_0_valueQuantity["unit"] = "HIGH FLOW";
        component_0_valueQuantity["system"] = "http://unitsofmeasure.org";
        component_0_valueQuantity["code"] = "HIGH FLOW";
        break;
      }
  }

  JsonObject component_1 = component.createNestedObject();

  JsonObject component_1_code = component_1.createNestedObject("code");

  JsonObject component_1_code_coding_0 = component_1_code["coding"].createNestedObject();
  component_1_code_coding_0["system"] = "http://loinc.org";
  component_1_code_coding_0["code"] = "XXXXXXX";
  component_1_code_coding_0["display"] = "Set FiO2";
  component_1_code["text"] = "Set FiO2";

  JsonObject component_1_valueQuantity = component_1.createNestedObject("valueQuantity");
  component_1_valueQuantity["value"] = dlog_arr[8].toInt();
  component_1_valueQuantity["unit"] = "%";
  component_1_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_1_valueQuantity["code"] = "%";

  JsonObject component_2 = component.createNestedObject();

  JsonObject component_2_code = component_2.createNestedObject("code");

  JsonObject component_2_code_coding_0 = component_2_code["coding"].createNestedObject();
  component_2_code_coding_0["system"] = "http://loinc.org";
  component_2_code_coding_0["code"] = "60839-8";
  component_2_code_coding_0["display"] = "Total Delivered flow";
  component_2_code["text"] = "Total Delivered flow";

  JsonObject component_2_valueQuantity = component_2.createNestedObject("valueQuantity");
  component_2_valueQuantity["value"] = dlog_arr[3].toFloat();
  component_2_valueQuantity["unit"] = "LPM";
  component_2_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_2_valueQuantity["code"] = "LPM";

  JsonObject component_3 = component.createNestedObject();

  JsonObject component_3_code = component_3.createNestedObject("code");

  JsonObject component_3_code_coding_0 = component_3_code["coding"].createNestedObject();
  component_3_code_coding_0["system"] = "http://loinc.org";
  component_3_code_coding_0["code"] = "60839-8";
  component_3_code_coding_0["display"] = "Current FiO2 Flow";
  component_3_code["text"] = "Current FiO2 Flow";

  JsonObject component_3_valueQuantity = component_3.createNestedObject("valueQuantity");
  component_3_valueQuantity["value"] = dlog_arr[7].toFloat();
  component_3_valueQuantity["unit"] = "LPM";
  component_3_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_3_valueQuantity["code"] = "LPM";

  JsonObject component_4 = component.createNestedObject();

  JsonObject component_4_code = component_4.createNestedObject("code");

  JsonObject component_4_code_coding_0 = component_4_code["coding"].createNestedObject();
  component_4_code_coding_0["system"] = "http://loinc.org";
  component_4_code_coding_0["code"] = "XXXXXXX";
  component_4_code_coding_0["display"] = "SPO2";
  component_4_code["text"] = "SPO2";

  JsonObject component_4_valueQuantity = component_4.createNestedObject("valueQuantity");
  component_4_valueQuantity["value"] = dlog_arr[31].toInt();
  component_4_valueQuantity["unit"] = "%";
  component_4_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_4_valueQuantity["code"] = "%";


  JsonObject component_5 = component.createNestedObject();

  JsonObject component_5_code = component_5.createNestedObject("code");

  JsonObject component_5_code_coding_0 = component_5_code["coding"].createNestedObject();
  component_5_code_coding_0["system"] = "http://loinc.org";
  component_5_code_coding_0["code"] = "75951-4";
  component_5_code_coding_0["display"] = "System Sub-Mode";
  component_5_code["text"] = "System Sub-Mode";

  JsonObject component_5_valueQuantity = component_5.createNestedObject("valueQuantity");
  component_5_valueQuantity["value"] = dlog_arr[1].toInt();
  component_5_valueQuantity["unit"] = "NONE";
  component_5_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_5_valueQuantity["code"] = "NONE";

  JsonObject component_6 = component.createNestedObject();

  JsonObject component_6_code = component_6.createNestedObject("code");

  JsonObject component_6_code_coding_0 = component_6_code["coding"].createNestedObject();
  component_6_code_coding_0["system"] = "http://loinc.org";
  component_6_code_coding_0["code"] = "75927-4";
  component_6_code_coding_0["display"] = "CPAP Pressure";
  component_6_code["text"] = "CPAP Pressure";

  JsonObject component_6_valueQuantity = component_6.createNestedObject("valueQuantity");
  component_6_valueQuantity["value"] = dlog_arr[11].toInt();
  component_6_valueQuantity["unit"] = "CmH2O";
  component_6_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_6_valueQuantity["code"] = "CmH2O";


  JsonObject component_7 = component.createNestedObject();

  JsonObject component_7_code = component_7.createNestedObject("code");

  JsonObject component_7_code_coding_0 = component_7_code["coding"].createNestedObject();
  component_7_code_coding_0["system"] = "http://loinc.org";
  component_7_code_coding_0["code"] = "XXXXXXX";
  component_7_code_coding_0["display"] = "Pulse Rate";
  component_7_code["text"] = "Pulse Rate";

  JsonObject component_7_valueQuantity = component_7.createNestedObject("valueQuantity");
  component_7_valueQuantity["value"] = dlog_arr[30].toInt();
  component_7_valueQuantity["unit"] = "BPM";
  component_7_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_7_valueQuantity["code"] = "BPM";

  JsonObject component_8 = component.createNestedObject();

  JsonObject component_8_code = component_8.createNestedObject("code");

  JsonObject component_8_code_coding_0 = component_8_code["coding"].createNestedObject();
  component_8_code_coding_0["system"] = "http://loinc.org";
  component_8_code_coding_0["code"] = "29463-7";
  component_8_code_coding_0["display"] = "Set Flow";
  component_8_code["text"] = "Set Flow";

  JsonObject component_8_valueQuantity = component_8.createNestedObject("valueQuantity");
  component_8_valueQuantity["value"] = dlog_arr[2].toInt();
  component_8_valueQuantity["unit"] = "LPM";
  component_8_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_8_valueQuantity["code"] = "LPM";

  JsonObject component_9 = component.createNestedObject();

  JsonObject component_9_code = component_9.createNestedObject("code");

  JsonObject component_9_code_coding_0 = component_9_code["coding"].createNestedObject();
  component_9_code_coding_0["system"] = "http://loinc.org";
  component_9_code_coding_0["code"] = "8867-4";
  component_9_code_coding_0["display"] = "Current Air Flow";
  component_9_code["text"] = "Current Air Flow";

  JsonObject component_9_valueQuantity = component_9.createNestedObject("valueQuantity");
  component_9_valueQuantity["value"] = dlog_arr[5].toInt();
  component_9_valueQuantity["unit"] = "LPM";
  component_9_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_9_valueQuantity["code"] = "LPM";

  JsonObject component_11 = component.createNestedObject();

  JsonObject component_11_code = component_11.createNestedObject("code");

  JsonObject component_11_code_coding_0 = component_11_code["coding"].createNestedObject();
  component_11_code_coding_0["system"] = "http://loinc.org";
  component_11_code_coding_0["code"] = "73793-2";
  component_11_code_coding_0["display"] = "Current Initial Pressure";
  component_11_code["text"] = "Current Initial Pressure";

  JsonObject component_11_valueQuantity = component_11.createNestedObject("valueQuantity");
  component_11_valueQuantity["value"] = dlog_arr[12].toInt();
  component_11_valueQuantity["unit"] = "Bar";
  component_11_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_11_valueQuantity["code"] = "Bar";

  JsonObject component_12 = component.createNestedObject();

  JsonObject component_12_code = component_12.createNestedObject("code");

  JsonObject component_12_code_coding_0 = component_12_code["coding"].createNestedObject();
  component_12_code_coding_0["system"] = "http://loinc.org";
  component_12_code_coding_0["code"] = "73793-2";
  component_12_code_coding_0["display"] = " Current Apnea pressure";
  component_12_code["text"] = " Current Apnea pressure";

  JsonObject component_12_valueQuantity = component_12.createNestedObject("valueQuantity");
  component_12_valueQuantity["value"] = dlog_arr[13].toInt();
  component_12_valueQuantity["unit"] = "CmH2O";
  component_12_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_12_valueQuantity["code"] = "CmH2O";

  JsonObject component_13 = component.createNestedObject();

  JsonObject component_13_code = component_13.createNestedObject("code");

  JsonObject component_13_code_coding_0 = component_13_code["coding"].createNestedObject();
  component_13_code_coding_0["system"] = "http://loinc.org";
  component_13_code_coding_0["code"] = "73793-2";
  component_13_code_coding_0["display"] = "Air Pressure";
  component_13_code["text"] = "Air Pressure";

  JsonObject component_13_valueQuantity = component_13.createNestedObject("valueQuantity");
  component_13_valueQuantity["value"] = dlog_arr[14].toInt();
  component_13_valueQuantity["unit"] = "Bar";
  component_13_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_13_valueQuantity["code"] = "Bar";

  JsonObject component_14 = component.createNestedObject();

  JsonObject component_14_code = component_14.createNestedObject("code");

  JsonObject component_14_code_coding_0 = component_14_code["coding"].createNestedObject();
  component_14_code_coding_0["system"] = "http://loinc.org";
  component_14_code_coding_0["code"] = "73793-2";
  component_14_code_coding_0["display"] = "Oxygen Pressure";
  component_14_code["text"] = "Oxygen Pressure";

  JsonObject component_14_valueQuantity = component_14.createNestedObject("valueQuantity");
  component_14_valueQuantity["value"] = dlog_arr[15].toInt();
  component_14_valueQuantity["unit"] = "Bar";
  component_14_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_14_valueQuantity["code"] = "Bar";

  JsonObject component_15 = component.createNestedObject();

  JsonObject component_15_code = component_15.createNestedObject("code");

  JsonObject component_15_code_coding_0 = component_15_code["coding"].createNestedObject();
  component_15_code_coding_0["system"] = "http://loinc.org";
  component_15_code_coding_0["code"] = "73794-0";
  component_15_code_coding_0["display"] = "Battery Percent";
  component_15_code["text"] = "Battery Percent";

  JsonObject component_15_valueQuantity = component_15.createNestedObject("valueQuantity");
  component_15_valueQuantity["value"] = dlog_arr[19].toInt();
  component_15_valueQuantity["unit"] = "%";
  component_15_valueQuantity["system"] = "http://unitsofmeasure.org";
  component_15_valueQuantity["code"] = "%";

  serializeJson(inc_log, return_data);
}