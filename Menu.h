
#define FADE_TIME 2
void Key_Mode_Update(void) {
      System.Mode++;
      if(System.Mode <= RUN_TEST_LIMIT){
        if(System.Mode > RUN_HIGH)System.Mode = RUN_OFF;
        Reset_Run_Modes();
      }
      else{
        if(System.Mode > TEST_HIGH)System.Mode = TEST_OFF;    
      } 
      Color.Fade = OFF;
       Key.ColorFade_timer = FADE_TIME;
    //  Set_Sleep_Off_Key();
}
#define DOUBLEPRESS 1
#define SINGLEPRESS 0
void Key_Functions_Digital(void) {
    if(Key.Double_Press_timeout){
      Key.Double_Press_timeout--;
      if(Key.Double_Press_timeout==0 && Key.Mode == DOUBLEPRESS) Key.Mode = SINGLEPRESS;     
    }
   
  Key.Key1 = digitalRead(KEY); //release 1
  if (!Key.Key1_Rel && Key.Key1) {  // default
    Key.TimerPress = 0;
    return;
  }
  if(!Key.Key1_Rel && !Key.Key1) {  // key1 pressedd   Key.Key1_Rel = 0 normally
    Key.Key1_Rel = 1;//    0 && 0   rel && press
    Key.TimerPress ++;
    return;
  }
  if(Key.Key1_Rel && !Key.Key1) {  // still pressed
    Key.TimerPress ++; 
    if(Key.TimerPress > 350)ESP.restart(); //20ms*350 = 7000mS 7 sec
  }
  if(Key.Key1_Rel && Key.Key1) {  // key released job done
    Key.Key1_Rel = 0;
    if((Key.Inhibit_Timer == 0) && (!Key.Inhibit)){
        Key_Press();
    }
    if(Key.Inhibit)Key.Inhibit = OFF;
   }
}

void Key_Press(void){

  if(Key.Mode == SINGLEPRESS){
    if(Key.TimerPress > 6) {
      Key_Mode_Update(); //normal key pres
    }
    else {
      Key.Mode = DOUBLEPRESS;
      Key.Double_Press_timeout = 12;
    }
  }
  else{ 
    Key.Mode = SINGLEPRESS;
     Key.DoubPress = ON;
     Led.Candle = !Led.Candle;
     // digitalWrite(LED_CANDLE, ON);
  }
}
   
#define SECONDS_10 10
 void Reset_Run_Modes(void) {
  System.Index = 0;
  System.RunTimer = 0;
  System.Index_UpdateTimer = SECONDS_10;
  System.Light_SleepTimer  = 25;
}

#define TIMESCALE 600 // 60 minutes x 100msec = 60x10 = 600
bool Run_Mode_Timer(uint8_t*p) {
  uint8_t i;
  bool FanStatus = OFF;
   System.RunTimer++;
  if(System.Index> (p[4])){
    Reset_Run_Modes();
    System.Mode = RUN_OFF;
    return OFF;
  }
  if(System.Index >= 4) i = System.Index % 4; // 4 cycles
  else  i = System.Index ;
  if((System.Index & 0X01) == 0X00){ // even number
     FanStatus = ON;
     digitalWrite(BOOST_CONV_POWER, ON);   
      if(System.RunTimer > (p[i]*TIMESCALE)){
        System.RunTimer = 0;
        System.Index++;
        System.Index_UpdateTimer = SECONDS_10;
      }
  }
  else { // odd
      Fan.DutyCycle = PWM_MINIMUM;   
      digitalWrite(BOOST_CONV_POWER, OFF);// 
      FanStatus = OFF;    
      if(System.RunTimer > (p[i]*TIMESCALE)){
        System.RunTimer = 0;
        System.Index++;
        System.Index_UpdateTimer = SECONDS_10;
      } 
  }
  return FanStatus;
 }

void Convert24bitToRGB(uint32_t color24, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = (color24 >> 16) & 0xFF; // Extract red (bits 16-23)
    *g = (color24 >> 8) & 0xFF;  // Extract green (bits 8-15)
    *b = color24 & 0xFF;        // Extract blue (bits 0-7)
}
void Color_High(void){
  uint8_t Fader = 1;
  if (Color.Fade)Fader = 8;
  ledcWrite(LED_RED, (Color.High_R*4)/Fader);  // write red component to channel 1, etc.
  ledcWrite(LED_GREEN, (Color.High_G*4)/Fader);
  ledcWrite(LED_BLUE, (Color.High_B*4)/Fader);
}
void Color_Mid(void){
  uint8_t Fader = 1;
  if (Color.Fade)Fader = 8;
  ledcWrite(LED_RED, (Color.Mid_R*4)/Fader);  // write red component to channel 1, etc.
  ledcWrite(LED_GREEN, (Color.Mid_G*4)/Fader);
  ledcWrite(LED_BLUE, (Color.Mid_B*4)/Fader);
}
void Color_Low(void){
  uint8_t Fader = 1;
  if (Color.Fade)Fader = 8;
  ledcWrite(LED_RED, (Color.Low_R*4)/Fader);  // write red component to channel 1, etc.
  ledcWrite(LED_GREEN, (Color.Low_G*4)/Fader);
  ledcWrite(LED_BLUE, (Color.Low_B*4)/Fader);
}

