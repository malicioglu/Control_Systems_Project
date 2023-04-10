#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#if !defined ESP32
 #error This sketch is only for an ESP32Cam module
#endif

#include "esp_camera.h"

 #include <Arduino.h>

bool cameraImageSettings();  
/*void sendText(String text);
bool initialiseCamera() ;
bool cameraImageSettings() ;
void resetCamera(bool type = 0) ;
void changeResolution(framesize_t tRes = FRAMESIZE_96X96) ;
byte storeImage();
*/
// ---------------------------------------------------------------
//                           -SETTINGS
// ---------------------------------------------------------------

 const char* stitle = "ESP32Cam-demo";                  // title of this sketch
 const char* sversion = "25Jan22";                      // Sketch version

 bool sendRGBfile = 0;                                  // if set '/rgb' will just return raw rgb data which can be saved as a file rather than display a HTML pag

 uint16_t dataRefresh = 2;                              // how often to refresh data on root web page (seconds)
 uint16_t imagerefresh = 2;                             // how often to refresh the image on root web page (seconds)

 const bool serialDebug = 1;                            // show debug info. on serial port (1=enabled, disable if using pins 1 and 3 as gpio)

 #define useMCP23017 0                                  // if MCP23017 IO expander chip is being used (on pins 12 and 13)

 // Camera related
   bool flashRequired = 1;                              // If flash to be used when capturing image (1 = yes)
   framesize_t FRAME_SIZE_IMAGE = FRAMESIZE_QQVGA;        // Image resolution:
                                                        //               default = "const framesize_t FRAME_SIZE_IMAGE = FRAMESIZE_VGA"
                                                        //               160x120 (QQVGA), 128x160 (QQVGA2), 176x144 (QCIF), 240x176 (HQVGA),
                                                        //               320x240 (QVGA), 400x296 (CIF), 640x480 (VGA, default), 800x600 (SVGA),
                                                        //               1024x768 (XGA), 1280x1024 (SXGA), 1600x1200 (UXGA)
   #define PIXFORMAT PIXFORMAT_JPEG;                    // image format, Options =  YUV422, GRAYSCALE, RGB565, JPEG, RGB888
   int cameraImageExposure = 0;                         // Camera exposure (0 - 1200)   If gain and exposure both set to zero then auto adjust is enabled
   int cameraImageGain = 0;                             // Image gain (0 - 30)
   int cameraImageBrightness = 0;                       // Image brightness (-2 to +2)

 const int TimeBetweenStatus = 600;                     // speed of flashing system running ok status light (milliseconds)

 const int serialSpeed = 115200;                        // Serial data speed to use

 // NTP - Internet time
   const char* ntpServer = "pool.ntp.org";
   const char* TZ_INFO    = "GMT+0BST-1,M3.5.0/01:00:00,M10.5.0/02:00:00";  
   long unsigned lastNTPtime;
   tm timeinfo;
   time_t now;
 #define CAMERA_MODEL_AI_THINKER
 #define PWDN_GPIO_NUM     32      // power to camera (on/off)
 #define RESET_GPIO_NUM    -1      // -1 = not used
 #define XCLK_GPIO_NUM      0
 #define SIOD_GPIO_NUM     26      // i2c sda
 #define SIOC_GPIO_NUM     27      // i2c scl
 #define Y9_GPIO_NUM       35
 #define Y8_GPIO_NUM       34
 #define Y7_GPIO_NUM       39
 #define Y6_GPIO_NUM       36
 #define Y5_GPIO_NUM       21
 #define Y4_GPIO_NUM       19
 #define Y3_GPIO_NUM       18
 #define Y2_GPIO_NUM        5
 #define VSYNC_GPIO_NUM    25      // vsync_pin
 #define HREF_GPIO_NUM     23      // href_pin
 #define PCLK_GPIO_NUM     22      // pixel_clock_pin



//#include "esp_camera.h"         
// #include "camera_pins.h"
#include <base64.h>             // for encoding buffer to display image on page
#include "driver/ledc.h"        // used to configure pwm on illumination led

// spiffs used to store images if no sd card present
 #include <SPIFFS.h>
 #include <FS.h>                // gives file access on spiffs

