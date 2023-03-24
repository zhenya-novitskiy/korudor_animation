#include <FastLED.h>

#include "ParalelTimer.h"

#include <ColorTransaction.h>
#define LED_PIN     6
//#define NUM_LEDS    880
#define NUM_LEDS    880
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#include <DS3231.h>
#include <Wire.h>

DS3231 clock1;

ColorTransaction transactions[NUM_LEDS];

ParalelTimer timer2 = ParalelTimer(2);
ParalelTimer clockTimer = ParalelTimer(1000);

#define VANNA_START 492
#define VANNA_END 543

#define VANNA_BACK_CENTER 370

#define WC_BACK_CENTER 261

#define OFFICE_BACK_CENTER 91


#define WC_START 602
#define WC_END 651

#define OFFICE_START 768
#define OFFICE_END 817


ParalelTimer timer = ParalelTimer(100);


CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
bool isOn;
int prevState = 0;
int isInAnimation = false;

CRGB Colors[8] = {
CRGB::Red,
CRGB::Green,
CRGB::Blue,
CRGB::Orange,
CRGB::Aqua,
CRGB::Pink,
CRGB::White,
CRGB::Yellow
};

CRGB ColorPairs[8][2] = 
{
  {CRGB::White,CRGB::Aqua},
  
  {CRGB::Green,CRGB::Red},
  
  {CRGB::Blue,CRGB::Orange},

  {CRGB::Purple,CRGB::Yellow},

  {CRGB::Orange,CRGB::Red},

  {CRGB::White,CRGB::White},

  {CRGB::Aqua,CRGB::Aqua},

  {CRGB::Red,CRGB::Red},
};

void setup() {
   // delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    Serial.begin(9600);
    pinMode(52, INPUT);
     checkDayTime();
    if (digitalRead(52) == 0)
    {
       enable();  
    }else
    {
       disable();
    }

//     for( int i = 0; i < 443; ++i) {
//          leds[i] = CRGB::Blue;
//     }
//    for( int i = 443; i < 880; ++i) {
//          leds[i] = CRGB::Yellow;
//     }


//    
//     for( int i = VANNA_START; i < VANNA_END; ++i) {
//          leds[i] = CRGB::Red;
//     }
//
//
//     for( int i = WC_START; i < WC_END; ++i) {
//          leds[i] = CRGB::Red;
//     }
//
//     for( int i = OFFICE_START; i < OFFICE_END; ++i) {
//          leds[i] = CRGB::Red;
//     }
//
//
//
//     for( int i = VANNA_BACK_CENTER-20; i < VANNA_BACK_CENTER+20; ++i) {
//          leds[i] = CRGB::Red;
//     }
//
//
//   for( int i = WC_BACK_CENTER-20; i < WC_BACK_CENTER+20; ++i) {
//          leds[i] = CRGB::Red;
//     }
//
//  for( int i = OFFICE_BACK_CENTER-20; i < OFFICE_BACK_CENTER+20; ++i) {
//          leds[i] = CRGB::Red;
//     }

    
//     for( int i = 0; i < 110; ++i) {
//          leds[i] = CRGB::White;
//     }
//     for( int i = 110; i < 220; ++i) {
//          leds[i] = CRGB::Red;
//     }
//     for( int i = 220; i < 330; ++i) {
//          leds[i] = CRGB::White;
//     }
//     for( int i = 330; i < 440; ++i) {
//          leds[i] = CRGB::Red;
//     }
//
//          for( int i = 440; i < 550; ++i) {
//          leds[i] = CRGB::White;
//     }
//     for( int i = 550; i < 660; ++i) {
//          leds[i] = CRGB::Red;
//     }
//     for( int i = 660; i < 770; ++i) {
//          leds[i] = CRGB::White;
//     }
//     for( int i = 770; i < 880; ++i) {
//          leds[i] = CRGB::Red;
//     }
     
  currentPalette = LavaColors_p;
    currentBlending = LINEARBLEND;
  //delay( 1000 ); 
  Wire.begin();
  checkDayTime();
 

}
bool isAnimationForDiana = false;
bool isNigth = false;

