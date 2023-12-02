#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Timer variables
unsigned long onTime = 0;
unsigned long offTime = 0;
unsigned long lastStatusChange = 0;
bool machineOn = false;

void setup()
{
  Serial.begin(9600);

  // Initialize the LCD
  lcd.init();

  // Initialize the I2C bus
  Wire.begin();

  // Turn on the backlight on LCD.
  lcd.backlight();

  lcd.print("Light Intensity:");

  // Begin reading the light meter
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
  {
    Serial.println(F("BH1750 Textile machine reading begin"));
  }
  else
  {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop()
{
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Print lux to serial plotter
  Serial.println(lux);

  // Check machine status
  if (lux < 100 && !machineOn)
  {
    machineOn = true;
    lastStatusChange = millis();
    Serial.println("Machine status: on");
  }
  else if (lux >= 100 && machineOn)
  {
    machineOn = false;
    lastStatusChange = millis();
    Serial.println("Machine status: off");
  }

  // Update timer variables
  if (machineOn)
  {
    onTime += millis() - lastStatusChange;
  }
  else
  {
    offTime += millis() - lastStatusChange;
  }

  // Print machine status and total time
  lcd.setCursor(0, 1);
  lcd.print(lux);
  lcd.print(" Lux");

  Serial.print("Machine On time: ");
  Serial.print(onTime / 10000.0);
  Serial.println(" s");

  Serial.print("Machine Off time: ");
  Serial.print(offTime / 10000.0);
  Serial.println(" s");

  delay(1000);
}