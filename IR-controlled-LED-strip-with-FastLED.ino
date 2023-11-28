/*
Author: Blake Walker
Description: a collection of LED strip effects using the
             FastLED library, made to be controlled with
             an InfraRed Controller.
Credits: Dave Plummer from Dave's garage for the Comet Effect
         and the Creator of FastLED, Daniel Garcia, for the
         library and the Fire and Cyclon Effects.
Date: 11/27/2023
*/

#include <FastLED.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

const int kRecvPin = 22; // the signal pin of the IR reciever
#define LED_PIN 14 // the signal pin of the LED strip
#define NUM_LEDS 40 // out of the total
#define TOTAL_LEDS 144 // total number of LEDs in the strip
#define BRIGHTNESS 10 // out of 255
#define LED_TYPE WS2812B // type of LED strip
#define COLOR_ORDER GRB // most LED strips are in this format
#define UPDATES_PER_SECOND 100 
#define VOLTS 5 // max voltage that your LED strip takes
#define MILLIAMPS 500 // my usb port can output 500, my batteries can output 2200
CRGB leds[NUM_LEDS] = {0}; // Creates the empty array of LED's

// For Fire effect (DIY1)
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  60
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120
bool gReverseDirection = false;

// defines common colors
#define Black  CRGB::Black
#define Gray   CRGB::Grey // used for white because white is too bright
#define Red    CRGB::Red
#define Orange CRGB::Orange
#define Yellow CRGB::Yellow
#define Green  CRGB::Green
#define Blue   CRGB::Blue
#define Purple CRGB::Purple


// IR signals from pressing each button
// These will be different for each remote, so assign them accordingly
// the codes will be printed in the serial monitor when you press them
#define hold_down_value "18446744073709551615" // this is the code that is output when I hold down a button, I just chose to ignore it
#define on_button    "16712445"
#define red_button   "16718565"
#define green_button "16751205"
#define blue_button  "16753245"
#define white_button "16720605"
#define flash_button "16764975"
#define quick_button "16771095"
#define slow_button  "16762935"
#define diy1_button  "16724175"
#define diy2_button  "16756815"
#define diy3_button  "16740495"
#define diy4_button  "16716015"
#define diy5_button  "16748655"
#define diy6_button  "16732335"
#define auto_button  "16773135"

// initalizes the effect toggles and the speed
double speed = 1.0;
bool on = false;
bool red_on = false;
bool green_on = false;
bool blue_on = false;
bool white_on = false;
bool flash_on = false;
bool fire_on = false;
bool cyclon_on = false;
bool comet_on = false;
bool twinkle_on = false;
bool alternate_on = false;
bool wave_on = false;
bool auto_on = false;

// sets up the IR reciever
IRrecv irrecv(kRecvPin);
decode_results results;

// sets up the LED strip
void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  Serial.println("ESP32 Startup");
  pinMode(LED_PIN, OUTPUT);
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MILLIAMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

