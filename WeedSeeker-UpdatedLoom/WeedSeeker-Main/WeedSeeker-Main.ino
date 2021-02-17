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
//#include "Snapshot.h"
//#include "Loom_AS7265X.cpp"

// Include configuration
const char* json_config = 
#include "config.h"
;

#define VBATPIN A7  // pin connected to the 3.7V battery 
#define LVEn 5      // pin 5 is the 3.3V Enable
//#define HVEn 6      // pin 6 is the 5V and V+ Enable 
#define Buzzer 6   // Set the buzzer pin as A2 

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

// Make global variables for calibration variables
 float total_ndvi = 0;
 float  ndvi[16];
 float STD; 
 int sensitivity = 1;


// Setup Function
void setup() 
{ 
  
  pinMode(LVEn, OUTPUT);    // LVEn is the pin that enables the hypnos board 3.3V 
//  pinMode(HVEn, OUTPUT);    // Enable the 5V output
  pinMode(Buzzer, OUTPUT); 

//  digitalWrite(HVEn, HIGH);  // Turn on the growlight
  digitalWrite(LVEn, LOW);  // Turn on the 3.3V Rail  for I2C control and SD card logging
  digitalWrite(Buzzer, LOW); // Start with the buzzer off 

  // Configure Loom and Serial 
	Loom.begin_serial(true);
  Serial.begin(9600); 
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
   
} // Setup 



//================================================================
//===                     calibrate                           ====
//===                                                         ====
//=== Function will take samples of dirt to find a baseline   ====
//=== NDVI Value that can be used as a threshold              ==== 
//================================================================
void calibrate()
{
    //DO THESE FIRST
  //Get NDVIB lights on average for dirt to compare with grass
  //plug into double if statement for NDVIB below
  for(int z=0; z<16; z++) // Loop to take calibration samples
  {
    Loom.measure(); 
      // Store data in JSON package
    JsonObject calibrationData = Loom.package(); 

    serializeJsonPretty(calibrationData, Serial);

      //// ****  NDVI Calculation  **** ////
    JsonObject as7265x = (calibrationData["contents"].as<JsonArray>())[1]["data"];
    uint16_t a = as7265x["a"]; a = (int)a; Serial.println(a);
    uint16_t b = as7265x["b"]; b = (int)b; Serial.println(b);
    uint16_t c = as7265x["c"]; c = (int)c; Serial.println(c);
    uint16_t d = as7265x["d"]; d = (int)d; Serial.println(d);
    uint16_t e = as7265x["e"]; e = (int)e; Serial.println(e);
    uint16_t f = as7265x["f"]; f = (int)f; Serial.println(f);
    uint16_t g = as7265x["g"]; g = (int)g; Serial.println(g);
    uint16_t h = as7265x["h"]; h = (int)h; Serial.println(h);
    uint16_t i = as7265x["i"]; i = (int)i; Serial.println(i);
    uint16_t j = as7265x["j"]; j = (int)j; Serial.println(j);
    uint16_t k = as7265x["k"]; k = (int)k; Serial.println(k);
    uint16_t l = as7265x["l"]; l = (int)l; Serial.println(l); 
    uint16_t r = as7265x["r"]; r = (int)r; Serial.println(r);
    uint16_t s = as7265x["s"]; s = (int)s; Serial.println(s);
    uint16_t t = as7265x["t"]; t = (int)t; Serial.println(t);
    uint16_t u = as7265x["u"]; u = (int)u; Serial.println(u);
    uint16_t v = as7265x["v"]; v = (int)v; Serial.println(v);
    uint16_t w = as7265x["w"]; w = (int)w; Serial.println(w);


   // calculate ndvi value 
   float nd = ((u + v + w + e + f + g) - 2*(b + c + d));  // calculate top half of NDVI 
   float vib = ((u + v + w + e + f + g) + 2*(b + c + d)); // Calculate bottom half of NDVI
   ndvi[z] = nd/vib;  // Store NDVI values in an array

   LPrintln(ndvi[z]); 
   
   // delay(500);   // Put this delay back in if things get messed up 
  } // for loop 

    // loop to average ndvi values 
  for(int i=0; i<16; i++)
    total_ndvi += ndvi[i];
  
  total_ndvi=(total_ndvi/16)+.015;

  // loop to calculate Standard deviation 
  float STD_sigma = 0;
  for(int i=0; i<16; i++)
  {
    STD_sigma += ((ndvi[i]-total_ndvi)*2);
    if(i==15)
    {
      STD =(STD_sigma/16)*(1/2);
    } // if
  } // for 

  // Print out the standard deviation 
  LPrint("STD = "); 
  LPrintln(STD); 

  // Maybe add something in here that if the STD is too high we need to recalibrate 

  // Turn on the buzzer for 6 seconds so that you know the calibration worked
  digitalWrite(Buzzer, HIGH); 
  delay(6000);               
  digitalWrite(Buzzer, LOW); 
} // Calibrate 



