void Print_DC_Error(void){
   // Serial.println(F("Min %15 /Max %99"));  
}
void Print_Color_Error(void){
  //  Serial.println(F("!!! Min-0 Max-255"));  
}

void StoreData(void){
  if(Key.Task) {
      Key.Task = OFF;
      NV_Mem.putUChar("NV_Mode", System.Mode);
  }
  if(System.Update) {
      System.Update  = OFF;
      NV_Mem.putUChar("NV_Mode", System.Mode);
  }
  if(System.Index_Update) {
      System.Index_Update = OFF;
      NV_Mem.putUChar("NV_Index", System.Index);
  }
  if(Fan.HighSave){
    Fan.HighSave = OFF;  
    NV_Mem.putUChar("NV_Fan_High", Fan.HighSpeed);  
  } 
  if(Fan.MidSave){
    Fan.MidSave = OFF;  
    NV_Mem.putUChar("NV_Fan_Mid", Fan.MidSpeed);  
  } 
  if(Fan.LowSave){
    Fan.LowSave = OFF;  
    NV_Mem.putUChar("NV_Fan_Low", Fan.LowSpeed); 
  } 
  if(Led.HighSave){
    Led.HighSave = OFF;  
  //  NV_Mem.putUChar("NV_Col_High", Led.ColorHigh);  
      NV_Mem.putUInt("NV_Col_High",  Color.High_Code );// green
  } 
  if(Led.MidSave){
    Led.MidSave = OFF;  
    //NV_Mem.putUChar("NV_Col_Mid", Led.ColorMid); 
     NV_Mem.putUInt("NV_Col_Mid",  Color.Mid_Code );
  } 
  if(Led.LowSave){
    Led.LowSave = OFF;  
  //  NV_Mem.putUChar("NV_Col_Low", Led.ColorLow); 
    NV_Mem.putUInt("NV_Col_Low",  Color.Low_Code );
  }  
  if(Connection.WIFI_Save){
    Connection.WIFI_Save = OFF;  
    NV_Mem.putString("ssid", WIFI_SSID); 
    NV_Mem.putString("password", WIFI_PASS);
  }    
}

