//  https://www.ditig.com/256-colors-cheat-sheet

/*
First Time
git init
git add README.md
git add Branch main

GitHub Bash Commands
Adding new file
https://help.github.com/articles/adding-a-file-to-a-repository-using-the-command-line/#platform-windows

git add .

git commit -m "Add existing file"
// For Common git
git push https://github.com/ilkerya/Fragrance_Firmware main

 
In case in the first push gives error use below command
git remote add origin remote repository URL
git push origin master
git push --force https://github.com/ilkerya/Fragrance_Firmware main --force

To create a branch:
$ git branch <branch_name> 
To switch to that branch:
$ git checkout <same_branch_name> 
To do the above operations in one line, it will create and switch branch:
$ git checkout -b <new_branch> 
For example, say you want to create a branch named `learning_git`
$ git branch learning_git 
$ git checkout learning_git 
or
$ git checkout -b learning_git 
I2C scanner. Scanning ...
Found address: 26 (0x1A)
Found address: 56 (0x38) //aht20
Found address: 83 (0x53)
Found 3 device(s).
*/
#define uint8_t byte
#define uint16_t unsigned int
#define uint32_t unsigned long

#define int8_t signed char
#define int16_t signed int
#define int32_t signed long

#define ON 1
#define OFF 0

#define MODE_COUNT 3000 //  1000->100sec  600->60sec    5 minutes 5*60= 300sec->3000 
//#define MODE_COUNT 200 //  20sec  

//#define FAN_STANDBYE 64
#define RUN_OFF 0
#define RUN_LOW 1
#define RUN_MID 2
#define RUN_HIGH 3

#define RUN_TEST_LIMIT 4

#define TEST_OFF 5
#define TEST_LOW 6
#define TEST_MID 7
#define TEST_HIGH 8

#define PWM_MINIMUM 1 //

#define ONLY_BATTERY 32
#define ONLY_USB     16
#define BATTERY_USB  64
#define USB_ON_LEVEL  1024  // adc level    5V  47K / 10 K GND
/*
#define LED_PIN 13  // GPIO13_A12 TOUCH4
#define BATTERY_ADC 35 //I35/A1_7 A13_I35
#define FAN_POWER 12 // IO12_A11 OE  TOUCH5
#define I2C_1_PWR 14  // TOUCH6 
#define I2C_2_PWR 15 // TOUCH3 
#define USB_DETECT_ADC 33 // ADC 9 IO33 JP3->7  KICAD-> IO22 
*/
///#define I2C_1_PWR 14  // TOUCH6 

//#define FIRST_PROTOTYPE // 
#define SECOND_PROTOTYPE // 2026.06.01

#ifdef FIRST_PROTOTYPE
  #define BATTERY_ADC 35 //I35/A1_7 A13_I35
  #define BAT_CHARGE 22 //
  #define BAT_STANDBYE 21 //
  //#define BAT_VOLT 15 //34 RTC_GPIO4 eski hali yeri suan bos
   #define FAN_PWM  15 //SENSOR_VN  39 du
  //pinMode(BAT_VOLT, INPUT);  
  //#define USB_DETECT 35 // 39//SENSOR_VN ile yer degisti  
 //   pinMode(USB_DETECT, INPUT);  
  #define FAN_FEEDBACK  36//SENSOR_VP 
  #define BOOST_CONV_POWER 2 //34 RTC_GPIO4 

  #define KEY 4
  #define LED_CANDLE 26//IO23 attach tp NFC_RST
  #define NFC_RST 26//IO23 attach tp 
  
  #define SENSOR_3V_POWER 23//IO23
  #define SENSOR_3V_ENABLE 1// FALSE Logic
  #define SENSOR_3V_DISABLE 0// 
#endif
#ifdef SECOND_PROTOTYPE
  #define BATTERY_ADC 34
  #define BAT_CHARGE 22 //
  #define BAT_STANDBYE 21 //
  #define FAN_PWM  32 //
  #define FAN_FEEDBACK  36//SENSOR_VP 
  #define USB_ADC 35 // 39//SENSOR_VN ile yer degisti  
  #define BOOST_CONV_POWER 15 //34 RTC_GPIO4    
  #define KEY 4
  #define LED_CANDLE 33//

  #define NFC_IRQ 25// input
  #define NFC_RST 26//  output
  #define NFC_CS 27//  output
  #define NFC_CLK 14//   output
  #define NFC_MISO 12// input  nfc -> uc
  #define NFC_MOSI 13// output uc-> nfc

  #define PERIPHERAL_POWER  2 //34 RTC_GPIO4    

  #define PERIPHERAL_ENABLE 1// 
  #define PERIPHERAL_DISABLE 0// 