//================================================================
//===                     Measure battery                     ====
//===                                                         ====
//=== Function displays the battery voltage of a 3.7V battery ====
//=== that is connected to the JST connector of the Feather   ==== 
//================================================================
int MeasureBat()
{
  // Read battery pin
  float measuredvbat = analogRead(VBATPIN);  

  // Perform some calculations as specified by the Feather M0 guide 
  measuredvbat *= 2; 
  measuredvbat *= 3.3; 
  measuredvbat /= 1024; 

  // Return Battery voltage
  return measuredvbat; 
}



// Main loop! 
void loop()
{

  // Run the calibration function
  calibrate(); 

  while(1){
    int LightState = 1;   // make the light state on 

    // Measure the data from sensor 
    Loom.measure();                         

    // Store data in JSON package
    JsonObject SensorData = Loom.package(); 

    // Add the state of the Growlight to the file 
    Loom.add_data("LightState", "LightState", LightState);

    // Serialize the JSON Object and print it to the Serial Monitor
    serializeJsonPretty(SensorData, Serial); 

    //// ****  NDVI Calculation  **** ////
    Serial.println("+++++ START +++++");
    JsonObject as7265x = (SensorData["contents"].as<JsonArray>())[1]["data"];
    uint16_t a = as7265x["a"]; a = (float)a; //Serial.println(a); // 410
    uint16_t b = as7265x["b"]; b = (float)b; //Serial.println(b); // 435
    uint16_t c = as7265x["c"]; c = (float)c; //Serial.println(c); // 460
    uint16_t d = as7265x["d"]; d = (float)d; //Serial.println(d); // 485
    uint16_t e = as7265x["e"]; e = (float)e; //Serial.println(e); // 510
    uint16_t f = as7265x["f"]; f = (float)f; //Serial.println(f); // 535
    uint16_t g = as7265x["g"]; g = (float)g; //Serial.println(g); // 560
    uint16_t h = as7265x["h"]; h = (float)h; //Serial.println(h); // 585
    uint16_t i = as7265x["i"]; i = (float)i; //Serial.println(i); // 645
    uint16_t j = as7265x["j"]; j = (float)j; //Serial.println(j); // 705
    uint16_t k = as7265x["k"]; k = (float)k; //Serial.println(k); // 900
    uint16_t l = as7265x["l"]; l = (float)l; //Serial.println(l); // 940
    uint16_t r = as7265x["r"]; r = (float)r; //Serial.println(r); // 610
    uint16_t s = as7265x["s"]; s = (float)s; //Serial.println(s); // 680
    uint16_t t = as7265x["t"]; t = (float)t; //Serial.println(t); // 730
    uint16_t u = as7265x["u"]; u = (float)u; //Serial.println(u); // 760
    uint16_t v = as7265x["v"]; v = (float)v; //Serial.println(v); // 810
    uint16_t w = as7265x["w"]; w = (float)w; //Serial.println(w); // 860

    float total = a + b + c + d + e + f + g + h + i + j + k + l + r + s + t + u + v + w;
    
    Serial.println("++++++++++++++++");
    Serial.println("++++++++++++++++");    
    float a_w = a/total; //Serial.println(a_w);
    float b_w = b/total; //Serial.println(b_w);
    float c_w = c/total; //Serial.println(c_w);
    float d_w = d/total; //Serial.println(d_w);
    float e_w = b/total; //Serial.println(e_w);
    float f_w = b/total; //Serial.println(f_w);
    float g_w = g/total; //Serial.println(g_w);
    float j_w = j/total; //Serial.println(j_w); 
    float k_w = k/total; //Serial.println(k_w);
    float r_w = r/total; //Serial.println(r_w);
    float s_w = s/total; //Serial.println(s_w);
    float t_w = t/total; //Serial.println(t_w);
    float u_w = u/total; //Serial.println(u_w);
    float v_w = v/total; //Serial.println(v_w);
    float w_w = w/total; //Serial.println(w_w);

    float nd = ((u + v + w + e + f + g) - 2*(b + c + d));
    float vib = ((u + v + w + e + f + g) + 2*(b + c + d));
    float ndvib = nd/vib; 

    float ps = v-f;
    float nd2 = v+f;
    float psnd = ps/nd2;
    bool isgreen = 0; 
  
    float evi = ((2.5*(v-s))/(v+6*s-7.5*b+1));

    // Add Values to the JSON Package
    Loom.add_data("NDVIB", "NDVIB", ndvib);
    Loom.add_data("EVI", "EVI", evi);
    Loom.add_data("Threshold", "Threshold", total_ndvi);

    float ndvib_w = ((u_w + v_w + w_w + e_w + f_w + g_w) - 2*(b_w + c_w + d_w))/((u_w + v_w + w_w + e_w + f_w + g_w) + 2*(b_w + c_w + d_w));
    float psnd_w = ((v_w-f_w)/(v_w+f_w));
    float evi_w = ((2.5*(v_w-s_w))/(v_w+6*s_w-7.5*b_w+1));

    // Add Values to the JSON Package
    Loom.add_data("NDVIB_W", "NDVIB_W", ndvib_w);
    Loom.add_data("EVI_W", "EVI_W", evi_w);
    Loom.add_data("PSND_W", "PSND_W", psnd_w);

    // Log the data to SD Card 
    Loom.SDCARD().log(); 

    Serial.println("++++++++++++++++");
    LPrint("Total = "); LPrintln(total);
    
    // LPrint("b_w = ");   LPrintln(b_w);
    LPrint("ndvi = "); LPrintln(ndvib);
    LPrint("total_ndvi = "); LPrintln(total_ndvi);  
    // LPrint("evi = "); LPrintln(evi);
    // LPrint("evi_w = "); LPrintln(evi_w);
    // LPrint("psnd = "); LPrintln(psnd);
    // LPrint("psnd_w = "); LPrintln(psnd_w);
    // LPrint("Growlight State = "); LPrintln(LightState); 

    int success_tier = 0;

    //add tiered sureness of green... more loops, each loop adding +1 to tier
    if(total_ndvi < ndvib && sensitivity > 0)
    {
      success_tier++;

        Serial.println("++++++++++++++++++++++++++++++++++++++");
        Serial.println("++++++++++ This Is A Target ++++++++++");
        Serial.println("++++++++++++++++++++++++++++++++++++++");

        digitalWrite(Buzzer, HIGH);   // Turn on the Buzzer if grass is detected

    } // if
    else {
      Serial.println("----------------------------------");
      Serial.println("++++++++++ Not A Target ++++++++++");
      Serial.println("----------------------------------");

      digitalWrite(Buzzer, LOW);  // Turn the buzzer off if dirt is detected
    } // else

    Serial.println("+++++ End +++++"); 
    Loom.pause();
  
  } // while 

} // main loop 
