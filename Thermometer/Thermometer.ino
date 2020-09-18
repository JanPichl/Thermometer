/*
 * Program vyčítá teplotu ze snímačů teploty DS18B20 a vypisuje ji na displej
 * Stiskem obou tlačítek zahájí/ukončí režim nastavení zobrazovaného pořadí 
 * 
 * 
 * Procesor ATMEGA328
 * 
 * 
 * 
 */

#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <OneWire.h>

#define I2C_PIN 4

#define DISPLAY_RESET 10
#define DISPLAY_ENABLED 9
#define DISPLAY_DATA_4 8 //digital pin 5
#define DISPLAY_DATA_5 7 //digital pin 4
#define DISPLAY_DATA_6 6 //digital pin 3
#define DISPLAY_DATA_7 5 //digital pin 2

#define MAX_COUNT_SENSORS 8  //Počet použitých snímačů teploty
#define SENSOR_RESOLUTION 10 //9,10,11,12 bit

#define BTN_SETTING 11 //pin18
#define BTN_VALUE 12   //pin17

byte SensorArray[MAX_COUNT_SENSORS][8]; // [senzor] [adresa 8 bit]

int SensorCount = 0;

int transferDelay = 1000;

int SensorPositions[MAX_COUNT_SENSORS]; // poradí v poli je po setřídění a hodnota je umístění na displeji

// inicializace I2C sběrnice se senzory
OneWire dallas(I2C_PIN);

// inicializace LCD displeje
LiquidCrystal lcd(DISPLAY_RESET, DISPLAY_ENABLED, DISPLAY_DATA_4, DISPLAY_DATA_5, DISPLAY_DATA_6, DISPLAY_DATA_7);

void setup()
{

  pinMode(BTN_SETTING, INPUT_PULLUP);

  pinMode(BTN_VALUE, INPUT_PULLUP);

  SetupDisplay();

  SetupSensors();

  SetupEprom();
}

void loop()
{
  // stisk obou tlačítek vyvolá nastavení
  if ((digitalRead(BTN_SETTING) == LOW) && (digitalRead(BTN_VALUE) == LOW))
  {
    NastaveniPoradi();
  }
  else
  {
    ReadSensorValues();
  }
}

void NastaveniPoradi()
{
  bool exit = false;
  int cursorPosition = 8;

  lcd.blink();

  WriteDisplay("Snimac ", 0, 0, true, 0);
  WriteDisplay("1", 8, 0, false, 0);
  WriteDisplay("2", 9, 0, false, 0);
  WriteDisplay("3", 10, 0, false, 0);
  WriteDisplay("4", 11, 0, false, 0);
  WriteDisplay("5", 12, 0, false, 0);
  WriteDisplay("6", 13, 0, false, 0);
  WriteDisplay("7", 14, 0, false, 0);
  WriteDisplay("8", 15, 0, false, 0);

  WriteDisplay("Poradi ", 0, 1, false, 0);
  WriteDisplay(String(SensorPositions[0]), 8, 1, false, 0);
  WriteDisplay(String(SensorPositions[1]), 9, 1, false, 0);
  WriteDisplay(String(SensorPositions[2]), 10, 1, false, 0);
  WriteDisplay(String(SensorPositions[3]), 11, 1, false, 0);
  WriteDisplay(String(SensorPositions[4]), 12, 1, false, 0);
  WriteDisplay(String(SensorPositions[5]), 13, 1, false, 0);
  WriteDisplay(String(SensorPositions[6]), 14, 1, false, 0);
  WriteDisplay(String(SensorPositions[7]), 15, 1, false, 0);

  do
  {
    // Potlačení zákmitu tlačítka
    delay(300);

    lcd.setCursor(cursorPosition, 1);

    // tisknuto tlačítko nastavení
    if ((digitalRead(BTN_SETTING) == LOW) && (digitalRead(BTN_VALUE) == HIGH))
    {
      if (cursorPosition++ == 15)
      {
        cursorPosition = 8;
      }
    }

    // Stisknuto tlačítko hodnoty
    if ((digitalRead(BTN_SETTING) == HIGH) && (digitalRead(BTN_VALUE) == LOW))
    {
      switch (cursorPosition)
      {
      case 8:
        if (SensorPositions[0]++ == 8)
        {
          SensorPositions[0] = 1;
        }

        lcd.write((char)String(SensorPositions[0])[0]);
        break;

      case 9:
        if (SensorPositions[1]++ == 8)
        {
          SensorPositions[1] = 1;
        }

        lcd.write((char)String(SensorPositions[1])[0]);
        break;

      case 10:
        if (SensorPositions[2]++ == 8)
        {
          SensorPositions[2] = 1;
        }

        lcd.write((char)String(SensorPositions[2])[0]);
        break;

      case 11:
        if (SensorPositions[3]++ == 8)
        {
          SensorPositions[3] = 1;
        }
        lcd.write((char)String(SensorPositions[3])[0]);
        break;

      case 12:
        if (SensorPositions[4]++ == 8)
        {
          SensorPositions[4] = 1;
        }
        lcd.write((char)String(SensorPositions[4])[0]);
        break;

      case 13:
        if (SensorPositions[5]++ == 8)
        {
          SensorPositions[5] = 1;
        }
        lcd.write((char)String(SensorPositions[5])[0]);
        break;

      case 14:
        if (SensorPositions[6]++ == 8)
        {
          SensorPositions[6] = 1;
        }
        lcd.write((char)String(SensorPositions[6])[0]);
        break;

      case 15:
        if (SensorPositions[7]++ == 8)
        {
          SensorPositions[7] = 1;
        }
        lcd.write((char)String(SensorPositions[7])[0]);
        break;
      }

      lcd.setCursor(cursorPosition, 1);
    }

    if ((digitalRead(BTN_SETTING) == LOW) && (digitalRead(BTN_VALUE) == LOW))
    {
      exit = true;
    }
  } while (!exit);

  UpdateSensorPositions();

  lcd.noBlink();

  delay(300);

  lcd.clear();
}
