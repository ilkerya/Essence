#ifdef WIFI_INCLUDE
  #include  "./Lib/NTP.h" 
//#include "NTP.h"
//#include <NTPClient.h>
//#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
void   Get_NTP_Time(void){
 if(WiFi.status() != WL_CONNECTED) return;
// while(!timeClient.update()) {
 //   timeClient.forceUpdate();
//  }
  if(!timeClient.update())timeClient.forceUpdate();
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
   //   formattedDate = timeClient.getFormattedTime();
// Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  //Serial.print("DATE: ");Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
 // Serial.print("HOUR: ");Serial.println(timeStamp);



   uint32_t Splitter1 = timeStamp.indexOf(":");
   uint32_t Splitter2 = timeStamp.indexOf(':', Splitter1 + 1);
    TimeFrag.Hour = (uint8_t)timeStamp.substring(0, Splitter1).toInt();
     TimeFrag.Minute = (uint8_t)timeStamp.substring(Splitter1 + 1, Splitter2).toInt();
     TimeFrag.Second = (uint8_t)timeStamp.substring(Splitter2 + 1).toInt(); //To the end of the string  



   Splitter1 = dayStamp.indexOf("-");
   Splitter2 = dayStamp.indexOf('-', Splitter1 + 1);
    TimeFrag.Year = (uint16_t)dayStamp.substring(0, Splitter1).toInt();
     TimeFrag.Month = (uint8_t)dayStamp.substring(Splitter1 + 1, Splitter2).toInt();
     TimeFrag.Date = (uint8_t)dayStamp.substring(Splitter2 + 1).toInt(); //To the end of the string  

     Connection.NTP_Done = ON;
/*
  Serial.print("Year: ");Serial.print(TimeFrag.Year);
  Serial.print("  Month: ");Serial.print(TimeFrag.Month);
  Serial.print("  Date: ");Serial.println(TimeFrag.Date)  ;

    Serial.print("Hour: ");Serial.print(TimeFrag.Hour);
  Serial.print("  Minute: ");Serial.print(TimeFrag.Minute);
  Serial.print("  Second: ");Serial.println(TimeFrag.Second)  ;
  */
}
void Start_NTP_Time(void){
  //if(WiFi.status() == WL_CONNECTED) {
    timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
    timeClient.setTimeOffset(3600);
    Get_NTP_Time();
//  }
 // else timeClient.end();
  
  //rtc.setTime( Second, Minute,Hour , Date, Month, Year);  // 17th Jan 2021 15:24:30
  rtc.setTime(TimeFrag.Second, TimeFrag.Minute, TimeFrag.Hour, TimeFrag.Date, TimeFrag.Month, TimeFrag.Year);  // 17th Jan 2021 15:24:30   
   //Serial.print(rtc.getTime(" %H:%M:%S %d.%B.%Y")); 
}

void Set_NTP_Time(void){
  Get_NTP_Time();
 // Serial.print(rtc.getTime(" %H:%M:%S %d.%B.%Y")); 
 // delay(1000);
  
}

#endif
