#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS 9 
#define RED_SW_PIN 45
#define ANTLED_PIN 43
#define SPINNER_BUF_SIZE 1000
#define LCD_REFRESH_RATE 1
const byte led_pins[] = {12, 11, 7, 6, 5, 10, 9, 13, 8}; // led pin numbers
byte led_vals[NUM_LEDS]; // led intensity values
int t = 0;
bool spinner_state = 0;
unsigned int spinner_cnt = 0;
unsigned int spinner_rpm = 0;
unsigned int spinner_thresh_hi = 300;
unsigned int spinner_thresh_lo = 150;
int spinner_buf[SPINNER_BUF_SIZE];
int spinner_buf_idx =0;
unsigned long current_time = millis();
unsigned long prev_time = millis();
unsigned long prev_lcd_update = millis();

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

//  Serial.begin(115200);
  
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
  lcd.print("KRISTO SEIN!");
  delay(500);
  byte alien[8] = {
    0b11111,
    0b10101,
    0b11111,
    0b11111,
    0b01110,
    0b01010,
    0b11011,
    0b00000
  };
  lcd.createChar(0,alien);
  lcd.setCursor(0, 1);
  for(int i=0; i<16; i++)
  {
    lcd.write(byte(0));
    delay(200);
  }
  delay(500);
  lcd.clear();
}

// This function updates all led intensities on the output pins.
void writeLeds()
{
  for (int i = 0; i < NUM_LEDS; i++)
    analogWrite(led_pins[i], led_vals[i]);
}

void spinnerPulseDetect()
{
  // Spinner pulse detection
  int spinner_val = analogRead(A0);
  spinner_buf[spinner_buf_idx] = spinner_val;
  ++spinner_buf_idx %= SPINNER_BUF_SIZE;

  // Adjust hi and lo thresholds based on previous readings
  unsigned int new_lo = 1024;
  unsigned int new_hi = 0;
  for (int i=0; i < SPINNER_BUF_SIZE; i++)
  {
    new_lo = min(new_lo, spinner_buf[i]);
    new_hi = max(new_hi, spinner_buf[i]);
  }
  
  if (new_hi - new_lo > 150) // have to have at least some signal to work with
  {
    spinner_thresh_lo = new_lo;
    spinner_thresh_hi = new_hi;
  }

  int thresh_delta = spinner_thresh_hi - spinner_thresh_lo;
  if (spinner_val > spinner_thresh_hi - thresh_delta * 0.3 and spinner_state == 0)
  {
    spinner_state = 1; // rising edge
    spinner_cnt++;
  }
  
  if (spinner_val < spinner_thresh_lo + thresh_delta * 0.3 and spinner_state == 1)
  {
    spinner_state = 0; // falling edge
  }
}


void loop() {
  prev_time = current_time;
  current_time = millis();
  
  // Set led intensities
  for (int i = 0; i < NUM_LEDS; i++)
  {
    led_vals[i] = max(sin(0.01 * t - 0.2 * i) * 128 + 70, 0);
  }
  ++t %= 620; // increment t and keep it in range of 2*pi*100
  writeLeds();
  
  if ((current_time - prev_lcd_update) > 1000.0/LCD_REFRESH_RATE)
  {
    // Calculate spinner RPM
    spinner_rpm = 60/3*spinner_cnt*LCD_REFRESH_RATE;

    // Update LCD display
    char buf[16];
    sprintf(buf,"Lo: %03d Hi: %03d",spinner_thresh_lo, spinner_thresh_hi);
    lcd.setCursor(0, 0);
    lcd.print(buf);
    sprintf(buf,"Kiirus: %04d",spinner_rpm);
    lcd.setCursor(0,1);
    lcd.print(buf);
    
    prev_lcd_update = current_time;
    spinner_cnt=0;
  }

  // Drive antenna with the RED switch
  int val = digitalRead(RED_SW_PIN);   // read the input pin
  digitalWrite(ANTLED_PIN, !val);  // sets the LED to the button's value

  spinnerPulseDetect();
  
  delay(1);
}
