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
#define HVEn 6      // pin 6 is the 5V and V+ Enable 


// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

// int ledpin = A1; commented out because we should tie the LED to the same output as the sprayer

void setup() 
{ 
  pinMode(LVEn, OUTPUT);    // LVEn is the pin that enables the hypnos board 3.3V 
  pinMode(HVEn, OUTPUT);    // Enable the 5V output

  digitalWrite(HVEn, LOW);  // Initally set the sprayer to be off 
  digitalWrite(LVEn, LOW);  // Turn on the 3.3V Rail  for I2C control and SD card logging

	// Loom.begin_LED();      dont need this linbe if we tie LED to sprayer power 

	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();
 
	LPrintln("\n ** Setup Complete ** ");
 
  // pinMode(ledpin, OUTPUT);
  
}

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

void loop()
{
  // Measure the data from sensor 
  Loom.measure();                         

  // Store data in JSON package
  JsonObject SensorData = Loom.package(); 

  // Serialize the JSON Object and print it to the Serial Monitor
  serializeJsonPretty(SensorData, Serial); 

  // Log the data to SD Card 
  Loom.SDCARD().log(); 

  //// ****  NDVI Calculation  **** ////
  Serial.println("+++++ START +++++");
  JsonObject as7265x = (SensorData["contents"].as<JsonArray>())[1]["data"];
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
  uint16_t m = as7265x["r"]; m = (int)m; Serial.println(m);
  uint16_t n = as7265x["s"]; n = (int)n; Serial.println(n);
  uint16_t o = as7265x["t"]; o = (int)o; Serial.println(o);
  uint16_t p = as7265x["u"]; p = (int)p; Serial.println(p);
  uint16_t q = as7265x["v"]; q = (int)q; Serial.println(q);
  uint16_t r = as7265x["w"]; r = (int)r; Serial.println(r);

  float total = a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r;
  
  Serial.println("++++++++++++++++");  
  float a_w = a/total; Serial.println(a_w);
  float b_w = b/total; Serial.println(b_w);
  float c_w = c/total; Serial.println(c_w);
  float d_w = d/total; Serial.println(d_w);
  float e_w = b/total; Serial.println(e_w);
  float f_w = b/total; Serial.println(f_w);
  float j_w = j/total; Serial.println(j_w); 
  float k_w = k/total; Serial.println(k_w);
  float m_w = m/total; Serial.println(m_w);
  float n_w = n/total; Serial.println(n_w);
  float o_w = o/total; Serial.println(o_w);

  float ndvib = (((m_w + n_w + j_w + k_w) - 2*(b_w + c_w))/((m_w + n_w + j_w + k_w) + 2*(b_w* + c_w)));
 
  Serial.println("++++++++++++++++");
  LPrint("Total = "); LPrintln(total);
  LPrint("b_w = ");   LPrintln(b_w);
  LPrint("ndvib = "); LPrintln(ndvib);

  if(ndvib > .25)
  {
    if ((a < c && b < c && d < c && e < c && f < c && k < o  && k < p))
    { // testing ratios
      Serial.println("++++++++++++++++++++++++++++++++++++++");
      Serial.println("++++++++++ This Is A Target ++++++++++");
      Serial.println("++++++++++++++++++++++++++++++++++++++");

      Loom.SDCARD().log("Green.csv");
      delay(500);
      digitalWrite(ledpin, HIGH);
      // Need to turn on the 12V rail here
      digitalWrite(HVEn, HIGH);   // This line will set the 5V and V+ Hypnos rail to high enabling the sprayer
      delay(1000);                // Set this line to delay for however long you want the sprayer to spray for
      digitalWrite(HVEn, LOW);    // Turn off the sprayer
    }

    else 
    {
      Serial.println("----------------------------------");
      Serial.println("++++++++++ Not A Target ++++++++++");
      Serial.println("----------------------------------");
    }
  }

  else {
//  unlatchedRelay(0);
    Serial.println("----------------------------------");
    Serial.println("++++++++++ Not A Target ++++++++++");
    Serial.println("----------------------------------");
  }

  Serial.println("+++++ End +++++"); 
  Loom.pause();
}