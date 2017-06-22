//Processing incoming serial data 
#include <Messenger.h>
//Contain definition of maximum limits of various data type
#include <limits.h>

//Messenger object
Messenger Messenger_Handler = Messenger();

///////////////////////////////////////////////////////////////////////////////////////

#define OUTPUT_READABLE_QUATERNION


//Encoder pins definition
// Left encoder

#define Left_Encoder_PinA 38
#define Left_Encoder_PinB 37

volatile long Left_Encoder_Ticks = 0;
volatile bool LeftEncoderBSet;

//Right Encoder

#define Right_Encoder_PinA 36
#define Right_Encoder_PinB 35
volatile long Right_Encoder_Ticks = 0;
volatile bool RightEncoderBSet;

/////////////////////////////////////////////////////////////////
//Motor Pin definition
//Left Motor pins

#define A_1 34
#define B_1 33

//PWM 1 pin number
#define PWM_1 19


//Right Motor
#define A_2 32 
#define B_2 31

//PWM 2 pin number
#define PWM_2 39

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ultrasonic pins definition
const int echo = 9, Trig = 5;
long duration, cm;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Reset pin for resetting Tiva C, if this PIN set high, Tiva C will reset
#define RESET_PIN PB_2

/////////////////////////////////////////////////////////////////////////////////////////
//Time  update variables

unsigned long LastUpdateMicrosecs = 0;    
unsigned long LastUpdateMillisecs = 0;
unsigned long CurrentMicrosecs = 0;
unsigned long MicrosecsSinceLastUpdate = 0;
float SecondsSinceLastUpdate = 0;

///////////////////////////////////////////////////////////////////////////////////////
//Motor speed from PC
//Motor left and right speed
float motor_left_speed = 0;
float motor_right_speed = 0;
/////////////////////////////////////////////////////////////////


//Setup serial, encoders, ultrasonic, MPU6050 and Reset functions
void setup()
{
  
  //Init Serial port with 115200 baud rate
  Serial.begin(115200);  
  
  //Setup Encoders
  SetupEncoders();
  //Setup Motors
  SetupMotors();
  //Setup Ultrasonic
  SetupUltrasonic();  
  //Setup Reset pins
  SetupReset();
  //Set up Messenger 
  Messenger_Handler.attach(OnMssageCompleted);
   
}

//SetupEncoders() Definition