void loop()
{
if (clockTimer.ItsTime())
{
  checkDayTime();
}




  if (timer.ItsTime())
  {

    
    int buttonState = digitalRead(52);
      // print out the state of the button:
    
    if (prevState != buttonState)
    {
      prevState = buttonState;
      
      if (buttonState == 0)
      {
        
           enable();
        
       
      }else
      {
      disable();
      }
    }

    
  }
if (isAnimationForDiana)
{
 static uint8_t startIndex = 0;
  startIndex = startIndex + 3; /* motion speed */
  
  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
  return;
}



   if (timer2.ItsTime())
  {
    isInAnimation = false;
 for (int i = 0; i < NUM_LEDS; i++) 
    {
      if (transactions[i].InAnimation())
      {
          isInAnimation = true;
          transactions[i].NextFrame();
          leds[i]=transactions[i].CurrentColor;
      }
    }
  }

  
  if (isInAnimation)
  {

    FastLED.show();

   
  }


      
//    ChangePalettePeriodically();
//    
//    static uint8_t startIndex = 0;
//    startIndex = startIndex + 1; /* motion speed */
//    
  ////  FillLEDsFromPaletteColors( startIndex);


 
//    FastLED.show();
//    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < 440; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        leds[880-i]=leds[i];
        colorIndex += 3;
    }
}



int fadeInTransactions = 50;
int maximum = 40;
int funcindex = 1;
int funcindex2 = 1;
int disableindex = 1;

void enable()
{
  if (isNigth)
  {
    
       
if (funcindex2 == 1) currentPalette = LavaColors_p;
if (funcindex2 == 2) currentPalette = PartyColors_p;
if (funcindex2 == 3) currentPalette = CloudColors_p;
if (funcindex2 == 4) currentPalette = OceanColors_p;
if (funcindex2 == 5) currentPalette = RainbowStripeColors_p;
        funcindex2++;
    if (funcindex2 > 5) funcindex2 = 1;
    
    isAnimationForDiana = true;
      //night();
  }else
  {
    isAnimationForDiana = false;
if (funcindex == 1) func1();
if (funcindex == 2) func2();
if (funcindex == 3) func3();
if (funcindex == 4) func4();
if (funcindex == 5) func5();

    funcindex++;
    if (funcindex > 5) funcindex = 1;
    }


}
bool h12Flag;
bool pmFlag;
void checkDayTime()
{
  int currentHour = clock1.getHour(h12Flag, pmFlag);
 
   if (currentHour >= 1 && currentHour <= 6)
      {
        isNigth = true;  
      }else
      {
        isNigth = false;////  
      }

}
void disable()
{
     isAnimationForDiana = false;
      if (disableindex == 1) disable1();
      if (disableindex == 2) disable2();
      if (disableindex == 3) disable3();

      
   disableindex++;
    if (disableindex > 3) disableindex = 1;
}
void disable1()
{
    for (int i = 0; i < 880; i++) 
    {

       int randNumber = random(5, 20);
      transactions[i].ToColor(leds[i], CRGB::Black , randNumber);
    }
}


void disable2()
{
    int extime = 5;
       for( int i = 374; i < 445; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 10);
       }
  
       for( int i = 445; i < 518; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 10);
       }
  
       for( int i = 308; i < 374; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 25);
       }
        for( int i = 518; i < 584; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 25);
       }
  
   
       for( int i = 242; i < 308; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 40);
       }
       for( int i = 584; i < 650; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 40);
       }
  
  
       for( int i = 176; i < 242; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 55);
       }
       for( int i = 650; i < 716; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 55);
       }
  
  
       for( int i = 110; i < 176; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 70);
       }
       for( int i = 716; i < 782; ++i) {
  
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 70);
       }
  
       for( int i = 44; i < 110; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 85);
       }
       for( int i = 782; i < 848; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 85);
       }
  
  
       for( int i = 0; i < 44; ++i) {
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 100);
       }
       for( int i = 848; i < 880; ++i) {
            
            transactions[i].ToColor(leds[i], CRGB::Black , extime, 100);
       }
}


void disable3()
{ 
       for( int i = 221, j=221; i < 443; ++i, --j) {
            transactions[i].ToColor(leds[i], CRGB::Black , fadeInTransactions, (i-221)/10);
            transactions[j].ToColor(leds[j], CRGB::Black , fadeInTransactions, (i-221)/10);
       }

       for( int i = 442, y=879; i < 661; ++i, --y) {
            
            transactions[i].ToColor(leds[i], CRGB::Black , fadeInTransactions, 22 + (i-442)/10);
            transactions[y].ToColor(leds[y], CRGB::Black , fadeInTransactions, 22 + (i-442)/10);
       }
}

