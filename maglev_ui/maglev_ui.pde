import processing.serial.*;


final int INIT_THRESHOLD = 615;
final int INIT_PID_K = 195; // 0 -255
final int INIT_PID_KP = 35;  // > 0
final int INIT_PID_KD = 40;
final int UNDEF = -1;

Serial port;  // Create object from Serial class

int threshold; // holds reference value
int curr_val;      // current value read by arduino
int pid_k, pid_kp, pid_kd;

boolean waiting;  // wainting response from arduino 

final int FONT_SIZE = 20;
final int SPACING = FONT_SIZE + FONT_SIZE/10;

/*
* NB: Serial() init needs time (up to 3 secs) to complete. 
*     In the meantime serial comunication is not working.
*/
void setup() 
{
  
  threshold = UNDEF; // undef value
  curr_val = UNDEF; // undef value
  pid_k = pid_kp = pid_kd = UNDEF;
  
  String serial0 = Serial.list()[0];
  port = new Serial(this, serial0, 115200); // 9600);
  print("Initializing port", serial0, " ...");
  port.buffer(3);  // trigger serialEvent() after 3 bytes received (1 code + 2 integer)
    
  // window size
  size(400,200);
  // font size
  textSize(FONT_SIZE);
  // define text color
  fill(0);
  
  println("\nInitialization completed!");
}


void draw()
{  
  if ( threshold == UNDEF ) {
    delay(500); // serial init needs time to complete 
    // try to send initial value to arduino
    sendInt16( THRESHOLD_CODE, INIT_THRESHOLD );
    sendInt16( PID_K_CODE, INIT_PID_K );
    sendInt16( PID_KP_CODE, INIT_PID_KP );
    sendInt16( PID_KD_CODE, INIT_PID_KD );
  }
  
  // clear window
  background(255);
  // draw updated text
  textAlign(LEFT);
  text("Usage: ", 10, SPACING);
  text("'q' / 'Q': increase threshold (+1/+10)", 20, 2*SPACING);
  text("'a' / 'A': decrease threshold (-1/-10)", 20, 3*SPACING);
  textAlign(CENTER);
  if ( threshold > -1 )
    text("Threshold: "+threshold, 200, 6*SPACING);
  else
    text("Threshold: ...", 200, 6*SPACING);
}

//called everytime a key is pressed
void keyTyped(){
  //print(key);
  byte delta, code;
  int value;
  switch (key) {
    case 'q':
      code = THRESHOLD_CODE; value = threshold; delta = +1;
      break;
    case  'a':
      code = THRESHOLD_CODE; value = threshold; delta = -1;
      break;
    case 'Q':
      code = THRESHOLD_CODE; value = threshold; delta = +10;
      break;
    case  'A':
      code = THRESHOLD_CODE; value = threshold; delta = -10;
      break;
      
    case 'w':
      code = PID_K_CODE; value = pid_k; delta = +1;
      break;
    case  's':
      code = PID_K_CODE; value = pid_k; delta = -1;
      break;
    
    case 'e':
      code = PID_KP_CODE; value = pid_kp; delta = +1;
      break;
    case  'd':
      code = PID_KP_CODE; value = pid_kp; delta = -1;
      break;  
    
    case 'r':
      code = PID_KD_CODE; value = pid_kd; delta = +1;
      break;
    case  'f':
      code = PID_KD_CODE; value = pid_kd; delta = -1;
      break;
      
    default:
      return;  // nothing to do, exit 
  }

  if ( value != UNDEF )
    sendInt16(code, value+delta );
}


final byte VALUE_CODE     = 101;
final byte THRESHOLD_CODE = 102;
final byte PID_K_CODE     = 103;
final byte PID_KP_CODE     = 104;
final byte PID_KD_CODE     = 105;

// called when port buffer is full
void serialEvent(Serial p) {
  int code = port.read();
  switch (code) {
    case THRESHOLD_CODE:
      threshold = readInt16();
      print("+");
      break;
    case VALUE_CODE:
      curr_val = readInt16();
      print("*");
      break;
    case PID_K_CODE:
      pid_k = readInt16();
      print("#");
      break;
    case PID_KP_CODE:
      pid_kp = readInt16();
      print("%");
      break;
    case PID_KD_CODE:
      pid_kd = readInt16();
      print("$");
      break;
    default:
      //println("unknown code: "+ code);
      return;
  }
  println("threshold: "+threshold+" | current value: "+curr_val
         +" | pid K: "+pid_k+" | pid Kp: "+pid_kp+" | pid Kd: "+pid_kd);
}

void sendInt16(final byte code, final int value) {
  //println("sendInt16()");
  // little endian order
  port.write( code );
  port.write( (byte)(value&0xff) ); // low byte
  port.write( (byte)(value>>8&0xff) ); // high byte
}

int readInt16() {
  //println("readInt16()");
  return port.read() + (port.read()<<8); // little endian order
}
