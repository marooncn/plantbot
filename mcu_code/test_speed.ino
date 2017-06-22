///Left Motor  Pins
#define INA_1 32 //PC7
#define INB_1 31 //PD6
#define PWM_1 39 //PF2

///Right Motor Pins
#define INA_2 34  //PD7
#define INB_2 33  //PF4
#define PWM_2 19  //PF3

//Encoder pins definition

// Left encoder

#define Left_Encoder_PinA 36
#define Left_Encoder_PinB 35

#define Ticks_per_mm 4.685

volatile long Left_Encoder_Ticks = 0;

//Variable to read current state of left encoder pin
volatile bool LeftEncoderBSet;
volatile long Left_Encoder_Ticks_last;
//Right Encoder

#define Right_Encoder_PinA 38
#define Right_Encoder_PinB 37
volatile long Right_Encoder_Ticks = 0;
//Variable to read current state of right encoder pin
volatile bool RightEncoderBSet;
volatile long Right_Encoder_Ticks_last;

void SetupEncoders();
void do_Left_Encoder();
void do_Right_Encoder();
 


void setup()
{  //Init Serial port with 115200 buad rate
  Serial.begin(115200);  
  SetupEncoders();

//Setting Left Motor pin as OUTPUT
 pinMode(INA_1,OUTPUT); 
 pinMode(INB_1,OUTPUT);

//Setting Right Motor pin as OUTPUT
 pinMode(INA_2,OUTPUT);
 pinMode(INB_2,OUTPUT);
}


void loop()
{
   for(int i=60;i<=255;i++)
   {
//Setting CW rotation to and Left Motor  and CCW to Right Motor 
 //Left Motor    
     Serial.print("pwm is ");
     Serial.print(i);
     Serial.print("\t");
     Left_Encoder_Ticks_last = Left_Encoder_Ticks;
     Right_Encoder_Ticks_last = Right_Encoder_Ticks;
     
   digitalWrite(INA_1,HIGH);
   digitalWrite(INB_1,LOW);
   analogWrite(PWM_1,i);

//Right Motor 
  // digitalWrite(INA_2,LOW);
   //digitalWrite(INB_2,HIGH);
  // analogWrite(PWM_2,i);
    delay(5000);
    Update_Encoders();   
   } 
   while(1);
}
void SetupEncoders()
{
  // Quadrature encoders
  // Left encoder
  pinMode(Left_Encoder_PinA, INPUT_PULLUP);      // sets pin A as input  
  pinMode(Left_Encoder_PinB, INPUT_PULLUP);      // sets pin B as input
  attachInterrupt(Left_Encoder_PinA, do_Left_Encoder, RISING);
  

  // Right encoder
//  pinMode(Right_Encoder_PinA, INPUT_PULLUP);      // sets pin A as input
//  pinMode(Right_Encoder_PinB, INPUT_PULLUP);      // sets pin B as input
//Setting Left Motor pin as OUTPUT
  pinMode(INA_1,OUTPUT); 
  pinMode(INB_1,OUTPUT);

//Setting Right Motor pin as OUTPUT
  pinMode(INA_2,OUTPUT);
  pinMode(INB_2,OUTPUT);
//  attachInterrupt(Right_Encoder_PinA, do_Right_Encoder, RISING); 
}
 void do_Left_Encoder()
{
  LeftEncoderBSet = digitalRead(Left_Encoder_PinB);   // read the input pin
  Left_Encoder_Ticks -= LeftEncoderBSet ? -1 : +1;
   
}
 void do_Right_Encoder()
{
  RightEncoderBSet = digitalRead(Right_Encoder_PinB);   // read the input pin
  Right_Encoder_Ticks += RightEncoderBSet ? -1 : +1;
 } 
 void Update_Encoders()
{
  float left_vel,right_vel;
  left_vel = (Left_Encoder_Ticks - Left_Encoder_Ticks_last)/(Ticks_per_mm*5);
  Serial.print(left_vel);
  Serial.print("\n");
} 
 

