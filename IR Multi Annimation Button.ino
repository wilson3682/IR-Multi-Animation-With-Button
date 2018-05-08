#include <Arduino.h>
#include "FastLED.h"
#include <EEPROM.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

//-------------------------------- LED variables ----------------------------------
#define LED_PIN1 3
#define LED_PIN2 5
#define COLOR_ORDER RGB                                   // It's GRB for WS2812B
#define LED_TYPE WS2812B
#define NUM_LEDS 118
#define BRIGHTNESS 180
#define FRAMES_PER_SECOND  120
CRGB realleds[NUM_LEDS];
//CRGBSet leds(realleds, NUM_LEDS);
//CRGBArray<NUM_LEDS> leds1;
//CRGBArray<NUM_LEDS> leds2;
CRGB leds[NUM_LEDS];            // base LED array initialization
CRGB leds1[NUM_LEDS];           // LED Row 1 initialization
CRGB leds2[NUM_LEDS];           // LED Row 2 initialization
CRGB base1[NUM_LEDS];            //base pattern Row 1 that's always running
CRGB base2[NUM_LEDS];           //base pattern Row 2 that's always running
CRGB overlay1[NUM_LEDS];        //Row 1 this is added to base
CRGB overlay2[NUM_LEDS];        //Row 2 this is added to base

//--------------------------------- INPUT variables ---------------------------------

#define BUTTON 2
byte selectedEffect = 1;

const byte IRpin = 0;                        //IR pin value
const byte IRnormalDeviation = 5;            //value IR changes (tuning)
int previousIRreading = 0;                   //A variable to hold the previous input reading of the IR



void setup() {
  delay(3000);                                // 3 second delay for recovery
  Serial.begin(57600);                        // Initialize serial port for debugging.


  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds1, NUM_LEDS);       //LED_PIN1
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS);       //LED_PIN2

  digitalWrite (BUTTON, HIGH);                          // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (BUTTON), changeEffect, CHANGE); // pressed



}
void IRflash(){
  //uint8_t BRIGHTNESS = 180;
    int IRinput = analogRead(0);

    if (IRinput > 50) {
  //    BRIGHTNESS = 180;
      //fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
    }
    else if (IRinput < 40){
      //BRIGHTNESS = 100;
      fill_solid(leds1, NUM_LEDS, CRGB(255,255,255));
      fill_solid(leds2, NUM_LEDS, CRGB(255,255,255));

      FastLED.delay(500);
    //  fadeToBlackBy(leds2, NUM_LEDS, 255);
    }
    /*
    //  --------------------- serial read to debug and tune IRinput -----------------------------
      EVERY_N_MILLISECONDS(5)
        {
              Serial.println(analogRead(0));
              Serial.println(BRIGHTNESS);
              delay(50);
        }
    //  --------------------- serial read to debug and tune IRinput -----------------------------
*/
}

void loop() {
  EEPROM.get(0,selectedEffect);

  if(selectedEffect>3) {
    selectedEffect=0;
    EEPROM.put(0,0);
  }

  IRflash();        //flashes all white when IR is triped.

//EVERY_N_MILLISECONDS(100){
//LaserFlash();
//rainbow2();
//rainbowWithGlitter();
//}



  switch(selectedEffect) {

    case 0  : {
                LaserFlash();
                FastLED.show();
                break;
              }

    case 1  : {
                LaserFlash2();
                FastLED.show();
                break;
              }

    case 2  : {
                rainbowWithGlitter();
                FastLED.show();
                break;
              }

    case 3  : {
                rainbow2();
                FastLED.show();
                break;
            }
}

//FastLED.show();

} // End loop


void changeEffect() {
  if (digitalRead (BUTTON) == HIGH) {
    selectedEffect++;
    EEPROM.put(0, selectedEffect);
   // asm volatile ("  jmp 0");
  }
}
//----------------------------- Animations -------------------------------------

