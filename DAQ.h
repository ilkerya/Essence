  


#define INTERRUPT_BASED 1
#define LOOP_BASED 0





void DAQ_Send_Data(bool Int_Loop);

#define  PREAMBLE_BYTES  2 // preamble+length 4 byte + 2 byte
#define DATALENGTH_BYTES  2
#define CRC_BYTES 2
#define DEFAULT_PREAMBLE = 0XAAAA;
#define DEFAULT_CRC_INIT 0XAAAA; // preamble+length 4 byte + 4 byte
//#define DEFAULT_LENGTH  133//26 // The whole packet including preamble and CRC
#define DEFAULT_LENGTH  142
/*
uint8_t Counter_100mS;
uint8_t Counter_10mS;
bool Tick_100mS;
bool Tick_10mS;
*/
struct DAQ{
  uint8_t Busy;
  uint8_t SendEnable; // SeriPort.SendEnable
  uint8_t ContBytes;
  uint16_t Length;
  uint8_t SendBuf[DEFAULT_LENGTH];
  uint16_t CRC_Send;
}DAQ; 

struct Accelometer{
  float  x;
  float  y;
  float  z;   
}Accelometer;

int32_t GetMod_Float(float Acc){
   if(Acc > 128)  return 1280000;
   if(Acc < -128) return -1280000;
   return (int32_t)(Acc*10000)  ;
  }
  uint16_t ShowCounter; 
