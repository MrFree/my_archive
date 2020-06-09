#include <LPD6803.h>
//LPD6803 strip = LPD6803(50, dataPin, clockPin);
const unsigned int  led_num = 25;
const byte spd_ms = 25;
const unsigned long some_wait=3000;
LPD6803 strip = LPD6803(led_num, 2, 3);
const int buttonPin = 4;
const int r_flame = 20;
const int g_flame = 7;
const int b_flame = 2;


byte effect_num = 1 ;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastint = 0 ;
unsigned long dododo = 0 ;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

enum  direction { FORWARD, REVERSE };
direction Direction;     // direction to run the pattern
unsigned long Interval;   // milliseconds between updates
unsigned long lastUpdate; // last update of position
uint16_t Index;
uint16_t TotalSteps;

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint16_t c, uint8_t wait) {
  int i;

  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    //strip.setPixelColor(i+1, c);
    //strip.setPixelColor(i+2, c);
    strip.show();
    delay(wait);
  }
}

void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
//        ActivePattern = RAINBOW_CYCLE;
        Interval = interval;
        TotalSteps = 127;
        Index = 0;
        Direction = dir;
    }
    
 void Increment()
    {
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                /*
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
                */
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
              /*  if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }*/
            }
        }
    }  
      uint32_t Wheel(byte WheelPos)
    {
        WheelPos = 127 - WheelPos;
        if(WheelPos < 43)
        {
            return Color(0, WheelPos * 2,127 - WheelPos * 2);
        }
        else if(WheelPos < 85)
        {
            WheelPos -= 43;
            return Color(WheelPos * 2, 127 - WheelPos * 2,0);
        }
        else
        {
            WheelPos -= 85;
            return Color(127 - WheelPos * 2, 0,WheelPos * 2);
        }
    }  
    
   void RainbowCycleUpdate()
    {
        for(int i=0; i< led_num; i++)
        {
            strip.setPixelColor(i, Wheel(((i * 56 / led_num) + Index) & 127));
        }
        strip.show();
        Increment();
    }
/* Helper functions */

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return ( ((unsigned int)g & 0x1F ) << 10 | ((unsigned int)b & 0x1F) << 5 | (unsigned int)r & 0x1F);
}

void Flame()
{
   //  Flicker, based on our initial RGB values
  for(int i=0; i<strip.numPixels(); i++) {
    int flicker = random(5,9);
    int r1 = r_flame-flicker;
    int g1 = g_flame-flicker;
    int b1 = b_flame-flicker;
    if(g1<0) g1=0;
    if(r1<0) r1=0;
    if(b1<0) b1=0;
    strip.setPixelColor(i,g1,b1, r1);
  }
  strip.show();
}


void do_it()
{
switch (effect_num) {
  case 1:
    colorWipe(Color(28, 28, 28), spd_ms);
    break;
  case 2: 
    RainbowCycleUpdate();
    break;
  case 3: 
    Flame();
    break;
  case 4: 
    colorWipe(Color(3, 3, 3), spd_ms);
    break;
  case 0: //off
  default: //off
    colorWipe(Color(0, 0, 0), spd_ms);
  break;
}
  
}
void change_effect()
{
effect_num++;
if (effect_num>4) {effect_num=0;}
do_it();
}

void setup() {


  strip.setCPUmax(50);  // start with 50% CPU usage. up this if the strand flickers or is slow

  // Start up the LED counter
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  
  pinMode(buttonPin, INPUT);
  lastint=millis();
  RainbowCycle (70,127);
colorWipe(Color(127, 0, 0), spd_ms*2); // Red /G
colorWipe(Color(0, 127, 0), spd_ms*2); // Green /B
colorWipe(Color(0, 0, 127), spd_ms*2); // Blue /R
colorWipe(Color(0, 0, 0), spd_ms*2); // Black  
 change_effect(); 
}
void loop() {
  // put your main code here, to run repeatedly:
  int reading = digitalRead(buttonPin);
   if (reading != lastButtonState) {      lastDebounceTime = millis();  }
    
   if ((millis() - lastDebounceTime) > debounceDelay) {
   
          // if the button state has changed:
    if (reading != buttonState)
     {
      buttonState = reading; 
      if (buttonState == HIGH)
       {
        change_effect();       
       }
            
     }
  }
  lastButtonState = reading;
  if (effect_num==2) 
     { dododo= millis()-lastint;
      if (dododo >= random(50,100))
        {
          RainbowCycleUpdate();
          lastint=millis();
        }
     }
     if (effect_num==3) 
     { dododo= millis()-lastint;
      if (dododo >= random(80,110))
        {
          Flame();
          lastint=millis();
        }
     }
  //RainbowCycleUpdate();
  //delay(50);
 // colorWipe(Color(28, 0, 28), spd_ms);  // blue fill
 // delay(some_wait);
  
}
