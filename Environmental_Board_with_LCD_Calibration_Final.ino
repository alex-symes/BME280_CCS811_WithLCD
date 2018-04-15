

/* Environmental Board with LCD4884; uses help from: https://arduino-info.wikispaces.com/LCD4884-Examples (wikispace is down as of July 2018) */
/* LCD4884 menu help from: https://gist.github.com/HorlogeSkynet/1df491c2ded935be178c9c6b2a026201#file-lcd4884-h-L25

/* This program uses a SparkFun Qwiic BME280+CCS811 and along with an LCD4884 shield (sainsmart version used)
 *  to display various environmental variables, such as TVOC, CO2 (TVOC implied), temp, pressure and humidity.
 *  The program loads any EEPROM saved baseline, and the interface allows for loading a baseline, saving a new 
 *  baseline and clearing EEPROM. Readings can be saved into excel using PLX-DAQ version 2.
 */

/*-----( Import needed libraries )-----*/
#include <EEPROM.h>
#include <LCD4884.h> // UPDATED version 2/16 Yourduino
#include <Wire.h>
#include <SparkFunCCS811.h>
#include <SparkFunBME280.h>
//#include <Time.h>
#include <SoftwareSerial.h>




/*-----( Declare Constants and Pin Numbers )-----*/
#define LCD_BACKLIGHT_PIN 7 
#define CCS811_ADDR 0x5B

/*-----( Declare objects )-----*/
//None: Included in library
/*-----( Declare Variables )-----*/
int displayDelay = 1000;
int switchDelay = 100; // Switch scanning delay
//int switchVoltage; // From Analog read of the button resistors


#define TIMEBACKLIGHT 5000
#define TIMELONGPRESS 1000
#define TIMEINFCT     8000
#define LCD_BACKLIGHT  7



/*----- (BUTTON VARS) ---------*/
bool bIn=0;
bool bUp=0;
bool bDown=0;
bool bLeft=0;
bool bRight=0;
bool bCalibrate = 0;
bool bMenu = 1;
bool longPress = 0;



#define NUM_KEYS       5
    short int button_count[NUM_KEYS];
    short int button_status[NUM_KEYS];
    short int button_flag[NUM_KEYS];

CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280; //Global sensor object for BME280

//Global variables obtained from the two sensors
unsigned int tVOC = 0;
unsigned int CO2 = 0;
float tempf = 0;
float humidity = 0;
float pressureInHg = 0;
unsigned long time;

/*Menu Vars */
 const char *projectName = "CO2 Meter";
 int current_menu_item = 0;
 short int min_menu_item = 0;
 short int max_menu_item = 4;

#define NUM_MENU_ITEM  5

 const char menuList[NUM_MENU_ITEM][NBCHAR_X] = {
    "Read",
    "Baseline",
    "Load",
    "Save",
    "Clear"
    
  };

void fRead(void);
void fBaseline(void);
void fLoad(void);
void fSave(void);
void fClear(void);


FONCTION menuFunction[NUM_MENU_ITEM] = {
  fRead,
  fBaseline,
  fLoad,
  fSave,
  fClear
};

/*Calibration Vars */

  unsigned int result;
  unsigned int baselineToApply;
  String HexResult;
  CCS811Core::status errorStatus;


/// These vars will allow the screen to remain while still able to read a button. See: https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
bool bEndDelay = 0; 
unsigned long previousMillis = 0;
const long interval = 1000;


/* while instead of delay */
/*int period = 2000;
unsigned long time_now = 0;
*/