void SetupEncoders()
{
  // Left encoder
  pinMode(Left_Encoder_PinA, INPUT_PULLUP);      // sets pin A as input  
  pinMode(Left_Encoder_PinB, INPUT_PULLUP);      // sets pin B as input
  //Attaching interrupt in Left_Enc_PinA.
  attachInterrupt(Left_Encoder_PinA, do_Left_Encoder, RISING);
  

  // Right encoder
  pinMode(Right_Encoder_PinA, INPUT_PULLUP);      // sets pin A as input
  pinMode(Right_Encoder_PinB, INPUT_PULLUP);      // sets pin B as input
  //Attaching interrupt in Right_Enc_PinA.
  attachInterrupt(Right_Encoder_PinA, do_Right_Encoder, RISING); 

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup Motors() function

void SetupMotors()
{
 
 //Left motor
 pinMode(A_1,OUTPUT);
 pinMode(B_1,OUTPUT); 
 

 //Right Motor
 pinMode(A_2,OUTPUT);
 pinMode(B_2,OUTPUT);  
  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup UltrasonicsSensor() function
void SetupUltrasonic()
{
 pinMode(Trig, OUTPUT);
 pinMode(echo, INPUT); 
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup MPU6050 function


//Setup Reset() function

void SetupReset()

{
  
  
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RESET_PIN,OUTPUT);
  
  
  ///Conenect RESET Pins to the RESET pin of launchpad,its the 16th PIN
  digitalWrite(RESET_PIN,HIGH);
  
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MAIN LOOP

void loop()
{

    //Read from Serial port
    Read_From_Serial();
    
    
    //Send time information through serial port
    Update_Time();
    
    //Send encoders values through serial port
    Update_Encoders();
    
    //Send ultrasonic values through serial port
   // Update_Ultra_Sonic();

    //Update motor values with corresponding speed and send speed values through serial port
    Update_Motors();
  
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Read from Serial Function

void Read_From_Serial()

{
   while(Serial.available() > 0)
    {
     
       int data = Serial.read();
       Messenger_Handler.process(data);
     
     
    } 
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OnMssg Complete function definition

void OnMssageCompleted()
{
   
  char reset[] = "r";
  char set_speed[] = "s";
  
  if(Messenger_Handler.checkString(reset))
  {
    
     Serial.println("Reset Done"); 
     Reset();
    
  }
  if(Messenger_Handler.checkString(set_speed))
  {
    
     //This will set the speed
     Set_Speed();
     return; 
    
    
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Left_Encoder() Definitions
void do_Left_Encoder()
{
   // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  LeftEncoderBSet = digitalRead(Left_Encoder_PinB);   // read the input pin
  Left_Encoder_Ticks -= LeftEncoderBSet ? -1 : +1;
   
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Right_Encoder() Definitions

void do_Right_Encoder()
{
  
  RightEncoderBSet = digitalRead(Right_Encoder_PinB);   // read the input pin
  Right_Encoder_Ticks += RightEncoderBSet ? -1 : +1;
 
 
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Set speed
void Set_Speed()
{
    
  motor_left_speed = Messenger_Handler.readLong();
  motor_right_speed = Messenger_Handler.readLong();
  
  
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Reset function
void Reset()
{
 
  digitalWrite(GREEN_LED,HIGH);
  delay(1000);
  digitalWrite(RESET_PIN,LOW);
  digitalWrite(GREEN_LED,LOW); 
 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Will update both motors
void Update_Motors()
{
  
  moveRightMotor(motor_right_speed);
  moveLeftMotor(motor_left_speed);

  Serial.print("s");                  
  Serial.print("\t");
  Serial.print(motor_left_speed);
  Serial.print("\t");
  Serial.print(motor_right_speed);  
  Serial.print("\n");

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Will update both encoder value through serial port
void Update_Encoders()
{
 
  Serial.print("e");
  Serial.print("\t");
  Serial.print(Left_Encoder_Ticks);
  Serial.print("\t");
  Serial.print(Right_Encoder_Ticks);
  Serial.print("\n");
  
  
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Will update ultrasonic sensors through serial port

void Update_Ultra_Sonic()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  // The echo pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echo, HIGH);
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  
  //Sending through serial port
  Serial.print("u");
  Serial.print("\t");
  Serial.print(cm);
  Serial.print("\n");
  
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Update time function
void Update_Time()
{
  
      
  CurrentMicrosecs = micros();
  LastUpdateMillisecs = millis();
  MicrosecsSinceLastUpdate = CurrentMicrosecs - LastUpdateMicrosecs;
  if (MicrosecsSinceLastUpdate < 0)
    {
  MicrosecsSinceLastUpdate = INT_MIN - LastUpdateMicrosecs + CurrentMicrosecs;

    }
  LastUpdateMicrosecs = CurrentMicrosecs;
  SecondsSinceLastUpdate = MicrosecsSinceLastUpdate / 1000000.0;

  Serial.print("t");
  Serial.print("\t");
  Serial.print(LastUpdateMicrosecs);
  Serial.print("\t");
  Serial.print(SecondsSinceLastUpdate);
  Serial.print("\n");
 
  
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Conversion of microsecond to centimeter
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}


//Motor running function


void moveLeftMotor(float leftServoValue)
{
  if (leftServoValue>0)
  {
       
 digitalWrite(A_1,HIGH);
 digitalWrite(B_1,LOW);
 analogWrite(PWM_1,leftServoValue);
    
  }
  else if(leftServoValue<0)
  {
 digitalWrite(A_1,LOW);
 digitalWrite(B_1,HIGH);
 analogWrite(PWM_1,abs(leftServoValue));
 
  }
  
  else if(leftServoValue == 0)
  {
 digitalWrite(A_1,HIGH);
 digitalWrite(B_1,HIGH);
    
    
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void moveRightMotor(float rightServoValue)
{
 if (rightServoValue > 0)
  {
digitalWrite(A_2,LOW);
digitalWrite(B_2,HIGH);
analogWrite(PWM_2,rightServoValue);
  }
  else if(rightServoValue < 0)
  {
 digitalWrite(A_2,HIGH);
 digitalWrite(B_2,LOW);
 analogWrite(PWM_2,abs(rightServoValue));

  }
  else if(rightServoValue == 0)
  {

   digitalWrite(A_2,HIGH);
   digitalWrite(B_2,HIGH);
  
   }  
}