void System_Set_Off(void){
      ledcWrite(LED_RED, 0);  // write red component to channel 1, etc.
      ledcWrite(LED_GREEN, 0);
      ledcWrite(LED_BLUE, 0);
      Fan.DutyCycle = PWM_MINIMUM;   
      digitalWrite(BOOST_CONV_POWER, OFF);// 

}
 void Mode_Select(void) {
  if(System.Light_Sleep || System.RTC_Sleep){
      System_Set_Off();  
      ledcWrite(FAN_PWM, 255-((Fan.DutyCycle*255)/100) ); 
      Key.Inhibit = ON;
      if(System.Light_Sleep){
        System.Light_Sleep = OFF;  
        Set_Light_Sleep();
      }
      if(System.RTC_Sleep){
        System.RTC_Sleep = OFF;   
        Set_RTC_Sleep();
      }         
    return;
  }
  switch(System.Mode){
    case TEST_OFF :
      System_Set_Off();
      break;
    case RUN_OFF :
      System_Set_Off();
      break;
    case TEST_HIGH :     
      Fan.DutyCycle =Fan.HighSpeed; 
      Color_High();
      break;
    case TEST_MID : 
      Fan.DutyCycle = Fan.MidSpeed; 
      Color_Mid();
      break;
    case TEST_LOW : 
      Fan.DutyCycle =Fan.LowSpeed;  
   Color_Low();
      break;
    case RUN_HIGH :
      if(Run_Mode_Timer(&System.Time_High[0])) Fan.DutyCycle =Fan.HighSpeed; 
      Color_High();
      break;
    case RUN_MID :
       if(Run_Mode_Timer(&System.Time_Mid[0]))Fan.DutyCycle =Fan.MidSpeed; 
      Color_Mid();
      break;
    case RUN_LOW :
      if(Run_Mode_Timer(&System.Time_Low[0]))Fan.DutyCycle =Fan.LowSpeed; 
    Color_Low();
      break;
    default:
    break;
    }
   if(!(System.Mode == TEST_OFF || System.Mode == RUN_OFF)){
      digitalWrite(BOOST_CONV_POWER, ON);
   //  SetColor(Led.Color,Led.Bright); // Color // brightness 
  }else 
    System_Set_Off();

    ledcWrite(FAN_PWM, 255-((Fan.DutyCycle*255)/100));   
}
/*
void hueToRGB(uint8_t hue, uint8_t brightness) {
  uint16_t scaledHue = (hue * 6);
  uint8_t segment = scaledHue / 256;                     // segment 0 to 5 around the
                                                         // color wheel
  uint16_t segmentOffset = scaledHue - (segment * 256);  // position within the segment

  uint8_t complement = 0;
  uint16_t prev = (brightness * (255 - segmentOffset)) / 256;
  uint16_t next = (brightness * segmentOffset) / 256;

  if (Led.invert) {
    brightness = 255 - brightness;
    complement = 255;
    prev = 255 - prev;
    next = 255 - next;
  }

  switch (segment) {
    case 0:  // red
      Led.R = brightness;
      Led.G = next;
      Led.B = complement;
      break;
    case 1:  // yellow
      Led.R = prev;
      Led.G = brightness;
      Led.B = complement;
      break;
    case 2:  // green
      Led.R = complement;
     Led.G = brightness;
      Led.B = next;
      break;
    case 3:  // cyan
      Led.R = complement;
      Led.G = prev;
      Led.B = brightness;
      break;
    case 4:  // blue
      Led.R = next;
      Led.G = complement;
      Led.B = brightness;
      break;
    case 5:  // magenta
    default:
      Led.R = brightness;
      Led.G = complement;
      Led.B = prev;
      break;
  }
}
void  SetColor(uint8_t Col,uint8_t Brg){
    hueToRGB(Col, Brg);  // call function to convert hue to RGB
    // write the RGB values to the pins
    ledcWrite(LED_RED, Led.R);  // write red component to channel 1, etc.
    ledcWrite(LED_GREEN, Led.G);
    ledcWrite(LED_BLUE, Led.B);
}
*/
/*
void Scanner ()
{
  Serial.println ();
  Serial.println (F("I2C scanner. Scanning ..."));
  byte count = 0;

  //Wire.begin();
    Wire.begin (SDA, SCL);   // sda= GPIO_18 /scl= GPIO_19
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
    if (Wire.endTransmission () == 0)  // Receive 0 = success (ACK response)
    {
      Serial.print (F("Found address: "));
      Serial.print (i, DEC);
      Serial.print (F(" (0x"));
      Serial.print (i, HEX);     // PCF8574 7 bit address
      Serial.println (")");
      count++;
    }
  }
  Serial.print (F("Found "));
  Serial.print (count, DEC);        // numbers of devices
  Serial.println (F(" device(s)."));
}
*/

