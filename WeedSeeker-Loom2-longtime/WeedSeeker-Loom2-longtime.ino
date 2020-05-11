///////////////////////////////////////////////////////////////////////////////

// This is simple one of the simplest examples of using Loom. The code used
// in this example shows up in the majority of other cases as well. 

// The include, config, LoomFactory, and LoomManager are typically used in
// this way.

// The setup almost always includes what is in this example, and in many 
// cases, no further setup is necessary

// The loop is also the start of what most cases use, namely: measure,
// package, display, and wait (according to 'interval' in the config).
// Other examples simply build off of this sequence

// The configuration currently expects the relay is connected to pin 10

///////////////////////////////////////////////////////////////////////////////
// WeedWarden
#include <Loom.h>
#include "Snapshot.h"
//#include "Loom_AS7265X.cpp"

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

int relaySet = A5;
int ledpin = A1;

void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();
  setup_ttl();
	LPrintln("\n ** Setup Complete ** ");
 
  pinMode(relaySet, OUTPUT);
//  pinMode(ledpin, OUTPUT);
  
  digitalWrite(relaySet, LOW);
//  digitalWrite(ledpin, LOW); 
  
}

void loop() 
{
	Loom.measure();
  auto package = Loom.package();
	Loom.display_data();
  Loom.SDCARD().log();

  String time = package["timestamp"]["time"].as<const char*>();
  String seconds = time.substring(6);
  String minutes = time.substring(3);
  int mins = minutes.toInt();
  int sec = seconds.toInt();

  while(sec%2 != 0){
    package = Loom.package(); 
    time = package["timestamp"]["time"].as<const char*>();
    seconds = time.substring(6);
    sec = seconds.toInt();
  }

  if (mins%15 == 0){       
//    snapten();
    Loom.SDCARD().log("10sec.csv");
    delay(60000);
    if (sec%20 == 0){
      //trun on pin13
    }
  }

//  JsonObject as7265x = (package["contents"].as<JsonArray>())[1]["data"];
//  const uint16_t a = as7265x["410nm"];
//  const uint16_t b = as7265x["435nm"];
//  const uint16_t c = as7265x["460nm"];
//  const uint16_t d = as7265x["485nm"];
//  const uint16_t e = as7265x["510nm"];
//  const uint16_t f = as7265x["535nm"];
//  const uint16_t g = as7265x["560nm"];
//  const uint16_t h = as7265x["585nm"];
//  const uint16_t i = as7265x["645nm"];
 
//  Serial.println(sec);Serial.println(" Seconds+++"); 

//  if ((int)b < (int)c && (int)d < (int)c && (int)e < (int)f && (int)i < (int)f && (((int)f + (int)g + (int)h)/3) <= (int)c && (int)c <= 10){
// (int)h < (int)g
//    snapgreen();
//    Loom.SDCARD().log("Green.csv");
//    digitalWrite(ledpin, HIGH);
//    delay(1000);
//    digitalWrite(ledpin, LOW);
//  }
//  else{
//    digitalWrite(ledpin, LOW);
//  }
//  if ((int)a < (int)c && (int)b < (int)c && (int)d < (int)c && (int)e < (int)c && (int)f < (int)c && (int)b < (int)a && (int)e < (int)f){
//    snapgreen();
//    Loom.SDCARD().log("Green.csv");
//    
//   
//    digitalWrite(ledpin, HIGH);
//    delay(500);
////
////    snapred();
////    Loom.measure();
////    Loom.package(); 
////    Loom.SDCARD().log("IRgre.csv"); 
////
//////    if (NIRA > NIRB){  
//////    unlatchedRelay(1);
//////    }
////    digitalWrite(ledpin, LOW);
//  }
//  
//  else{
////    unlatchedRelay(0);
//  }
//  
  Loom.pause();
}

// ================================================================ 
// ===                        LATCHED RELAY                     ===
// ================================================================  
void unlatchedRelay(bool io)
{
  if(io)
  {
    digitalWrite(relaySet, HIGH);   // turn A5 on
    delay(2500);
    digitalWrite(relaySet, LOW);    //turn A5 off
  }
  else                       
  {  
    digitalWrite(relaySet, LOW);    //turn A5 off
  }
  
  delay(10); // wait for stability
  
}
