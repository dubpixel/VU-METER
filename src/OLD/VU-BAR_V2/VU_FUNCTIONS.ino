void setcolorG(int group, CRGB kolor){

int Sgroup = group*groupSize; 
  for ( i=Sgroup; i< Sgroup+groupSize; i++){
   int SStart=segments[i];
    for ( j=SStart; j< SStart+4; j++){
          
       
      

      leds[j] = kolor;
      //ledsr[j]=kolor;
    }//end for j
  }//end for i

   // delay(10);

    
 
}//end

void updateDimming(){
  int ranger = 256/numSegments;
  

int vals[numSegments];

int currSeg =0;

 for (i=0; i< numSegments; i++){

int ADJUSTED= map(a2,0,1024,0,255);
int temp=0;

temp= map(ADJUSTED, i*12, (((i+1)*12)-1), 255, 0);
vals[i]=constrain(temp,0,255);

Serial.println(vals[i]);

  int SStart=segments[i];
  for ( j=SStart; j< SStart+4; j++){
          
       
      

      leds[j].fadeToBlackBy( vals[i] );
      //ledsr[j]=kolor;
    }//end for j

 
 }


 
//int dimval = map(a2, 0, 1024, 0 ,255);
//leds[0].fadeToBlackBy( dimval );

}