void setup() /****** SETUP: RUNS ONCE ******/ {
        //Serial.begin(115200);
        Serial.begin(9600);
        pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
        lcd.LCD_init(); // creates instance of LCD
        lcd.LCD_clear(); // blanks the display
        for (int a = 0; a < 2; a++) {
            digitalWrite(LCD_BACKLIGHT_PIN, LOW);
            delay(300);
            digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
            delay(300);
        }

        lcd.LCD_write_string(0, 1, "CCS811+BME280 Read Example", MENU_NORMAL); // ignore MENU_NORMAL for now
        delay(displayDelay);

        delay(displayDelay);
        lcd.LCD_clear(); // blanks the display
        delay(500);

        CCS811Core::status returnCode = myCCS811.begin();
        if (returnCode != CCS811Core::SENSOR_SUCCESS) {
            lcd.LCD_write_string(0, 1, "Problem with CCS811", MENU_NORMAL);
            //printDriverError(returnCode);
        } else {
            lcd.LCD_write_string(0, 1, "CCS811 online", MENU_NORMAL);
        }

        //Setup the BME280 for basic readings
        myBME280.settings.commInterface = I2C_MODE;
        myBME280.settings.I2CAddress = 0x77;
        myBME280.settings.runMode = 3; //  3, Normal mode
        myBME280.settings.tStandby = 0; //  0, 0.5ms
        myBME280.settings.filter = 0; //  0, filter off
        myBME280.settings.tempOverSample = 1;
        myBME280.settings.pressOverSample = 1;
        myBME280.settings.humidOverSample = 1;

        delay(10); //Give BME280 time to come on
        //Calling .begin() causes the settings to be loaded
        byte id = myBME280.begin(); //Returns ID of 0x60 if successful
        if (id != 0x60) {
            lcd.LCD_write_string(0, 3, "Problem with BME280", MENU_NORMAL);
        } else {
            lcd.LCD_write_string(0, 3, "BME280 online", MENU_NORMAL);
        }
  
        delay(4000);

        ///START LOOKING FOR CALLIBRATED DATA
        //This looks for previously saved data in the eeprom at program start
          if ((EEPROM.read(0) == 0xA5) && (EEPROM.read(1) == 0xB2))
          {
            lcd.LCD_write_string(0, 3, "EEPROM contains saved data.", MENU_NORMAL);
            
          }
          else
          {
             lcd.LCD_write_string(0, 3, "Saved data not found!", MENU_NORMAL);
            
          }
          delay(displayDelay);
          lcd.LCD_clear(); // blanks the display
          delay(500);
          

        
          Serial.println("Program running.  Send the following characters to operate:");
          Serial.println(" 's' - save baseline into EEPROM");
          Serial.println(" 'l' - load and apply baseline from EEPROM");
          Serial.println(" 'c' - clear baseline from EEPROM");
          Serial.println(" 'r' - read and print sensor data");
      showMenu(menuList);  

        //Labels for Serial Collection 
        Serial.println("CLEARDATA");
        Serial.println("LABEL,Date,Time,CO2 (PPM),TVOC (PPB), TEMP(F),Pressure (Pa),humidity(%)"); 

       
    } //--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
   /* Check to see if need calibration or loading of data  (In button)*/ 
   unsigned long currentMillis = millis();
  // switchVoltage = analogRead(0);

    browseMenu(menuList, menuFunction);
 
   
  
}  //--(end main loop )---



  




//printDriverError decodes the CCS811Core::status type and prints the
//type of error to the serial terminal.
//
//Save the return value of any function of type CCS811Core::status, then pass
//to this function to see what the output was.
void printDriverError( CCS811Core::status errorCode )
{
  switch ( errorCode )
  {
    case CCS811Core::SENSOR_SUCCESS:
      Serial.print("SUCCESS");
      break;
    case CCS811Core::SENSOR_ID_ERROR:
      Serial.print("ID_ERROR");
      break;
    case CCS811Core::SENSOR_I2C_ERROR:
      Serial.print("I2C_ERROR");
      break;
    case CCS811Core::SENSOR_INTERNAL_ERROR:
      Serial.print("INTERNAL_ERROR");
      break;
    case CCS811Core::SENSOR_GENERIC_ERROR:
      Serial.print("GENERIC_ERROR");
      break;
    default:
      Serial.print("Unspecified error.");
  }
}

void printCalibration() {
   char buffer[10];
  //  time = millis();
    //Serial.print("DATA,DATE,TIME");
    //Serial.print(time/1000);
    String sCO2 = String("CO2: ") + dtostrf(myCCS811.getCO2(),5,1,buffer);
  
    int n = sCO2.length();
    char cCO2[n+1]; 
    strcpy(cCO2, sCO2.c_str()); 
    
   
    Serial.println();
}
//*********( THE END )***********
