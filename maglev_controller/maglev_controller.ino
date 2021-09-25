/* Author: Santa
 * 
 * Controller use hall effect sensor to measure object distance (few centimeters). When
 * distance is above the threshold 
 * Electromagnet, controlled by a transistor, is turn ON. 
 * Viceversa when distance is lowen than threshold electromagnet is turned OFF.
 * Threshold value can be changed by an external program using a simple serial protocol.
 * 
 * TODO:
 *  - replace digitalRead()/digitalWrite() with fastDigital*()
 */

#define HALL_PIN 0  // analog input pin 0
int threshold, hallVal, hallValOld;

byte power; // electromagnet current power (0: OFF, 255: full power)

// PID settings
byte K;   // used to balance object weight 
byte Kp, Ki, Kd;  // usual PID parameters

void setup() {
  threshold = -1; // undef
  hallVal = hallValOld = -1; // undef

  Serial.begin(115200); //(9600);  //Initialization of Serial Port
  initMagnet(); // output magnet
  initPID();
  
  delay(1000);

  initStats();
  // Setup completed  
}

void loop() {
  handleEdit();
  
  if ( threshold < 0 )
    delay(100);  // wait for threshold init
  else {
    hallVal = analogRead(HALL_PIN);
    //Serial.println(hallVal);
    
    byte newPower = computePower(hallVal, hallValOld, threshold);
    setMagnetPower( newPower );
    hallValOld = hallVal;
  }

  loopCompleted();
}


/* stats utilities */

unsigned int loopCount, powerChangeCount;
unsigned long startT;
#define NOTIFY_TIME 500   // millis

void initStats() {
  loopCount = powerChangeCount = 0;
  startT = micros();
}

void updateMagnetStats() {
  ++powerChangeCount;
}

void loopCompleted() {
  ++loopCount;
  if (loopCount%100==0) {
    unsigned long delta = micros() - startT;
    if (delta > NOTIFY_TIME*1000L) {
      /*//Serial.print(loopCount); Serial.print(" - "); Serial.println(delta);
      Serial.print("loop freq. : "); Serial.println(freq); */
      //unsigned long freq = (1000000.0 * loopCount) / delta;   // loops per second
      unsigned long freq = (1000000.0 * powerChangeCount) / delta;   // power changes per second
      //Serial.print("magnet freq. : "); Serial.println(freq);

      // notify stats
      sendHallValue();
      //sendStat( (unsigned int)freq );
      //sendStat( (unsigned int)power );
      
      // reset stats
      loopCount = powerChangeCount = 0;
      startT = micros();
    }
  }
}

void sendHallValue() {
  #define VAL_CODE 101
  if (hallVal<0)
    return;  //nop
  
  int lo, hi;
  lo = hallVal & 0xff;
  hi = (hallVal >> 8) & 0xff;
  Serial.write( VAL_CODE );
  Serial.write( lo );
  Serial.write( hi );
}


void sendStat(unsigned int val) {
  Serial.write( VAL_CODE );
  Serial.write( val & 0xff );  // low byte
  Serial.write( (val >> 8) & 0xff );  // high byte
}

void handleEdit() {
  #define THRESHOLD_CODE 102
  #define PID_K_CODE 103
  #define PID_KP_CODE 104
  #define PID_KD_CODE 105
  if (Serial.available() > 2) { // new msg received 
    int lo, hi;
    int code = Serial.read();
    switch( code ) {
      case THRESHOLD_CODE:
        lo = Serial.read();
        hi = Serial.read();
        threshold = lo + (hi<<8);
        // TEST ONLY
        //Serial.println(threshold);
        break;
      case PID_K_CODE:
        lo = Serial.read();
        hi = Serial.read();
        K = lo;
        break;
      case PID_KP_CODE:
        lo = Serial.read();
        hi = Serial.read();
        Kp = lo;
        break;
      case PID_KD_CODE:
        lo = Serial.read();
        hi = Serial.read();
        Kd = lo;
        break;
      default:
        // not recognized, discard and exit
        return;
    }
    // send back echo
    Serial.write( code );
    Serial.write( lo );
    Serial.write( hi );
  }  
}


/* Magnet utilities */
#define MAGNET_PIN 6  // PWM pin 6 at 980 Hz, pin 9 at 490 Hz

void  initMagnet() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MAGNET_PIN, OUTPUT);
  // initially magnet is OFF
  power = 0; 
  analogWrite(MAGNET_PIN, power);
}

void setMagnetPower(byte newPower) {
  if ( newPower != power ) {
    updateMagnetStats();
    power = newPower;
    analogWrite(MAGNET_PIN, power);
    // every time a change in PWM value occurs its pin output is reset to HIGH, producing a spike.
    // to mitigate this issue we avoid too frequent changes with a delay
    delayMicroseconds(800);
  }
}

/* PID utilities */
void initPID() {
  K = Kp = Ki = Kd = 0;
}

byte computePower(int hallVal, int hallValOld, int threshold) {
  //TODO PID rule power = K + Kp*err + Kd*diff_err + Ki*sum_err
  int err = hallVal - threshold;
  int diff_err = hallVal - hallValOld;
  int pid = (Kp*err + 10*Kd*diff_err)/10;
  int pw = K + pid;
  pw = max(0, min(255, pw));  // p in [0, 255]
  return (byte)pw;
}