// Used to disable brownout detection
 #include "soc/soc.h"
 #include "soc/rtc_cntl_reg.h"

// sd-card
 #include "SD_MMC.h"                         
 #include <SPI.h>
 #include <FS.h>                             // gives file access
 #define SD_CS 5                             // sd chip select pin = 5


// Define some global variables:
 uint32_t lastStatus = millis();           // last time status light changed status (to flash all ok led)
 bool sdcardPresent;                       // flag if an sd card is detected
 int imageCounter;                         // image file name on sd card counter
 String spiffsFilename = "/image.jpg";     // image name to use when storing in spiffs
 String ImageResDetails = "Unknown";       // image resolution info


// ----------------------------------------------------------------
//                        Initialise the camera
// ----------------------------------------------------------------
// returns TRUE if successful

bool initialiseCamera() {

   camera_config_t config;
   config.ledc_channel = LEDC_CHANNEL_0;
   config.ledc_timer = LEDC_TIMER_0;
   config.pin_d0 = Y2_GPIO_NUM;
   config.pin_d1 = Y3_GPIO_NUM;
   config.pin_d2 = Y4_GPIO_NUM;
   config.pin_d3 = Y5_GPIO_NUM;
   config.pin_d4 = Y6_GPIO_NUM;
   config.pin_d5 = Y7_GPIO_NUM;
   config.pin_d6 = Y8_GPIO_NUM;
   config.pin_d7 = Y9_GPIO_NUM;
   config.pin_xclk = XCLK_GPIO_NUM;
   config.pin_pclk = PCLK_GPIO_NUM;
   config.pin_vsync = VSYNC_GPIO_NUM;
   config.pin_href = HREF_GPIO_NUM;
   config.pin_sscb_sda = SIOD_GPIO_NUM;
   config.pin_sscb_scl = SIOC_GPIO_NUM;
   config.pin_pwdn = PWDN_GPIO_NUM;
   config.pin_reset = RESET_GPIO_NUM;
   config.xclk_freq_hz = 20000000;               // XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
   config.pixel_format = PIXFORMAT_GRAYSCALE;              // Options =  YUV422, GRAYSCALE, RGB565, JPEG, RGB888
   config.frame_size = FRAMESIZE_QQVGA;         // Image sizes: 160x120 (QQVGA), 128x160 (QQVGA2), 176x144 (QCIF), 240x176 (HQVGA), 320x240 (QVGA),
                                                 //              400x296 (CIF), 640x480 (VGA, default), 800x600 (SVGA), 1024x768 (XGA), 1280x1024 (SXGA),
                                                 //              1600x1200 (UXGA)
   config.jpeg_quality = 12;                     // 0-63 lower number means higher quality (can cause failed image capture if set too low at higher resolutions)
   config.fb_count = 1;                          // if more than one, i2s runs in continuous mode. Use only with JPEG

   if (!psramFound()) {
      Serial.println("Warning: No PSRam found so defaulting to image size 'CIF'");
     config.frame_size = FRAMESIZE_CIF;
   }


   esp_err_t camerr = esp_camera_init(&config);  // initialise the camera
   if (camerr != ESP_OK) {
      Serial.printf("ERROR: Camera init failed with error 0x%x", camerr);
   }

   cameraImageSettings();                        // apply custom camera settings

   return (camerr == ESP_OK);                    // return boolean result of camera initialisation
}


void initCamera(){

 WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);     // Turn-off the 'brownout detector'

  Serial.print(("\nInitialising camera: "));
  if (initialiseCamera()) {
    Serial.println("OK");
  }
  else {
   Serial.println("failed");
  }

  if (!SPIFFS.begin(true)) {
    Serial.println(("An Error has occurred while mounting SPIFFS - restarting"));
    delay(5000);
    ESP.restart();                               // restart and try again
    delay(5000);
  } 
  else {
    Serial.print(("SPIFFS mounted successfully: "));
    Serial.printf("total bytes: %d , used: %d \n", SPIFFS.totalBytes(), SPIFFS.usedBytes());
  
  }

}

