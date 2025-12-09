#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD: (0x27 or 0x3F are common I2C addresses)
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define GAS_IN   D5   // Input pin
#define GAS_OUT  D6   // Output pin

void setup() {
  pinMode(GAS_IN, INPUT);
  pinMode(GAS_OUT, OUTPUT);

  lcd.begin();       // Initialize LCD
  lcd.backlight();  // Turn on backlight

  lcd.setCursor(0,0);
  lcd.print("Gas Monitor Ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  int gasVal = digitalRead(GAS_IN);

  if (gasVal == 0) {
    digitalWrite(GAS_OUT, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GasDetected");
  } else {
    digitalWrite(GAS_OUT, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Safe");
  }

  delay(500);  // update every 0.5s
}