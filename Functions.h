#include <array>
//uint16_t Led_Que = 0;
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

/*
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
volatile uint32_t lastIsrAt_Prev = 0;
volatile uint32_t lastIsrAt_Diff= 0;
*/
#define WDT_TIMEOUT 10000 // 10000mS = 10 second... ->
//#define CONFIG_FREERTOS_NUMBER_OF_CORES 1
void WatchdogTimer_Set(){
  esp_task_wdt_config_t twdt_config =
    {
        .timeout_ms = WDT_TIMEOUT,
        .idle_core_mask = 0,    // Bitmask of cores
        .trigger_panic = true,
    };
  esp_task_wdt_deinit(); //wdt is enabled by default, so we need to 'deinit' it first
  esp_task_wdt_init(&twdt_config); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}
void ARDUINO_ISR_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
 // isrCounter = isrCounter + 1;
 // lastIsrAt = millis();
//  lastIsrAt = micros();
 // lastIsrAt_Diff = lastIsrAt - lastIsrAt_Prev;
 // lastIsrAt_Prev = lastIsrAt;
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
   //1 msec
  Fan_Feedback(); //call 10 uSeconds

  System.Loop_20mSecCounter++;

}
void Interrupt_Set(void){
  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();
  // Set timer frequency to 1Mhz resolution
  timer = timerBegin(1000000);
  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer);
  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
 // timerAlarm(timer, TIME_USEC, true, 0);// 100000 usec => 1000 msec => 1 sec
  #define TIME_1MSEC 1000 // 1 msec resolution
  #define TIME_100USEC 100 // 1 msec resolution
  #define TIME_10USEC 10 // 1 msec resolution
  #define TIME_1USEC 1 // 1 msec resolution

  timerAlarm(timer, TIME_10USEC, true, 0);// 1000 usec => 1 msec
 // pinMode(BTN_STOP_ALARM, INPUT);
}

void Fan_Feedback(void) {
  #define TACHO_ERROR 30000 // low than 100 rpm
   if(digitalRead(FAN_FEEDBACK)){
    if(Fan.Pulse_Low > 0){
        Fan.Pulse_Low_Latch = Fan.Pulse_Low;
        Fan.Pulse_Low = 0;
        Fan.Error = OFF;
    }
    Fan.Pulse_High++;
    if(Fan.Pulse_High > TACHO_ERROR)Fan.Error = ON;  
   }
   else {
    if(Fan.Pulse_High > 0){
        Fan.Pulse_High_Latch = Fan.Pulse_High;
        Fan.Pulse_High = 0;
        Fan.Error = OFF;
    }
    Fan.Pulse_Low++;
    if(Fan.Pulse_Low > TACHO_ERROR) Fan.Error = ON;
   }
}
void Set_Sleep_Run_Off(void) {
  /*
      if((System.Mode == RUN_OFF) && !System.Light_Sleep_Inhibit){
        System.Light_Sleep_Inhibit = ON;
        System.Light_SleepTimer  = 25;
      }
      */
  //if(System.Index == 0 && System.RunTimer == 0 && System.Mode == RUN_OFF){

  if(System.Light_SleepTimer){
    if(System.Mode == RUN_OFF){System.Light_SleepTimer--;
    if(System.Light_SleepTimer == 0){
      System.Light_Sleep = ON; 
      System.Light_SleepTimer  = 25;
    }
   }
  }
}

