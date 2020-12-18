#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS 9
const byte led_pins[] = {12, 11, 7, 6, 5, 10, 9, 13, 8}; // led pin numbers
byte led_vals[NUM_LEDS]; // led intensity values
int t = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Set led pins to output
  for (int i = 0; i < NUM_LEDS; i++)
  {
    pinMode(led_pins[i], OUTPUT);
  }

  // Initialize led pins to 0
  for (int i = 0; i < NUM_LEDS; i++)
    led_vals[i] = 0;

  // Initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Kristo sein!");
  lcd.setCursor(0, 1);
  lcd.print("V1.0");
  lcd.setCursor(8, 1);
  lcd.print("t=");
}

// This function updates all led intensities on the output pins.
void writeLeds()
{
  for (int i = 0; i < NUM_LEDS; i++)
    analogWrite(led_pins[i], led_vals[i]);
}


void loop() {

  // Set led intensities
  for (int i = 0; i < NUM_LEDS; i++)
  {
    led_vals[i] = max(sin(0.01 * t - 0.2 * i) * 128 + 80, 0);
  }

  ++t %= 620;
  
  writeLeds();

  // Update t value on the lcd on every 10th iteration
  if (t % 10 == 0)
  {
    lcd.setCursor(10, 1);
    lcd.print((float)t, 1);
  }

  delay(10);
}
