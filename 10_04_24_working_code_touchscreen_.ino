#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
byte byte1=0;
byte byte2=0;
byte byte3=0;
byte byte4=0;
int incomingByte = 0;
unsigned int xPos = 0;
unsigned int yPos = 0;
byte state=1;

// SERVOS
#include <Servo.h> 
int servoPin1 = 9;
int servoPin2 = 8; 
Servo Servo_y; 
Servo Servo_x;
double flat_y = 95;
double flat_x = 90;

double set_x = 500;
double set_y = 500;
double cmd_x;
double cmd_y;

double high_x =150;
double low_x = 50;
double high_y = 90;
double low_y = 50;




/////////////////PID CONSTANTS/////////////////
// TODO: MAY NEED DIFFERENT CONSTANTS FOR X AND Y. 
// each axis has a different length, so different moment of inertia, etc.
const double Kpx = 10;
const double Kix = 0.05; //.1
const double Kdx = .275; //.25

const double Kpy = 10;
const double Kiy = 0.05;
const double Kdy = .16; //.15


void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(230400);
  while (!Serial) ; // wait for serial port to connect. Needed for Native USB only

  // set the data rate for the SoftwareSerial port
  Serial.println("Goodnight moon!");
  mySerial.begin(9600);
  mySerial.write(0xE1); // works without?
  mySerial.write(0xA1);
  //mySerial.println("Hello");

  Servo_y.attach(servoPin1); 
  Servo_x.attach(servoPin2);
  
}

void loop() // run over and over
{
  if (mySerial.available() > 0)
  {
    incomingByte = mySerial.read(); 
      switch (state) 
      {
        case 1:
          if (incomingByte & 0x80)
          { //check ph bit
            byte1 = incomingByte;
            state = 2;
          }
          else state = 1;
          break;

        case 2:
          if (incomingByte | 0x3F == 0x3F)
          { //check first two bits are zero
            byte2 = incomingByte;
            state = 3;
          }
          else state = 1;
          break;
        case 3:
          if (incomingByte | 0x0F == 0x7F)
          { //check
            byte3 = incomingByte;
            state = 4;
          }
          else state = 1;
          break;
        case 4:
          if (incomingByte | 0x3F == 0x3F)
          { //check
            byte4 = incomingByte;
            state = 1;
          }
          else state = 1;
          xPos = byte2; //x is long side
          xPos = (xPos << 4) | (byte1 & 0x0F);
          yPos = byte4;
          yPos = (yPos << 4) | (byte3 & 0x0F);

          cmd_x = -Kpx*(set_x - xPos)+flat_x;
          if (cmd_x>high_x)
            cmd_x=high_x;
          if (cmd_x<low_x)
            cmd_x=low_x;
          Servo_x.write(cmd_x); 
          
          cmd_y = -Kpy*(set_y - yPos)+flat_y;
          if (cmd_y>high_y)
            cmd_y=high_y;
          if (cmd_y<low_y)
            cmd_y=low_y;
          Servo_y.write(cmd_y); 
          
//          Serial.print(xPos, DEC);
//          Serial.print(", ");
//          Serial.print(yPos, DEC);
//          Serial.print(", ");
//          Serial.print(cmd_x, DEC);
//          Serial.print(", ");
//          Serial.println(cmd_y, DEC);
          break;
        %% hi
      }
  }
  
}
