#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS 9 
#define RED_SW_PIN 45
#define ANTLED_PIN 43
const byte led_pins[] = {12, 11, 7, 6, 5, 10, 9, 13, 8}; // led pin numbers
byte led_vals[NUM_LEDS]; // led intensity values
int t = 0;
bool spinner_state = 0;
unsigned int spinner_cnt = 0;
unsigned int spinner_speed = 0;
unsigned long current_time = millis();
unsigned long prev_time = millis();

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(115200);
  
  // Set led pins to output
  for (int i = 0; i < NUM_LEDS; i++)
  {
    pinMode(led_pins[i], OUTPUT);
  }

  pinMode(RED_SW_PIN, INPUT);
  digitalWrite(RED_SW_PIN, HIGH);       // turn on pullup resistors

  pinMode(ANTLED_PIN, OUTPUT);
  // Initialize led pins to 0
  for (int i = 0; i < NUM_LEDS; i++)
    led_vals[i] = 0;

  // Initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Kristo sein!");
  lcd.setCursor(0, 1);
  lcd.print("Kiirus: ");
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
    led_vals[i] = max(sin(0.01 * t - 0.2 * i) * 128 + 70, 0);
  }

  //++t %= 620; // increment t and keep it in range of 2*pi*100
  
  writeLeds();

  // Update t value on the lcd on every 10th iteration
  if (t % 100 == 0)
  {
    if ((millis() - prev_time) > 1000)
    {
      spinner_speed = 0;
    }
    lcd.setCursor(8, 1);
    lcd.print("    ");
    lcd.setCursor(8, 1);
    lcd.print((int)spinner_speed);
  }

  t = spinner_cnt*10 % 620;

  int val = digitalRead(RED_SW_PIN);   // read the input pin
  digitalWrite(ANTLED_PIN, !val);  // sets the LED to the button's value


  // Spinner speed detection
  int spinner_val = analogRead(A0);
  if (spinner_val > 250 and spinner_state == 0)
  {
    spinner_state = 1; // rising edge
    spinner_cnt++;
    
    prev_time = current_time;
    current_time = millis();
    unsigned long dt = current_time - prev_time;

    spinner_speed = 1000.0*60/3/dt;
  }
  
  if (spinner_val < 150 and spinner_state == 1)
  {
    spinner_state = 0; // falling edge
  }

      Serial.println(spinner_val);

 // delay(10);
}
