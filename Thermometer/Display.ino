/*
 * Metody pro práci s displejem
 * 
 * 
 * 
 * 
 * 
 */

// Degree symbol
byte DegreeSymbol[8] = {
    B11100,
    B10100,
    B11100,
    B00000,
    B00000,
    B00000,
    B00000,
};

void SetupDisplay()
{
  //Vytvoření znaku stupně
  lcd.createChar(0, DegreeSymbol);

  // Nastavení počtu sloupců a řádků displeje
  lcd.begin(16, 2);
  lcd.clear();

  WriteDisplay("TEPLOMER", 3, 0, true, 400);
  WriteDisplay("8xDS18B20 v:2.00", 0, 1, false, 2000);

  WriteDisplay(" -55 oC +125 oC", 0, 0, true, 0);
  lcd.setCursor(5, 0);
  lcd.write(byte(0)); //prepsani stupne na lcd

  lcd.setCursor(13, 0);
  lcd.write(byte(0)); //prepsani stupne na lcd

  WriteDisplay(" +-0.5 oC " + (String)SENSOR_RESOLUTION + "bit ", 0, 1, false, 0);

  lcd.setCursor(7, 1);
  lcd.write(byte(0)); //prepsani stupne na lcd

  delay(2000);

  WriteDisplay("jan.pichl", 0, 0, true, 0);
  //WriteDisplay("an.pichl@seznam.", 0, 1, false, 400);
  // WriteDisplay("n.pichl@seznam.c", 0, 1, false, 400);
  WriteDisplay("      @seznam.cz", 0, 1, false, 2000);

  //smazani displeje
  WriteDisplay("", 0, 0, true, 0);
}

void WriteDisplay(String text, int column, int row, bool beforeClear, int pause)
{
  if (beforeClear)
  {
    lcd.clear();
  }

  lcd.setCursor(column, row);
  lcd.print(text);
  delay(pause);
}

void WriteDisplayTemperature(int temperature, int orderInDisplay, bool error)
{
  int k = 0; //column
  int l = 0; // row

  int  whole= temperature / 100; // separate off the whole and fractional portions
  int Fract = temperature % 100;

  String wholeDec = String(whole, DEC);
  String fractDec = String(Fract, DEC);
  String strResult;
  int cent = 0;

  if (whole <= -10)
  {
    strResult = "";
  }
  else if (whole < 0)
  {
    strResult = " ";
  }
  else if (whole >= 100)
  {
    strResult = "";
  }
  else if (whole >= 10)
  {
    strResult = " ";
  }
  else if (whole >= 0)
  {
    strResult = "  ";
  }

  for (unsigned int x = 0; x < sizeof(fractDec); x++) // zaokrouhleni vysledku
  {
    char number = fractDec[x] - 48;
    if (x == 0)
    {
      cent = number;
    }
    if (x == 1)
    {
      if (number >= 5)
      {
        cent++;
      }
    }
  }
  strResult += wholeDec;

  if (cent < 0)
  {
    cent = cent * (-1);
  }

  strResult += String(cent);

  // Chyba převodu
  if (error)
  {
    strResult = " Err";
  }

  switch (displayPositionconversion(orderInDisplay))
  {
  case 0:
    k = 0;
    l = 0;
    break;
  case 1:
    k = 4;
    l = 0;
    break;
  case 2:
    k = 8;
    l = 0;
    break;
  case 3:
    k = 12;
    l = 0;
    break;
  case 4:
    k = 0;
    l = 1;
    break;
  case 5:
    k = 4;
    l = 1;
    break;
  case 6:
    k = 8;
    l = 1;
    break;
  case 7:
    k = 12;
    l = 1;
    break;

  default:
    k = 0;
    l = 0;
  }

  WriteDisplay("    ", k, l, false, 0);    //smazani puvodni teploty
  WriteDisplay(strResult, k, l, false, 0); //nahrazeni novou teplotou
}

int displayPositionconversion(int Puvodniporadi)
{
  return (SensorPositions[Puvodniporadi]) - 1;
}

//vypise adresy senzorů na displej, není nepoužité
void ShowAllAdress()
{
  for (int i = 0; i < SensorCount; i++)
  {
    WriteDisplay("poradi nalezu: " + String(i + 1), 0, 0, true, 0);    
    WriteDisplay("adresa: " + String(SensorArray[i][0], HEX) + String(SensorArray[i][1], HEX) + String(SensorArray[i][2], HEX) + String(SensorArray[i][3], HEX) + String(SensorArray[i][4], HEX) + String(SensorArray[i][5], HEX) + String(SensorArray[i][6], HEX) + String(SensorArray[i][7], HEX), 0, 1, false, 1500);
  }
}
