// git add .
// git commit -m "Time Stamp"
// https://github.com/ilkerya/Scent_Diffuser-Firmware.git

/*
 Repeat timer example

 This example shows how to use hardware timer in ESP32. The timer calls onTimer
 function every second. The timer can be stopped with button attached to PIN 0
 (IO0).
 This example code is in the public domain.

 ESP WROOM 32E ->
 https://documentation.espressif.com/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf

 ESP32-D0WD-V3 chip or ESP32-D0WDR2-V3 chip
https://documentation.espressif.com/esp32_datasheet_en.pdf

 */

#define WIFI_INCLUDE
//#define WIFI_EXCLUDE
#include <Preferences.h>
Preferences NV_Mem;
#define RW_MODE false
#define RO_MODE true

  #include <Wire.h>

#ifdef WIFI_INCLUDE
  #include <WiFi.h>

  //WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
#endif


#include <esp_task_wdt.h>
#include  "Defs.h"
#include "driver/rtc_io.h"
#include <ESP32Time.h>
      ESP32Time rtc;
#include <driver/uart.h>
#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(NFC_CS, NFC_RST);

#include "Variables.h"
#include "DAQ.h"
#include "Sensors.h"
#include "MemSave.h"
#include "Functions.h"
#include "Menu.h"
#include "Connect.h"
// STR25100

//ESP32 Update Link from preferences
// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

void setup() {
  WatchdogTimer_Set();
  Init_IO();
  Serial.begin(115200);

 // rtc.setTime(30, 0, 20, 15, 3, 2026);  // 17th Jan 2021 15:24:30
 // rtc.setTime(TimeFrag.Second, TimeFrag.Minute, TimeFrag.Hour, TimeFrag.Date, TimeFrag.Month, TimeFrag.Year); 
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 //  WiFi.mode(WIFI_MODE_STA); 
  print_wakeup_reason();

  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("Reset reason: ");
  Serial.println(reason); // Prints integer value

  Wire.begin (SDA, SCL);   // sda= GPIO_18 /scl= GPIO_19
//  Found address: 26 (0x1A)
// Found address: 56 (0x38)
// Found address: 83 (0x53)
 // Scanner ();
   //analogWrite(ledPin, dutyCycle);
 // print_wakeup_reason();

  Init_TempHSensors();     
  Init_Light_Sensor();
  Init_TVoc();
  Init_NV_MemData();     
  Interrupt_Set();
  Key.Inhibit_Timer = 3;  
  rtc_gpio_hold_dis(GPIO_NUM_2);// FAN 12V ON/OFF Control
  Color_Dec2Hex();
  Key.ColorFade_timer = FADE_TIME;

  #ifdef WIFI_INCLUDE
 Connection.WIFI_Est_Connect = ON;
 Connection.WIFI_Terminal_Update = ON;
//  Start_NTP_Time();
  #endif

  SPI.begin(NFC_CLK, NFC_MISO, NFC_MOSI, NFC_CS);  
  // Initialize the Ci523 / MFRC522 card reader
  mfrc522.PCD_Init();  

}