void Execute_Serial_Commands(void){
  while (Serial.available()) {
    char incomingChar = Serial.read();  // Read each character from the buffer
    //  static const char LOG_5MSEC[]   PROGMEM = "  5 mS"; //12
    if (incomingChar == '\n') {  // Check if the user pressed Enter (new line character)
      System.RxUnknown = ON; 

       if (receivedMessage.substring(0,5) == "Net?") {  //(0,9) == "ColorHigh")  (10,14))
          Connection.WIFI_Info = ON;
          System.RxSuccess = ON;   
      }
       if (receivedMessage.substring(0,5) == "Col?") {  //(0,9) == "ColorHigh")  (10,14))
          Color.Info = ON;
          System.RxSuccess = ON;   
      }
       if (receivedMessage.substring(0,5) == "Fan?") {  //(0,9) == "ColorHigh")  (10,14))
          Fan.Info = ON;
          System.RxSuccess = ON;   
      }
       if (receivedMessage.substring(0,4) == "Key") {  //(0,9) == "ColorHigh")  (10,14))
         Color.Fade = OFF;
          Key_Mode_Update();
          System.RxSuccess = ON;   
      }
      if (receivedMessage.substring(0,4) == "WIFI") {
        //  Serial.println(receivedMessage.substring(5));
// Source - https://stackoverflow.com/a/26822491
// Posted by mct, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-16, License - CC BY-SA 3.0
     //   uint8_t  Length = receivedMessage.length();   
        if(receivedMessage.length() > 13 ){ // ssid and pass min 4 character + 2 space + 4 *wifi = 14
          int Space1 = receivedMessage.indexOf(' ');
          int Space2 = receivedMessage.indexOf(' ', Space1 + 1);
      //   String firstValue = receivedMessage.substring(0, Space1);
          WIFI_SSID = receivedMessage.substring(Space1 + 1, Space2);
          WIFI_PASS = receivedMessage.substring(Space2 + 1);
          Connection.WIFI_Terminal_Update = ON;  
          Connection.WIFI_Save = ON; 
          Connection.WIFI_Est_Connect = ON;    
          System.MonitorTimer = 10;
          System.RxSuccess = ON; 
        }              
      }
      if (receivedMessage.substring(0,5) == "Smart") {
          System.RxSuccess = ON;  
          Connection.Smart_Conn_Enable = !Connection.Smart_Conn_Enable;
      }
      if (receivedMessage.substring(0,4) == "Mode") { // SpeedHigh (0,9)
        System.RxSuccess = ON;  
        System.Update = ON;   
        if(System.Mode <= RUN_TEST_LIMIT){ // Run 2Test  
          switch(System.Mode){
            case RUN_HIGH:System.Mode = TEST_HIGH;
              break;
            case RUN_MID:System.Mode = TEST_MID;
              break;
           case RUN_LOW:System.Mode = TEST_LOW;
              break;
            case RUN_OFF:System.Mode = TEST_OFF; 
              break;   
            default:    System.RxSuccess = OFF;  
                      System.Update = OFF;    
            break;
          }
        }
        else{ // Test2 Run
          Reset_Run_Modes();
          System.MonitorTimer = 10;
         switch(System.Mode){
          case TEST_HIGH:System.Mode = RUN_HIGH;
          break;
          case TEST_MID:System.Mode = RUN_MID;
          break;
           case TEST_LOW:System.Mode = RUN_LOW;
          break;
          case TEST_OFF:System.Mode = RUN_OFF; 
          break;   
          default:    System.RxSuccess = OFF;  
                      System.Update = OFF;    
          break;
        }
       } 
    }       
      if (receivedMessage.substring(0,4) == "FanH") { // SpeedHigh (0,9)
       uint8_t Temp = (uint8_t)(receivedMessage.substring(5,8)).toInt();  // (10,13)
        if((Temp > 15) && (Temp < 99)){    
          Fan.HighSpeed = Temp;
           Fan.HighSave = ON;                 
          if((System.Mode == TEST_HIGH) || (System.Mode == RUN_HIGH))Fan.DutyCycle =Fan.HighSpeed;  
          System.RxSuccess = ON;                   
        }
        else Print_DC_Error(); 
      }
      if (receivedMessage.substring(0,4) == "FanM") { // (0,8) == "SpeedMid") {
       uint8_t Temp = (uint8_t)(receivedMessage.substring(5,8)).toInt();  // (9,12))
        if((Temp > 15) && (Temp < 99)){    
          Fan.MidSpeed = Temp;
          Fan.MidSave = ON;         
          if((System.Mode == TEST_MID) || (System.Mode == RUN_MID))Fan.DutyCycle =Fan.MidSpeed;  
          System.RxSuccess = ON;   
        }     
        else Print_DC_Error(); 
      } 
       if (receivedMessage.substring(0,4) == "FanL") {
       uint8_t Temp = (uint8_t)(receivedMessage.substring(5,8)).toInt(); 
        if((Temp > 15) && (Temp< 99)){
          Fan.LowSpeed = Temp;   
          Fan.LowSave = ON;                 
          if((System.Mode == TEST_LOW) || (System.Mode == RUN_LOW))Fan.DutyCycle =Fan.LowSpeed; 
          System.RxSuccess = ON;   
        }     
        else Print_DC_Error();        
      }
       if (receivedMessage.substring(0,4) == "ColH") {  //(0,9) == "ColorHigh")  (10,14))
        String Hex_Number = receivedMessage.substring(5,12);  
        uint32_t number32 = (uint32_t)strtol(&Hex_Number[0], NULL, 16);
    //   Serial.print("Hex_Number:"); Serial.print(Hex_Number);       
   //    Serial.print("   number32:"); Serial.println(number32);
// Hex_Number:E5FFCCd   number32:241171661 E5FFCCd
     //  uint32_t Temp = (uint32_t)(receivedMessage.substring(5,12))toInt();  // (5,9))
        if (number32 <= 16777215){
          Color.High_Code = number32;
          ColorHigh_Hex = Hex_Number;
          Convert24bitToRGB(Color.High_Code , &Color.High_R, &Color.High_G, &Color.High_B);
          Led.HighSave = ON;
          System.RxSuccess = ON;  
        }
      } 
        if (receivedMessage.substring(0,4) == "ColM") {  // 
          String Hex_Number = receivedMessage.substring(5,12);  
          uint32_t number32 = (uint32_t)strtol(&Hex_Number[0], NULL, 16);
          if (number32 <= 16777215){
            Color.Mid_Code = number32;
            ColorMid_Hex = Hex_Number;
            Convert24bitToRGB(Color.Mid_Code, &Color.Mid_R, &Color.Mid_G, &Color.Mid_B);              
            Led.MidSave = ON;  
            System.RxSuccess = ON;  
          }       
        }      
       if (receivedMessage.substring(0,4) == "ColL") {  // SpeedMid ColorLow
        String Hex_Number = receivedMessage.substring(5,12);  
         uint32_t number32 = (uint32_t)strtol(&Hex_Number[0], NULL, 16); 
        if (number32 <= 16777215){
          Color.Low_Code = number32;
          ColorLow_Hex = Hex_Number;
          Convert24bitToRGB(Color.Low_Code, &Color.Low_R, &Color.Low_G, &Color.Low_B);
          Led.LowSave = ON;
          System.RxSuccess = ON;  
        }
      } 
      if (receivedMessage.substring(0,5) == "Reset") {  // SpeedMid ColorLow
        System.RxSuccess = ON;   
        ESP.restart(); 
      }   
      if (receivedMessage.substring(0,5) == "Ver?") {  // SpeedMid ColorLow
        System.RxSuccess = ON;   
         System.Version = ON;
      }         

       if (receivedMessage.substring(0,6) == "DSleep") {  // SpeedMid ColorLow
       // digitalWrite(BOOST_CONV_POWER, OFF);
       // Sleep_Inhibit_Timer = 5;   
        System.RxSuccess = OFF;
        System.RxUnknown = OFF;    
     //   Set_Deep_Sleep();
        System.Deep_SleepTimer  = 3;   
      } 
      if (receivedMessage.substring(0,6) == "RSleep") { // SpeedHigh (0,9)
        System.RxSuccess = OFF;  
        System.RxUnknown = OFF;  
        System.RTC_SleepTimer  = 3;                   
      }          
       if (receivedMessage.substring(0,6) == "LSleep") {  // SpeedMid ColorLow
       // digitalWrite(BOOST_CONV_POWER, OFF);
       // Sleep_Inhibit_Timer = 5;   
        System.RxSuccess = OFF;
        System.RxUnknown = OFF;   
        Reset_Run_Modes();
        System.Mode = RUN_OFF; 
        System.Light_SleepTimer  = 5;    
      }   
    //  uint8_t Hour,Minute,Second,Date,Month;
      
      if (receivedMessage.substring(0,4) == "Time") { // SpeedHigh (0,9)      
        TimeFrag.Hour = (uint8_t)(receivedMessage.substring(5,7)).toInt();
        if (TimeFrag.Hour < 24){
          TimeFrag.Minute = (uint8_t)(receivedMessage.substring(8,10)).toInt();
          if (TimeFrag.Minute < 60){
            TimeFrag.Second = (uint8_t)(receivedMessage.substring(11,13)).toInt();
            if (TimeFrag.Second < 60){
              TimeFrag.Date = (uint8_t)(receivedMessage.substring(14,16)).toInt();
              if ((TimeFrag.Date > 0) && (TimeFrag.Date < 32)){  
                TimeFrag.Month = (uint8_t)(receivedMessage.substring(17,19)).toInt();
                if ((TimeFrag.Month > 0) && (TimeFrag.Month < 13)){  
                  TimeFrag.Year = (uint16_t)(receivedMessage.substring(20,25)).toInt();
                  if ((TimeFrag.Year > 2025) && (TimeFrag.Year < 2040)){ 
                    rtc.setTime(TimeFrag.Second, TimeFrag.Minute, TimeFrag.Hour, TimeFrag.Date, TimeFrag.Month, TimeFrag.Year);  // 17th Jan 2021 15:24:30      
                    System.RxSuccess = ON;
                  }
                }
              }  
            }
          }
        }                 
      } 

      if(System.RxSuccess)System.RxUnknown = OFF;  
      receivedMessage = "";
    } else receivedMessage += incomingChar;   
  } // end of while
} // end of serial check function