void NFC_Init_Func(void){
  // NFC_Type ="No Card";
   // if ( !mfrc522.PICC_ReadCardSerial())    return;
   //bool Read = mfrc522.PICC_ReadCardSerial();
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    NFC_Type  = mfrc522.PICC_GetTypeName(piccType);
   // Serial.print(mfrc522.PICC_GetTypeName(piccType));
          // Print the Unique Identifier (UID) bytes in hexadecimal format
   Nfc.Id_Size = mfrc522.uid.size;
   if(Nfc.Id_Size < 1)NFC_Type ="Nothing";
   Nfc.Id = (uint8_t*)&mfrc522.uid.uidByte[0];
   /*
    for (uint8_t i = 0; i < Nfc.Id_Size; i++) {
      Serial.print((uint8_t)Nfc.Id[i] < 0x10 ? " 0" : " ");  
      Serial.print((uint8_t)Nfc.Id[i], HEX); 
    } 
*/
        mfrc522.PICC_HaltA();
        // Stop encryption on PCD (resets the reader's state machine)
    mfrc522.PCD_StopCrypto1();

}
void NFC_Func(void){
  /*
    if ( !mfrc522.PICC_IsNewCardPresent())    return;
    Nfc.NewCard =ON;
   // Serial.println(F("New Card !!! "));
    if ( !mfrc522.PICC_ReadCardSerial())    return;
    Nfc.CardRead =ON  ;

    */
            // Print Tag UID information to the Serial Monitor
   // Serial.print(F("    Card Detected! UID Tag Type: "));
     bool Read = mfrc522.PICC_IsNewCardPresent(); 
    Read = mfrc522.PICC_ReadCardSerial();
    if(!Read){
       // NFC_Type ="Nothing";
        //Nfc.Id_Size = 0;
       // return;
    }
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    NFC_Type  = mfrc522.PICC_GetTypeName(piccType);


/*
    for (byte i = 0; i < Nfc.Id_Size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);  
       // Nfc.IDOrg += mfrc522.uid.uidByte[i] << 4*(mfrc522.uid.size-1-i);      
    }
       */  
    Nfc.Id_Size = mfrc522.uid.size;   
    Nfc.Id = (uint8_t*)&mfrc522.uid.uidByte[0];
    if(Nfc.Id_Size < 2)NFC_Type ="Nothing";
    /*
    for (uint8_t i = 0; i < Nfc.Id_Size; i++) {
      Serial.print((uint8_t)Nfc.Id[i] < 0x10 ? " 0" : " ");  
      Serial.print((uint8_t)Nfc.Id[i], HEX); 
    }   
*/
  //  Serial.println();
   // Serial.println(F("----------------------------------------------"));
        // Halt PICC (stops the card from constantly being re-read while held near the antenna)
      
    mfrc522.PICC_HaltA();
        // Stop encryption on PCD (resets the reader's state machine)
    mfrc522.PCD_StopCrypto1();
}
void SystemTimers(void){
/*
  System.Loop_1mSecCounter++;
  if(System.Loop_1mSecCounter >= 100){ //10uSecx100 = 1000 uSec  
    System.Loop_1mSecCounter = 0;
   
  }
*/

  if(System.Loop_20mSecCounter >= 2000){ //10uSecx2.000 = 20.000 uSec  
    System.Loop_20mSecCounter = 0;
    System.LOOP_20mSec = ON;
    Key_Functions_Digital();

    
    System.Loop_100mSecCounter++;
    if(System.Loop_100mSecCounter >= 5){
      System.Loop_100mSecCounter = 0;
      System.Loop_100mSec = ON;

     if(Led.Candle)analogWrite(LED_CANDLE, random(2, 200));
     else analogWrite(LED_CANDLE, 0);

      System.Loop_500mSecCounter++;
      if(System.Loop_500mSecCounter >= 5){
        System.Loop_500mSecCounter = 0;
        System.Loop_500mSec = ON;
        System.Loop_1SecCounter++;
        if(System.Loop_1SecCounter >= 2){
          System.Loop_1SecCounter = 0;
          System.LOOP_1Second = ON;

            if(Key.Inhibit_Timer)Key.Inhibit_Timer--;

 
             Set_Sleep_Run_Off();
            if(System.RTC_SleepTimer){
              System.RTC_SleepTimer--;
              if(System.RTC_SleepTimer == 0)System.RTC_Sleep = ON; 
            }    
            if(System.Deep_SleepTimer){
              System.Deep_SleepTimer--;
              if(System.Deep_SleepTimer == 0)Set_Deep_Sleep();
            } 
            if(System.Index_UpdateTimer){
              System.Index_UpdateTimer--;
             if(System.Index_UpdateTimer == 0)System.Index_Update = ON; 
            }  
          
          System.Loop_5SecCounter++;
          if(System.Loop_5SecCounter >= 5){
            System.Loop_5SecCounter = 0;
            System.LOOP_5Second = ON;
            if(Connection.WIFI_Reconn_Timer){
              Connection.WIFI_Reconn_Timer--;
             if(Connection.WIFI_Reconn_Timer == 0)Connection.WIFI_Est_Connect = ON; 
            }  
            if(Key.ColorFade_timer){
              Key.ColorFade_timer--;
             if(Key.ColorFade_timer == 0)Color.Fade = ON; 
            }  
           


            if(System.Loop_30MinuteCounter >= 360){ //60*30/5 = 1800/5 = 360
              System.Loop_30MinuteCounter = 0;
              System.LOOP_30Minute = ON;


            }      
          }
        }      
      } 
    }   
  }
}
void Rpm_Calculate(void){
      uint32_t Temp = 2 * (Fan.Pulse_Low_Latch + Fan.Pulse_High_Latch)+1;
     // if(Fan.Rpm !=0)  Fan.Rpm = 6000000 / Fan.Rpm;  // be careful for divide by 0 errror    
    if(!Fan.Error) Temp =  6000000 /Temp; 
    else Temp = 0;
    Fan.RpmTemp += Temp;
    Fan.Avg_Counter++;
    if(Fan.Avg_Counter >=10){
      Fan.Rpm = Fan.RpmTemp/10;
      Fan.RpmTemp = 0;
      Fan.Avg_Counter = 0;
    }
}