void sendText(String text){
  Serial.println(text);
}
bool cameraImageSettings() {

   Serial.println("Applying camera settings");

   sensor_t *s = esp_camera_sensor_get();
   // something to try?:     if (s->id.PID == OV3660_PID)
   if (s == NULL) {
      Serial.println("Error: problem reading camera sensor settings");
     return 0;
   }

   // if both set to zero enable auto adjust
   if (cameraImageExposure == 0 && cameraImageGain == 0) {
     // enable auto adjust
       s->set_gain_ctrl(s, 1);                       // auto gain on
       s->set_exposure_ctrl(s, 1);                   // auto exposure on
       s->set_awb_gain(s, 1);                        // Auto White Balance enable (0 or 1)
       s->set_brightness(s, cameraImageBrightness);  // (-2 to 2) - set brightness
   } else {
     // Apply manual settings
       s->set_gain_ctrl(s, 0);                       // auto gain off
       s->set_awb_gain(s, 1);                        // Auto White Balance enable (0 or 1)
       s->set_exposure_ctrl(s, 0);                   // auto exposure off
       s->set_brightness(s, cameraImageBrightness);  // (-2 to 2) - set brightness
       s->set_agc_gain(s, cameraImageGain);          // set gain manually (0 - 30)
       s->set_aec_value(s, cameraImageExposure);     // set exposure manually  (0-1200)
   }

   return 1;
}  // cameraImageSettings

void resetCamera(bool type = 0) {
  if (type == 1) {
    // power cycle the camera module (handy if camera stops responding)
      digitalWrite(PWDN_GPIO_NUM, HIGH);    // turn power off to camera module
      delay(300);
      digitalWrite(PWDN_GPIO_NUM, LOW);
      delay(300);
      initialiseCamera();
    } else {
    // reset via software (handy if you wish to change resolution or image type etc. - see test procedure)
      esp_camera_deinit();
      delay(50);
      initialiseCamera();
    }
}

void changeResolution(framesize_t tRes = FRAMESIZE_96X96) {
  esp_camera_deinit();     // disable camera
  delay(50);
  if (tRes == FRAMESIZE_96X96) {      // taken as none supplied so cycle through several
    if (FRAME_SIZE_IMAGE == FRAMESIZE_QVGA) tRes = FRAMESIZE_VGA;
    else if (FRAME_SIZE_IMAGE == FRAMESIZE_VGA) tRes = FRAMESIZE_XGA;
    else if (FRAME_SIZE_IMAGE == FRAMESIZE_XGA) tRes = FRAMESIZE_UXGA;
    else tRes = FRAMESIZE_QVGA;
  }
  FRAME_SIZE_IMAGE = tRes;
  initialiseCamera();
   Serial.println("Camera resolution changed to " + String(tRes));
  ImageResDetails = "Unknown";   // set next time image captured
}

byte storeImage() {

 byte sRes = 0;                // result flag
 fs::FS &fs = SD_MMC;          // sd card file system

 
 camera_fb_t *fb = esp_camera_fb_get();             // capture image frame from camera
   
   if (!fb) {
      Serial.println("Error: Camera capture failed");
     return 0;
   }

 // save image to Spiffs
   if (!sdcardPresent) {
      Serial.println("Storing image to spiffs only");
     SPIFFS.remove(spiffsFilename);                         // if file name already exists delete it
     File file = SPIFFS.open(spiffsFilename, FILE_WRITE);   // create new file
     if (!file) {
        Serial.println("Failed to create file in Spiffs - will format and try again");
       if (!SPIFFS.format()) {                              // format spiffs
          Serial.println("Spiffs format failed");
       } else {
         file = SPIFFS.open(spiffsFilename, FILE_WRITE);    // try again to create new file
         if (!file) {
            Serial.println("Still unable to create file in spiffs");
         }
       }
     }
     if (file) {       // if file has been created ok write image data to it
       if (file.write(fb->buf, fb->len)) {
         sRes = 1;    // flag as saved ok
       } else {
          Serial.println("Error: failed to write image data to spiffs file");
       }
     }
     if (sRes == 1 && serialDebug) {
       Serial.print("The picture has been saved to Spiffs as " + spiffsFilename);
       Serial.print(" - Size: ");
       Serial.print(file.size());
       Serial.println(" bytes");
     }
     file.close();
   }



 esp_camera_fb_return(fb);        // return frame so memory can be released

 return sRes;

} // storeImage

#endif

