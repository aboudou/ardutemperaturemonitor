#include <EEPROM.h>
#include <LiquidCrystal.h>

#include "EEPROMAnything.h"
#include "RunningMedian.h"

// LCD display pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Min and max temperatures
float minTemp = 999.0;
float maxTemp = 0.0;

// Temperature sensor pin
const byte sensorPin = A0;

// EEPPROM management
const byte eepromValidationAddress = 0;
const byte eepromValidationValue = 99;
const byte eepromMinTempStartAddress = eepromValidationAddress + sizeof(byte);
const byte eepromMaxTempStartAddress = eepromMinTempStartAddress + sizeof(float);

// ° character
byte degree[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000
};

// Bottom to top arrow character
byte maxArrow[8] = {
  B11111,
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100
};

// Top to botton arrow character
byte minArrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
  B11111
};

void setup() {
  // Serial
  Serial.begin(9600);
  
  // Initialize LCD display
  lcd.begin (16,2);
  lcd.createChar(0, degree);
  lcd.createChar(1, minArrow);
  lcd.createChar(2, maxArrow);

  // Uncomment to clear stored min and max temperature
  //  at sketch startup
  /*
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Erasing EEPROM");
  
  EEPROM_writeAnything(eepromValidationAddress, eepromValidationValue);
  EEPROM_writeAnything(eepromMinTempStartAddress, minTemp);
  EEPROM_writeAnything(eepromMaxTempStartAddress, maxTemp);
  delay(2000);
  */
  
  // Restore min and max temperature
  byte testValue;
  EEPROM_readAnything(eepromValidationAddress, testValue);
  if (testValue == eepromValidationValue) {
    // Min and max temperature were previously saved
    // We read them from EEPROM
    EEPROM_readAnything(eepromMinTempStartAddress, minTemp);
    EEPROM_readAnything(eepromMaxTempStartAddress, maxTemp);
    
  } else {
    // Min and max temperature were not previously saved
    // We initialize them
    EEPROM_writeAnything(eepromValidationAddress, eepromValidationValue);
    EEPROM_writeAnything(eepromMinTempStartAddress, minTemp);
    EEPROM_writeAnything(eepromMaxTempStartAddress, maxTemp);
  }
}

void loop() {
  float _lowest;
  float _highest;
  
  // Temperature samples, used to calculate median measurement
  // and ignore erronous spikes
  RunningMedian<float,10> samples;

  // Read voltage from temperature sensor (TMP36)
  // then convert it to Celcius degrees, and store
  // them into samples
  float temperature = 0.0;
  for (int i = 0; i < 10; i++) {
    int sensorVal = analogRead(sensorPin);
    float voltage = (sensorVal / 1024.0) * 5.0;
    float currentTemp = (voltage - 0.5) * 100;
    samples.add(currentTemp);

    delay(100);
  }
  // Get median temperature
  samples.getMedian(temperature);
  
  // Check for min temperature measured, and save it to EEPROM
  //   if a new min temperature is found
  if (temperature < minTemp) {
    minTemp = temperature;
    EEPROM_writeAnything(eepromMinTempStartAddress, minTemp);
  }
  // Check for max temperature measured, and save it to EEPROM
  //   if a new max temperature is found
  if (temperature > maxTemp) {
    maxTemp = temperature;
    EEPROM_writeAnything(eepromMaxTempStartAddress, maxTemp);
  }
  
  // Clear LCD display    
  lcd.clear();

  // Display current temp    
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.write(byte(0));
  lcd.print("C");
  
  // Display min and max temp
  lcd.setCursor(0,1);
  lcd.write(byte(1));
  lcd.print(" ");
  lcd.print(minTemp);
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.print(maxTemp);
  
  // Wait for five seconds before measuring temperature again
  delay(5000);
}