void USB_Volt(void){
 // uint16_t Battery_Volt; 
//  Battery.Adc = analogRead(35);
 // uint16_t temp = analogRead(BATTERY_ADC);
// temp *= 246;
 // Battery.Volt = (uint16_t)(temp / 1000); 
 //  Battery.Volt =(uint16_t)analogReadMilliVolts(35);  // 300/980 = 306 / 1000
  uint32_t temp = analogReadMilliVolts(USB_ADC); 
// temp *= 20; //100+100
 // temp /= 10;  // /100

   // temp *= 2; //100+100
/*
    Usb.Temp += temp;
    Usb.Avg_Counter++;
    if(Usb.Avg_Counter >=2){
      Usb.Volt =(uint16_t)( Usb.Temp/10);
      Usb.Temp = 0;
      Usb.Avg_Counter = 0;
    }
    */
    Usb.Volt =(uint16_t)(temp*2);
}

void Battery_Volt(void){
 // uint16_t Battery_Volt; 
//  Battery.Adc = analogRead(35);
 // uint16_t temp = analogRead(BATTERY_ADC);
// temp *= 246;
 // Battery.Volt = (uint16_t)(temp / 1000); 
 //  Battery.Volt =(uint16_t)analogReadMilliVolts(35);  // 300/980 = 306 / 1000
  uint32_t temp = analogReadMilliVolts(BATTERY_ADC); 
  temp *= 98; //300+680
  temp /= 30;  // /300

    Battery_Scent.Temp += temp;
    Battery_Scent.Avg_Counter++;
    if(Battery_Scent.Avg_Counter >=10){
      Battery_Scent.Volt =(uint16_t)( Battery_Scent.Temp/10);
      Battery_Scent.Temp = 0;
      Battery_Scent.Avg_Counter = 0;
    }
 // Battery.Volt =(uint16_t)temp;
  if(digitalRead(BAT_CHARGE))Battery_Scent.Charge = OFF;
  else Battery_Scent.Charge = ON;

 // pinMode(BAT_STANDBYE, INPUT_PULLUP);
  if(digitalRead(BAT_STANDBYE))Battery_Scent.Stbye = OFF;
  else Battery_Scent.Stbye = ON;

}
 void Color_Dec2Hex(void){
    char myHex[10] = "";
    ultoa(Color.Low_Code,myHex,16); //convert to c string base 16
    ColorLow_Hex= String(myHex); 
    ultoa(Color.Mid_Code,myHex,16); //convert to c string base 16
    ColorMid_Hex = String(myHex);
    ultoa(Color.High_Code,myHex,16); //convert to c string base 16
    ColorHigh_Hex = String(myHex);  
 }
