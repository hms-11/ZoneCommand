//  ------------------------- ZONE COMMAND ---------------------
//  ---------------------6 ZONE CONTROL BOARD-------------------

//  -------------------DESIGNED AND PROGRAMMED BY:--------------
// --------------------------COREY EARL-------------------------
// -------REVISION 3 (Simplified Code) (OCTOBER 12th, 2020)-----
// --------------All Zones Appear to Function And Debug---------
// ---------------------More Testing Needed--------------------
//----------------Ready for on-site BETA testing---------------


// print debug messages or not to serial
const bool serialDisplay = true;


///  ---------------SETTINGS-----------------

// Which zones are active. If a zone is active set as true, if a zone is deactived or not needed set to false. Zones are then placed in an array.

const bool zone1Active = true;
const bool zone2Active = true;
const bool zone3Active = true;
const bool zone4Active = true;
const bool zone5Active = true;
const bool zone6Active = true;
const bool zoneActive[6] {zone1Active, zone2Active, zone3Active, zone4Active, zone5Active, zone6Active};

// Counters for progressing through each Zone

int zoneNumber = 0; // Which zone the loop is controlling
int serialNumber = 0; // Which zone the Serial Debugger is showing

// If the relay goes active when driven low set this to true, if the relays go active when drive high set to false

const bool relayLow = true;

// If you want to trigger the outputs based on a falling value set this to true, if you want to trigger based on a raising input, set to false

const bool fallingTrigger = true;


// Sensor trigger points to trigger each zones output on. High or low "on" decided by relayLow setting

// NOTE: GIKFUN 1.2 Moisture sensors seem to range from ~340(Capacitor submerged in water) to ~670(Dry air)
//       GIKFUN 2.0 Seem to run this range (~290 wet, ~700 Dry) but responds much slower as moisture increases, responds quickly to drying).
//       These tests done with factory ~8" jumper harness. Will retest with final length harnesses and adjust



// Zone 1 setpoints
int trigger1Low = 330;
int trigger1High = 400;

// Zone 2 setpoints
int trigger2Low = 330;
int trigger2High = 400;

// Zone 3 setpoints
int trigger3Low = 330;
int trigger3High = 400;

// Zone 4 setpoints
int trigger4Low = 330;
int trigger4High = 400;

// Zone 5 setpoints
int trigger5Low = 330;
int trigger5High = 400;

// Zone 6 setpoints
int trigger6Low = 330;
int trigger6High = 400;

// Zone setpoint arrays

int triggerHigh[] {trigger1High, trigger2High, trigger3High, trigger4High, trigger5High, trigger6High}; // Puts triggerLow points in an array
int triggerLow[] {trigger1Low, trigger2Low, trigger3Low, trigger4Low, trigger5Low, trigger6Low}; // Puts triggerHigh points in an array


//// -------------------CODE----------------------

// ---Pin Assignments--

// Set Pin Assignments for Zone Status LED's

const int zone1 = 8;
const int zone2 = 9;
const int zone3 = 10;
const int zone4 = 11;
const int zone5 = 12;
const int zone6 = 13;
const int zoneLED[] {zone1, zone2, zone3, zone4, zone5, zone6}; // Puts output LEDs in an array


// Set pin assignments for OUT pins


const int out1 = 2;
const int out2 = 3;
const int out3 = 4;
const int out4 = 5;
const int out5 = 6;
const int out6 = 7;
const int output[] {out1, out2, out3, out4, out5, out6}; // Puts output pins in an array

// Set pin asignments for sensor inputs and read delays (done with millis, not delay)

const byte sens1 = A0;
const byte sens2 = A1;
const byte sens3 = A2;
const byte sens4 = A3;
const byte sens5 = A4;
const byte sens6 = A5;
const byte sens[] {sens1, sens2, sens3, sens4, sens5, sens6}; // Put analog input pins in an array

// --Variables--

// Delay Variables

