//#include "TempRh.h" // use the files in the project directory 
 #include  "./Lib/TempRh.h"
  #include  "./Lib/Light.h"
  #include  "./Lib/Voc.h" 
DFRobot_AHT20 aht20;
      uint8_t status;

//#include "Light.h"    // use the files in the project directory 
//#include "Frag_LTR308.cpp" 
 //#include <LTR308.h>  // not using from the standart library anymore
  LTR308  light;
      
//#include "Voc.h"   // use the files in the project directory 
 //#include <AGS10.h> // not using from the standart library anymore
 AGS10 sensor = AGS10();

// declare data variable
//int32_t tvoc;
uint8_t Que_tvoc=2;
void Init_TVoc(){
      sensor.begin();
}
void Read_TVoc(){
        //  read TVOC sensor data every 1.6 seconds, according to datasheet max speed is 1.5 seconds/reading
      Que_tvoc++;
      if(Que_tvoc > 2){
            Que_tvoc = 0;
           // tvoc = sensor.readTVOC();  
            Values.TVoc =  sensor.readTVOC();    
            if(Values.TVoc <= 24)  Values.TVoc_Error = ON;
            Values.TVoc_Error = OFF;
      }
    //  Serial.print("  TVOC:");Serial.print(Values.TVoc);Serial.print("ppb  ");
}

// Global variables:
   
// If gain = 0, device is set to 1X gain
// If gain = 1, device is set to 3X gain (default)
// If gain = 2, device is set to 6X gain
// If gain = 3, device is set to 9X gain
// If gain = 4, device is set to 18X gain
unsigned char gain = 1;     // Gain setting, values = 0-4 

//------------------------------------------------------
// If integrationTime = 0, integrationTime will be 400ms / 20 bits result
// If integrationTime = 1, integrationTime will be 200ms / 19 bits result 
// If integrationTime = 2, integrationTime will be 100ms / 18 bits result (default)
// If integrationTime = 3, integrationTime will be 50ms / 17 bits result 
// If integrationTime = 4, integrationTime will be 25ms / 16 bits result 
unsigned char integrationTime = 0;  // Integration ("shutter") time, values 0 - 4

//------------------------------------------------------
// If measurementRate = 0, measurementRate will be 25ms
// If measurementRate = 1, measurementRate will be 50ms
// If measurementRate = 2, measurementRate will be 100ms (default)
// If measurementRate = 3, measurementRate will be 500ms
// If measurementRate = 5, measurementRate will be 1000ms
// If measurementRate = 6, measurementRate will be 2000ms
// If measurementRate = 7, measurementRate will be 2000ms
unsigned char measurementRate = 3;  // Interval between DATA_REGISTERS update, values 0 - 7, except 4

//------------------------------------------------------
// Chip ID - should be 0xB1 for all LTR-308
unsigned char ID;

//------------------------------------------------------
// Main Control Register
unsigned char control;


void Init_TempHSensors(){
   if((status = aht20.begin()) != 0)Values.Temp_Error = ON; // fail
   else Values.Temp_Error = OFF; // success
}

void Read_Temperature(){

  if(Values.Temp_Error){
     // Serial.println("AHT20 failed. ");
      Values.Temperature = NAN;
       Values.Humidity = NAN;
       Init_TempHSensors();
    return;
  }
  if(aht20.startMeasurementReady(/* crcEn = */true)){
   // Serial.print("temperature(-40~85 C): ");
    // Get temp in Celsius (℃), range -40-80℃
    Values.Temperature = aht20.getTemperature_C();
  //  Serial.print(Values.Temperature);    Serial.print("°C   Rh%"); Serial.print(Values.Humidity);   
    // Get temp in Fahrenheit (F)
 //   Serial.print(aht20.getTemperature_F());
  //  Serial.print(" F\t");
  //  Serial.print("humidity(0~100): ");
    // Get relative humidity (%RH), range 0-100℃

    Values.Humidity = aht20.getHumidity_RH();
  //  Serial.print(aht20.getHumidity_RH());
       
  //  Serial.println(" %RH");
   // delay(1000);
  }
  else{
      Values.Temperature = NAN;
       Values.Humidity = NAN;
        Init_TempHSensors();
  }
}
void printError(byte error) {
  // If there's an I2C error, this function will
  // print out an explanation.
 // Serial.print("L.Sen.err ");
  Values.Lux_Error = ON;
  return;

  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error) {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}
void Init_Light_Sensor(){
  light.begin();
  // Get factory ID from sensor:
  // (Just for fun, you don't need to do this to operate the sensor)
  if (light.getPartID(ID)) {
 //   Serial.print("Got Sensor Part ID: 0X");
 //   Serial.print(ID, HEX);
  //  Serial.println();
    Values.Lux_Error = OFF;
  }
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else {
    byte error = light.getError();
    printError(error);
  }
  // To start taking measurements, power up the sensor
  if (light.setPowerUp()) {
 //   Serial.print("Powering up...");
  //  Serial.println();
  }
  else {
    byte error = light.getError();
    printError(error);
  }
  // Allow for a slight delay in power-up sequence (typ. 5ms from the datasheet)
  delay(10);

  if (light.getPower(control)) {
  //  Serial.print("Control byte is: 0X");
   // Serial.print(control, HEX);
  //  Serial.println();
  }
  else {
    byte error = light.getError();
    printError(error);
  }
  // The light sensor has a default integration time of 100ms,
  // and a default gain of low (3X).
  // If you would like to change either of these, you can
  // do so using the setGain() and setMeasurementRate() command.
 // Serial.println("Setting Gain...");
  
  if (light.setGain(gain)) {
  //  light.getGain(gain);
 //   Serial.print("Gain Set to 0X");
 //   Serial.print(gain, HEX);
 //   Serial.println();
  }
  else {
    byte error = light.getError();
    printError(error);
  }

  //Serial.println("Set timing...");
  if (light.setMeasurementRate(integrationTime, measurementRate)) {
    light.getMeasurementRate(integrationTime, measurementRate);
    /*
    Serial.print("Timing Set to ");
    Serial.print(integrationTime, HEX);
    Serial.println();
    Serial.print("Meas Rate Set to ");
    Serial.print(measurementRate, HEX);
    Serial.println();
    */
  }
  else {
    byte error = light.getError();
    printError(error);
  }
}

unsigned long rawData;

void Read_Light(){ 
    float lux_f;    // Resulting lux value 
    if(Values.Lux_Error){
        Init_Light_Sensor();
        return;
  }
  if (light.getData(rawData)){
   // Serial.print("Raw Data: ");
   // Serial.println(rawData);
  
    // To calculate lux, pass all your settings and readings
    // to the getLux() function. 
    // The getLux() function will return 1 if the calculation
    // was successful, or 0 if the sensor was
    // saturated (too much light). If this happens, you can
    // reduce the integration time and/or gain.
    double lux;    // Resulting lux value
   // boolean good;  // True if sensor is not saturated
    // Perform lux calculation:
    Values.Lux_Error = OFF;
    boolean good = light.getLux(gain, integrationTime, rawData, lux);
    good = good;
      lux_f =(float)lux;
    // Print out the results:
  //  Serial.print(Values.Lux); Serial.println("Lux"); 	
 //   Serial.print(lux_f); Serial.println("Lux"); 
  //  Serial.println(lux);
   // if (good) Serial.println(" (valid data)"); 
   // else Serial.println(" (BAD)");
  }
  else {
    byte error = light.getError();
    printError(error);
  }
    if(!Values.Lux_Error)Values.Lux = (float)lux_f;
    else Values.Lux= NAN;
}