#endif



  
 // #define LED_BLUE 5 //34 RTC_GPIO4 
 // #define LED_GREEN 17// RTC_GPIO3 
 // #define LED_RED 16// no



  uint8_t LED_BLUE = 5; //34 RTC_GPIO4 330R                           500R
  uint8_t LED_GREEN = 17;// RTC_GPIO3 330 x5.5 = 1815R               2750R
  uint8_t LED_RED = 16;// no 330 x2 = 660R                           1000R
  #define SDA  18
  #define SCL  19

  #define MIN_PWM  18
  // 255/5=52

/*
  #define TASK_1MSEC     1000
  #define TASK_5MSEC     5000   
  #define TASK_10MSEC   10000
  #define TASK_20MSEC   20000
  #define TASK_50MSEC   50000
  #define TASK_100MSEC 100000
  #define TASK_250MSEC 250000
  #define TASK_500MSEC 500000
  #define TASK_1SEC   1000000
  
  #define TASK_2SEC   2000000
  #define TASK_5SEC   5000000
  #define TASK_10SEC 10000000
  #define TASK_20SEC 200
  #define TASK_60SEC 202 
  */
// 001BC5067010E312
/*
typedef  byte               uint8;
typedef  signed char        int8;
typedef  unsigned char      uint8;
//typedef  signed short       int16;
//typedef  unsigned short     uint16;
typedef  signed int         int16;
typedef  unsigned int       uint16;
typedef  signed long        int32;
typedef  unsigned long      uint32;
typedef  signed long long   int64;
typedef  unsigned long long uint64
C:\Program Files (x86)\Arduino\libraries
*/

  #define ANALOG_RES_12BIT
  //#define ANALOG_RES_10BIT

 // #define DEBUG_SIMULATOR_MODE // For DEbugging As A Simulator
// Select Hardware Type
//#define FIRST_PROTOTYPE  // with LEM current Transdcucer

#define ON 1 //
#define OFF 0 //
void Key_Press(void);
void Color_High(void);
void Color_Mid(void);
void Color_Low(void);
void Key_Mode_Update(void);
void Convert24bitToRGB(uint32_t color24, uint8_t *r, uint8_t *g, uint8_t *b);
 void Color_Dec2Hex(void);
void SystemTimers(void);
void Fan_Feedback(void) ;
void Reset_Run_Modes(void);
void  SetColor(uint8_t ,uint8_t );
void Set_Light_Sleep(void);
void Set_Deep_Sleep(void);
void Led_Control(void);
void Rpm_Calculate(void);

void Fan_Standbye(void);
void Device_OFF(void);
void Position_Mid(void);
void Position_High(void);
void Position_Low(void);
void PI_Control(void);
// function prototypes
void Key_Functions_Digital(void);
void Common_Loop(); 
void ResetCasePrint();
void IO_Settings();

/*
void MicroInit(void);
void Display_ReInit_Start(uint8_t Timer);
void Display_ReInit_End(void);



void RTC_Init();
void SensorInit_Si072(uint8_t);
void SensorAlt_Init();
void SensorLight_Init();
void SensorACccel_GyroInit();
void Sensor_LidarInit(void);
void Sensors_PeripInit();


*/


/*
C:\Users\ilker\Documents\Atmel Studio\7.0\ArduinoSketch6\ArduinoSketch6\ArduinoCore\src\libraries\SD\utility\Sd2Card.cpp 
 // send command and return error code.  Return zero for OK
uint8_t Sd2Card::cardCommand(uint8_t cmd, uint32_t arg) {
  chipSelectLow();


  static uint8_t chip_select_asserted = 0;

void Sd2Card::chipSelectHigh(void) {
  digitalWrite(chipSelectPin_, HIGH);
  #ifdef USE_SPI_LIB
  if (chip_select_asserted) {
    chip_select_asserted = 0;
    SDCARD_SPI.endTransaction();
  }
  #endif
}
//------------------------------------------------------------------------------
void Sd2Card::chipSelectLow(void) {
  #ifdef USE_SPI_LIB
  if (!chip_select_asserted) {
    chip_select_asserted = 1;
    SDCARD_SPI.beginTransaction(settings);
  }
  #endif
  digitalWrite(chipSelectPin_, LOW);
}


 * /
 */

 