void  Init_IO(void){
  pinMode(BAT_CHARGE, INPUT);



//  pinMode(BAT_STANDBYE, INPUT);
  pinMode(BAT_STANDBYE, INPUT_PULLUP);
   //gpio_pullup_dis(BAT_STANDBYE); 

//gpio_set_direction(BAT_STANDBYE, GPIO_MODE_INPUT);
//gpio_set_pull_mode(BAT_STANDBYE, GPIO_FLOATING); // Disables pull-up and pull-down

  pinMode(FAN_FEEDBACK, INPUT);
  pinMode(KEY, INPUT);
  pinMode (KEY, INPUT_PULLUP);
  pinMode(LED_CANDLE, OUTPUT);

  pinMode(NFC_IRQ, INPUT);
  pinMode(NFC_MISO, INPUT); 

  pinMode(NFC_RST, OUTPUT);
  pinMode(NFC_CS, OUTPUT);
  pinMode(NFC_CLK, OUTPUT);
  pinMode(NFC_MOSI, OUTPUT); 

  pinMode(FAN_PWM, OUTPUT);
  digitalWrite(FAN_PWM, OFF);
   ledcAttach(FAN_PWM, 25000, 8);
  pinMode(BOOST_CONV_POWER, OUTPUT);
  digitalWrite(BOOST_CONV_POWER, ON);
 // pinMode(LED_CANDLE, OUTPUT);
 // digitalWrite(LED_CANDLE, ON);
  pinMode(PERIPHERAL_POWER, OUTPUT);
       digitalWrite(PERIPHERAL_POWER, PERIPHERAL_ENABLE);
  //digitalWrite(SENSOR_3V_POWER, SENSOR_3V_ENABLE);
  ledcAttach(LED_RED, 1000, 12);  // 12 kHz PWM, 12-bit resolution
  ledcAttach(LED_GREEN, 1000, 12);
  ledcAttach(LED_BLUE, 1000, 12);
 //  analogSetWidth(12);               // 11Bit resolution
  //analogSetAttenuation(ADC_0db);

  #ifdef SECOND_PROTOTYPE
    pinMode(PERIPHERAL_POWER, OUTPUT);
    digitalWrite(PERIPHERAL_POWER, ON);  
  

  #endif
}

#define WAKEUP_GPIO_KEY              GPIO_NUM_4     // Only RTC IO are allowed - ESP32 Pin example
#define BOOST_CONV_ENABLE            GPIO_NUM_2  
#define RTC_WAKEUP_IO                GPIO_NUM_33  
void Set_IOs_Sleep(void){
  /*
  rtc_gpio_pullup_dis(WAKEUP_GPIO_KEY2);
  rtc_gpio_pulldown_en(WAKEUP_GPIO_KEY2);
*/
  rtc_gpio_pullup_en(WAKEUP_GPIO_KEY);
  rtc_gpio_pulldown_dis(WAKEUP_GPIO_KEY);

  //rtc_gpio_pullup_dis(GPIO_NUM_2); 
  //rtc_gpio_pulldown_en(GPIO_NUM_2);

  //rtc_gpio_pullup_dis(GPIO_NUM_2); 
  //rtc_gpio_pulldown_en(GPIO_NUM_2);

 // rtc_gpio_hold_en(BOOST_CONV_POWER);
  // rtc_gpio_hold_en(GPIO_NUM_2); //  BOOST_CONV_POWER

 pinMode(BOOST_CONV_ENABLE, OUTPUT);
 digitalWrite(BOOST_CONV_ENABLE, LOW); // Set desired state

 // 2. Enable hold on the pin
 rtc_gpio_init(BOOST_CONV_ENABLE);
 rtc_gpio_set_direction(BOOST_CONV_ENABLE, RTC_GPIO_MODE_OUTPUT_ONLY);
 rtc_gpio_hold_en(BOOST_CONV_ENABLE);
}
//RTC_DATA_ATTR int bootCount = 0;

