#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SD.h>

#define chipSelect 4
#define min(a,b) ((a)<(b)?(a):(b))
int snap_count = 0;
Adafruit_VC0706 cam = Adafruit_VC0706(&Serial1);

void setup_ttl() {
  pinMode(4, OUTPUT);
  
  // see if the card is present and can be initialized:
  SD.begin(chipSelect);
  Serial.println("Chip Found"); 
  
  // Try to locate the camera
//  cam.begin();
//  Serial.println("Camera Found");

  if (cam.begin()) {
      Serial.println(F("Cam OK!"));
      
  } else {
      Serial.println(F("NO CAM!"));
      while(1);
  }
  
  // Print out the camera version information (optional)
  char *reply = cam.getVersion();

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120 
  // Remember that bigger pictures take longer to transmit!
  
//  cam.setImageSize(VC0706_640x480);        // biggest
//  cam.setImageSize(VC0706_320x240);        // medium
  cam.setImageSize(VC0706_160x120);          // small


}

void snapten() {
   if (!cam.takePicture()) {
     Serial.println("ERROR PIC!");
   } else {
     Serial.println("Picture Taken!");
   }

   char filename[13];

  strcpy(filename, "IMAG000.JPG");
  
  for (int i = 0; i < 1000; i++) {
    filename[4] = '0' + i/100;
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  // Open the file for writing
  File imgFile = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();

  Serial.println(filename);Serial.println("   FILE NAME***");
  Serial.println(jpglen);Serial.println("   JPG LENGTH***");

  int32_t time = millis();
//  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(64, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      wCount = 0;
    }
    jpglen -= bytesToRead;
  }
  imgFile.close();
//  snap_count++;
//  }
  cam.reset();
}

void snapgreen() {
   if (!cam.takePicture()) {
//      Serial.println("ERROR PIC!");
   } else {
//      Serial.println("Picture Taken!");
   }

   char filename[13];

  strcpy(filename, "GREE000.JPG");
  for (int i = 0; i < 100; i++) {
    filename[4] = '0' + i/10;
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  // Open the file for writing
  File imgFile = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();

  int32_t time = millis();
//  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(64, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      wCount = 0;
    }
    jpglen -= bytesToRead;
  }
  imgFile.close();
//  snap_count++;
//  }
  cam.reset();
}
