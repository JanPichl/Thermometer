/*
 * Práce se sběrnicí I2C
 * 
 * 
 * 
 */

byte SenzorAddress[MAX_COUNT_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

//Vyhledání, nastavení a setřídění senzorů na sběrnici
void SetupSensors()
{
  do
  {
    SearchSensors();
  } while (SensorCount == 0);

  SortSensors();

  SetSensorResolutions(SENSOR_RESOLUTION);
}

// prohledá i2c sběrnici a najde všechna připojená zařízení
void SearchSensors()
{
  int i = 0;
  bool found;
  do
  {
    found = SearchAddress();

    if (found)
    {
      for (int x = 0; x < MAX_COUNT_SENSORS; x++)
      {
        SensorArray[i][x] = SenzorAddress[x];
      }
      i++;
    }
  } while (found);
}

void SortSensors()
{
  byte FieldA[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte FieldB[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  for (int i = 0; i < MAX_COUNT_SENSORS - 1; i++)
  {
    for (int j = 0; j < MAX_COUNT_SENSORS - i - 1; j++)
    {
      for (int n = 0; n < 8; n++)
      {
        FieldA[n] = SensorArray[j + 1][n];
      }
      for (int m = 0; m < 8; m++)
      {
        FieldB[m] = SensorArray[j][m];
      }

      if (ByteArrayComparer(FieldA, FieldB) == true)
      {
        byte HelpArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        for (int k = 0; k < 8; k++)
        {
          HelpArray[k] = FieldA[k];
        }

        for (int q = 0; q < 8; q++)
        {
          SensorArray[j + 1][q] = FieldB[q];
        }

        for (int p = 0; p < 8; p++)
        {
          SensorArray[j][p] = HelpArray[p];
        }
      }
    }
  }
}

bool ByteArrayComparer(byte A[8], byte B[8])
{
  for (int i = 0; i < 8; i++)
  {
    if (A[i] != B[i])
    {
      if (A[i] > B[i])
      {
        return true;
      }
      else
      {
        return false;
      }
    }    
  }
  return false;
}

// Hleda 65 bit 8x Byte adresy na i2c sbernici ,pokud jiz dalsi neni vraci false;
bool SearchAddress()
{
  if (!dallas.search(SenzorAddress))
  {
    WriteDisplay("Celkem nalezeno:", 0, 0, true, 0);
    WriteDisplay("" + String(SensorCount) + " cidel teploty", 0, 1, false, 1000);
    dallas.reset_search();
    return false;
  }
  else
  {
    SensorCount++;
    WriteDisplay("Hledam cidla:", 0, 0, true, 0);
    WriteDisplay("" + String(SensorCount) + " nalezeno", 0, 1, false, 200);
    return true;
  }
}

// nastavení bitového rozlišení snímačů
void SetSensorResolutions(int bitu)
{
  byte accuracy = 0x1F;

  switch (bitu)
  {
  case 9:
    accuracy = 0x1F; //9bit
    transferDelay = 94;
    break;
  case 10:
    accuracy = 0x3F; //10bit
    transferDelay = 188;
    break;
  case 11:
    accuracy = 0x5F; //11bit
    transferDelay = 375;
    break;
  case 12:
    accuracy = 0x7F; //12bit
    transferDelay = 750;
    break;
  }

  for (int i = 0; i < SensorCount; i++)
  {

    dallas.reset();
    dallas.select(SensorArray[i]); //adresa senzoru

    dallas.write(0x4E);     //zapis do senzoru
    dallas.write(0xFF);     //zapis user 1
    dallas.write(0xFF);     //zapis user 2
    dallas.write(accuracy); //zapis konfigurace

    delay(100);
  }
}

void ReadSensorValues()
{
  byte data[9];

  for (int i = 0; i < SensorCount; i++)
  {

    dallas.reset();
    dallas.select(SensorArray[i]); //adresa cidla
    dallas.write(0x44, 1);         // start conversion, with parasite power on at the end
  }

  delay(transferDelay); // pro 9 bit je prevod < 100 ms

  for (int i = 0; i < SensorCount; i++)
  {
    dallas.reset();
    dallas.select(SensorArray[i]); //adresa cidla

    dallas.select(SensorArray[i]);
    dallas.write(0xBE); // Read Scratchpad

    for (int j = 0; j < 9; j++)
    {
      // we need 9 bytes
      data[j] = dallas.read();
    }

    if (OneWire::crc8(data, 8) != data[8]) //CRC kontrola
    {
      WriteDisplayTemperature(TemperatureConversion(data), i, true);
    }
    else
    {
      WriteDisplayTemperature(TemperatureConversion(data), i, false);
    }
  }
}

// Vytažení teploty z datového paketu
int TemperatureConversion(byte data[9])
{
  int HighByte, LowByte, TReading, Tc_100;

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;

  Tc_100 = (6 * TReading) + TReading / 4; 

  return Tc_100;
}