//#define UART_NUM_0 0

void Set_RTC_Sleep(void){
 // pinMode(BOOST_CONV_ENABLE, OUTPUT);
 // digitalWrite(BOOST_CONV_ENABLE, LOW); // Set desired state

 //Serial.println(F("Lgt Slp"));
 Serial.flush();
  esp_sleep_enable_ext0_wakeup(RTC_WAKEUP_IO, 0);  //1 = High, 0 = Low

    esp_light_sleep_start();

}

void Set_Light_Sleep(void){
 // pinMode(BOOST_CONV_ENABLE, OUTPUT);
 // digitalWrite(BOOST_CONV_ENABLE, LOW); // Set desired state

 //Serial.println(F("Lgt Slp"));
 Serial.flush();
 // esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO_KEY, 0);  //1 = High, 0 = Low

  #define BUTTON_PIN_BITMASK(GPIO) (1ULL << WAKEUP_GPIO_KEY)  // (GPIO) (1ULL << GPIO) 2 ^ GPIO_NUMBER in hex
  //esp_sleep_enable_ext1_wakeup_io(BUTTON_PIN_BITMASK(WAKEUP_GPIO_KEY), ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_sleep_enable_ext1_wakeup_io(BUTTON_PIN_BITMASK(WAKEUP_GPIO_KEY), ESP_EXT1_WAKEUP_ALL_LOW); // calisti



  
  uart_set_wakeup_threshold(UART_NUM_0, 3);
// Enable UART wake-up
 esp_sleep_enable_uart_wakeup(UART_NUM_0);
 // Set_IOs_Sleep();
  //Go to sleep now
  esp_light_sleep_start();
 //  Serial.println(F("Back from  Light Sleep.."));
   //Init_IO();
   System.Light_Sleep_Inhibit = OFF;
}

void Set_Deep_Sleep(void){
  Serial.flush();
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO_KEY, 0);  //1 = High, 0 = Low
  Set_IOs_Sleep();
/*
#if USE_EXT0_WAKEUP
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 0);  //1 = High, 0 = Low
  // Configure pullup/downs via RTCIO to tie wakeup pins to inactive level during deepsleep.
  // EXT0 resides in the same power domain (RTC_PERIPH) as the RTC IO pullup/downs.
  // No need to keep that power domain explicitly, unlike EXT1.
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);

#else  // EXT1 WAKEUP
  //If you were to use ext1, you would use it like
  esp_sleep_enable_ext1_wakeup_io(BUTTON_PIN_BITMASK(WAKEUP_GPIO), ESP_EXT1_WAKEUP_ANY_HIGH);

  //  If there are no external pull-up/downs, tie wakeup pins to inactive level with internal pull-up/downs via RTC IO
   //      during deepsleep. However, RTC IO relies on the RTC_PERIPH power domain. Keeping this power domain on will
   //      increase some power consumption. However, if we turn off the RTC_PERIPH domain or if certain chips lack the RTC_PERIPH
  //       domain, we will use the HOLD feature to maintain the pull-up and pull-down on the pins during sleep.

  rtc_gpio_pulldown_en(WAKEUP_GPIO);  // GPIO33 is tie to GND in order to wake up in HIGH
  rtc_gpio_pullup_dis(WAKEUP_GPIO);   // Disable PULL_UP in order to allow it to wakeup on HIGH
#endif
*/
  //Go to sleep now
 // Serial.println(F("Going Deep Sleep.. To wake up touch the key"));
  esp_deep_sleep_start();
 // Serial.println(F("This will never be printed"));

}
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:    //Serial.println(F("Wakeup caused by external signal using RTC_IO")); 
                                  Serial.println(F("Key pressed!"));     break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println(F("Wakeup caused by external signal using RTC_CNTL")); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println(F("Wakeup caused by timer")); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println(F("Wakeup caused by touchpad")); break;
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println(F("Wakeup caused by ULP program")); break;
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}