const int sensorReadDelay[] {500, 500, 500, 500, 500, 500}; // delay between sensor readings in millis
unsigned long lastSensorReadTime[] {0, 0, 0, 0, 0, 0}; //the last time a sensor was read
unsigned long serialDelay = 3000; // delay between printing to serial monitor in millis
unsigned long lastSerialPrintTime = 0; // the last time the serial monitor was printed too


// Analog sensor reading values sensors 1-6
int sensor[6]; //Sensor reading values array


// Sensor pins 1-6 states ("1" = Low, "2" = Neutral, "3" = High)
int zoneState1;
int zoneState2;
int zoneState3;
int zoneState4;
int zoneState5;
int zoneState6;
int zoneState[] {zoneState1, zoneState2, zoneState3, zoneState4, zoneState5, zoneState6}; //Zone pin state array

// Output pin values outputs 1-6

int outRead1;
int outRead2;
int outRead3;
int outRead4;
int outRead5;
int outRead6;
int outRead[] {outRead1, outRead2, outRead3, outRead4, outRead5, outRead6}; // Output pin array

// Output LED pin values outputs 1-6

int outReadLED1;
int outReadLED2;
int outReadLED3;
int outReadLED4;
int outReadLED5;
int outReadLED6;
int ledState[] {outReadLED1, outReadLED2, outReadLED3, outReadLED4, outReadLED5, outReadLED6}; // Zone Active LED high/low array

// Serial States where neede
void setup() {

  // start the serial monitor and display welcome message
  Serial.begin(9600);
  delay(1000);
  Serial.println( "           Processes Started   ");
  Serial.println("           --ZONE COMMAND VERSION 1.5 (FOR LOOP CONTROL)--    ");
  Serial.println("            --6 ZONE CONTROL BOARD--     ");
  Serial.println("                       --                ");

  // set LED indicator pins to output.
  pinMode(zone1, OUTPUT);
  pinMode(zone2, OUTPUT);
  pinMode(zone3, OUTPUT);
  pinMode(zone4, OUTPUT);
  pinMode(zone5, OUTPUT);
  pinMode(zone6, OUTPUT);
  // set OUT pins to output
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  pinMode(out5, OUTPUT);
  pinMode(out6, OUTPUT);
  // set sensor pins to input
  pinMode(sens1, INPUT);
  pinMode(sens2, INPUT);
  pinMode(sens3, INPUT);
  pinMode(sens4, INPUT);
  pinMode(sens5, INPUT);
  pinMode(sens6, INPUT);
}

void zoneControl() {
  if (zoneActive[zoneNumber]) {
    sensor[zoneNumber] = analogRead(sens[zoneNumber]);
    if ((unsigned long)(millis() - lastSensorReadTime[zoneNumber]) >= sensorReadDelay[zoneNumber]) {
      lastSensorReadTime[zoneNumber] = millis();

      // Set the sensor reading levels for the output trigger for sensor

      if (!fallingTrigger) { // if you want your outputs triggering on a rising value
        if (sensor[zoneNumber] <= triggerLow[zoneNumber]) {
          zoneState[zoneNumber] = 1; // Reading is below trigger, turn on relay
        }
        else if (sensor[zoneNumber] >= triggerLow[zoneNumber] && sensor[zoneNumber] <= triggerHigh[zoneNumber]) {
          zoneState[zoneNumber] = 2; // Reading is in between triggers, maintain relay state
        }
        else if (sensor[zoneNumber] >= triggerHigh[zoneNumber]) {
          zoneState[zoneNumber] = 3; // Reading is above trigger, turn off relay
        }
      }
      else if (fallingTrigger) { // if you want your outputs triggering on a falling value
        if (sensor[zoneNumber] >= triggerLow[zoneNumber]) {
          zoneState[zoneNumber] = 1; // Reading is above trigger, turn on relay
        }
        else if (sensor[zoneNumber] >= triggerLow[zoneNumber] && sensor[zoneNumber] <= triggerHigh[zoneNumber]) {
          zoneState[zoneNumber] = 2; // Reading is in between triggers, maintain relay state
        }
        else if (sensor[zoneNumber] <= triggerHigh[zoneNumber]) {
          zoneState[zoneNumber] = 3; // Reading is below trigger, turn off relay
        }
      }
    }
    
    // Trigger relays based on High/Low triggering
    if (relayLow) { //if the relays are activated by being driven low
      if (zoneState[zoneNumber] == 1) { // if the output needs to be turned on
        digitalWrite(output[zoneNumber], LOW);
        digitalWrite(zoneLED[zoneNumber], HIGH); //Turn Zone Indicator LED On
      }
      else if (zoneState[zoneNumber] == 3) { //if the output needs to be turned off
        digitalWrite(output[zoneNumber], HIGH);
        digitalWrite(zoneLED[zoneNumber], LOW); //Turn Zone Indicator LED Off
      }
    }
    else if (!relayLow) { //if the relays are actived by being driven high
      if (zoneState[zoneNumber] == 1) { // if the output needs to be turned on
        digitalWrite(output[zoneNumber], HIGH);
        digitalWrite(zoneLED[zoneNumber], HIGH); //Turn Zone Indicator LED On
      }
      else if (zoneState[zoneNumber] == 3) { //if the output needs to be turned off
        digitalWrite(output[zoneNumber], LOW);
        digitalWrite(zoneLED[zoneNumber], LOW); //Turn Zone Indicator LED Off
      }
    }
  }
}


