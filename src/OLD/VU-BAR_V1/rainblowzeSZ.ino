
     //--------------------------------------SIEZURE COLOR/fast rainbloze--------------------------------//

void raneblozeSZ(){
 
  
  
    
 int shibs_up = 1;
  static uint8_t shibs = 0;
  shibs+=shibs_up;
   
 fill_rainbow(leds, NUM_LEDS, shibs);

 FastLED.show();
 

  
   // delay(dels);
}
  


