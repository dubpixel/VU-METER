/*
 *
 *  vu meter for AIT SXSW 2019
 *
 *  mofdifed for DPX driver board + 2b2p controller
 *
 *  2.25.2019 - v2 removed more code trash for prep to install artnet 
 *
*/



// #define FORCE_SOFTWARE_SPI
// #define FORCE_SOFTWARE_PINS
#include <Artnet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

#include "FastLED.h"

//FASTLED SETTINGS

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif


#define DEBUG 
#ifdef DEBUG
#define DBP(x)  Serial.print (x)
#else
 #define DBP(x)
#endif

#define DEBUG 
#ifdef DEBUG
 #define DBPl(x)  Serial.println (x)
#else
 #define DBPl(x)
#endif


#define DEBUG_l2 0

#define LED_TYPE    WS2812
#define COLOR_ORDER RGB


//new pins v3 as of 2.2018 with new rev2 non dgs-adaptor whip
#define DATA_PIN1 17
#define CLK_PIN1 6

#define DATA_PIN2 7
#define CLK_PIN2 8


#define DATA_PIN3 5
#define CLK_PIN3 2

#define DATA_PIN4 3
#define CLK_PIN4 4

#define A_PIN 21
#define A2_PIN 20
#define B_PIN 15
#define B2_PIN 14

byte ip[] = {2,0,0,102};
byte mac[] ={0x00, 0x66, 0x06, 0x6B, 0x84, 0x58};
const int startUniverse = 1;


#define FRAMES_PER_SECOND  120


// This is an array of leds.  One item for each led in your strip.

const int NUM_LEDS = 99;
const int NUM_STRIPS = 1;

CRGB leds[NUM_LEDS];
CRGB ledsR[NUM_LEDS];


int timer = 0; //used for no delay timing



//artnet settings
// Artnet settings
Artnet artnet;
const int maxUniverses = 1; //set to one because we just care about first 3 channels //ceil((NUM_LEDS*2)/512);  //should beset equoal to num_strips
bool universesReceived[maxUniverses];
bool sendFrame = 0;
byte channelBuffer[512]; 
 
//global vals for patterns.

//variables for pulse serial write
int pulse_ctr=0; //for counting heartbeat print loops
int pulse_freq=5000;


boolean hasblanked = false;


int a1, a2, b1, b2,artVal1,artVal2;

//global counter variables
int i = 0;
int j = 0;

//delclare pixel map to segment break

int segments[20] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
int colorGroup[4] = {0, 5, 10, 15};

int groupSize = 4;

const int numSegments =20;
 boolean hasInitColor=false;


//numbr of patterns




char dim = 240;








// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(3000);

///this FULL code required now by latest updates to ethernet library
 
  /*
  pinMode(9 , OUTPUT);
  digitalWrite(9, LOW);   // reset the WIZ820io
  delay(100);
  digitalWrite(9, HIGH);   // reset the WIZ820io
  delay(150);

*/

///try new init code...


pinMode(9, OUTPUT);
  digitalWrite(9, LOW);    // begin reset the WIZ820io
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);  // de-select WIZ820io
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);   // de-select the SD Card
  digitalWrite(9, HIGH);   // end reset pulse

  
  pinMode(B_PIN, INPUT); //digital input pins
  pinMode(B2_PIN, INPUT); //digital input pins

  //pinMode(13, OUTPUT);   //led pin

  
  Serial.begin(9600);
  artnet.begin(mac, ip);
  artnet.setArtDmxCallback(onDmxFrame);
  


  // Uncomment one of the following lines for your leds arrangement.

  //FastLED.addLeds<WS2801, DATA_PIN,CLOCK_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN1, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN2, GRB>(ledsR, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE,DATA_PIN1,CLK_PIN1,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN2,CLK_PIN2,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN3,CLK_PIN3,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2801,DATA_PIN4,CLK_PIN4,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);



  //     FastLED.addLeds<WS2801, 6,7 , RGB>(leds, NUM_LEDS);
  //     FastLED.addLeds<WS2801, 4, 5, RGB>(leds, NUM_LEDS);
  //      FastLED.addLeds<WS2801, 3, 2, RGB>(leds, NUM_LEDS);
  //   FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  //FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);

 // bootSeqLED();

  delay(1000);
  initTest();
  delay(3000);



}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {

  FastLED.setBrightness(255);

  artnet.read();
  // DBPl("read");
  pulse();


  //raneblozeSZ();

                      CRGB RealOrng = CRGB(255,69,0);
                       setcolorG(0, CRGB::Green);
                       setcolorG(1, CRGB::Green);
                       setcolorG(2, CRGB::Yellow);
                       setcolorG(3,RealOrng);
                       setcolorG(4, CRGB::Red);


                       //update brightness to dim
                      // readAnalog();
                       
                       a1 = analogRead(A_PIN);
                       a2 = analogRead(A2_PIN);
                       setBright(); //map a1 to  brightness
                      // int poop= map(a2,0,1024,0,255);
                      updateDimmingL(artVal1);
                       updateDimmingR(artVal2);

                       //  b1 = digitalRead(B_PIN);
                       //b2 =  digitalRead(B2_PIN);

                       //buttonCheck(); //go check the status of the pins, increment/decrement thing


                       //setPAL();

                       // patternBin[currPATT](); //call the actual pattern.

  FastLED.show();


                       // delay just enough to keep a steady frame rate, e.g 100 FPS
                       // delayToSyncFrameRate( FRAMES_PER_SECOND);


}//end loop



