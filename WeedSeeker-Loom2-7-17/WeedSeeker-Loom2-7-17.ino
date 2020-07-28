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
// testing comment
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
  pinMode(ledpin, OUTPUT);
  
  digitalWrite(relaySet, LOW);
//  digitalWrite(ledpin, LOW); 
  
}

void loop() 
{


//// ****  Sense and Store  **** ////
  
	Loom.measure();
  auto package = Loom.package();
	Loom.display_data();
  Loom.SDCARD().log();

  String time = package["timestamp"]["time"].as<const char*>();
  String seconds = time.substring(6);
  int sec = seconds.toInt();
 
  while(sec%2 != 0){
    package = Loom.package(); 
    time = package["timestamp"]["time"].as<const char*>();
    seconds = time.substring(6);
    sec = seconds.toInt();
  }


//// ****  NDVI Calculation  **** ////

  JsonObject as7265x = (package["contents"].as<JsonArray>())[1]["data"];
  const uint16_t a = as7265x["410nm"];
  const uint16_t b = as7265x["435nm"];
  const uint16_t c = as7265x["460nm"];
  const uint16_t d = as7265x["485nm"];
  const uint16_t e = as7265x["510nm"];
  const uint16_t f = as7265x["535nm"];
  const uint16_t g = as7265x["560nm"];
  const uint16_t h = as7265x["585nm"];
  const uint16_t i = as7265x["610nm"];
  const uint16_t j = as7265x["645nm"];
  const uint16_t k = as7265x["680nm"];
  const uint16_t l = as7265x["705nm"];
  const uint16_t m = as7265x["730nm"];
  const uint16_t n = as7265x["760nm"];
  const uint16_t o = as7265x["810nm"];
  const uint16_t p = as7265x["860nm"];
  const uint16_t q = as7265x["900nm"];
  const uint16_t r = as7265x["940nm"];
//  Serial.println(b);
//  Serial.println(m);
//  Serial.println(n);
  float total = a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r;
  float b_w = b/total;
  float c_w = c/total;
  float j_w = j/total;
  float k_w = k/total;
  float m_w = m/total;
  float n_w = n/total;
.
  float ndvib = (((m_w*730.0 + n_w*760.0 + b_w*435.0 + c_w*460.0) - (j_w*645.0 + k_w*680.0))/((m_w*730.0 + n_w*760.0 + b_w*435.0 + c_w*460.0) + (j_w*645.0 + k_w*680.0)));
//  Serial.println("++++++++++++++++");
//  Serial.println(total);
//  Serial.println(b_w);
//  Serial.println(ndvib);
//  Serial.println(sec);Serial.println(" Seconds+++"); 


//// ****  10-Sec Intervals  **** ////
 
//  if (sec%10 == 0){       
////    snapten();
//    Loom.SDCARD().log("10sec.csv");
//    //turn off pin13
//    if (sec%20 == 0){
//      //trun on pin13
//    }
//  }


//// ****  ALGORITHM  **** ////

 if ((int)a < (int)c && (int)b < (int)c && (int)d < (int)c && (int)e < (int)c && (int)f < (int)c && (int)e < (int)f){
//    snapgreen();
    if (total <= 200){
      if (ndvib > 0.35){
        digitalWrite(ledpin, HIGH);
        unlatchedRelay(1); 
        Loom.SDCARD().log("Green.csv");      
      }
    }
    if (total > 200){
      // ** NDVI Check ** //
      if (ndvib > 0.5){
        digitalWrite(ledpin, HIGH);
        unlatchedRelay(1);
        Loom.SDCARD().log("Green.csv");        
      }
    }
    delay(500);
    unlatchedRelay(0);
    digitalWrite(ledpin, LOW);
  }
  
  else{
    unlatchedRelay(0);
  }
  
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