void loop() {
  if(irrecv.decode(&results)) {
    //ignores the "hold down" code and prints the other signal codes
    if(String(results.value) != hold_down_value) { // this i
      Serial.println(results.value, HEX);
      Serial.println(String(results.value));
      Serial.println("");
    }   
    if(String(results.value) == on_button) {
      if(on) {
        on = false;
        flash_on = false;
        auto_on = false;
        TurnOff();
      }
      else {
        on = true;
      }
    }
    if(String(results.value) == red_button) {
      if(red_on) {
        red_on = false;
      }
      else {
        TurnOff();
        red_on = true;
        auto_on = false;
      }
      ToggleRed();
    }
    if(String(results.value) == green_button) {
      if(green_on) {
        green_on = false;
      }
      else {
        TurnOff();
        green_on = true;
        auto_on = false;
      }
      ToggleGreen();
    }
    if(String(results.value) == blue_button) {
      if(blue_on) {
        blue_on = false;
      }
      else {
        TurnOff();
        blue_on = true;
        auto_on = false;
      }
      ToggleBlue();
    }
    if(String(results.value) == white_button) {
      if(white_on) {
        white_on = false;
      }
      else {
        TurnOff();
        white_on = true;
        auto_on = false;
      }
      ToggleWhite();
    }
    if(String(results.value) == flash_button) {
      if(flash_on) {
        flash_on = false;
      }
      else {
        flash_on = true;
        auto_on = false;
      }
    }
    if(String(results.value) == quick_button) {
      speed *= 2;
    }
    if(String(results.value) == slow_button) {
      speed *= 0.5;
    }
    if(String(results.value) == diy1_button) { // fire
      if(fire_on) {
        fire_on = false;
        fill_solid(leds, NUM_LEDS, Black);
      }
      else {
        TurnOff();
        fire_on = true;
        auto_on = false;
      }
    }
    if(String(results.value) == diy2_button) { // cyclon
      if(cyclon_on) {
        cyclon_on = false;
        fill_solid(leds, NUM_LEDS, Black);
      }
      else {
        TurnOff();
        cyclon_on = true;
        auto_on = false;
      }
    }
    if(String(results.value) == diy3_button) { // comet
      if(comet_on) {
        comet_on = false;
        fill_solid(leds, NUM_LEDS, Black);
      }
      else {
        TurnOff();
        comet_on = true;
        auto_on = false;
      }
    }
    if(String(results.value) == diy4_button) { // twinkle
      if(twinkle_on) {
        twinkle_on = false;
        fill_solid(leds, NUM_LEDS, Black);
      }
      else {
        TurnOff();
        twinkle_on = true;
        auto_on = false;
      }
    }
    if(String(results.value) == diy5_button) { // alternate
      if(alternate_on) {
        alternate_on = false;
        fill_solid(leds, NUM_LEDS, Black);
      }
      else {
        TurnOff();
        alternate_on = true;
        auto_on = false;
      }
    }
    if(String(results.value) == diy6_button) { // wave
      if(wave_on) {
        wave_on = false;
        fill_solid(leds, NUM_LEDS, Black);
      }
      else {
        TurnOff();
        wave_on = true;
        auto_on = false;
      }
    }
    if(String(results.value) == auto_button) {
      if(auto_on) {
        auto_on = false;
        fill_solid(leds, NUM_LEDS, Black);
      }
      else {
        TurnOff();
        auto_on = true;
      }
    }
    irrecv.resume();  // Ready to receive the next signal
  }

  // runs the active effect
  if(fire_on) {Fire();}
  if(flash_on) {Flash();}
  if(cyclon_on) {Cyclon();}
  if(comet_on) {Comet();}
  if(twinkle_on) {Twinkle();}
  if(alternate_on) {Alternate();}
  if(wave_on) {Wave();}
  if(auto_on) {Auto();}

  //updates the LEDs
  FastLED.show();
  FastLED.delay((1000 / (UPDATES_PER_SECOND * speed))); // alters the framerate based on the speed
}

// resets all toggle values (besides flash and auto)
void TurnOff() {
  fill_solid(leds, NUM_LEDS, Black);
  red_on = false;
  green_on = false;
  blue_on = false;
  white_on = false;
  fire_on = false;
  cyclon_on = false;
  comet_on = false;
  twinkle_on = false;
  alternate_on = false;
  wave_on = false;
}

void ToggleRed() {
  if(on) {
    if(red_on) {
      fill_solid(leds, NUM_LEDS, Red);
    }
    else {
      fill_solid(leds, NUM_LEDS, Black);
    }
  }
}

void ToggleGreen() {
  if(on) {
    if(green_on) {
      fill_solid(leds, NUM_LEDS, Green);
    }
    else {
      fill_solid(leds, NUM_LEDS, Black);
    }
  }
}

void ToggleBlue() {
  if(on) {
    if(blue_on) {
      fill_solid(leds, NUM_LEDS, Blue);
    }
    else {
      fill_solid(leds, NUM_LEDS, Black);
    }
  }
}

void ToggleWhite() {
  if(on) {
    if(white_on) {
      fill_solid(leds, NUM_LEDS, Gray); // Gray because white is too bright
    }
    else {
      fill_solid(leds, NUM_LEDS, Black);
    }
  }
}

void Flash() {
  static uint8_t brightness = 0;
  static bool increasing = true;
  CRGB color;
  if(on && flash_on) {
    // Update brightness
      if (increasing) {
        brightness++;
        if (brightness == BRIGHTNESS) {
          increasing = false;
        }
      }
      else {
        brightness--;
        if (brightness == 0) {
          increasing = true;
        }
      }
    if(red_on || green_on || blue_on) {
      // Update the color based on the state of red, green, and blue
      CRGB color = CRGB(
      red_on   ? brightness : 0,
      green_on ? brightness : 0,
      blue_on  ? brightness : 0
      );
      // Fill the LEDs with the updated color
      fill_solid(leds, NUM_LEDS, color);
    }
    else if(white_on) {
      fill_solid(leds, NUM_LEDS, CRGB(brightness, brightness ,brightness)); // same color for all rgb pixels
    }
  }
}

