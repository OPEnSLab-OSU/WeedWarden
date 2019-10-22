// ================================================================
// ===              INCLUDE CONFIGURATION FILE                  ===
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     ===
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================

// Config has to be first has it hold all user specified options
#include "./config.h"


// Preamble includes any relevant subroutine files based 
// on options specified in the above config
#include "loom_preamble.h"

int relaySet = A5;
float clock_time_mins;
float clock_time_secs;
//int led = 13;
// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{
//  while ( ! Serial) {
//    delay(100);
//  }
//  Serial.begin(115200);
//  Serial.println("Start!");
	pinMode(led, OUTPUT);   
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();
 
  //set relay pins:
  pinMode(relaySet, OUTPUT);
  digitalWrite(relaySet, LOW);
  
	//setup code for camera and triad sensor
	setup_as7265X();
//  Serial.println("Triad Set Up!");
  setup_ttl();
  
	// Any custom setup code
	//pinMode(5, INPUT_PULLUP);
	//pinMode(6, INPUT_PULLUP);

}

// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle bndl;

	//Using Triad sensor
	//High Gain Values Required

	 measure_sensors();				// Read sensors, store data in sensor state struct
	 package_data(&bndl);			// Copy sensor data from state to provided bundle
//   snapshots();

   //comment out for debugging
	 print_bundle(&bndl);			// Display data

   if ((int)state_as7265X_0x49.a < (int)state_as7265X_0x49.c && (int)state_as7265X_0x49.b < (int)state_as7265X_0x49.c && (int)state_as7265X_0x49.d < (int)state_as7265X_0x49.c  
   && (int)state_as7265X_0x49.e < (int)state_as7265X_0x49.c && (int)state_as7265X_0x49.f < (int)state_as7265X_0x49.c && (int)state_as7265X_0x49.b < (int)state_as7265X_0x49.a
   && (int)state_as7265X_0x49.e < (int)state_as7265X_0x49.f){
//     snapshots();
     setup_ttl();
     clock_time_mins = millis()/60000;
     clock_time_secs = (millis()%60000)/1000;
     append_to_bundle_key_value(&bndl, "Minutes", clock_time_mins);
     append_to_bundle_key_value(&bndl, "Seconds", clock_time_secs);
     log_bundle(&bndl, SDCARD, "green.csv");
     unlatchedRelay(1);
   }
   
   else{
     unlatchedRelay(0);
   }

	 delay(100);

	 additional_loop_checks();	// Miscellaneous checks

	log_bundle(&bndl, SDCARD, "trials1.csv");	// Send bundle to Google Sheet
//	}
}

// ================================================================ 
// ===                        LATCHED RELAY                     ===
// ================================================================  
void unlatchedRelay(bool io)
{
  if(io)
  {
    digitalWrite(relaySet, HIGH);   // turn A5 on
    delay(3000);
    digitalWrite(relaySet, LOW);    //turn A5 off
  }
  else                       
  {  
    digitalWrite(relaySet, LOW);    //turn A5 off
  }
  
  delay(20); // wait for stability
  
}