void LaserFlash(){
  //Variables for Eyes1
  byte red1 = random(0,255);
  byte blue1 = random(0,0);
  byte green1 = random(0,120);
  int EyeWidth1 = random(3,20);
  int EyeSpace1 = random(1,15);
  int EndPause1 = beatsin16(40, 0, 118, 0, 0);
  //int EndPause1 = beatsin8(2,64,128);

  byte red2 = random(0,0);
  byte blue2 = random(0,255);
  byte green2 = random(0,255);
  int EyeWidth2 = random(3,20);
  int EyeSpace2 = random(1,15);
  int EndPause2 = 300;

  EVERY_N_MILLISECONDS(EndPause1) {
                LaserFlashRow1(red1, blue1, green1, EyeWidth1, EyeSpace1, EndPause1);

                LaserFlashRow2(red2, blue2, green2, EyeWidth2, EyeSpace2, EndPause2);
//  FastLED.show();
}
  /*
  EVERY_N_MILLISECONDS(5) {
                Row1(random(0,255),random(0,255),random(0,255),
                              random(10,20), random(1,15), triwave8(3));

                Row2(random(0,255),random(0,255),random(0,255),
                              random(10,40), random(1,15), quadwave8(3));
FastLED.show();
}
*/
}
//----------------------------------------------------------------------------------------------------------

void LaserFlash2(){
  //Variables for Eyes1
  /*
  byte red1 = random(0,255);
  byte blue1 = random(0,0);
  byte green1 = random(0,69);
  int EyeWidth1 = random(3,20);
  int EyeSpace1 = random(1,15);
  int EndPause1 = beatsin16(40, 0, 118, 0, 0);
  //int EndPause1 = beatsin8(2,64,128);

  byte red2 = random(0,0);
  byte blue2 = random(0,69);
  byte green2 = random(0,255);
  int EyeWidth2 = random(3,20);
  int EyeSpace2 = random(1,15);
  int EndPause2 = 300;

  EVERY_N_MILLISECONDS(EndPause1) {
                LaserFlashRow1(red1, blue1, green1, EyeWidth1, EyeSpace1, EndPause1);

                LaserFlashRow2(red2, blue2, green2, EyeWidth2, EyeSpace2, EndPause2);
  FastLED.show();
}
  */
  EVERY_N_MILLISECONDS(beatsin16(40, 0, 118, 0, 0)) {
                LaserFlashRow1(random(0,255),random(0,255),random(0,255),
                              random(3,20), random(1,15), beatsin16(40, 0, 118, 0, 0));

                LaserFlashRow2(random(0,255),random(0,255),random(0,255),
                              random(3,20), random(1,15), quadwave8(3));
//FastLED.show();
}

}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------