void Init_NV_MemData(void){
    NV_Mem.begin("NV_MEMORY",RO_MODE );   // RW_MODE  false // RO_MODE true
    bool tpInit = NV_Mem.isKey("nvsInit");  
  if (tpInit == false) {
      // If tpInit is 'false', the key "nvsInit" does not yet exist therefore this
      //  must be our first-time run. We need to set up our Preferences namespace keys. So...
      NV_Mem.end();                             // close the namespace in RO mode and...
      NV_Mem.begin("NV_MEMORY", RW_MODE);        //  reopen it in RW mode.
      // The .begin() method created the "STCPrefs" namespace and since this is our
      //  first-time run we will create
      //  our keys and store the initial "factory default" values.

      NV_Mem.putString("ssid", WIFI_SSID); 
      NV_Mem.putString("password", WIFI_PASS);
      NV_Mem.putUChar("NV_Index", 0);
      NV_Mem.putUChar("NV_Mode", RUN_OFF);
      NV_Mem.putUChar("NV_Fan_High", 80);
      NV_Mem.putUChar("NV_Fan_Mid", 60);
      NV_Mem.putUChar("NV_Fan_Low", 40);
      // https://www.rapidtables.com/web/color/RGB_Color.html
      NV_Mem.putUInt("NV_Col_High", 65280);//	Lime	#00FF00	(0,255,0)
      NV_Mem.putUInt("NV_Col_Mid", 16711935); // Magenta / Fuchsia	#FF00FF	(255,0,255)
      NV_Mem.putUInt("NV_Col_Low", 65535); //Cyan / Aqua	#00FFFF	(0,255,255)

      NV_Mem.putBool("nvsInit", true);          // Create the "already initialized"
                                                  //  key and store a value.
      // The "factory defaults" are created and stored so...
      NV_Mem.end();                             // Close the namespace in RW mode and...
   //   NV_Mem.begin("NV_MEMORY", RO_MODE);        //  reopen it in RO mode so the setup code
                                                  //  outside this first-time run 'if' block
                                                  //  can retrieve the run-time values
                                                  //  from the "STCPrefs" namespace.
   }
   NV_Mem.end(); 
   NV_Mem.begin("NV_MEMORY", RW_MODE);        //  reopen it in RW mode.
    

WIFI_SSID = NV_Mem.getString("ssid");  
WIFI_PASS = NV_Mem.getString("password");  
    System.Index = NV_Mem.getUChar("NV_Index");     
    System.Mode = NV_Mem.getUChar("NV_Mode");
    Fan.HighSpeed = NV_Mem.getUChar("NV_Fan_High");
    Fan.MidSpeed = NV_Mem.getUChar("NV_Fan_Mid");
    Fan.LowSpeed = NV_Mem.getUChar("NV_Fan_Low");


    Color.High_Code = NV_Mem.getUInt("NV_Col_High");
    Color.Mid_Code = NV_Mem.getUInt("NV_Col_Mid");
    Color.Low_Code = NV_Mem.getUInt("NV_Col_Low");
/*
    Led.ColorHigh = NV_Mem.getUChar("NV_Col_High");
    Led.ColorMid = NV_Mem.getUChar("NV_Col_Mid");
    Led.ColorLow = NV_Mem.getUChar("NV_Col_Low");
    */
   // All done. Last run state (or the factory default) is now restored.
  // NV_Mem.end(); 
}
/*
      if (receivedMessage.substring(0,4) == "Test") { // SpeedHigh (0,9)
        System.RxSuccess = ON;  
        System.Update = ON;    
        System.MonitorTimer = 10;
        switch(System.Mode){
          case RUN_HIGH:System.Mode = TEST_HIGH;
          break;
          case RUN_MID:System.Mode = TEST_MID;
          break;
           case RUN_LOW:System.Mode = TEST_LOW;
          break;
          case RUN_OFF:System.Mode = TEST_OFF; 
          break;   
          default:    System.RxSuccess = OFF;  
                      System.Update = OFF;    
          break;
        }         
      }
      if (receivedMessage.substring(0,3) == "Run") { // SpeedHigh (0,9)
        Reset_Run_Modes();
        System.RxSuccess = ON;  
        System.Update = ON;      
         switch(System.Mode){
          case TEST_HIGH:System.Mode = RUN_HIGH;
          break;
          case TEST_MID:System.Mode = RUN_MID;
          break;
           case TEST_LOW:System.Mode = RUN_LOW;
          break;
          case TEST_OFF:System.Mode = RUN_OFF; 
          break;   
          default:    System.RxSuccess = OFF;  
                      System.Update = OFF;    
          break;
        }          

      }
*/