//uint32_t isrCount = 0, isrTime = 0;
uint8_t RunTimer;
void loop() {
   esp_task_wdt_reset(); // 10 seconds
  // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    //uint32_t isrCount = 0, isrTime = 0;
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux);
  //  isrCount = isrCounter;
   // isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
  }
  SystemTimers();


  if(System.LOOP_30Minute){
     System.LOOP_30Minute = OFF;
      #ifdef WIFI_INCLUDE
      Set_NTP_Time();
      #endif
  }

  if(System.Loop_100mSec){
    System.Loop_100mSec = OFF;
    Mode_Select(); 
    Rpm_Calculate();
     Battery_Volt();
     NFC_Func();

  }
   if(System.Loop_500mSec){
     System.Loop_500mSec = OFF;
     Execute_Serial_Commands();
     USB_Volt();

  }
  if(System.LOOP_5Second){
     System.LOOP_5Second = OFF;
      StoreData();
     // Read_TVoc();
  }

 if(System.LOOP_1Second){
     System.LOOP_1Second = OFF;  

    


  // Select one of the cards and read its data


     if(Connection.NTP_Init){ // WIFI Connection OK Triggers
      if(WiFi.status() == WL_CONNECTED) {
        Connection.NTP_Init = OFF;
        Start_NTP_Time();
      }
     }

     if(Connection.Smart_Conn_Enable){
        Serial.print("Smart Conn. : ON ");
     }

  // digitalWrite(SENSOR_3V_POWER, SENSOR_3V_ENABLE); 
    Read_Temperature();
    Read_Light();
   

    #ifdef WIFI_INCLUDE
    if(Connection.WIFI_Est_Connect){
       Connection.WIFI_Est_Connect = OFF;
        if(WiFi.status() != WL_CONNECTED) {
          Connection.WIFI_Reconn_Timer = 9;
          WiFi.disconnect();
          WiFi.mode(WIFI_STA);
          WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
          Connection.NTP_Init = ON;
        }
    }
      
        
     //   if(WiFi.status() != WL_CONNECTED) {
          //  WiFi.disconnect();
       //     Connection.WIFI_Reconn_Timer = 3;// 10sec base 60 sec recheck
       //     Connection.NTP_Done = OFF;
     //   }
       // else Connection.NTP_Init = ON;
        
     //  Connection.NTP_Init = ON;
    //}
    #endif
    System.PC_Serial_Mode = OFF;
    if(System.PC_Serial_Mode)
        DAQ_Send_Data(LOOP_BASED); 
    else{

      if(Key.DoubPress){
          Key.DoubPress = OFF; 
          Serial.println(F("Double Press!")); 
      }

       // Serial.print(System.Light_Sleep);Serial.print('.');Serial.print(System.Light_SleepTimer); Serial.println('.');

      if((System.Light_SleepTimer==1) || (System.Light_Sleep) ){
        Serial.println(F("Light Sleep!")); 
        return;
      }
      if((System.RTC_SleepTimer==1) || (System.RTC_Sleep) ){
        Serial.println(F("RTC Sleep!")); 
        return;
      }      
      if(System.Deep_SleepTimer==1){
        Serial.println(F("Deep Sleep!")); 
        return;
      }

      if(System.RxSuccess){
        System.RxSuccess = OFF;   
        Serial.println(F("Message Success!")); 
      }
      if(System.RxUnknown){
        System.RxUnknown = OFF;   
        Serial.println(F("Message Failed!")); 
      }   
      if(System.Version){        
        System.Version = OFF;  
        Serial.print(F("Compile Date & Time ")); 
        Serial.print(__DATE__ ", " __TIME__); 

        uint64_t chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
        Serial.printf("    ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
        Serial.printf("%08X\n", (uint16_t)chipid); //print Low 4bytes.
      }
      if(Connection.WIFI_Info){        
        Connection.WIFI_Info = OFF;  
        Serial.print(F("Wifi Ssid: "));   Serial.print(WIFI_SSID);    
        Serial.print(F("   Pass: ")); Serial.print(WIFI_PASS); 
        Serial.print(F("   Mac: "));Serial.println(WiFi.macAddress());
      }
      if(Color.Info){        
        Color.Info= OFF; 
        Serial.print(F("ColorCodes: Low:0x"));   
        Serial.print(ColorLow_Hex );Serial.print(F("  Mid:0x")); Serial.print(ColorMid_Hex);Serial.print(F("  High:0x")); Serial.print(ColorHigh_Hex);    
        Serial.print(F(" RGB:  Low:"));        
        Serial.print(Color.Low_R);Serial.print(F(".")); Serial.print(Color.Low_G);Serial.print(F(".")); Serial.print(Color.Low_B);
        Serial.print(F("   Mid:"));
        Serial.print(Color.Mid_R);Serial.print(F(".")); Serial.print(Color.Mid_G);Serial.print(F(".")); Serial.print(Color.Mid_B);             
        Serial.print(F("   High:"));
        Serial.print(Color.High_R);Serial.print(F(".")); Serial.print(Color.High_G);Serial.print(F(".")); Serial.println(Color.High_B);  
      }
      if(Fan.Info){        
        Fan.Info= OFF;  
        Serial.print(F("Fan DutyCycle  Low:%")); 	
        Serial.print(Fan.LowSpeed);Serial.print(F("  Mid:%")); Serial.print(Fan.MidSpeed);Serial.print(F("  High:%")); Serial.println(Fan.HighSpeed);
      }
      if(Connection.WIFI_Terminal_Update){        
        Connection.WIFI_Terminal_Update = OFF;  
        #ifdef WIFI_INCLUDE
         Serial.print(F("SSID: "));   Serial.print(WIFI_SSID);    
          Serial.print(F("   PASSW: ")); Serial.print(WIFI_PASS); 
          Serial.print(F("    MAC Address: "));Serial.println(WiFi.macAddress());
        #endif
       #ifdef WIFI_EXCLUDE
          Serial.println(F("WIFI Excluded: "));
        #endif
     }    
    if(System.Mode <= RUN_TEST_LIMIT) {
          System.MonitorTimer++;
          if(System.MonitorTimer <  5) return;
    }
    System.MonitorTimer = 0;
      if(System.Mode == TEST_OFF) Serial.print(F("TEST_Off ")); 
      if(System.Mode == TEST_HIGH)   Serial.print(F("TEST_High ")); 
      if(System.Mode == TEST_MID) Serial.print(F("TEST_Mid ")); 
      if(System.Mode == TEST_LOW)    Serial.print(F("TEST_Low "));      

      if(System.Mode == RUN_OFF) Serial.print(F("OFF ")); 
      if(System.Mode == RUN_HIGH)   Serial.print(F("HIGH ")); 
      if(System.Mode == RUN_MID) Serial.print(F("MID ")); 
      if(System.Mode == RUN_LOW)    Serial.print(F("LOW "));   


    if(System.Mode <= RUN_TEST_LIMIT) {
        //Serial.print((System.Cycle+1));Serial.print(".");
        Serial.print((System.Index+1));Serial.print(".");Serial.print(System.RunTimer/10); Serial.print(F("  "));        
    }
    Serial.print(Fan.Rpm); Serial.print(F("Rpm-%"));Serial.print(Fan.DutyCycle); 

      Serial.print(F("  Col:")) ; 
      if(System.Mode == TEST_OFF  || System.Mode == RUN_OFF) Serial.print(F("OFF")); 
      else {
        Serial.print(F("0x")) ; 
        if(System.Mode == TEST_HIGH || System.Mode == RUN_HIGH )Serial.print(ColorHigh_Hex); //Serial.print(ColorHigh); 
        if(System.Mode == TEST_MID  || System.Mode == RUN_MID) Serial.print(ColorMid_Hex); //Serial.print(ColorMid); 
        if(System.Mode == TEST_LOW  || System.Mode == RUN_LOW) Serial.print(ColorLow_Hex); //Serial.print(ColorLow);   
        if (Color.Fade)Serial.print("-Dimm");
      } 
      Serial.print(F("  Bat:")) ;    
      Battery.F_Val = (float)Battery.Volt;
      Battery.F_Val /= 1000;
      Serial.print(Battery.F_Val,2); 

      Serial.print(F("V  Usb:")) ; 

      Usb.F_Val = (float)Usb.Volt;
      Usb.F_Val /= 1000;
      Serial.print(Usb.F_Val,2); 

      Serial.print(F("V  Stdb:")); 
      if(Battery.Standbye)Serial.print(F("ON "));
      else Serial.print(F("OFF "));

      Serial.print(F(" Chg:"));
      if(Battery.Charge)Serial.print(F("ON  "));
      else Serial.print(F("OFF  "));     
    //  Serial.print(Battery.Standbye);Serial.print(F("  Chg:")); Serial.print(Battery.Charge);

     Serial.print(Values.Temperature,1);Serial.print(F("°C %")); Serial.print(Values.Humidity,0);Serial.print(F("rh "));
   //  if(Values.TVoc_Error== ON)Serial.print(F("  "));
  //   else Serial.print(Values.TVoc);
   //  Serial.print(F("ppb "));
     Serial.print(Values.Lux,1);Serial.print(F("Lux ")); 
 
    if(Connection.NTP_Done)Serial.print(rtc.getTime(" %H:%M:%S %d.%B.%Y")); 

      #ifdef WIFI_INCLUDE
     if (WiFi.status() == WL_CONNECTED) {
            Serial.print(" IP:");Serial.print(WiFi.localIP());
      }
    //   Serial.print(F("/")); Serial.print(Connection.WIFI_Reconn_Timer);Serial.print(F("."));Serial.print(Connection.WIFI_Est_Connect); 
    //   Serial.print(F("TimerPress")); Serial.print(Key.TimerPress);Serial.print(F("Mode"));Serial.print(Key.Mode);
    
      #endif
  //   }
    Serial.println(""); 
    }  
  }
}


 





