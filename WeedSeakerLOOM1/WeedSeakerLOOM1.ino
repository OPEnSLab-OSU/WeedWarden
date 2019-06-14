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

//int led = 13;

// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{

	pinMode(led, OUTPUT);   
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();	
	setup_as7265X();
	// Any custom setup code
	pinMode(5, INPUT_PULLUP);
	pinMode(6, INPUT_PULLUP);
	// pinMode(7, INPUT_PULLUP);
	// pinMode(8, INPUT_PULLUP);
	// pinMode(9, INPUT_PULLUP);
	// pinMode(10, INPUT_PULLUP);
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
	
	 print_bundle(&bndl);			// Display data



	 delay(1000);

	 additional_loop_checks();	// Miscellaneous checks




	

	// // --- Common Example ---

	// receive_bundle(&bndl, WIFI);	// Receive messages
	// if (bndl.size()) {
	// 	print_bundle(&bndl);		// Print bundle if LOOM_DEBUG enabled
	// }
	// process_bundle(&bndl);			// Dispatch message to correct handling functions

	// measure_sensors();				// Read sensors, store data in sensor state struct
	// package_data(&bndl);			// Copy sensor data from state to provided bundle

	// // print_bundle(&bndl);			// Print bundle if LOOM_DEBUG enabled

	// send_bundle(&bndl, WIFI);		// Send bundle of packaged data
	// // log_bundle(&bndl, PUSHINGBOX);	// Send bundle to Google Sheet
	// // log_bundle(&bndl, SDCARD, "Ishield.csv");	// Send bundle to Google Sheet

	// additional_loop_checks();			// Miscellaneous checks
	// // --- End Example ---




}