void pulse() {

  pulse_ctr++;
  if (pulse_ctr == (pulse_freq * 25)) {
    Serial.println(".//");
    pulse_ctr = 0;

  }//end freq
}//end pulse
void dummy(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  Serial.println("dummy!");
  delay(100);

}


                //--------------------------------------DECLARE SIZE OF ARRAY JAMS------------------------------------//

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


 //--------------------------------------READ ANALOG PINS------------------------------------//

void readAnalog(){

                       a1 = analogRead(A_PIN);
                       a2 = analogRead(A2_PIN);
}

                //---------------------------------------SET/UPDATE DIM (brightness) ------------------------------------//
void setBright() {

  dim = map(a1, 1023, 0, 0, 255);

}







//---------------------------------------HUE TO CRGB------------------------------------//
CRGB toCOLOR(uint8_t hue) {
  CRGB temp;
  temp.setHue(hue);
  return temp;
}



//invert LEDS
int SLDO(int incoming) {
  int temp;
  temp = map(incoming, 0, (NUM_LEDS - 1), (NUM_LEDS - 1), 0);
  return temp;

}



//---------------------------------------FADE TO BLACK------------------------------------//
void fadeToBlack()
{
  //  fadeToBlackBy(leds, NUM_LEDS, 1);

  for (int j = 0; j < NUM_LEDS; j++) leds[j]--;


  FastLED.show();

}

//--------------------------------------BPOOP------------------------------------//
void checkblank() {
  if (!hasblanked) {
    setcolor(CRGB::Black);
    hasblanked = true;
  }
}
//this should go in any pattern that is not required to blank first
void resetblank() {
  hasblanked = false;
}

//--------------------------------------BPOOP------------------------------------//
// Create a 24 bit color value from R,G,B ////these strips are BRG, but you might need to change this
unsigned int Color(byte g/*r*/, byte b, byte /*b*/r)
{
  //Take the lowest 8 bits of each value and append them end to end
  return ( ((unsigned int)g & 0x1F ) << 16 | ((unsigned int)b & 0x1F) << 8 | (unsigned int)r & 0x1F);
}


// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  //  progmemPrint(str);
  Serial.println();
}














///-----------------BOOT SEQ  ----//

void bootSeqLED() {

  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(50);

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(50);

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(50);



  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(50);


  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(1000);

}


///-----------------CALLBACK FRAME  ----//
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  DBPl("DOIN THE FRAME JAM");
 
  int relative_uni=(universe - startUniverse);

 
 if ((universe >= startUniverse) && (universe <= (startUniverse+(maxUniverses-1)))){ //if the universe inbound is equal or greaterthan the starting
   
    //debug activated
    DBPl("YUP");

  //  if ((universe - startUniverse) < maxUniverses){ //and  if the unvierse requested is within the range we're dealing with
     universesReceived[relative_uni] = 1; //start filing the array at basically zero to say that we;ve got it
     
     //debug activated
     DBP("uni: ") ;
     DBPl(universe);
  
  // }

  for (int i = 0 ; i < length ; i++)
  {
     
      
      //channelBuffer[relative_uni][i]
   // int bufferIndex = i + ((universe - startUniverse) * length);
      channelBuffer[i] = byte(data[i]);
      DBP("DATA: ");
      DBPl(channelBuffer[i]);
      
      
  }      

//checkto see if all the universes have arrived
  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      //debug activated
      DBPl("NOTYET");
     
      sendFrame = 0;
      break;
    }
   sendFrame = 1;
  }

  // read universe and put into the right part of the display buffer
  
if (sendFrame) //if everyone is here!!
     {      

  // send to leds

      artVal1 = channelBuffer[0];
      artVal2 = channelBuffer[1];
    
   // updateDimmingL(artVal1);

      
  
 
        //leds.show();
      //  FastLED.show();
        DBP("THIS:");
        DBPl(artVal1);
        Serial.println("^'DATIN LEDS");
   
    
    
       // Reset universeReceived to 0
       memset(universesReceived, 0, maxUniverses);
      // universesReceieved=0;
      // universesReceived[universe - startUniverse]
    
      } //endif sendframe
      
      FastLED.delay(5);
      //Serial.println("what");
      
      DBPl("-//end//-");
    }//if universe equal to or greater then
    
}//end callback function loop



void initTest()
{
  int y = 0;
  for (int i = 0 ; i < NUM_LEDS ; i++){
      leds[i].setRGB(255, 0, 0);
      ledsR[i].setRGB(255, 0, 0);
      }


  FastLED.show();
  delay(500);
  for (int i = 0 ; i < NUM_LEDS ; i++){
    //  leds.setPixelColor(i, 0, 127, 0)
    
      leds[i].setRGB(0, 255, 0);
       ledsR[i].setRGB(0, 255, 0);
   }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < NUM_LEDS ; i++){
    //  leds.setPixelColor(i, 0, 0, 127);
   
      leds[i].setRGB(0, 0, 255);
      ledsR[i].setRGB(0, 0, 255);
 }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < NUM_LEDS ; i++){
   
      leds[i].setRGB(0, 0, 0);
      ledsR[i].setRGB(0, 0, 0);
   }
  //leds.setPixelColor(i, 0, 0, 0);
  FastLED.show();

}