void func1()
{
  
   
  int randNumber = random(0, 8);
  CRGB color1 = ColorPairs[randNumber][0];
  CRGB color2 = ColorPairs[randNumber][1];

  randNumber = random(0, maximum);
       for( int i = 374; i < 440; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, randNumber);
       }
  
  randNumber = random(0, maximum);
       
       for( int i = 452; i < 518; ++i) {
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions,randNumber);
       }
  
  
  randNumber = random(0, maximum);
  
       for( int i = 308; i < 374; ++i) {
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions,randNumber);
       }
  randNumber = random(0, maximum);
        for( int i = 518; i < 584; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, randNumber);
       }
  
  
  
       for( int i = 242; i < 308; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, randNumber);
       }
  randNumber = random(0, maximum);
       for( int i = 584; i < 650; ++i) {
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions,randNumber);
       }
  randNumber = random(0, maximum);
  
  
  
       for( int i = 176; i < 242; ++i) {
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions,randNumber);
       }
  randNumber = random(0, maximum);
       for( int i = 650; i < 716; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, randNumber);
       }
  
  
  randNumber = random(0, maximum);
  
       for( int i = 110; i < 176; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, randNumber);
       }
  randNumber = random(0, maximum);
       for( int i = 716; i < 782; ++i) {
  
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions, randNumber);
       }
  
  randNumber = random(0, maximum);
  
       for( int i = 44; i < 110; ++i) {
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions, randNumber);
       }
    randNumber = random(0, maximum);
     for( int i = 782; i < 848; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, randNumber);
       }
  
  randNumber = random(0, maximum);
  
  
       for( int i = 0; i < 44; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, randNumber);
       }
   randNumber = random(0, maximum);
      for( int i = 848; i < 880; ++i) {
            
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions,randNumber);
       }
  
}
void func2()
{

CRGB color1 = Colors[random(0,8)];
  
       for( int i = 0; i < 442; ++i) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, i/10);
       }
CRGB color2 = Colors[random(0,8)];
 
       for( int i = 442; i < 880; ++i) {
            
            transactions[i].ToColor(leds[i], color2 , fadeInTransactions, (880-i)/10);
       }
}

void night()
{
     for( int i = VANNA_START; i < VANNA_END; ++i) {
          transactions[i].ToColor(leds[i], CRGB::Aqua , 10, 30);
     }


     for( int i = WC_START; i < WC_END; ++i) {
          transactions[i].ToColor(leds[i], CRGB::Aqua , 10, 15);
     }

     for( int i = OFFICE_START; i < OFFICE_END; ++i) {
          transactions[i].ToColor(leds[i], CRGB::Red , 10, 0);
     }
}


void func3()
{
CRGB color1 = Colors[random(0,8)];
  
       for( int i = 221, j=221; i < 442; ++i, --j) {
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, (i-221)/10);
            transactions[j].ToColor(leds[j], color1 , fadeInTransactions, (i-221)/10);
       }

       for( int i = 442, y=879; i < 660; ++i, --y) {
            
            transactions[i].ToColor(leds[i], color1 , fadeInTransactions, 22 + (i-442)/10);
            transactions[y].ToColor(leds[y], color1 , fadeInTransactions, 22 + (i-442)/10);
       }
    

}


void func4()
{
    CRGB color1 = Colors[random(0,8)];
  
       for( int i = 440, j=440; i > 0; --i, j++) {
            int randNumber = random(5, 20);
            int randNumber2 = random((j-430)/10, 100);
            transactions[i].ToColor(leds[i], color1 , randNumber, randNumber2);
            transactions[j].ToColor(leds[j], color1 , randNumber, randNumber2);
       }
 
}

void func5()
{
   
    
int x=0;
int pause=5;
int prevx=0;
bool isLeftSide = true;

  
  do
  {

     uint8_t speed = beatsin8( 25, 100, 255);
   
     CRGB color1 = Colors[random(0,8)];
    x+= random(5,10);
    pause+=1;
    isLeftSide = !isLeftSide;
    if (!isLeftSide)
    {
          for( int i = prevx, j=1; i < x; ++i, j+=2) {
              transactions[i].ToColor(leds[i], GetStep(CRGB::Green,CRGB::Red, 400,  i) , j, pause);
              transactions[879-i].ToColor(leds[879-i], GetStep(CRGB::Green,CRGB::Red, 400,  i) , j, pause);
          }
    }else
    {
          for( int i = prevx, j=1; i < x; ++i, j+=2) {
              transactions[i].ToColor(leds[i], CRGB::Black , j, pause);
              transactions[879-i].ToColor(leds[879-i], CRGB::Black , j, pause);
          }
    }
    prevx = x;
  }while(x<440);

  
   
 
}
// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.
CRGB GetStep(CRGB startColor, CRGB endColor, int steps, int currentStep)
{
  double deltaR = (endColor.r - startColor.r)/(double)steps;
  double deltaG = (endColor.g - startColor.g)/(double)steps;
  double deltaB = (endColor.b - startColor.b)/(double)steps;

 
  return CRGB(startColor.r + deltaR * currentStep, startColor.g + deltaG * currentStep,startColor.b + deltaB * currentStep);
  
}
