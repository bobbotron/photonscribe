#include <FastLED.h>
#include "./Flippy.h"

#define LED_PIN     6
#define NUM_LEDS    41
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define BUTTON_1 A1
#define BUTTON_2 A0
#define BUTTON_3 A3 
// A2
#define BUTTON_4 A3

#define MODES 5

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 60

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int mode = 0;

int fade = -255;

int cylonMin = 0;
int cylonMax = NUM_LEDS;
int cylon = 0;
int cylonAdder = 1;

int globalHue = 0;
int haloTexture = 0;

unsigned cautionCounter = 0;
int cautionIntro = 0;
#define CAUTION_INTRO_PIXELS 20

/**
 *  There are 4 buttons in the circuit, saves creating
 *  similar variables for each button.
 **/
struct ButtonState {
  unsigned long lastUpdate = 0;
  int currentState = 0;
  int pin;
};

ButtonState buttons[4];

void setup() {
    delay( 3000 ); // power-up safety delay

    // initialize button state
    buttons[0].pin = BUTTON_1;
    buttons[1].pin = BUTTON_2;
    buttons[2].pin = BUTTON_3;
    buttons[3].pin = BUTTON_4;

    for (int i=0; i< 4; i++)
    {
      pinMode(buttons[i].pin, INPUT_PULLUP);
      buttons[i].currentState = digitalRead(buttons[i].pin);
      buttons[i].lastUpdate = millis();
    }

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    playStartupAnimation();
}

void playStartupAnimation()
{
  for (int i=0; i<4; i++)
  {
    for (int j=0; j<3; j++)
    {
      leds[j] = CHSV( 50, 256, 60);
    }

    FastLED.show();
    FastLED.delay(100);

    blackOut();
    FastLED.delay(100);
  }
}

void loop()
{

    // Call render function based on mode, should be a switch statement
    if (mode == 0)
    {
      RandoLED();
    }
    if (mode == 1)
    {
      RainbowLED();
    }
    if (mode == 2)
    {
      Cylon();
    }
    if (mode == 3)
    {
     Halo();
    }
    if (mode == 4)
    {
      SolidColour();
    }
//    if (mode == 6)
//    {
//      imageRender(NGPC_IMG_WIDTH, NGPC_IMG_HEIGHT, ngpc_image);
//    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);

    const long t = millis();

    // check mode select button
    if(t-buttons[1].lastUpdate > 50)
    {

      int key2S = digitalRead(BUTTON_2);
      if (buttons[1].currentState != key2S) {
        buttons[1].currentState = key2S;

        if (!key2S)
        {
          mode++;
          mode = mode % MODES;
          blackOut();
          for (int i=0; i< mode + 1; i++)
          {
            leds[i].setRGB(100,0,0);
          }
          FastLED.show();
          
          delay(200);
        }
        else
        {
          blackOut();
        }
      }

      buttons[1].lastUpdate = t;
    }
}

void RandoLED()
{
  for( int i = 0; i < NUM_LEDS; i++) {
    int distanceToEnd = i < (NUM_LEDS / 2) ? i : NUM_LEDS - i - 1;
    int rounder = distanceToEnd > 10 ? 0 : - ( (10 - distanceToEnd)  * 20 );
    leds[i] = CHSV( 0, 0, max(0, random8(255) + fade + rounder));
  }

    const long t = millis();
    if (t - buttons[0].lastUpdate > 50) {
      int key1S = digitalRead(BUTTON_1);// read if key1 is pressed
       if(!key1S){
        // fade in
        fade = min(0, fade + 5);
       }
       else{
        // fade out
        fade = max(-255, fade - 5);
       }

       buttons[0].lastUpdate = t;
    }
}

void RandoGoldLED()
{
  for( int i = 0; i < NUM_LEDS; i++) {
    if (random8() > 30)
    {
      leds[i] = CHSV( random8(40), 255, max(0, random8(255) + fade));
    }
    else
    {
      leds[i] = CHSV( random8(10)+150, 255, max(0, random8(255) + fade));
    }
  }

    const long t = millis();
    if (t - buttons[0].lastUpdate > 50) {
      int key1S = digitalRead(BUTTON_1);// read if key1 is pressed
       if(!key1S){
        // fade in
        fade = min(0, fade + 2);
       }
       else{
        // fade out
        fade = max(-255, fade - 2);
       }

       buttons[0].lastUpdate = t;
    }
}

void RainbowLED()
{
  int key1S = digitalRead(BUTTON_1);// read if key1 is pressed
  if(!key1S){
    for( int i = 1; i < 3; i++) {
      leds[i].setRGB(255,255,255);
    }

    // red
    for( int i = 3; i < 9; i++) {
      leds[i].setRGB(228, 3, 3);
    }

    // orange
    for( int i = 9; i < 15; i++) {
      leds[i].setRGB(255, 140, 0);
    }

    // yellow 255, 237, 0
    for( int i = 15; i < 21; i++) {
      leds[i].setRGB(255, 237, 0);
    }

    // green 0, 128, 38
    for( int i = 21; i < 27; i++) {
      leds[i].setRGB(0, 128, 38);
    }

    // indigo 36, 64, 142
    for( int i = 27; i < 33; i++) {
      leds[i].setRGB(36, 64, 142);
    }

    // violet 115, 41, 130
    for (int i = 33; i < 39; i++) {
      leds[i].setRGB(115, 41, 130);
    }

    // white strip
    for (int i = 39; i < NUM_LEDS; i++) {
      leds[i].setRGB(255,255,255);
    }

  }
  else
  {
    blackOut();
  }
}

