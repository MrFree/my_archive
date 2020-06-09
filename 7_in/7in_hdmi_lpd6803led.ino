#include <LPD6803.h>
//LPD6803 strip = LPD6803(50, dataPin, clockPin);
const int  led_num = 25;
const byte spd_ms = 22;
LPD6803 strip = LPD6803(led_num, 2, 3);
const int inPin[]={4,5,6,7,8,9,10,11};
//const int inBlock[]={1,2,0,3,4,5,6};
const int inBlock[]={1,2,0,6,3,5,4};
int CurrentLed = 0;


void colorWipe(uint16_t c, uint8_t wait) {
  int i;

  for (i = 0; i < strip.numPixels(); i=i+3) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i+1, c);
    strip.setPixelColor(i+2, c);
    strip.show();
    delay(wait);
  }
}
void colorBlock (uint16_t c, uint16_t cb, unsigned int num ,uint8_t wait) 
{
  int i;
  for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, cb);    
    }
  //strip.show();
  //delay(wait);
  strip.setPixelColor(num*3, c);
  strip.setPixelColor(num*3+1, c);
  strip.setPixelColor(num*3+2, c);
  if (num==6) { strip.setPixelColor(num*3+3, c);
                strip.setPixelColor(num*3+4, c);
                strip.setPixelColor(num*3+5, c);
              }
  strip.show();
  delay(wait);
  
}
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return ( ((unsigned int)g & 0x1F ) << 10 | ((unsigned int)b & 0x1F) << 5 | (unsigned int)r & 0x1F);
}









void setup() {
  // put your setup code here, to run once:
    int i;
for (i = 0; i < 8; i++) {
  pinMode(inPin[i], INPUT);
  digitalWrite(inPin[i], LOW); 
  }
strip.setCPUmax(50);  // start with 50% CPU usage. up this if the strand flickers or is slow

  // Start up the LED counter
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();

  colorWipe(Color(127, 0, 0), spd_ms*3); // Red /G
  colorWipe(Color(0, 127, 0), spd_ms*3); // Green /B
  colorWipe(Color(0, 0, 127), spd_ms*3); // Blue /R
  //colorWipe(Color(0, 0, 0), spd_ms*5); // Black 

for (i = 0; i < 7; i++) {
    colorBlock (Color(0,0,28), Color(10,20,5), i ,50) ;
    delay (150);
    }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
/*for (i = 0; i < 7; i++) {
colorBlock (Color(0,0,28), Color(10,20,5), i ,50) ;
delay (1000);
*/

    for (i = 0; i < 7; i++) 
       {
         if (digitalRead(inPin[i])==HIGH) { CurrentLed=i; break; }
       }
            
    if (digitalRead(inPin[7])==HIGH) 
        {
          colorBlock (Color(28,0,0), Color(10,20,5), inBlock[CurrentLed] ,50) ;
        } else {
               
                colorBlock (Color(0,28,0), Color(8,16,3), inBlock[CurrentLed] ,50) ;
                }
delay (100);    

}
