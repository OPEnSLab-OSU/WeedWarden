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

//int relaySet = A5;
int ledpin = A1;

void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();
//  setup_ttl();
	 LPrintln("\n ** Setup Complete ** ");
 
//  pinMode(relaySet, OUTPUT);
  pinMode(ledpin, OUTPUT);
  
//  digitalWrite(relaySet, LOW);
  digitalWrite(ledpin, HIGH);
  delay(2000);
  digitalWrite(ledpin, LOW); 
  
}

void loop() 
{

//// ****  Sense and Store  **** ////
//   LPrintln("1111111111111111");
	Loom.measure();
  auto package = Loom.package();
	Loom.display_data();
//   LPrintln("222222222222");
  Loom.SDCARD().log("ALL.csv");
//   LPrintln("333333333333");

  String time = package["timestamp"]["time"].as<const char*>();
  String seconds = time.substring(6);
  int sec = seconds.toInt();
//   LPrintln("444444444444444444");
//  while(sec%2 != 0){
//    package = Loom.package(); 
//    time = package["timestamp"]["time"].as<const char*>();
//    seconds = time.substring(6);
//    sec = seconds.toInt();
//  }
//   LPrintln("5555555555555555555");

//// ****  NDVI Calculation  **** ////
  LPrintln("+++++ START +++++");
  JsonObject as7265x = (package["contents"].as<JsonArray>())[1]["data"];
  uint16_t a = as7265x["410nm"]; a = (int)a;  LPrintln(a);
  uint16_t b = as7265x["435nm"]; b = (int)b;  LPrintln(b);
  uint16_t c = as7265x["460nm"]; c = (int)c;  LPrintln(c);
  uint16_t d = as7265x["485nm"]; d = (int)d;  LPrintln(d);
  uint16_t e = as7265x["510nm"]; e = (int)e;  LPrintln(e);
  uint16_t f = as7265x["535nm"]; f = (int)f;  LPrintln(f);
  uint16_t g = as7265x["560nm"]; g = (int)g;  LPrintln(g);
  uint16_t h = as7265x["585nm"]; h = (int)h;  LPrintln(h);
  uint16_t i = as7265x["610nm"]; i = (int)i;  LPrintln(i);
  uint16_t j = as7265x["645nm"]; j = (int)j;  LPrintln(j);
  uint16_t k = as7265x["680nm"]; k = (int)k;  LPrintln(k);
  uint16_t l = as7265x["705nm"]; l = (int)l;  LPrintln(l); 
  uint16_t m = as7265x["730nm"]; m = (int)m;  LPrintln(m);
  uint16_t n = as7265x["760nm"]; n = (int)n;  LPrintln(n);
  uint16_t o = as7265x["810nm"]; o = (int)o;  LPrintln(o);
  uint16_t p = as7265x["860nm"]; p = (int)p;  LPrintln(p);
  uint16_t q = as7265x["900nm"]; q = (int)q;  LPrintln(q);
  uint16_t r = as7265x["940nm"]; r = (int)r;  LPrintln(r);

  float total = a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r;
  
   LPrintln("++++++++++++++++");  
  float a_w = a/total;  LPrintln(a_w);
  float b_w = b/total;  LPrintln(b_w);
  float c_w = c/total;  LPrintln(c_w);
  float d_w = d/total;  LPrintln(d_w);
  float e_w = b/total;  LPrintln(e_w);
  float f_w = b/total;  LPrintln(f_w);
  float j_w = j/total;  LPrintln(j_w); 
  float k_w = k/total;  LPrintln(k_w);
  float m_w = m/total;  LPrintln(m_w);
  float n_w = n/total;  LPrintln(n_w);
  float o_w = o/total;  LPrintln(o_w);

  float ndvib = (((m_w + n_w + j_w + k_w) - 2*(b_w + c_w))/((m_w + n_w + j_w + k_w) + 2*(b_w* + c_w)));
 
  LPrintln("++++++++++++++++");
  LPrintln(total);
//  LPrintln(b_w);
  LPrintln(ndvib);
//  LPrintln(sec);// LPrintln(" Seconds+++"); 


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

  if (ndvib > 0.25) {
     if ((a < c && b < c && d < c && e < c && f < c && k < o && i < o)){ // testing ratios
          LPrintln("++++++++++++++++++++++++++++++++++++++");
          LPrintln("++++++++++ This Is A Target ++++++++++");
          LPrintln("++++++++++++++++++++++++++++++++++++++");
          digitalWrite(ledpin, HIGH);
    //    snapgreen();
    
//        if (total <= 200){
//          if (ndvib > 0.35){
//            digitalWrite(ledpin, HIGH);
//            unlatchedRelay(1); 
//            Loom.SDCARD().log("Green.csv");      
//          }
//        }
//        if (total > 200){
//          // ** NDVI Check ** //
//          if (ndvib > 0.5){
//            digitalWrite(ledpin, HIGH);
//            unlatchedRelay(1);
//            Loom.SDCARD().log("Green.csv");        
//          }
//        }
//      unlatchedRelay(1);
      Loom.SDCARD().log("Green.csv");
      delay(500);
//      unlatchedRelay(0);
      digitalWrite(ledpin, LOW);
    }
    else {
  //    unlatchedRelay(0);
      digitalWrite(ledpin, LOW);
      LPrintln("----------------------------------");
      LPrintln("++++++++++ Not A Target ++++++++++");
      LPrintln("----------------------------------");
    }
  }
  
  else {
//    unlatchedRelay(0);
    digitalWrite(ledpin, LOW);
    LPrintln("----------------------------------");
    LPrintln("++++++++++ Not A Target ++++++++++");
    LPrintln("----------------------------------");
  }
  LPrintln("+++++ End +++++"); 
  Loom.pause();
}

// ================================================================ 
// ===                        LATCHED RELAY                     ===
// ================================================================  
//void unlatchedRelay(bool io)
//{
//  if(io)
//  {
//    digitalWrite(relaySet, HIGH);   // turn A5 on
//    delay(2500);
//    digitalWrite(relaySet, LOW);    //turn A5 off
//  }
//  else                       
//  {  
//    digitalWrite(relaySet, LOW);    //turn A5 off
//  }
//  
//  delay(10); // wait for stability
//  
//}
