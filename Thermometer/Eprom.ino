/*
 * Do EEPROM je ukládáno uživatelské pořadí jednotlivých senzorů
 * 
 * 
 * 
 * 
 * 
 */

// Inicializace uloženého nastavení
void SetupEprom()
{
  ReadSensorPositions();

  WriteDisplay("Snimac ", 0, 0, true, 0);
  WriteDisplay("1", 8, 0, false, 0);
  WriteDisplay("2", 9, 0, false, 0);
  WriteDisplay("3", 10, 0, false, 0);
  WriteDisplay("4", 11, 0, false, 0);
  WriteDisplay("5", 12, 0, false, 0);
  WriteDisplay("6", 13, 0, false, 0);
  WriteDisplay("7", 14, 0, false, 0);
  WriteDisplay("8", 15, 0, false, 0);

  WriteDisplay("Pozice ", 0, 1, false, 0);

  WriteDisplay(String(SensorPositions[0]), 8, 1, false, 0);
  WriteDisplay(String(SensorPositions[1]), 9, 1, false, 0);
  WriteDisplay(String(SensorPositions[2]), 10, 1, false, 0);
  WriteDisplay(String(SensorPositions[3]), 11, 1, false, 0);
  WriteDisplay(String(SensorPositions[4]), 12, 1, false, 0);
  WriteDisplay(String(SensorPositions[5]), 13, 1, false, 0);
  WriteDisplay(String(SensorPositions[6]), 14, 1, false, 0);
  WriteDisplay(String(SensorPositions[7]), 15, 1, false, 0);

  delay(2000);

  lcd.clear();
}

// Uložení přadí senzorů do EEPROM
void UpdateSensorPositions()
{
  EEPROM.update(0, SensorPositions[0]);
  EEPROM.update(1, SensorPositions[1]);
  EEPROM.update(2, SensorPositions[2]);
  EEPROM.update(3, SensorPositions[3]);
  EEPROM.update(4, SensorPositions[4]);
  EEPROM.update(5, SensorPositions[5]);
  EEPROM.update(6, SensorPositions[6]);
  EEPROM.update(7, SensorPositions[7]);
}

// Načtení pořadí senzorů z EEPROM
void ReadSensorPositions()
{
  byte value;

  for (int address = 0; address < MAX_COUNT_SENSORS; address++)
  {
    value = EEPROM.read(address);

    SensorPositions[address] = (int)value;
  }
}
