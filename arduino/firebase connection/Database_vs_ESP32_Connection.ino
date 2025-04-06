#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "FOE-Student"
#define WIFI_PASSWORD "abcd@1234"

// Insert Firebase project API Key
#define API_KEY "AIzaSyD-DoytcfFd_NdvcpyppC1cb3biFM1-kVE"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://smart-house-water-management-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 120;
String Quality = "good";
bool signupOK = false;
//float inflow

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    //Input solonoid valve 
    if (Firebase.RTDB.getBool(&fbdo, "InValve")) {
      bool InValue = fbdo.boolData();
      Serial.print("Received Boolean from Firebase: ");
      Serial.println(InValue ? "true" : "false");
    } else {
      Serial.print("Failed to read Boolean: ");
      Serial.println(fbdo.errorReason());
    }
    
    //Output solonoid valve
    if (Firebase.RTDB.getBool(&fbdo, "OutValve")) {
      bool OutValue = fbdo.boolData();
      Serial.print("Received Boolean from Firebase: ");
      Serial.println(OutValue ? "true" : "false");
    } else {
      Serial.print("Failed to read Boolean: ");
      Serial.println(fbdo.errorReason());
    }
    
    //Input flow sensor value
    if (Firebase.RTDB.setFloat(&fbdo, "Inflow", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Output flow sensor value
    if (Firebase.RTDB.setFloat(&fbdo, "Outflow", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    //Motor Swich on/off
    if (Firebase.RTDB.getBool(&fbdo, "MotorSwich")) {
      bool MotorSwich = fbdo.boolData();
      Serial.print("Received Boolean from Firebase: ");
      Serial.println(MotorSwich ? "true" : "false");
    } else {
      Serial.print("Failed to read Boolean: ");
      Serial.println(fbdo.errorReason());
    }

    //TDS Sensor Value
    if (Firebase.RTDB.setInt(&fbdo, "TDS", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Temperature Sensor Value
    if (Firebase.RTDB.setFloat(&fbdo, "Temperature", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Water Level Value
    if (Firebase.RTDB.setInt(&fbdo, "WaterLevel", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Toggle Button String
    if (Firebase.RTDB.getString(&fbdo, "toggleState")) {
      String ToggleState = fbdo.stringData();
      Serial.print("Received String from Firebase: ");
      Serial.println(ToggleState);
    } else {
      Serial.print("Failed to read String: ");
      Serial.println(fbdo.errorReason());
    }

    //Water Quality Out
    if (Firebase.RTDB.setString(&fbdo, "WaterQuality", Quality)) {
      Serial.println("String write PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("VALUE: " + fbdo.stringData());
    } else {
      Serial.println("String write FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    /*
    //Out INT
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
    
    //Out Float
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Get Int
    if (Firebase.RTDB.getInt(&fbdo, "test/gg")) {
      int value = fbdo.intData();
      Serial.print("Received value from Firebase: ");
      Serial.println(value);
    } else {
      Serial.print("Failed to read from Firebase: ");
      Serial.println(fbdo.errorReason());
    }

    //Get Float
    if (Firebase.RTDB.getFloat(&fbdo, "test/dama")) {
      float value = fbdo.floatData();
      Serial.print("Received value from Firebase: ");
      Serial.println(value);
    } else {
      Serial.print("Failed to read from Firebase: ");
      Serial.println(fbdo.errorReason());
    }

    //Get String
    if (Firebase.RTDB.getString(&fbdo, "test/myString")) {
      String value = fbdo.stringData();
      Serial.print("Received String from Firebase: ");
      Serial.println(value);
    } else {
      Serial.print("Failed to read String: ");
      Serial.println(fbdo.errorReason());
    }

    //Get Bool
    if (Firebase.RTDB.getBool(&fbdo, "test/myBool")) {
      bool value = fbdo.boolData();
      Serial.print("Received Boolean from Firebase: ");
      Serial.println(value ? "true" : "false");
    } else {
      Serial.print("Failed to read Boolean: ");
      Serial.println(fbdo.errorReason());
    }
    */
  }
}