// Display Serial debugging messages to serial monitor if serialDisplay set to true
void serialDebug() {
  // ZONE DETAILS
  if (zoneActive[serialNumber]) { // if the zone is active
    outRead[serialNumber] = digitalRead(output[serialNumber]); //check state of output
    ledState[serialNumber] = digitalRead(zoneLED[serialNumber]); //check state of zone indicator LED
    //   sensor[serialNumber] = analogRead(sens[serialNumber]); // check value of zone sensor
    Serial.println(" ZONE STATUS ");
    Serial.println("    --    ");
    Serial.println("ZONE: ");
    Serial.println(serialNumber);
    Serial.println("Output Status: (1 is 5V or HIGH, 0 is LOW or 0V)");
    Serial.print(outRead[serialNumber]);
    Serial.println("");
    Serial.println("LED Status:   ");
    Serial.print(ledState[serialNumber]);
    Serial.println("");
    Serial.println(" Sensor Value:   ");
    Serial.print(sensor[serialNumber]);
    Serial.println("");
    Serial.println("Zone State ('1' = LOW, '2' = NEUTRAL, '3' = HIGH):   ");
    Serial.print(zoneState[serialNumber]);
    Serial.println("");
    Serial.println("");
    Serial.println("");
  }

}

// Loops to progress zoneControl and serialDebug incrementally
void zoneLoop() {
  for (zoneNumber = 0; zoneNumber < 5; zoneNumber++) {
    zoneControl();
  }
  for (zoneNumber = 5; zoneNumber > 0; zoneNumber--) {
    zoneControl();
  }
}

void serialLoop() {
  if (serialDisplay) { // if debug mode is turned on
    if ((unsigned long)(millis() - lastSerialPrintTime) >= serialDelay) {
      lastSerialPrintTime = millis();
      if (relayLow) { // if the Relays are activated by a low signal
        Serial.println(" Relays are set to be activated by a LOW (0V) signal ");
      }
      else if (!relayLow) { // if the Relays are activated by a high signal
        Serial.println(" Relays are set to be activated by a HIGH (5V) signal ");
      }
      if (fallingTrigger) { // If the outputs are triggered by a falling value
        Serial.println(" Outputs are triggered on a falling value");
      }
      else if (!fallingTrigger) { // if the outputs are triggered by a rising value
        Serial.println(" Outputs are triggered on a rising value");
      }
      for (serialNumber = 0; serialNumber < 5; serialNumber++) {
        serialDebug();
      }
      for (serialNumber = 5; serialNumber > 0; serialNumber--) {
        serialDebug();
      }
    }
  }
}

void loop() {
  zoneLoop();
  serialLoop();
}
