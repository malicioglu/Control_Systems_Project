#ifndef TAKE_IMAGE
#define TAKE_IMAGE
#include "functions.h"




#define DOWNSAMPLING_RATE 2
#define SHOW_PICTURE 0

int arrayGray[60][80] = {0};
int maxIndexSatir = 0;
int maxIndexSutun = 0;   

int maxCounter = 0;

void readRGBImage() {
  if (!psramFound()) {
    Serial.println("psram not available");  
  }
    camera_fb_t * fb = NULL;                                                                          // store time that image capture started
    fb = esp_camera_fb_get();
    if(SHOW_PICTURE)Serial.println("camera fb get gecildi.");
    if(SHOW_PICTURE)Serial.println(fb->width);
    if(SHOW_PICTURE)Serial.println(fb->height);
      
    int width = fb->width;
    int height = fb->height;                                                                     // create a pointer for memory location to store the data
    uint32_t ARRAY_LENGTH = width*height;  

     /* Serial.print("width: ");
      Serial.println(width);
      Serial.print("height: ");
      Serial.println(height);
     Serial.print("array size: ");
     Serial.println(ARRAY_LENGTH);
*/
                                  
    if (heap_caps_get_free_size( MALLOC_CAP_SPIRAM) <  ARRAY_LENGTH) {
      Serial.println("yeterli yer yok");
      return;
    }

    maxCounter = 0;

    for(int  i = 0 ; i < height/DOWNSAMPLING_RATE ; i++){
      int ballCounter = 0;
        
      for(int j = 0 ; j < width/DOWNSAMPLING_RATE ; j++){
        for(int k = 0 ; k < DOWNSAMPLING_RATE ; k++){
          for(int m = 0 ; m < DOWNSAMPLING_RATE ; m++){
            arrayGray[i][j] += fb->buf[DOWNSAMPLING_RATE*j + (DOWNSAMPLING_RATE*i+m)*160 + k];  
          }
        }
          
        arrayGray[i][j] = arrayGray[i][j]/(DOWNSAMPLING_RATE*DOWNSAMPLING_RATE);
        if(arrayGray[i][j] < 100){
          ballCounter++;
            if(SHOW_PICTURE) Serial.print(arrayGray[i][j]);
        }
        if(arrayGray[i][j] >= 100 || j == width/DOWNSAMPLING_RATE-1){
          if(ballCounter > maxCounter){
            //Serial.print("max counter degisti: ");
            maxCounter = ballCounter;
            if(SHOW_PICTURE) Serial.print(maxCounter);
            maxIndexSatir = i;
            maxIndexSutun = j - ballCounter/2;

            ballCounter = 0;
          }
          if(SHOW_PICTURE) Serial.print("..");
        }
        if(SHOW_PICTURE) Serial.print(" ");
        
      }
        //Serial.print("ball counter: ");
        //Serial.println(ballCounter);
        if(SHOW_PICTURE) Serial.println(" ");
    }

    Serial.print("max index satir: ");
    Serial.println(maxIndexSatir);
    if(SHOW_PICTURE)Serial.print("max index sutun: ");
    if(SHOW_PICTURE)Serial.println(maxIndexSutun);
    Serial.print("cap: ");
    Serial.println(maxCounter);
    if(SHOW_PICTURE)Serial.println(" ");

    if(SHOW_PICTURE)Serial.print(" ");

    esp_camera_fb_return(fb);   // camera frame buffer
    // heap_caps_free(ptrVal);     // rgb data
}


#endif