void DAQ_Send_Data(bool Int_Loop){

  uint8_t *p;
  p = &DAQ.SendBuf[0];
  uint8_t i = 0; //Uint32 CRC = 0;

  p[0] = 0XAA; // preamble D170
  p[1] = 0XAA; // preamble D170
  
  DAQ.Length = DEFAULT_LENGTH;
  p[2] = DAQ.Length >> 8;
  p[3] = DAQ.Length; //  packet total size

    ShowCounter++;

   // Multi_Gas_1.VOC = ShowCounter;
   // Multi_Gas_2.VOC = ShowCounter+1;
   // Multi_Gas_3.VOC = ShowCounter+2;
   // Multi_Gas_4.VOC = ShowCounter+3;
   // Values.Temperature_OnBoard = 23.56;
   // Values.Humidity_OnBoard = 52.45;

uint32_t Value_u32_PC = 45000;

  p[4] = (uint8_t)(Value_u32_PC >> 24);
  p[5] = (uint8_t)(Value_u32_PC >> 16);
  p[6] = (uint8_t)(Value_u32_PC >> 8);
  p[7] = Value_u32_PC;


  
//////////////////////////////////////////////////////////
/*
  uint8_t* p2;
  p2 =(uint8_t*) &Values.Temperature_OnBoard ;

  p[68] = p2[0];//
  p[69] = p2[1];//
  p[70] = p2[2];// 
  p[71] = p2[3];// 

  p2 =(uint8_t*) &Values.Humidity_OnBoard ;

  p[72] = p2[0];// 
  p[73] = p2[1];//; 
  p[74] = p2[2];//;
  p[75] = p2[3];// 
*/




/*
  int32_t Temp;
  Temp = (int32_t)roundf(Bosch_BME688_1.Temperature *100);
  p[68] = (uint8_t)(Temp >> 24); 
  p[69] = (uint8_t)(Temp >> 16); 
  p[70] = (uint8_t)(Temp >> 8); 
  p[71] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_1.Humidity *100);
  p[72] = (uint8_t)(Temp >> 24); 
  p[73] = (uint8_t)(Temp >> 16); 
  p[74] = (uint8_t)(Temp >> 8); 
  p[75] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_1.Gas *100);
  p[76] = (uint8_t)(Temp >> 24); 
  p[77] = (uint8_t)(Temp >> 16); 
  p[78] = (uint8_t)(Temp >> 8); 
  p[79] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_1.Pressure *100);
  p[80] = (uint8_t)(Temp >> 24); 
  p[81] = (uint8_t)(Temp >> 16); 
  p[82] = (uint8_t)(Temp >> 8); 
  p[83] = Temp ;

  Temp = (int32_t)roundf(Bosch_BME688_2.Temperature *100);
  p[84] = (uint8_t)(Temp >> 24); 
  p[85] = (uint8_t)(Temp >> 16); 
  p[86] = (uint8_t)(Temp >> 8); 
  p[87] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_2.Humidity *100);
  p[88] = (uint8_t)(Temp >> 24); 
  p[89] = (uint8_t)(Temp >> 16); 
  p[90] = (uint8_t)(Temp >> 8); 
  p[91] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_2.Gas *100);
  p[92] = (uint8_t)(Temp >> 24); 
  p[93] = (uint8_t)(Temp >> 16); 
  p[94] = (uint8_t)(Temp >> 8); 
  p[95] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_2.Pressure *100);
  p[96] = (uint8_t)(Temp >> 24); 
  p[97] = (uint8_t)(Temp >> 16); 
  p[98] = (uint8_t)(Temp >> 8); 
  p[99] = Temp ;

  Temp = (int32_t)roundf(Bosch_BME688_3.Temperature *100);
  p[100] = (uint8_t)(Temp >> 24); 
  p[101] = (uint8_t)(Temp >> 16); 
  p[102] = (uint8_t)(Temp >> 8); 
  p[103] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_3.Humidity *100);
  p[104] = (uint8_t)(Temp >> 24); 
  p[105] = (uint8_t)(Temp >> 16); 
  p[106] = (uint8_t)(Temp >> 8); 
  p[107] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_3.Gas *100);
  p[108] = (uint8_t)(Temp >> 24); 
  p[109] = (uint8_t)(Temp >> 16); 
  p[110] = (uint8_t)(Temp >> 8); 
  p[111] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_3.Pressure *100);
  p[112] = (uint8_t)(Temp >> 24); 
  p[113] = (uint8_t)(Temp >> 16); 
  p[114] = (uint8_t)(Temp >> 8); 
  p[115] = Temp ;

  Temp = (int32_t)roundf(Bosch_BME688_4.Temperature *100);
  p[116] = (uint8_t)(Temp >> 24); 
  p[117] = (uint8_t)(Temp >> 16); 
  p[118] = (uint8_t)(Temp >> 8); 
  p[119] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_4.Humidity *100);
  p[120] = (uint8_t)(Temp >> 24); 
  p[121] = (uint8_t)(Temp >> 16); 
  p[122] = (uint8_t)(Temp >> 8); 
  p[123] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_4.Gas *100);
  p[124] = (uint8_t)(Temp >> 24); 
  p[125] = (uint8_t)(Temp >> 16); 
  p[126] = (uint8_t)(Temp >> 8); 
  p[127] = Temp ;

 Temp = (int32_t)roundf(Bosch_BME688_4.Pressure *100);
  p[128] = (uint8_t)(Temp >> 24); 
  p[129] = (uint8_t)(Temp >> 16); 
  p[130] = (uint8_t)(Temp >> 8); 
  p[131] = Temp ;
*/
/*
  p[132] = (uint8_t)(Battery.Volt >> 8);
  p[133] = Battery.Volt;

  p[134] = Battery.Power;

  p[135] = (uint8_t)(Fan.Volt >> 8);
  p[136] = Fan.Volt;

  p[137] = (uint8_t)(Values.Luminosity >> 8);
  p[138] = Values.Luminosity;

  p[139] = (uint8_t)(Values.Infrared >> 8);
  p[140] = Values.Infrared;
*/


  DAQ.CRC_Send = DEFAULT_CRC_INIT;//2-14
  for(i=PREAMBLE_BYTES; i< (DEFAULT_LENGTH-CRC_BYTES); i++){ // crc haric
    DAQ.CRC_Send ^= p[i];
  }
  
  DAQ.CRC_Send <<= 8;
  for(i=PREAMBLE_BYTES; i< (DEFAULT_LENGTH-CRC_BYTES); i++){ // crc haric
    DAQ.CRC_Send ^= p[i];
  }

  p[DEFAULT_LENGTH-2] = DAQ.CRC_Send >> 8;// DEFAULT_LENGTH-2
  p[DEFAULT_LENGTH-1] = DAQ.CRC_Send; // DEFAULT_LENGTH-1
   
   //for(i=0; i< DEFAULT_LENGTH; i++){ // crc haric
   //Serial1.write(p[i]);
    //}
    Serial.write(p, DEFAULT_LENGTH);
      // for(i=0; i< DEFAULT_LENGTH; i++){ // crc haric
       // Serial.write(p[i]);}
  
   /*
  if(!Int_Loop){
    // if interrupt based do nor execute
     for(i=0; i< DEFAULT_LENGTH; i++){ // crc haric
      Serial.print(i);Serial.print(": ");
      Serial.print(p[i]);Serial.print(" ");
    }
    Serial.println();
    
  }
  */

}

  
  