void Halo()
{
  int key1S = digitalRead(BUTTON_1);// read if key1 is pressed
  if(!key1S){
    
    for( int i = 0; i < NUM_LEDS - 3; i++) {
      leds[i].setHSV( globalHue, 255, min( ( i * 6), 255)); //.setRGB(0,0,min( ( i * 6), 255));
    }



    // bright end strip
    for (int i = NUM_LEDS - 3; i < NUM_LEDS; i++) {
      leds[i].setHSV( globalHue, 255, 255 );
    }
    haloTexture = ( haloTexture + 1 ) % 20;
    
    for (int i = NUM_LEDS - 15; i < NUM_LEDS - 5; i++)
    {
      leds[i].fadeToBlackBy(min(255, abs(haloTexture - 10) * 25));
    }

    
  }
  else
  {
    blackOut();
  }

  UpdateHue();
}

void UpdateHue()
{
  int key3S = digitalRead(BUTTON_3);// read if key3 is pressed
  if(!key3S){
    globalHue = ( globalHue + 1 ) % 255;
    for (int i = 0; i < 3; i++) {
      leds[i].setHSV( globalHue, 255, 255 );
    }
  }
}

void SolidColour()
{
  int key1S = digitalRead(BUTTON_1);// read if key1 is pressed
  if(!key1S){
    // bright end strip
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].setHSV( globalHue, 255, 255 );
    }
  }
  else
  {
    blackOut();
  }

  UpdateHue();
}

void Cylon()
{
  int key1S = digitalRead(BUTTON_1);// read if key1 is pressed
  if(!key1S){
    cylon = cylon + cylonAdder;

    if (cylon >= cylonMax)
    {
      cylonAdder = -1;
    }
    else if (cylon <= cylonMin)
    {
      cylonAdder = 1;
    }

    fadeToBlackBy(leds, NUM_LEDS, 20);
    for( int i = 0; i < NUM_LEDS; i++) {
      int distanceToCylon = abs(cylon-i);
      if (distanceToCylon < 2)
      {
      //leds[i] = CHSV( 0, 0, min(255, (NUM_LEDS - distanceToCylon) * 6));
      leds[i] = CHSV( 0, 0, 255);
      }
      else
      {
        if (random8() < 2)
        {
          leds[i] = CHSV( random8(), 255, random8(50) + 205);
        }
        //leds[i] = CHSV( 0, 0, 0);
      }
    }

  }
  else
  {
    // blackOut();
    fadeToBlackBy(leds, NUM_LEDS, 20);
  }
}


void blackOut()
{
  for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV( 0, 0, 0);
    }
}

void CautionTape()
{
  int key1S = digitalRead(BUTTON_1);// read if key1 is pressed
  if(!key1S){
    if (cautionIntro < CAUTION_INTRO_PIXELS)
    {
      cautionIntro++;
      for (int i=0; i< NUM_LEDS; i++)
      {
        leds[i].setRGB(255, 237, 0);
      }
    }
    else
    {
      cautionCounter++;
      int midpoint = (int) (NUM_LEDS/2);
      for (int i=0; i< midpoint; i++)
      {
        unsigned ccl = ((i*2) + cautionCounter) % 180;
        if (ccl < 90)
        {
          leds[i] = CRGB::Black;
          leds[NUM_LEDS - i -1 ] = CRGB::Black;
        }
        else
        {
          leds[i].setRGB(255, 237, 0);
          leds[NUM_LEDS - i -1 ].setRGB(255, 237, 0);
        }
      }
      leds[0].setRGB(255, 237, 0);
      leds[NUM_LEDS-1].setRGB(255, 237, 0);
      if (NUM_LEDS % 2 == 1)
      {
        leds[midpoint] = leds[midpoint-1];
      }
    }
  }
  else
  {
    if (cautionIntro > 0)
    {
      for (int i=0; i< NUM_LEDS; i++)
      {
        leds[i].setRGB(255, 237, 0);
      }
      cautionIntro--;
    }
    else
    {
      blackOut();
    }
  }
}

int imgIndex = -1;

#define START_OF_IMG_CONTENT 1024
void imageRender(int width, int height, const uint16_t * imgArray)
{
  if (imgIndex == -1)
  {
    int key1S = digitalRead(BUTTON_1);
    if (buttons[0].currentState != key1S) {
      buttons[0].currentState = key1S;

      if (!key1S)
      {
        imgIndex = 0;
      }
    }
  }
  else
  {
    // slow things down a little bit
    FastLED.delay(10);
    if (imgIndex >= width)
    {
      // reset the image
      imgIndex = -1;
      blackOut();
    }
    else
    {
      for (int i = 0; i< height; i++)
      {
        //pgm_read_word(&tv[j])
        uint16_t indexVal = pgm_read_word(&imgArray[START_OF_IMG_CONTENT + (i*width) + imgIndex]);
        leds[i].setRGB(pgm_read_word(&imgArray[indexVal*4]), pgm_read_word(&imgArray[(indexVal*4) + 1]), pgm_read_word(&imgArray[(indexVal*4) + 2]));
      }

      imgIndex++;
    }
  } 
}