void LaserFlashRow1(byte red1, byte blue1, byte green1,
          int EyeWidth1, int EyeSpace1, int EndPause1)
          {
  randomSeed(cubicwave8(255));


  int StartPoint1  = random( 0, NUM_LEDS - (2*EyeWidth1) - EyeSpace1 );
  int Start2ndEye1 = StartPoint1 + EyeWidth1 + EyeSpace1;

  for(int i1 = 0; i1 < EyeWidth1; i1++) {
    leds1[StartPoint1 + i1]= CRGB(red1, green1, blue1);
    leds1[Start2ndEye1 + i1]= CRGB(red1, green1, blue1);
  }
  int8_t blurAmount = dim8_raw( beatsin8(6,32, 255) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
  blur1d( leds1, NUM_LEDS, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.
  fadeToBlackBy(leds1, NUM_LEDS, 128);

}
//----------------------------------------------------------------------------------------------------------

void LaserFlashRow2(byte red2,byte blue2, byte green2,
          int EyeWidth2, int EyeSpace2, int EndPause2)
          {
            randomSeed(cubicwave8(255));


            int StartPoint2  = random( 0, NUM_LEDS - (2*EyeWidth2) - EyeSpace2 );
            int Start2ndEye2 = StartPoint2 + EyeWidth2 + EyeSpace2;

            for(int i2 = 0; i2 < EyeWidth2; i2++) {
              leds2[StartPoint2 + i2]= CRGB(red2, green2, blue2);
              leds2[Start2ndEye2 + i2]= CRGB(red2, green2, blue2);
            }
            int8_t blurAmount = dim8_raw( beatsin8(3,32,192) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
            blur1d( leds2, NUM_LEDS, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.
            fadeToBlackBy(leds2, NUM_LEDS, 128);

          }
//----------------------------------------------------------------------------------------------------------
    void rainbow()
                    {
                      static uint8_t THue = 0; //slowly rotating hue color
                      static uint8_t BHue = 0; //slowly rotating hue color
                      //EVERY_N_MILLISECONDS( 50 ) {
                        fill_rainbow( base1, NUM_LEDS, THue++, 255 / NUM_LEDS / 1);
                        fadeToBlackBy( base1, NUM_LEDS, 1);  //dim the rainbow down
                      //}
                      //EVERY_N_MILLISECONDS( 50 ) {
                        fill_rainbow(base2, NUM_LEDS, BHue--, 255 / NUM_LEDS / 1);
                        fadeToBlackBy( base2, NUM_LEDS, 1);  //dim the rainbow down
                    //  }
                      //  FastLED.show();
                    }

//----------------------------------------------------------------------------------------------------------
void rainbow2()
          {
            static uint8_t THue = 0; //slowly rotating hue color
            static uint8_t BHue = 0; //slowly rotating hue color
            //EVERY_N_MILLISECONDS( 50 ) {
              fill_rainbow( leds1, NUM_LEDS, THue++, 255 / NUM_LEDS / 1);
              fadeToBlackBy( leds1, NUM_LEDS, 1);  //dim the rainbow down
            //}
            //EVERY_N_MILLISECONDS( 50 ) {
              fill_rainbow(leds2, NUM_LEDS, BHue--, 255 / NUM_LEDS / 1);
              fadeToBlackBy( leds2, NUM_LEDS, 1);  //dim the rainbow down
            //}
          }

//---------------------------------------------------------------------------------------------------------------------

void rainbowWithGlitter()
          {
            // built-in FastLED rainbow, plus some random sparkly glitter
            rainbow();
            addGlitter();
            combinePatterns();
            //  FastLED.show();
          }
//---------------------------------------------------------------------------------------------------------------------

void combinePatterns() {
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
              leds1[i] = base1[i];      //copy base to leds
              leds1[i] += overlay1[i];  //add overlay to leds

              leds2[i] = base2[i];      //copy base to leds
              leds2[i] += overlay2[i];  //add overlay to leds
            }
          }

//---------------------------------------------------------------------------------------------------------------------

void addGlitter()
          {
            //----------------- ROW1 -------------------------
            EVERY_N_MILLISECONDS(5) {
              uint8_t pos = beatsin16( 13, 0, NUM_LEDS - 1 );
              overlay1[pos] = overlay1[NUM_LEDS - 1 - pos] = CRGB::White;
              fadeToBlackBy( overlay1, NUM_LEDS, 2);  //creates fading tail effect
              pos++;
              blur1d(overlay1, NUM_LEDS, 120);
            }
            // a white dot sweeping back and forth, with fading trails
            EVERY_N_MILLISECONDS(1) {
              fadeToBlackBy( overlay1, NUM_LEDS, 3);
              int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
              overlay1[pos] += CRGB::White;
              blur1d(overlay1, NUM_LEDS, 120);
            }

            // random White speckles that blink in and fade smoothly
            EVERY_N_MILLISECONDS(1) {
              fadeToBlackBy( overlay1, NUM_LEDS, 10);
              int pos = random16(NUM_LEDS);
              overlay1[pos] += CRGB::White;
            }

            //----------------- ROW2 -------------------------
            EVERY_N_MILLISECONDS(5) {
              uint8_t pos = beatsin16( 13, 0, NUM_LEDS - 1 );
              overlay2[pos] = overlay2[NUM_LEDS - 1 - pos] = CRGB::White;
              fadeToBlackBy( overlay2, NUM_LEDS, 2);  //creates fading tail effect
              pos++;
              blur1d(overlay2, NUM_LEDS, 120);
            }
            // a white dot sweeping back and forth, with fading trails
            EVERY_N_MILLISECONDS(1) {
              fadeToBlackBy( overlay2, NUM_LEDS, 3);
              int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
              overlay2[pos] += CRGB::White;
              blur1d(overlay2, NUM_LEDS, 120);
            }

            // random White speckles that blink in and fade smoothly
            EVERY_N_MILLISECONDS(1) {
              fadeToBlackBy( overlay2, NUM_LEDS, 10);
              int pos = random16(NUM_LEDS);
              overlay2[pos] += CRGB::White;
            }


            /* if( random8() < chanceOfGlitter) {
               leds[ random16(NUM_LEDS) ] += CRGB::White;
              }*/
          }
//---------------------------------------------------------------------------------------------------------------------
