// variables will change:
int ledIntensity = 0;     // the current LED intensity
bool dir_up = true;
#define NUM_LEDS 9
const byte led_pins[] = {12,11,7,6,5,10,9,13,8}; //byte is big enough
byte led_vals[NUM_LEDS]; // led intensity values
int t = 0;

void setup() {
  // put your setup code here, to run once:
  for(int i=0;i<NUM_LEDS;i++)
  {
    pinMode(led_pins[i], OUTPUT);
  }
  for(int i=0; i<NUM_LEDS; i++)
    led_vals[i] = 0;
}

void writeLeds()
{
    for(int i=0; i<NUM_LEDS; i++)
      analogWrite(led_pins[i], led_vals[i]);
}


void loop() {

  // Set led intensities
  for(int i=0; i<NUM_LEDS; i++)
  {
    //led_vals[i] = (ledIntensity + i*10) % 256;
    led_vals[i] = sin(0.01*t - 0.2*i)*128+128;
  }

  // Increase intensity, and reset to zero if saturated
  if(dir_up)
    ledIntensity++;
  else
    ledIntensity--;
    
  if(ledIntensity >= 255)
   dir_up = false;
  if(ledIntensity <= 0)
   dir_up = true;

  ++t %= 620;
   writeLeds();
   
   delay(10);
}