void Fire() { // diy1
  static uint8_t heat[NUM_LEDS]; // Array of temperature readings at each simulation cell
  // Cools down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  
  // Randomly ignites new 'sparks' of heat near the bottom
  if( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160,255) );
  }

  // Maps heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if( gReverseDirection ) {
      pixelnumber = (NUM_LEDS-1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

void Cyclon() { // diy2
  	static uint8_t hue = 0;
	// First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show(); 
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
    }
		// Wait a little bit before we loop around and do it again
		delay(10);
	}

	// Now go in the other direction.  
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
    }
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
}

void Comet() { // diy 3
  const byte fadeAmt = 128; // half
  const int cometSize = 5;
  const int deltaHue  = 4;

  static byte hue = HUE_RED;
  static int iDirection = 1;
  static int iPos = 0;

  hue += deltaHue;

  iPos += iDirection;
  if (iPos == (NUM_LEDS - cometSize) || iPos == 0)
      iDirection *= -1;
  
  for (int i = 0; i < cometSize; i++)
      leds[iPos + i].setHue(hue);
  
  // Randomly fade the LEDs
  for (int j = 0; j < NUM_LEDS; j++)
      if (random(10) > 5)
          leds[j] = leds[j].fadeToBlackBy(fadeAmt);  

  delay(50);
}

void Twinkle() { // diy 4
  static const int NUM_COLORS = 7;
  static const CRGB TwinkleColors [NUM_COLORS] = {Red, Orange, Yellow, Green, Blue, Purple, Gray};
  static int passCount = 0;
    if (passCount++ == NUM_LEDS/2) { // only lights up the given portion of the leds
        passCount = 0;
        FastLED.clear();
    }
    leds[random(NUM_LEDS)] = TwinkleColors[random(0, NUM_COLORS)];
    delay(200);
}

void Alternate() { // diy 5
  static int state = 0;
  static uint8_t hue = 0;
  const int deltaHue = 4;
  static byte hue1 = 0; // hue is a value from 0 to 360
  static byte hue2 = 180; // hue 1 and hue 2 are set to be opposite on the color wheel (complementary colors)
  for(int i = 0; i < NUM_LEDS; i++) {
    if(state == 0) {
      if(i % 2 == 0) {leds[i].setHue(hue1);}
      else if(i % 2 == 1) {leds[i].setHue(hue2);} 
      if(i == (NUM_LEDS - 1)) {state = 1;} // switch on last loop
    }
    else if (state == 1) {
      if(i % 2 == 0) {leds[i].setHue(hue2);}
      if(i % 2 == 1) {leds[i].setHue(hue1);} 
      if(i == (NUM_LEDS - 1)) {state = 0;} // switch on last loop
    }
  }
  hue1 += deltaHue;
  hue2 += deltaHue;
  delay(10);
}

void Wave() { // diy6
  uint16_t posBeat  = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);
  // code for the second wave
  uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for the LED color
  uint8_t colBeat  = beatsin8(45, 0, 255, 0, 0);
  leds[(posBeat + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255); // adds a second wave
  fadeToBlackBy(leds, NUM_LEDS, 10);
}

void Auto() {
    uint8_t secondHand = (millis() / 1000) % 60; //ticks like a second hand on a clock
    static uint8_t startingTime = 0;
    static uint8_t lastSecond = 99;
    secondHand = startingTime;
    // switches effect every 10 seconds
    if(lastSecond != secondHand) {
      Serial.println("Current Second: " + String(secondHand));
      lastSecond = secondHand;
      if(secondHand ==  0) {TurnOff(); fire_on = true;}
      if(secondHand == 10) {TurnOff(); cyclon_on = true;}
      if(secondHand == 20) {TurnOff(); comet_on = true;}
      if(secondHand == 30) {TurnOff(); twinkle_on = true;}
      if(secondHand == 40) {TurnOff(); alternate_on = true;}
      if(secondHand == 50) {TurnOff(); wave_on = true;}
    }
}